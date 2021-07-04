#Author: David Hudák
#Subject: IPK
#Project: 1
#Description: Simple FSP and NSP protocols

from socket import *
import getopt, sys
import re

def NSP(IP, port, surl): # UDP
    """
    Function returns IP adress and port of nameserver gotten from surl
    Function connects via UDP connection.
    Function exits program in illegal states.
    """

    clientSocket = socket(AF_INET, SOCK_DGRAM) # Create new socket for UDP communication
    surlProt = surl.split("://") # Extract hostname from surl.
    if surlProt[0]!="fsp": 
        print("Wrong protocol")
        sys.exit(2)
    hostName = surlProt[1].split("/")

    if not(re.match("^([A-Za-z_.-])+$", hostName[0])): # Servername should contain only only alphanumerical characters or ".", "-" or "_".
        print("Illegal server name.")
        sys.exit(2)

    message = "WHEREIS " + hostName[0] + "\n" # creating message
    clientSocket.sendto(message.encode(), (IP, port)) # encoding message and sending it to exact IP and port
    clientSocket.settimeout(4) # Setting timeout to 4 - it may be long, but I want to be sure.
    
    connection = False
    i = 0
    while i<3 and connection==False:  # Cycle of connections - If server don't answer in 3 attempts, program will lead to exception.
        try:
            recieved, _ = clientSocket.recvfrom(4096)
        except timeout:
            i = i + 1
            continue
        connection = True


    if connection == False: # Program never reached connection.
        print("Server unreachable")
        sys.exit(3)

    if not("OK" in recieved.decode("utf-8")): # Received message isn't what we wanted.
        print(recieved.decode("utf-8"))
        sys.exit(4)

    adress = recieved.decode("utf-8") # Decoding message (maybe I shouldn't do this, but I did.)
    adress = adress.split("OK ")
    adress = adress[1].split(":")
    IP = adress[0].split(".") # Splitting IP to 4 numbers.
    if len(IP) != 4: # Ok, maybe no 4 numbers.
        print("Illegit IPv4 adress")
        sys.exit(2)

    for i in range(len(IP)): # Verification of IP adress
        if IP[i].isnumeric()==False:
            print("Received IP adress is not only from numerical letters.")
            sys.exit(4)
        if int(IP[i]) < 0 or int(IP[i])>255:
            print("Illegit IPv4 adress")
            sys.exit(2)

    if int(adress[1]) > 65535 or int(adress[1]) < 0: # Verification of port
        print("illegal port")
        sys.exit(2)
    if adress[1].isnumeric==False:
        print("Received port is not numeric.")
        sys.exit(4)
    return adress[0], adress[1]


