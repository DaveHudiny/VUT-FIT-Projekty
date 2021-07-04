# Author: David Hudák
# Project: IPP2
# Subject: IPP
# Description: This program works as interpret for xml version of language IPPcode21


import getopt, sys
import fileinput
import xml.etree.ElementTree as ET
import re

labels = {} # Dictionary of labels - expect structure label: line

frames = { # Dictionary of dictionaries, expecting GF: {name: [value, type]}
    "GF": {}, 
    "LF": {}, 
    "TF": {}
}
boolFrames = {"TF": False, "LF": False} # Saves just these two values - checks wheter were frames defined or not.
stackLF = [] # "Stack" for local frames.
returnVals = [] # "Stack" for call return pairs.

boolSTDIN = True # Trigger - was input STDIN or file?
stringInput = [] # If file, load all input to this huge big string.

dictInstr = { # Dictionary of arrays where key is name of instruction and values are types of arguments of these instructions.
    "MOVE" : ["var", "nlabel"],
	"DEFVAR" : ["var"],
	"TYPE" : ["var", "nlabel"],
	"CREATEFRAME" : [],
	"PUSHFRAME" : [],
	"POPFRAME" : [],
	"CALL" : ["label"],
	"RETURN" : [],
	"ADD" : ["var", "nlabel", "nlabel"],
	"SUB" : ["var", "nlabel", "nlabel"],
	"MUL" : ["var", "nlabel", "nlabel"],
	"IDIV" : ["var", "nlabel", "nlabel"],
	"LT" : ["var", "nlabel", "nlabel"],
	"GT" : ["var", "nlabel", "nlabel"],
	"EQ" : ["var", "nlabel", "nlabel"],
	"AND" : ["var", "nlabel", "nlabel"],
	"OR" : ["var", "nlabel", "nlabel"],
	"NOT" : ["var", "nlabel"],
	"INT2CHAR" : ["var", "nlabel"],
	"STRI2INT" : ["var", "nlabel", "nlabel"],
	"READ" : ["var", "type"],
	"WRITE" : ["nlabel"],
	"CONCAT" : ["var", "nlabel", "nlabel"],
	"STRLEN" : ["var", "nlabel"],
	"GETCHAR" : ["var", "nlabel", "nlabel"],
	"SETCHAR" : ["var", "nlabel", "nlabel"],
	"LABEL" : ["label"],
	"JUMP" : ["label"],
	"JUMPIFEQ" : ["label", "nlabel", "nlabel"],
	"JUMPIFNEQ" : ["label", "nlabel", "nlabel"],
	"EXIT" : ["nlabel"],
	"DPRINT" : ["nlabel"],
	"BREAK" : [],
	"POPS" : ["var"],
	"PUSHS" : ["nlabel"]
}


def wasAllocated(var): 
    """
    Checks whether the frame exists and whether the variable was defined.
    """
    frameName = var.split("@") # Split by @
    if frameName[0] == "GF":
        if not(frameName[1] in frames["GF"]):
            printEr(frameName[1] + " variable wasn't defined yet.", 54)
    elif frameName[0] == "LF":
        if boolFrames["LF"] == False:
            printEr("LF wasn't defined yet.", 55)
        if not(frameName[1] in frames["LF"]):
            printEr(frameName[1] + " variable wasn't defined yet.", 54)
    elif frameName[0] == "TF":
        if boolFrames["TF"] == False:
            printEr("TF wasn't defined yet.", 55)
        if not(frameName[1] in frames["TF"]):
            printEr(frameName[1] + " variable wasn't defined yet.", 54)
    else:
        printEr("Unknown frame.", 32)

stacky = [] # Stack for interpretation (some instructions work with stack.)

def POPS(var):
    """
    Function defines instruction POPS, read documentation for more info
    """
    wasAllocated(var)
    if len(stacky) == 0:
        printEr("Empty stack.",56)
    popper = stacky.pop()
    putVarFrame(var, popper[0], popper[1])
        

def PUSHS(valu, itypendo):
    """
    Function defines instruction PUSHS, read documentation for more info
    """
    val, typendo = getType(valu, itypendo, 1)
    stacky.append([val, typendo])

def BREAK(line):
    """
    Function defines instruction BREAK, read documentation for more info
    """
    print("Current line is " + str(line), sys.stderr)

