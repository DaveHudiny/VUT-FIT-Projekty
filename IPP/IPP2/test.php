<?php
// Author=David_Hudák
// Subject=IPP
// Project=test.php

// Auxilary class for test results
class MyResult{
    public $name; // Name of test
    public $correct; // Result of test
    public $info; // Info for test (if failed)
}

function createStarting($HTML) // Creating head part of HTML file.
{
    $HTML .= "<html>\n\n";
    $HTML .= "<head><style>
    table, th, td {
      border: 1px solid black;
    }
    </style>
    \n<title>Tests and more tests</title>\n</head>\n\n";
    $HTML .= "<body>\n\n";
    return $HTML;
}
function createEnding($HTML) // Creating end part of HTML
{
    $HTML .= "</body>\n\n";
    $HTML .= "</html>\n\n";
    return $HTML;
}
function createTitle($HTML, $size, $mes) // Creates HTML with size $size and with text in $mes
{
    if($size>6 or $size<1)
    {
        return false;
    }
    $HTML .= "\n<h$size >$mes </h$size >\n";
    return $HTML;
}

function getAllFiles($dir, $recursive) // Goes through folder which is needed. If $recursive == true, option recursive is turned on.
{
    $files = array(); // Scaning works with dynamic array - files are added when found.
    $actualFiles = scandir($dir);
    foreach($actualFiles as $key => $currentFile)
    {
        if($recursive and is_dir($dir. DIRECTORY_SEPARATOR .$currentFile) and $currentFile!=".." and $currentFile!=".") // Reads directories if recursive option is true
        {
            $recursiveFiles = getAllFiles($dir. DIRECTORY_SEPARATOR .$currentFile, $recursive);
            $files=array_merge($files, $recursiveFiles);
        }
        elseif(!is_dir($dir. DIRECTORY_SEPARATOR .$currentFile))
        {
            array_push($files, $dir. DIRECTORY_SEPARATOR .$currentFile); // Add file to files
        }
    }
    return $files;
}

$correct = 0;
$correctInt = 0;
$correctParse = 0;
$all = 0;
$HTML = "";
$HTML = createStarting($HTML);

$HTML = createTitle($HTML, 1, "Welcome to my test suite!"); // Some formalities.
$HTML = createTitle($HTML, 4, "Author: David Hudak");
$HTML = createTitle($HTML, 4, "Subject: IPP");



if($argc>1) // Trying read --help
{
    if($argv[1]=="--help")
    {
        if($argc>2)
        {
            echo "Wrong count of parameters combined with --help";
            exit(-1);
        }
        echo "Do the stuff...\n"; // Should be more complex :)
        exit(0);
    }
}

$longopts = array( // Saying which arguments for program are possible
    "directory:",
    "recursive",
    "parse-script:",
    "int-script:",
    "parse-only",
    "int-only",
    "jexamxml:",
    "jexamcfg:"
);

$options = getopt("", $longopts); // Getopt - If you want to know more, read documentation on https://www.php.net/manual/en/function.getopt.php
if($argc-1!=count($options))
{
    echo "Missing values for parameters or wrong arguments.\n";
    exit(41);
}

try{
    $path=getcwd(); // processing of argument
} catch (Exception $e){
    exit(11);
}
if(array_key_exists("directory", $options))
{
    $path = $options["directory"];
}
$recursive = false; // processing of argument
if(array_key_exists("recursive", $options))
{
    $recursive = true;
}
$parser = "./parse.php"; // processing of argument
if(array_key_exists("parse-script", $options))
{
    $parser = $options["parse-script"];
}
$script = "interpret.py"; // processing of argument
if(array_key_exists("int-script", $options))
{
    $script = $options["int-script"];
}
$onlyParse = false; // processing of argument
if(array_key_exists("parse-only", $options))
{
    if(array_key_exists("int-script", $options) or array_key_exists("int-only", $options))
    {
        print("It have to be interpret or parse only, not both.\n");
        exit(41);
    }
    $onlyParse = true;
}
$onlyScript = false; // processing of argument
if(array_key_exists("int-only", $options))
{
    if(array_key_exists("parse-script", $options) or array_key_exists("parse-only", $options))
    {
        print("It have to be interpret or parse only, not both.\n");
        exit(41);
    }
    $onlyScript = true;
}
$jar = "/pub/courses/ipp/jexamxml/jexamxml.jar"; // processing of argument
if(array_key_exists("jexamxml", $options))
{
    $jar = $options["jexamxml"];
}
$jarOpt = "/pub/courses/ipp/jexamxml/options"; // processing of argument
if(array_key_exists("jexamcfg", $options))
{
    $jarOpt = $options["jexamcfg"];
}