def FSP(adress, port, surl): # FSP
    """
    Function downloads file from server.
    """
    surlProt = surl.split("://")
    hostName = surlProt[1].split("/", 1)
    message = "GET " + hostName[1] +" FSP/1.0\nHostname: "+hostName[0]+"\nAgent: xhudak03\n\n" # Creating message

    #print(message) # Prints message, which will be sent

    clientSocket = socket(AF_INET, SOCK_STREAM) # Creating socket
    clientSocket.connect((adress, int(port))) # Creating connection with server
    clientSocket.settimeout(4) # Timeout may be high, but I don't mind.
    clientSocket.send(message.encode()) # Sending message.
    connection = False
    i = 0
    while i<3 and connection==False: # Cycle trying to connect and receive a message.
        try:
            data = clientSocket.recv(4069) 
        except timeout: # Timeout occurs, try it maybe more times?
            i = i+1
            print("Server inactive")
            continue
        connection = True

    if connection == False: # Connection failed.
        sys.exit(3)

    decoded = data.decode("utf-8") # Decoding of first message - may be redundant or dangerous, but I dont mind.
    if "FSP/1.0 Success"!=decoded[0:15]: # No success here? Ayayay.
        errMessage = decoded.split("\r\n", 2)
        if len(errMessage) < 3: # Server is ***
            print("Wrong syntax of message from server")
            print("It was: " + decoded)
            sys.exit(4)
        if "FSP/1.0 Bad Request" in decoded: # Our request was weird.
            print(errMessage[0])
            print(errMessage[2])
            sys.exit(3)
        if "FSP/1.0 Not found" in decoded: # Our file does not exist.
            print(errMessage[0])
            print(errMessage[2])
            sys.exit(3)
        if "FSP/1.0 Server error": # Our server is ***
            print(errMessage[0])
            print(errMessage[2])
            sys.exit(3)
        else: # Our server is really ***
            print("Illegal error message from server")
            sys.exit(4)

    #print(decoded)
    length = 0
    if "Success" in decoded:
        if not("Length:" in decoded):
            print("Missing message Length from server.")
            sys.exit(3)
        text = decoded.split("Length:")
        text = text[1].split("\r\n\r\n", 1)
        length = int(text[0])
    else:
        sys.exit(3)  


    f = open(hostName[1].split("/")[-1], "wb") # Opening file for binary writing.
    f.write(text[1].encode()) # Writing first received message.
    while length >= 0: # Repeat until we don't have whole file. (We read length from first message.)
        i=0
        connection=False

        while i<3 and connection==False: # Again, we are trying to communicate more than in 1 attempt.
            try:
                fileRecv = clientSocket.recv(4096) 
                f.write(fileRecv)
            except timeout:
                i = i+1
                print("Server inactive")
                continue
            connection = True
        length = length - 4096
        if connection == False:
            print("Error during downloading from server occured.")
            sys.exit(3)
    # End of downloading of file

    f.close()
    clientSocket.close()
        
    

if not("-n" in sys.argv): # These arguments are mandatory
    print("Missing argument -n")
    sys.exit(2)
if not("-f" in sys.argv):
    print("Missing argument -f")
    sys.exit(2)

try: 
    opt, args = getopt.getopt(sys.argv[1:], "n:f:", []) # Inspired by official Python web documentation.
except getopt.GetoptError as err:
    print(err)
    sys.exit(-1)
nameserver = ""
surl = ""

for option, inp in opt: # Getting arguments from command line. 
    if option == "-n":
        nameserver = inp
    elif option == "-f":
        surl = inp
    else:
        assert False

split = nameserver.split(":") # Getting IP adress and port
IP = split[0]

if split[1].isnumeric==False:
    print("Port contains illegal characters.")
    sys.exit(2)

port = int(split[1])
splitted = IP.split(".")

if len(splitted) != 4: # Verifies IP.
    print("Illegit IPv4 adress")
    sys.exit(2)
for i in range(len(splitted)):
    if int(splitted[i]) < 0 or int(splitted[i])>255 or splitted[i].isnumeric==False:
        print("Illegit IPv4 adress")
        sys.exit(2)

if port > 65535 or port < 0: # Verifies port.
    print("Illegal port")
    sys.exit(3)

adress, porter = NSP(IP, port, surl) # Gets adress and port of nameserver. 

if surl[-1] == "*": # We want download all files from server/folder on server.

    hostname = surl.split("://")
    hostnameName = hostname[1].split("/", 1)
    if not(re.match("^([A-Za-z_.-])+$", hostnameName[0])): # Regex which verifies illegal characters in hostname.
        print("Illegal server name.")
        sys.exit(2)

    index = hostname[0]+"://"+hostnameName[0]+"/"+ "index" # downloading of index file
    FSP(adress, porter, index)
    f = open("index", "r") # Opening index file.
    files = f.readlines()
    f.close()
    for line in files: # Downloads files by index file.
        if hostnameName[1].split("*")[0] in line: # Only files with correct folder should be downloaded.
            FSP(adress, porter, hostname[0]+"://"+hostnameName[0]+"/"+line.rstrip("\n"))
else:
    FSP(adress, porter, surl) # We want download only one file