def DPRINT(valu, itypendo):
    """
    Function defines instruction DPRINT, read documentation for more info
    """
    val, _ = getType(valu, itypendo, 1)
    print(val, file=sys.stderr)

def EXIT(valu, itypendo):
    """
    Function defines instruction EXIT, read documentation for more info
    """
    val, typendo = getType(valu, itypendo, 1)
    if typendo != "int":
        printEr("Wrong operand for exit",53)
    if int(val) < 0 or int(val) > 49: # Legal exit values
        printEr("Wrong value for exit", 57) # Error code for error code function. lol.
    printEr("",int(val))

def JUMP(label):
    """
    Function defines instruction JUMP, read documentation for more info
    """
    if label in labels:
        return labels[label]+1
    else:
        printEr("Non existing label",52)
    
def JUMPIFEQ(label, valu1, itypendo1, valu2, itypendo2, line):
    """
    Function defines instruction JUMPIFEQ, read documentation for more info
    """
    if label in labels:
        val1, typendo1 = getType(valu1, itypendo1, 1)
        val2, typendo2 = getType(valu2, itypendo2, 1)
        val1 = retype(val1, typendo1)
        val2 = retype(val2, typendo2)
        if typendo1 != typendo2 and (typendo1 != "nil" and typendo2 != "nil"): # Values got different types (and not nils)
            printEr("Comparsion between wrong operands", 53)
        elif (typendo1 == "nil" and typendo2 != "nil") or (typendo1 != "nil" and typendo2 == "nil"): # One type was nil and nothing is equal to nil (except for nil)
            return line + 1
        elif typendo1 == typendo2 == "nil":
            return labels[label] + 1 # Nils are always equal
        elif val1 == val2: # Are values equal?
            return labels[label] + 1
        else:
            return line + 1
    else:
        printEr("Non existing label",52) #Whoops

def JUMPIFNEQ(label, valu1, itypendo1, valu2, itypendo2, line):
    """
    Function defines instruction JUMPIFNEQ, read documentation for more info
    """
    if label in labels:
        val1, typendo1 = getType(valu1, itypendo1, True)
        val2, typendo2 = getType(valu2, itypendo2, True)
        val1 = retype(val1, typendo1)
        val2 = retype(val2, typendo2)
        if typendo1 != typendo2 and (typendo1 != "nil" and typendo2 != "nil"): # Some as for function before. Please, scroll
            printEr("Comparsion between wrong operands", 53)
        elif (typendo1 == "nil" and typendo2 != "nil") or (typendo1 != "nil" and typendo2 == "nil"):
            return labels[label] + 1
        elif typendo1 == typendo2 == "nil":
            return line + 1
        elif val1 == val2:
            return line + 1
        else:
            return labels[label] + 1
    else:
        printEr("Non existing label",52)

def CREATEFRAME():
    """
    Function defines instruction CREATEFRAME, read documentation for more info
    """
    boolFrames["TF"] = True
    frames["TF"] = {}

def DEFVAR(where):
    """
    Function defines instruction DEFVAR, read documentation for more info
    """
    frameName = where.split("@",1)
    if frameName[0] in ["LF", "TF"]:
        if boolFrames[frameName[0]] == False:
            printEr("Frame " + frameName[0] + " wasn't defined yet.", 55)
    if frameName[1] in frames[frameName[0]]:
        printEr("Redefinition of variable is illegal.", 52)
    frames[frameName[0]][frameName[1]] = ["", ""]

def MOVE(where, what, typendo):
    """
    Function defines instruction MOVE, read documentation for more info
    """
    wasAllocated(where)
    varName = where.split("@",1)
    if typendo == "var":
        wasAllocated(what) # Exits program if it wasn't allocated
        frames[varName[0]][varName[1]] = getType(what, "var", 1)
    else:
        frames[varName[0]][varName[1]] = [what, typendo]

def LABEL(name, line):
    """
    Function defines instruction LABEL, read documentation for more info
    """
    if not(isLabel(name)):
        printEr("Is not legal label", 32)
    if not(name in labels):
        labels[name] = line
    else:
        printEr("Overwriting labels is illegal", 52)

def getVarFrame(var):
    """
    Returns variable from frame with input name (frame@name)
    """
    varName = var.split("@",1)
    return frames[varName[0]][varName[1]]

