<?php
// Author: David Hudak
// Subject: IPP
// Project: Parser

# function returns string without IPPcode21 comment (for example: "MOVE var const # Some funny comment"
# => "MOVE var const")
function deleteComment($line)
{
	if (strpos($line, "#")!==false){ // Looks for comment existence.
		$line = substr($line, 0, strpos($line, "#"));
	}
	return $line;
}

# Function returns true, if variable/label name starts with correct character (special symbols and all letters) 
function isStartingCharacter($character) 
{
	$specialChars = array( # Legal special starting characters
		"_",
		"-",
		"$",
		"&",
		"%",
		"*",
		"!",
		"?"
	);
	return in_array($character[0], $specialChars) or ctype_alpha($character[0]); // logical expression - special character or letter
}

	if($argc>1){ # There is input argument
		if($argc>2) // There are too much input arguments
		{
			exit(10);
		}
		if($argv[1]=="--help") // Input argument is correct - --help
		{
			echo "Hello, this program reads IPPcode21 file from standard input and writes it's encoded version in xml to standard output.\nHave a nice day.\n";
			exit(0);
		}
		exit(10);
	}	
	ini_set('display_errors', 'stderr'); // Changing error output
	$line = fgets(STDIN); // reads first line
	$line = deleteComment($line); //deletes comment
	$words = preg_split('/\s+/', $line, -1, PREG_SPLIT_NO_EMPTY); // splits string into words
	while(count($words)==0 or $words[0][0]=='#') // First line is empty or comment - we need .IPPcode21, read again, and again... until we get what we want.
	{
		$line = fgets(STDIN);
		if($line === false)
		{
			exit(0);
		}
		$line = deleteComment($line);
		$words = preg_split('/\s+/', $line, -1, PREG_SPLIT_NO_EMPTY);
	}
	if($words[0]!=".IPPcode21") // No .IPPcode21 at first code line.
	{
		exit(21);
	}
	if(count($words)>1) // Some garbagge after .IPPcode21 => not correct.
	{
		exit(21);
	}
	// Monster code
	$instructionArray = array( // Array which contains all supported instructions
		"MOVE" => array(2, "var", "nlabel"),  // Key value is instruction name, element of instructionArray is array
		// which contains number of instruction arguments and type of each instruction argument.
		// var means variable, type means type name, nlabel means anything but label (and type) and label means label... simple 
		"DEFVAR" => array(1, "var"),
		"TYPE" => array(1, "var"),
		"CREATEFRAME" => array(0),
		"PUSHFRAME" => array(0),
		"POPFRAME" => array(0),
		"CALL" => array(1, "label"),
		"RETURN" => array(0),
		"ADD" => array(3, "var", "nlabel", "nlabel"),
		"SUB" => array(3, "var", "nlabel", "nlabel"),
		"MUL" => array(3, "var", "nlabel", "nlabel"),
		"IDIV" => array(3, "var", "nlabel", "nlabel"),
		"LT" => array(3, "var", "nlabel", "nlabel"),
		"GT" => array(3, "var", "nlabel", "nlabel"),
		"QT" => array(3, "var", "nlabel", "nlabel"),
		"AND" => array(3, "var", "nlabel", "nlabel"), // In some IDEs word AND is blue. And I like it.
		"OR" => array(3, "var", "nlabel", "nlabel"),
		"NOT" => array(2, "var", "nlabel"),
		"INT2CHAR" => array(2, "var", "nlabel"),
		"STRI2INT" => array(2, "var", "nlabel"),
		"READ" => array(2, "var", "type"),
		"WRITE" => array(1, "nlabel"),
		"CONCAT" => array(3, "var", "nlabel", "nlabel"),
		"STRLEN" => array(2, "var", "nlabel"),
		"GETCHAR" => array(3, "var", "nlabel", "nlabel"),
		"SETCHAR" => array(3, "var", "nlabel", "nlabel"),
		"LABEL" => array(1, "label"),
		"JUMP" => array(1, "label"),
		"JUMPIFEQ" => array(3, "label", "nlabel", "nlabel"),
		"JUMPIFNEQ" => array(3, "label", "nlabel", "nlabel"),
		"EXIT" => array(1, "nlabel"),
		"DPRINT" => array(1, "nlabel"),
		"BREAK" => array(0),
		"POPS" => array(1, "var"),
		"PUSHS" => array(1, "nlabel")
	);

	
	$xw = xmlwriter_open_memory(); // Creates memory for XML 
	xmlwriter_set_indent($xw, 1);
	$res = xmlwriter_set_indent_string($xw, ' ');
	xmlwriter_start_document($xw, '1.0', 'UTF-8'); // Sets header which is compulsory from zadání.

	xmlwriter_start_element($xw, 'program'); // starts main element of XML file - program.

	xmlwriter_start_attribute($xw, 'language'); // Types element of program - language (which is obvious)
	xmlwriter_text($xw, 'IPPcode21');
	xmlwriter_end_attribute($xw);
	$order = 1;

	while (($line = fgets(STDIN)) !== false) { // Main cycle of program - reads lines until it has some.
		$line = deleteComment($line); // Deletes comment from line, obvious.
		$words = preg_split('/\s+/', $line, -1, PREG_SPLIT_NO_EMPTY); // Divides line to words.

		if(count($words)==0)continue; // Empty line is generally useless.
		xmlwriter_start_element($xw, 'instruction'); // Start of writing of instruction
		xmlwriter_start_attribute($xw, 'order');
		xmlwriter_text($xw, $order);
		xmlwriter_end_attribute($xw); // Some formalities.
		xmlwriter_start_attribute($xw, 'opcode');
		if(array_key_exists(strtoupper($words[0]), $instructionArray)===false) // Does instruction exist? If not, exit(22)
		{
			echo "I don't know instruction $words[0]";
			exit(22);
		}
		if(count($words)-1!=$instructionArray[strtoupper($words[0])][0]) // Has instruction correct count of arguments? If not, exit(23)
		{
			echo "Different count of arguments.";
			exit(23);
		}
		xmlwriter_text($xw, strtoupper($words[0]));
		xmlwriter_end_attribute($xw);
		// for starts at 1, because $i=0 is instruction name
		for($i=1; $i<count($words); $i++) // For cycle which writes instruction arguments.
		{ 
			xmlwriter_start_element($xw, "arg$i"); // Start of new argument
			xmlwriter_start_attribute($xw, 'type'); // formality
			if (strpos($words[$i], "@")!==false) // if which decides if argument is label or type, or variable or const. 
			{ // variable or const branch.
				$var = false;
				$prefix=substr($words[$i], 0, strpos($words[$i], "@")); // Division to two strings - prefix contains left part of argument and postfix right part
				$postfix=substr($words[$i], strpos($words[$i], "@")+1); // "left part"@"right part"
				if(($prefix=="GF" or $prefix=="LF" or $prefix=="TF") and  // Prefix is variable prefix
				( $instructionArray[strtoupper($words[0])][$i] == "var" or $instructionArray[strtoupper($words[0])][$i] == "nlabel")) // and variable is expected!
				{ 
					$var = true; // later, we need to decide if here was variable or const (we write GF@name with prefix, but constants, for example int@10, without prefix)
					if(!isStartingCharacter($postfix)) // Variable name is correct :)
					{
						exit(23); // Oh, variable name is not correct :(
					}
					xmlwriter_text($xw, "var");
				}
				elseif($prefix=="int" and ($instructionArray[strtoupper($words[0])][$i] == "nlabel")) // We found int and we want int.
				{
					xmlwriter_text($xw, "int");
					if(strlen($postfix)==0)
					{
						echo "Int shouldn't be empty";
						exit(23);
					}
				}
				elseif($prefix=="string" and ($instructionArray[strtoupper($words[0])][$i] == "nlabel")) // We found string and we want string.
				{
					$lastPos=0;
					while(strpos($postfix, "\\", $lastPos)!==false) // Controlling escape sequencies
					{
						if(is_numeric(substr($postfix, strpos($postfix, "\\", $lastPos)+1, 3))===false) // After \ have to be 3 digits.
						{
							echo "Escape sequence isn't 3 digits";
							echo substr($postfix, strpos($postfix, "\\", $lastPos)+1, 3);
							exit(23);
						}
						$lastPos=strpos($postfix, "\\", $lastPos)+1; 
					} // End of controlling escape sequencies.
					
					xmlwriter_text($xw, "string");
				}
				elseif($prefix=="bool" and ($instructionArray[strtoupper($words[0])][$i] == "nlabel")) // We found bool and we want bool.
				{
					if($postfix!="true" and $postfix!="false") // boolean value isnt true or false => weird.
					{
						echo "Boolean should contain boolean value - contains $postfix\n";
						exit(23);
					}
					xmlwriter_text($xw, "bool");
				}
				elseif($prefix=="nil" and ($instructionArray[strtoupper($words[0])][$i] == "nlabel")) // we found nil and we want nil.
				{
					if($postfix!="nil") // Value of nil type should be nil.
					{
						echo "Nil should be nil@nil, not nil@$postfix\n";
						exit(23);
					}
					xmlwriter_text($xw, "nil");
				}
				else // We dont know wt... what we found, but we know, it isnt correct.
				{
					echo "Argument isn't correct.\n";
					exit(23);
				}
				xmlwriter_end_attribute($xw);
				if($var) // argument was variable => we need to print alse prefix (frame)
				{
					xmlwriter_text($xw, "$prefix@");
				}
				xmlwriter_text($xw, $postfix);
			} // End of variable or const branch.
			else
			{ // We found label or type.
				if(($instructionArray[strtoupper($words[0])][$i] == "type") or ($instructionArray[strtoupper($words[0])][$i] == "label")) // We found label or type 
				{
					if($instructionArray[strtoupper($words[0])][$i] == "type" and // We want type
						($words[$i]=="int" or $words[$i]=="string" or $words[$i]=="bool")) // and we found some type!
					{
						xmlwriter_text($xw, "type");
					}
					elseif($instructionArray[strtoupper($words[0])][$i] == "label") // We need label and... everything is label
					{
						xmlwriter_text($xw, "label");
					}
					else // We didn't want label or type, or type isn't correct.
					{
						echo "illegal parameter\n";
						exit(23);
					}

					if(!isStartingCharacter($words[$i][0]))
					{
						exit(23); // Oh no, name of label starts with some... something we dont like :)
					}
					xmlwriter_end_attribute($xw);
					xmlwriter_text($xw, $words[$i]);
				}
				else // We wanted nlabel or var, but we gotnt.
				{
					echo "illegal parameter - should contain @\n";
					exit(23);
				}
				
			}
			xmlwriter_end_element($xw);
		} // Ends for cycle which writes arguments of instructions.
		xmlwriter_end_element($xw);
		$order++;

	} // end of while line reading cycle
	
	xmlwriter_end_element($xw); // Ends program element
	xmlwriter_end_document($xw); // Ends document. Formality.
	
	echo xmlwriter_output_memory($xw);
?>