$files = getAllFiles($path, $recursive);
$srcFiles = preg_grep('/.*src\z/ ', $files); // Finding all .src files
$outFiles = preg_grep('/.*out\z/ ', $files); // Finding all .out files
$rcFiles = preg_grep('/.*rc\z/ ', $files); // Finding all .rc files
$testsDone = array();
$testsDoneInt = array();
if($onlyParse) // Activated tests for parser
{
    foreach($srcFiles as $key => $value)
    {
        $all++;
        $directory = pathinfo ($value, PATHINFO_DIRNAME); // Separating some useful info.
        $name = pathinfo($value, PATHINFO_FILENAME);
        $superPath = $directory. DIRECTORY_SEPARATOR.$name; // getting only myFile instead of myFile.src
        $info = "";
        /*echo $superPath;
        echo "\n";*/
        //echo "php7.4 $parser < $value > $superPath.my; java -jar $jar --options=$jarOpt $superPath.out $superPath.my";
        shell_exec("php7.4 $parser < $value > $superPath.my 2>log.txt; echo -n >$superPath.resv $?; # Turn on parser
        java -jar $jar $superPath.out $superPath.my $jarOpt; # Comparing by java jexamxml
        echo -n >$superPath.myr $? # Printing result of comparing by Java tool"); 
        $result = shell_exec("diff $superPath.resv $superPath.rc"); // Are exit values same?
        $correctos = false;
        if (strlen($result)==0)
        { // Output values are same
            if(shell_exec("cat $superPath.resv")=="0\n")
            { // Exit value was - test ended with file parsed value.
                if(shell_exec("cat $superPath.myr")=="0\n")
                { // Outputs are same
                    $correctos = true;
                }
                else
                { // Outputs are not same
                    $info .= "Output files differ";
                    $correctos = false;
                }
            }
            else
            { // Exit value was correct and file havent been parsed - it's simply correct.
                $correctos = true;
            }
        }
        else
        { // Output values are not same.
            $info .= "Exit values differ - expected: ";
            $info .= shell_exec("cat $superPath.rc");
            $info .= " got: ";
            $info .= shell_exec("cat $superPath.resv");
        }

        $resultStruct = new MyResult(); // Creating of object with results - important for printing results later.
        $resultStruct->name=$superPath;
        if($correctos === false)
        { // Test was not correct
            $resultStruct->correct=false;
            
        }
        else
        { // Test was correct
            $resultStruct->correct=true;
            $correctParse++;
        }
        $resultStruct->info = $info;
        array_push($testsDone, $resultStruct);
    }
    $HTML = createTitle($HTML, 4, "Parse tests passed: $correctParse"); // Some formalities for HTML file.
    $HTML = createTitle($HTML, 4, "Number of tests: $all");
    $HTML = createTitle($HTML, 2, "Parse tests: ");
}

if($onlyScript)
{
    foreach($srcFiles as $key => $value)
    {
        $all++;
        $directory = pathinfo ($value, PATHINFO_DIRNAME);
        $name = pathinfo($value, PATHINFO_FILENAME);
        $superPath = $directory. DIRECTORY_SEPARATOR.$name;
        $info = "";
        /*echo $superPath;
        echo "\n";*/
        //echo "php7.4 $parser < $value > $superPath.my; java -jar $jar --options=$jarOpt $superPath.out $superPath.my";
        shell_exec("if [ -f \"$superPath.in\" ]; then # Checking if files.in exist
                :
            else
                touch $superPath.in
            fi"
        );
        shell_exec("if [ -f \"$superPath.out\" ]; then # Checking if files.out exist
                :
            else
                touch $superPath.out
            fi"
        );
        shell_exec("python3.8 $script --source=$superPath.src --input=$superPath.in > $superPath.my 2>log.txt;  # Try to interpret code.
        echo -n >$superPath.resv $?;diff --ignore-all-space $superPath.out $superPath.my; 
        echo -n > $superPath.myr $? # Echo result, compare outputs, print result of diff of outputs");
        $result = shell_exec("diff --ignore-all-space $superPath.resv $superPath.rc");
        $correctos = false;
        if (strlen($result)==0)
        { // Exit values were same
            if(shell_exec("cat $superPath.resv")=="0\n")
            { // Exit values were 0, program was interpreted.
                if(shell_exec("cat $superPath.myr")=="0\n")
                { // Program was not interpreted correctly
                    $correctos = true;
                }
                else
                {
                    $correctos = false;
                    $info .= "Output files differ.";
                }
            }
            else
            { // Program have not been interpreted, no other controls are needed.
                $correctos = true;
            }
        }
        else
        { // Exit values were not same
            $info .= "Exit values differ - expected: ";
            $info .= shell_exec("cat $superPath.rc");
            $info .= " got: ";
            $info .= shell_exec("cat $superPath.resv");
        }

        $resultStruct = new MyResult(); // Same as in parse-only option before.
        $resultStruct->name=$superPath;
        if($correctos === false)
        {
            $resultStruct->correct=false;
        }
        else
        {
            $resultStruct->correct=true;
            $correctInt++;
        }
        $resultStruct->info = $info;
        array_push($testsDone, $resultStruct);
    }
    $HTML = createTitle($HTML, 4, "Interpret tests passed: $correctInt"); // Formalities
    $HTML = createTitle($HTML, 4, "Number of tests: $all");
    $HTML = createTitle($HTML, 2, "Interpret tests: ");
}

if(!$onlyParse and !$onlyScript)
{
    foreach($srcFiles as $key => $value)
    {
        $all++;
        $directory = pathinfo ($value, PATHINFO_DIRNAME);
        $name = pathinfo($value, PATHINFO_FILENAME);
        $superPath = $directory. DIRECTORY_SEPARATOR.$name;
        $info = "";
        /*echo $superPath;
        echo "\n";*/
        //echo "php7.4 $parser < $value > $superPath.my; java -jar $jar --options=$jarOpt $superPath.out $superPath.my";
        //Creating missing files.
        shell_exec("if [ -f \"$superPath.in\" ]; then 
                :
            else
                touch $superPath.in
            fi"
        );
        shell_exec("if [ -f \"$superPath.out\" ]; then
                :
            else
                touch $superPath.out
            fi"
        );
        shell_exec("if [ -f \"$superPath.rc\" ]; then
                :
            else
                echo -n > $superPath.rc 0
            fi"
        );
        
        shell_exec("if php7.4 $parser < $value > $superPath.xml 2> $superPath.log; then #Try to parse file
                        python3.8 $script --source=$superPath.xml --input=$superPath.in > $superPath.my 2> $superPath.log2; # Interpret xml code
                        echo -n > $superPath.resv $?; # Exit code
                        diff --ignore-all-space $superPath.out $superPath.my; echo -n > $superPath.myr $? # Comparing outputs.
                    else
                        echo -n > $superPath.resv $?; # File have not been parsed, print error code.
                    fi"
            );
        
        $correctos = false;

        $resultInt = shell_exec("diff --ignore-all-space $superPath.resv $superPath.rc"); // Again the same stuff, read comments before.
        if(strlen($resultInt) == 0)
        {
            if(shell_exec("cat $superPath.resv")=="0\n")
            { // Exit value zero, program interpreted...
                if(shell_exec("cat $superPath.myr")=="0\n")
                {
                    $correctos = true;
                }
                else
                {
                    $correctos = false;
                    $info .= "Output files differ";
                }
            }
            else
            {
                $correctos = true;
            }
        }
        else
        {
            $info .= "Exit values differ - expected: ";
            $info .= shell_exec("cat $superPath.rc");
            $info .= " got: ";
            $info .= shell_exec("cat $superPath.resv");
        }
        

        $resultStruct = new MyResult(); // Again same structure.
        $resultStruct->name=$superPath;
        if($correctos === false)
        {
            $resultStruct->correct=false;
        }
        else
        {
            $resultStruct->correct=true;
            $correctInt++;
        }
        $resultStruct->info = $info;
        array_push($testsDone, $resultStruct);
    }
    $HTML = createTitle($HTML, 4, "Both tests passed: $correctInt"); // Formalities
    $HTML = createTitle($HTML, 4, "Number of tests: $all");
    $HTML = createTitle($HTML, 2, "Both tests: ");
}

$HTML .= "\n<table>"; // Creating HTML table
$HTML .= "\n<tr>";
$HTML .= "\n<th>Name</th>"; // Some headings (bold)
$HTML .= "\n<th>Test result</th>";
$HTML .= "\n<th>Info (if failed)</th>";
$HTML .= "\n</tr>";
foreach($testsDone as $key => $value)
{ // Goes through array of objects.
    $HTML .= "\n<tr>";
    $HTML .= "\n<td>$value->name</td>";
    if($value->correct) 
    { // Found correct test -> print Success :)
        
        $HTML .= "\n<td style=\"color:green;\">Success</td>";
        $HTML .= "\n<td>Correct</td>";
    }
    else
    { // Found failed test -> print Failed :(
        $HTML .= "\n<td style=\"color:red;\">Failed</td>";
        $HTML .= "\n<td>$value->info</td>";
    }
    $HTML .= "\n</tr>";
}
$HTML .= "\n</table>";

$HTML = createEnding($HTML); // Some ending for a good story.
echo $HTML;
?>