def putVarFrame(var, value, typendo):
    """
    Saves input value to frames dictionary
    """
    varName = var.split("@",1)
    frames[varName[0]][varName[1]] = [value, typendo]

def TYPE(var, val, typendo):
    """
    Function defines instruction TYPE, read documentation for more info
    """
    wasAllocated(var)
    if typendo != "var":
        putVarFrame(var, typendo, "string")
    else:
        wasAllocated(val)
        putVarFrame(var, getVarFrame(val)[1], "string")
    
def PUSHFRAME():
    """
    Function defines instruction PUSHFRAME, read documentation for more info
    """
    if boolFrames["TF"] == False:
        printEr("Temporary frame does not exist, no PUSHFRAME", 55)
    if boolFrames["LF"] == False: # No local frame exists
        boolFrames["LF"] = True
        frames["LF"] = frames["TF"]
        
    else: # Local frame exists yet - push it back
        stackLF.append(frames["LF"])
        frames["LF"] = frames["TF"]

    frames["TF"] = {}
    boolFrames["TF"] = False

def POPFRAME():
    """
    Function defines instruction POPFRAME, read documentation for more info
    """
    if boolFrames["LF"] == True:
        frames["TF"] = frames["LF"]
        if len(stackLF) == 0:
            boolFrames["LF"] = False
            frames["LF"] = {}
        else:
            frames["LF"] = stackLF.pop()
    else:
        printEr("Any local frame does not exist, no POPFRAME", 55)
    boolFrames["TF"] = True

def CALL(label, line):
    """
    Function defines instruction CALL, read documentation for more info
    """
    returnVals.append(line+1) # Saves line value to stack.
    if label in labels:
        return labels[label]+1
    else:
        printEr("Undefined label", 52)

def RETURN():
    """
    Function defines instruction RETURN, read documentation for more info
    """
    if len(returnVals) == 0:
        printEr("Return when no functions called", 56)
    else:
        return returnVals.pop() # Return line number from stack of return values

def getIntValue(val, typendo):
    """
    Reads value from val (if int, it's simple, if variable, it's worse and if it's string or bool, it's worst.)
    """
    if typendo == "var":
        wasAllocated(val)
        if getVarFrame(val)[1] == "int":
            return getVarFrame(val)[0]
        elif getVarFrame(val)[1] == "":
            printEr("Uninitialized value", 56)
        else:
            printEr("Wrong types of operands", 53)
    elif typendo == "int":
        return val
    else:
        printEr("Operand should be integer", 53)

def ADD(var, valu1, typendo1, valu2, typendo2):
    """
    Function defines instruction ADD, read documentation for more info
    """
    wasAllocated(var)
    if typendo1 in ["var", "int"] and typendo2 in ["var", "int"]:
        val1 = getIntValue(valu1, typendo1)
        val2 = getIntValue(valu2, typendo2)
        val = str(int(val1) + int(val2))
        putVarFrame(var, val, "int")
    else:
        printEr("ADD is only for integers", 53)

def SUB(var, valu1, typendo1, valu2, typendo2):
    """
    Function defines instruction SUB, read documentation for more info
    """
    wasAllocated(var)
    if typendo1 in ["var", "int"] and typendo2 in ["var", "int"]:
        val1 = getIntValue(valu1, typendo1)
        val2 = getIntValue(valu2, typendo2)
        val = str(int(val1) - int(val2))
        putVarFrame(var, val, "int")
    else:
        printEr("SUB is only for integers", 53)  

def MUL(var, valu1, typendo1, valu2, typendo2):
    """
    Function defines instruction MUL, read documentation for more info
    """
    wasAllocated(var)
    if typendo1 in ["var", "int"] and typendo2 in ["var", "int"]:
        val1 = getIntValue(valu1, typendo1)
        val2 = getIntValue(valu2, typendo2)
        val = str(int(val1) * int(val2))
        putVarFrame(var, val, "int")
    else:
        printEr("MUL is only for integers", 53) 

def IDIV(var, valu1, typendo1, valu2, typendo2):
    """
    Function defines instruction IDIV, read documentation for more info
    """
    wasAllocated(var)
    if typendo1 in ["var", "int"] and typendo2 in ["var", "int"]:
        val1 = getIntValue(valu1, typendo1)
        val2 = getIntValue(valu2, typendo2)
        if int(val2) == 0:
            printEr("Division by zero!", 57)
        val = str(int(val1) // int(val2))
        putVarFrame(var, val, "int")
    else:
        printEr("IDIV is only for integers", 53)  

def getType(var, typendo, init):
    """
    Function returns real type and value of input (for 10, "int", 1 it returns simply 10 and "int", but sometimes we need to read value from variable.)
    """
    if typendo != "var": 
        return var, typendo
    else:
        wasAllocated(var)
        if init and getVarFrame(var)[1] == "": # If input was variable and user needs value initialized, make some additional verifications.
            printEr("Uninitialized value.", 56)
        return getVarFrame(var)


def retype(val, goalType):
    """
    Function returns real value of value from xml (value from XML is always string, but somtimes we need int, bool,...)
    """
    if goalType == "bool":
        if val == "true":
            return True
        else:
            return False
    elif goalType == "int":
        return int(val)
    elif goalType == "string":
        return str(val)
    elif goalType == "nil":
        return "nil"


def LT(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction LT, read documentation for more info
    """
    val1, typendo1 = getType(valu1, itypendo1, True)
    val2, typendo2 = getType(valu2, itypendo2, True)
    val1 = retype(val1, typendo1)
    val2 = retype(val2, typendo2)
    if typendo1 != typendo2:
        printEr("Comparsion between wrong operands", 53)
    if typendo1 == "nil" or typendo2 == "nil": # Nils unsupported in GT and LT
        printEr("Comparsion with nil", 53)
    wasAllocated(var)
    if val1 < val2:
        putVarFrame(var, "true", "bool")
    else:
        putVarFrame(var, "false", "bool")

    

def GT(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction GT, read documentation for more info
    """
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    val1 = retype(val1, typendo1)
    val2 = retype(val2, typendo2)
    if typendo1 != typendo2:
        printEr("Comparsion between wrong operands", 53)
    if typendo1 == "nil" or typendo2 == "nil": # Nils unsupported in GT and LT
        printEr("Comparsion with nil", 53)
    wasAllocated(var)
    if val1 > val2:
        putVarFrame(var, "true", "bool")
    else:
        putVarFrame(var, "false", "bool")


def EQ(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction EQ, read documentation for more info
    """
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    val1 = retype(val1, typendo1)
    val2 = retype(val2, typendo2)
    if typendo1 != typendo2 and typendo1 != "nil" and typendo2!="nil":
        printEr("Comparsion between wrong operands", 53)
    wasAllocated(var)
    if typendo1 == typendo2 == "nil":
        putVarFrame(var, "true", "bool")
    elif (typendo1 == "nil" and typendo2 != "nil") or (typendo1 != "nil" and typendo2 == "nil"): # Nothing is same as null
        putVarFrame(var, "false", "bool")
    elif val1 == val2:
        putVarFrame(var, "true", "bool")
    else:
        putVarFrame(var, "false", "bool")


def AND(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction AND, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    if typendo1 == typendo2 == "bool":
        val1 = retype(val1, "bool")
        val2 = retype(val2, "bool")
        if val1 and val2:
            putVarFrame(var, "true", "bool")
        else:
            putVarFrame(var, "false", "bool")
    else:
        printEr("Comparsion between wrong operands", 53)



def OR(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction OR, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    if typendo1 == typendo2 == "bool":
        val1 = retype(val1, "bool")
        val2 = retype(val2, "bool")
        if val1 or val2:
            putVarFrame(var, "true", "bool")
        else:
            putVarFrame(var, "false", "bool")
    else:
        printEr("Comparsion between wrong operands", 53)


def NOT(var, valu1, itypendo1):
    """
    Function defines instruction NOT, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    if typendo1 == "bool":
        val1 = retype(val1, "bool")
        if val1:
            putVarFrame(var, "false", "bool")
        else:
            putVarFrame(var, "true", "bool")
    else:
        printEr("Comparsion between wrong operands", 53)

def INT2CHAR(var, valu1, itypendo1):
    """
    Function defines instruction INT2CHAR, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    if typendo1 != "int":
        printEr("Invalid argument for INT2CHAR",53)
    else:
        val1 = int(val1)
        if 0 < val1 < 32 or val1 == 35 or val1 == 92:
            putVarFrame(var, "\0" + str(val1), "string")
        else:
            try:
                putVarFrame(var, chr(val1), "string")
            except:
                printEr("Invalid INT2CHAR value.", 58)


def STRI2INT(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction STRI2INT, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    if typendo1 != "string" or typendo2 != "int":
        printEr("Wrong operands for STRI2INT",53)
    if int(val2) >= len(val1) or int(val2) < 0:
        printEr("Wrong index for STRI2INT", 58)
    putVarFrame(var, str(ord(val1[int(val2)])), "int")


def READ(var, typendo):
    """
    Function defines instruction READ, read documentation for more info
    My longest instruction because of all these exceptions...
    """
    wasAllocated(var)
    if boolSTDIN:
        try: # Read value from stdin.
            inp = input()
        except:
            printEr("Missing value for instruction READ", 56)
    else:
        if len(stringInput) == 0:
            putVarFrame(var, "nil", "nil")
            return
        inp = stringInput[0]
        if typendo == "int":
            splited = re.match(r"([0-9]+)([a-z]+)", inp) # Divide strings to number and characters part.
            if splited:
                splited = splited.groups()
                if splited[0].isnumeric():
                    putVarFrame(var, int(splited[0]), "int")
                    return
                else:
                    putVarFrame(var, "nil", "nil")
                    return
            else:
                putVarFrame(var, "nil", "nil")
                return
        stringInput.pop(0)
    if typendo == "string":
        putVarFrame(var, str(inp), typendo)
    elif typendo == "bool": # bool is true only if input was literally "true". Anything else is false.
        if inp == "true":
            putVarFrame(var, "true", typendo)
        else:
            putVarFrame(var, "false", typendo)
    else:
        printEr("Wrong operand type for READ.",53)
        putVarFrame(var, inp, typendo)
    
def WRITE(valu, itypendo):
    """
    Function defines instruction WRITE, read documentation for more info
    """
    val, typendo = getType(valu, itypendo, 1)
    if typendo == "string":
        if val == "":
            return

        escapes = re.findall(r"\\[0-9][0-9][0-9]", val) # Translates escape sequencies to normal chars.
        for esc in escapes:
            val = val.replace(esc, chr(int(esc[1:4])))

        print(val, end="")
    elif typendo == "int":
        print(val, end="")
    elif typendo == "bool":
        print(val, end="")
    elif typendo == "nil":
        print("", end="")
    else:
        printEr("Unknown type", 53)

def CONCAT(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction CONCAT, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    if typendo1 == typendo2 == "string":
        putVarFrame(var, val1+val2, "string")
    else:
        printEr("Wrong operands for CONCAT", 53)


def STRLEN(var, valu, itypendo):
    """
    Function defines instruction STRLEN, read documentation for more info
    """
    wasAllocated(var)
    val, typendo = getType(valu, itypendo, 1)
    if typendo == "string":
        putVarFrame(var, str(len(val)), "int")
    else:
        printEr("STRLEN works with strings.", 53)

def GETCHAR(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction GETCHAR, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1)
    val2, typendo2 = getType(valu2, itypendo2, 1)
    if typendo1 != "string" or typendo2 != "int":
        printEr("Wrong operands for GETCHAR",53)
    if int(val2) >= len(val1) or int(val2) < 0:
        printEr("Wrong index for GETCHAR", 58)
    putVarFrame(var, val1[int(val2)], "string")

def SETCHAR(var, valu1, itypendo1, valu2, itypendo2):
    """
    Function defines instruction SETCHAR, read documentation for more info
    """
    wasAllocated(var)
    val1, typendo1 = getType(valu1, itypendo1, 1) # Translates variables, if used.
    val2, typendo2 = getType(valu2, itypendo2, 1)
    value, typos = getType(var, "var", 1)
    if typos != "string":
        printEr("Wrong operands for SETCHAR",53)
    if typendo2 != "string" or typendo1 != "int":
        printEr("Wrong operands for SETCHAR",53)
    if int(val1) >= len(value) or int(val1) < 0:
        printEr("Wrong index for SETCHAR", 58)
    if len(val2) == 0:
        printEr("Wrong input for SETCHAR", 58)
    escapes = re.findall(r"\\[0-9][0-9][0-9]", val2) # Just copy paste from another part of this program. :)
    for esc in escapes:
        val2 = val2.replace(esc, chr(int(esc[1:4]))) # Replaces occurencies of \xyz to normal characters
    putVarFrame(var, value[:int(val1)] + val2[0] + value[int(val1)+1:], "string")


def printEr(mess, code):
    """
    Function prints error message and exit program with chosen errorcode.
    """
    print(mess, file=sys.stderr)
    sys.exit(code)

specialChars = ["_", "-", "$", "&", "%", "*", "!", "?"]

def isLegalForName(name):
    """
    Verifies correctnes of string whether it is legal name or not
    """
    isIt = True
    for charac in name:
        if not(charac.isalnum()) and not(charac in specialChars):
            isIt = False
    return isIt

def isLabel(label):
    """
    Verifies corectness of string whether it is correct label or not
    """
    if "@" in label:
        return False
    return isLegalForName(label)

def isLegalType(typendo):
    """
        Function verifies corectness of string whether is legal type or not
    """
    return (typendo in ["int", "string", "bool"])

def isLegalVar(potentialVar):
    """
    Function verifies wheter the string is legal variable string or not.
    """
    if "@" in potentialVar:
        var = potentialVar.split("@")
        if len(var)>2:
            return False
        if not (var[0] in ["GF", "LF", "TF"]):
            return False
        if not (isLegalForName(var[1])):
            return False
        return True
    return False

def isLegalNLabel(potentialN, typendo): 
    """Functions verifies legality of not label string (in official documentation <symb>)
    potentialN - string, which should be verified
    typendo - type, which is expected"""
    if typendo == "var":
        return isLegalVar
    elif typendo == "int":
        return re.fullmatch(r"[+\-0-9][0-9]*", potentialN)
    elif typendo == "string":
        helper = str(potentialN)
        helper = re.sub(r"\\[0-9][0-9][0-9]", "", helper)
        helper = re.sub(r"(&at|&gt|&amp)", "", helper)
        return re.fullmatch(r"[a-zA-Z0-9!\"\$-\@\[\]-\`\{-ǿ]*", helper)
    elif typendo == "bool":
        return re.fullmatch("(true|false)", potentialN)
    elif typendo == "nil":
        return re.fullmatch("nil", potentialN)

def lexSynAnalys(code):
    """
    Does the stuff which should control parse.php and also initialize dictionary with labels.
    """
    if code.tag != "program":
        printEr("XML - Missing tag program", 32)
    if "language" in code.attrib:
        if code.attrib["language"] != "IPPcode21":
            printEr("Wrong language", 32)
    else:
        printEr("Missing attribute language",32)
    order = 0
    i = 0
    for child in code: # First cycle just checks if there are orders in tags. I hate it, but I don't know any different way.
        if not("order" in child.attrib):
            printEr("Missing order", 32)
        elif not(child.attrib["order"].isnumeric()):
            printEr("Order is not numeric", 32)

    code = sorted(code, key= lambda k: (k.tag, int(k.get("order")))) # This sorts code by order of instructions.
    for child in code: # Long cycle which goes through all xml code file.

        if child.tag != "instruction":
            printEr("Wrong argument tag", 32)
        if "order" in child.attrib:
            if not(child.attrib["order"].isnumeric()):
                printEr("order should be integer",32)
            else:
                if int(child.attrib["order"]) <= order:
                    printEr("Wrong order", 32)
                else:
                    order = int(child.attrib["order"])
        else:
            printEr("Missing order",32)

        if "opcode" in child.attrib:
            if not(child.attrib["opcode"].upper() in dictInstr):
                printEr("Unknown instruction.",32)
        else:
            printEr("Missing opcode", 32)

        if len(child)!=len(dictInstr[child.attrib["opcode"].upper()]): # Number of arguments of instruction is different from number of expected arguments.
            printEr("Expected arguments: " + str(len(dictInstr[child.attrib["opcode"].upper()])) +", got: " + str(len(child)), 32)
        
        if child.attrib["opcode"].upper() == "LABEL":
            LABEL(child[0].text, i) # Label initialization.
            
        child[:] = sorted(child, key = lambda k: k.tag) # Sorts arguments of instruction by their tags (arg1, arg2...)

        for k in range(len(child)): # Checks arguments.
            
            if "arg" + str(k+1) != child[k].tag:
                printEr("Invalid tag.",32)
            if not("type" in child[k].attrib):
                printEr("Missing type description", 32)

            elif dictInstr[child.attrib["opcode"].upper()][k] == "label": # Reads from dictionary expected type of argument and compares.
                if not(isLabel(child[k].text)) or not(child[k].attrib["type"] == "label"):
                    printEr(child[k].text + "is not legal label", 32)
            
            elif dictInstr[child.attrib["opcode"].upper()][k] == "type":
                if not(isLegalType(child[k].text)) or not(child[k].attrib["type"] == "type"):
                    printEr(child[k].text +" is not legal type", 32)

            elif dictInstr[child.attrib["opcode"].upper()][k] == "var":
                if not(isLegalVar(child[k].text)) or not(child[k].attrib["type"] == "var"):
                    printEr(child[k].text + " is not legal var", 32)

            elif dictInstr[child.attrib["opcode"].upper()][k] == "nlabel":
                if not(isLegalNLabel(child[k].text, child[k].attrib["type"])):
                    printEr(child[k].text + " is not legal symb", 32)
            
            else:
                printEr("Unknown argument.", 32)

        i = i + 1 # Counter needed for label initialization.

def interpretation(code):
    """
    Function which interprets code.
    """
    code = sorted(code, key= lambda k: (k.tag, int(k.get("order"))))
    line = 0
    while line < len(code): # Reads code line by line - it's important for 
        instruction = code[line].attrib["opcode"].upper()
        if instruction in ["JUMP", "CALL", "JUMPIFEQ", "JUMPIFNEQ", "RETURN"]: # Category of instruction - jump instructions.
            if instruction == "RETURN": # Return is only instruction which doesn't need label.
                line = RETURN()
                continue
            label = code[line][0].text
            if instruction == "JUMP": # Unconditional jumps
                line = JUMP(label)
            elif instruction == "CALL":
                line = CALL(label, line)
            else:
                valu1 = code[line][1].text
                typendo1 = code[line][1].attrib["type"]
                valu2 = code[line][2].text
                typendo2 = code[line][2].attrib["type"]
                if valu1 is None: # Some stupid mechanics of XML - if there is no text, it creates none object. And I hate it.
                    valu1 = str("")
                if valu2 is None:
                    valu2 = str("")
                if instruction == "JUMPIFEQ":
                    line = JUMPIFEQ(label, valu1, typendo1, valu2, typendo2, line)
                else:
                    line = JUMPIFNEQ(label, valu1, typendo1, valu2, typendo2, line)
            continue

        elif instruction in ["DEFVAR", "POPS"]: # Similar instructions - need only one argument.
            var = code[line][0].text
            if var is None:
                var = str("")
            if instruction == "DEFVAR":
                DEFVAR(var)
            else:
                POPS(var)

        elif instruction in ["ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "CONCAT", "GETCHAR", "SETCHAR", "STRI2INT"]: # Category of instruction, which counts something.
            var = code[line][0].text
            valu1 = code[line][1].text
            typendo1 = code[line][1].attrib["type"]
            valu2 = code[line][2].text
            typendo2 = code[line][2].attrib["type"]
            if var is None: # I really hate XML
                var = str("")
            if valu1 is None:
                valu1 = str("")
            if valu2 is None:
                valu2 = str("")
            if instruction == "ADD": # I would appreciate PHP here, because it would do it with $instruction(params)
                ADD(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "SUB":
                SUB(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "MUL":
                MUL(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "IDIV":
                IDIV(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "LT":
                LT(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "GT":
                GT(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "EQ":
                EQ(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "AND":
                AND(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "OR":
                OR(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "CONCAT":
                CONCAT(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "GETCHAR":
                GETCHAR(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "SETCHAR":
                SETCHAR(var, valu1, typendo1, valu2, typendo2)
            elif instruction == "STRI2INT":
                STRI2INT(var, valu1, typendo1, valu2, typendo2)
            else:
                printEr("Internal error", 99) # Lol

        elif instruction in ["CREATEFRAME", "PUSHFRAME", "POPFRAME", "BREAK"]: # Another category of instructions
            if instruction == "CREATEFRAME":
                CREATEFRAME()
            elif instruction == "PUSHFRAME":
                PUSHFRAME()
            elif instruction == "POPFRAME":
                POPFRAME()
            elif instruction == "BREAK":
                BREAK(line)
            else:
                printEr("Internal error.", 99)
        elif instruction in ["MOVE", "TYPE", "NOT", "INT2CHAR", "STRLEN"]: # Instructions with 2 arguments.
            var = code[line][0].text
            if var is None:
                var = str("")
            if code[line][1].text is None:
                valu1 = str("")
            else:
                valu1 = code[line][1].text
            
            typendo1 = code[line][1].attrib["type"]
            if instruction == "MOVE":
                MOVE(var, valu1, typendo1)
            elif instruction == "TYPE":
                TYPE(var, valu1, typendo1)
            elif instruction == "NOT":
                NOT(var, valu1, typendo1)
            elif instruction == "INT2CHAR":
                INT2CHAR(var, valu1, typendo1)
            elif instruction == "STRLEN":
                STRLEN(var, valu1, typendo1)
            else:
                printEr("Internal error", 99)

        elif instruction in ["WRITE", "EXIT", "DPRINT", "PUSHS"]: # Instructions with only one character.
            valu1 = code[line][0].text
            if valu1 is None:
                valu1 = str("")
            typendo1 = code[line][0].attrib["type"]
            if instruction == "WRITE":
                WRITE(valu1, typendo1)
            elif instruction == "EXIT":
                EXIT(valu1, typendo1)
            elif instruction == "DPRINT":
                DPRINT(valu1, typendo1)
            elif instruction == "PUSHS":
                PUSHS(valu1, typendo1)
            else:
                print("Internal error", 99)
        elif instruction == "READ":
            var = code[line][0].text
            valu1 = code[line][1].text
            READ(var, valu1)
        elif instruction == "LABEL":
            pass
        #    label = code[line][0].text
        #    LABEL(label, line)
        else:
            print(instruction, file=sys.stderr) # My bad.
            printEr("David forgot to write some instruction, sorry :(", 99)

        line = line + 1
        #print(code[line].tag)
        #print(code[line].attrib)

def parseArgumentsAndExtractXML():
    """
    Function parse arguments of program, verifies it's correctness and makes object with XML file.
    Functions returns code from XML and all input.
    """
    try: 
        opt, _ = getopt.getopt(sys.argv[1:], "", ["help", "source=", "input="]) # Inspired by official Python web documentation.
    except getopt.GetoptError as err:
        print(err)
        sys.exit(-1)
    inputBool = False
    inputFile = ""
    sourceBool = False
    sourceFile = ""
    for option, inp in opt: # Getting arguments from command line. 
        if option == "--help":
            if len(sys.argv)>2:
                sys.exit(10)
            print("This is handsome help.\n--input=file argument is for telling, which file is input. If missing, input is stdin.")
            print("--source=file is argument for source file. If missing, input is stdin.\nAt least one argument have to be contained, or program fails.\n")
            sys.exit(0)
        elif option == "--input":
            inputFile = inp
            inputBool = True
        elif option == "--source":
            sourceBool = True
            sourceFile = inp
        else:
            sys.exit(10)
        
    if not(inputBool or sourceBool): # Too few arguments of program.
        print("Missing argument.", file=sys.stderr)
        sys.exit(10)
    if inputBool:
        try:
            inputStream = open(inputFile, "r")
        except:
            print("Failed to open input file. Verify correctness of name of your file.", file=sys.stderr)
            sys.exit(11)
    try:
        if sourceBool: # Reading of xml structure.
            tree = ET.parse(sourceFile)
        else:
            tree = ET.parse(sys.stdin)

    except (ET.ParseError, FileNotFoundError, PermissionError) as e:
        if type(e) == FileNotFoundError or type(e) == PermissionError:
            print("Failed to open source file. Verify correctness of name of your file.", file=sys.stderr)
            sys.exit(11)
        else:
            print("XML has wrong format.")
            sys.exit(31)

    code = tree.getroot() # translating it to something more normal.    

    if inputBool: # Choosing input.
        global stringInput 
        stringInput = inputStream.readlines()
        global boolSTDIN 
        boolSTDIN = False
    
    return code
    

code = parseArgumentsAndExtractXML() # Load code from XML
lexSynAnalys(code) # Verify code from XML.
interpretation(code) # Interpret code from XML.

sys.exit(0)

