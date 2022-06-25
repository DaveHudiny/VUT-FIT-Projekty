/**
* File: main.cpp
* Author: David Hudak
* Description: header file for subject ISA project (option 3, reverse engineered client).
**/
#include "main.hpp"

enum mis{OK, EARG, EARGM, ECON, ECOM};
using namespace std;


enum machineFinita{ID, FROM, SPECIALFROM, SPACE, SUBJECT, SPECIALSUBJECT, START, END}; // Actually useless

int makeConnection(int sockfd, struct addrinfo *serv_addr, char* addr, char* port)
{
    int err;
    string textErr = ""; 
    if ( (err= connect(sockfd, serv_addr->ai_addr, serv_addr->ai_addrlen)) < 0) // create connection to server
    {
        cout << "tcp-connect: connection failed" << endl;
        cout << "  hostname: " << addr << endl;
        cout << "  port number: " << port << endl;
        if(errno == ECONNREFUSED) textErr = "Connection refused"; 
        if(errno == 113) textErr = "No route to host";
        if(errno == 110) textErr = "Connection timed out";
        cout << "  system error: " << textErr << "; errno="<< errno << endl;

        return ECON;
    }
    return OK;
}


void registerM(int sock, string name, string pass)
{
    pass = base64_encode(pass); //  taken from https://github.com/ReneNyffenegger/cpp-base64
    string mess = "(register \"" + name + "\" \"" + pass + "\")";
    char buffer[1024] = {0};
    send(sock , mess.c_str(), mess.size(), 0); // TCP
    int valread = read(sock, buffer, 1023); // TCP
    if(valread<0)
    {
        cout << "client: error during accepting packets";
        return;
    }
    string succ = string(buffer);
    if(succ.substr(0, strlen("(ok \"registered user")) == "(ok \"registered user") // Message parsing
    {
        cout << "SUCCESS: registered user " + name << endl;
    }
    else
    {
        cout << "ERROR: user already registered" << endl;
    }
}

void loginM(int sock, string name, string pass)
{
    pass = base64_encode(pass); //  taken from https://github.com/ReneNyffenegger/cpp-base64
    string mess = "(login \"" + name + "\" \"" + pass + "\")";
    char buffer[1024] = {0};
    send(sock , mess.c_str(), mess.size(), 0); // TCP
    int valread = read(sock, buffer, 1023); // TCP
    if(valread<0)
    {
        cout << "client: error during accepting packets";
        return;
    }
    string succ = string(buffer); // Retyping to string
    string token;
    ofstream myfile;
    int sizeOf;
    if(succ.substr(0, strlen("(ok \"user logged in")) == "(ok \"user logged in") // Message parsing
    {
        myfile.open("login-token");
        cout << "SUCCESS: user logged in"<< endl;
        sizeOf = sizeof("(ok \"user logged in\"");
        myfile << succ.substr(sizeOf, succ.size()-sizeOf-1);
        myfile.close();
        
    }
    else if(succ.substr(0, strlen("(err \"incorrect password")) == "(err \"incorrect password")
    {
        cout << "ERROR: incorrect password" << endl;
    }
    else
    {
        cout << "ERROR: unknown user" << endl;
    }
}

string loadToken()
{
    ifstream myfile;
    string token;
    myfile.open("login-token"); // Implicit name by zadani
    if(!myfile.is_open())
    {
        cout << "Not logged in" << endl;
        token.clear();
        return token;
    }
    getline(myfile, token); // Token is always on single line
    myfile.close();
    return token;
}

string loadMessage(string succ, int sock)
{
    int valread;
    char buffer[1024] = "";
    while((valread = read(sock, buffer, 1023)) > 0) // Reading more and more packets and adding them to message. Sometimes there are no waiting packets on start (when the message is short)
    {   
        if(valread<0)
        {
            cout << "client: error during accepting packets";
            return succ;
        }
        //buffer[1023] = '\0';
        //cout << buffer << endl << string(buffer) << endl;
        succ += string(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    return succ;
}

void list(int sock)
{
    string token = loadToken(); // Loading token for adding to message
    if(empty(token))
    {
        return;
    }
    string mess = "(list " + token + ")"; // Token improved message
    char buffer[1024] = {0};
    send(sock , mess.c_str(), mess.size(), 0); // TCP
    int valread;
    valread = read(sock, buffer, 1023); // TCP
    if(valread<0)
    {
        cout << "client: error during accepting packets";
        return;
    }
    string succ = string(buffer);
    if(succ.substr(0, strlen("(ok (")) == "(ok (")
    {
        succ.erase(0, size("(ok (")-1);
        succ = loadMessage(succ, sock); // Loading rest of packets waiting on input socket
    }
    else
    {
        cout << "ERROR: incorrect login token" << endl;
        return;
    }
    string message[3] = {"", "", ""}; // Space for parsing message - on first index number of message, second source of message and third is subject of message
    int i = 0;
    int j = 0;
    bool leftFound=false;
    bool rightFound = false;
    bool specialChar=false;
    bool inside=false;
    cout << "SUCCESS:" << endl;
    while(succ[i] != '\0') // Finite state machina
    {
        if(!specialChar and !leftFound and succ[i] == '(' and !inside) // State start of message
        {
            rightFound = false;
            j = 0;
            i++;
            inside = true;
            continue;
        }
        if(j == 0 and succ[i] == '\"') // State source
        {
            j = 1;
            inside = true;
            i++;
            continue;
        }
        if(succ[i] == '\\' and !specialChar) // state for special characters
        {
            specialChar = true;
            i++;
            continue;
        }
        if(specialChar) // state for printing special character
        {
            message[j]+=succ[i];
            i++;
            specialChar = false;
            continue;
        }
        if(succ[i] == '\"' and !specialChar and j == 1 and inside == true) // state for end of source message
        {
            inside = false;
            i++;
            continue;
        }
        if(succ[i] == '\"' and !specialChar and j == 1 and inside == false) // state for start of body message
        {
            inside = true;
            j = 2;
            i++;
            continue; 
        }
        if(succ[i] == '\"' and !specialChar and j == 2 and inside == true) // state for end of body message
        {
            inside = false;
            i++;
            continue; 
        }
        if(inside) // state for simply copying characters to message strings.
        {
            if(j == 0 and succ[i] == ' ')
            {
                i++;
                continue;
            }
            message[j]+=succ[i];
            i++;
            continue;
        }
        if(!inside and succ[i] == ')' and !rightFound) // State for end of message and printing of message.
        {
            if(j == 0)
            {
                i++;
                continue;
            }
            rightFound = true;
            cout << message[0] << ":" << endl;
            cout << "  From: " << message[1] << endl;
            cout << "  Subject: " << message[2] << endl;
            for(int k = 0; k<3; k++)
            {
                message[k]= "";
            }
            i++;
            continue;
        }
        i++; // For spaces and some other stuff
    }
}

string specialChars = "\a\b\f\n\r\t\v\'\"\?\\"; // String of characters which should be identified as special (escape sequenced)

char fromSpecialTranslator(char character)
{
    switch(character)
        {
            case '\a':
                return 'a';
                break;
            case '\b':
                return 'b';
                break;
            case '\f':
                return 'f';
                break;
            case '\n':
                return 'n';
                break;
            case '\r':
                return 'r';
                break;
            case '\t':
                return 't';
                break;
            case '\v':
                return 'v';
                break;
            default:
                return character;
                break; 
        }
}

char toSpecialTranslator(char character)
{
    switch(character)
        {
            case 'a':
                return '\a';
                break;
            case 'b':
                return '\b';
                break;
            case 'f':
                return '\f';
                break;
            case 'n':
                return '\n';
                break;
            case 'r':
                return '\r';
                break;
            case 't':
                return '\t';
                break;
            case 'v':
                return '\v';
                break;
            default:
                return character;
                break; 
        }
}

string makeSpecial(string notSpecial)
{
    string special = "";
    long unsigned int i = 0;
    while(i < size(notSpecial) and notSpecial[i] != '\0')
    {
        if(specialChars.find(notSpecial[i])!=string::npos) // Character found in specialChars string -> rewrite it
        {
            special += '\\';
        }
        special += fromSpecialTranslator(notSpecial[i]);
        
        i++;
    }
    return special;
}

void send(int sock, string recipient, string subject, string body)
{
    string token = loadToken();
    
    if(empty(token))
    {
        return;
    }

    recipient = makeSpecial(recipient); // Making message parts ideal for transmitting (adding '\\' before special characters as '\"')
    subject = makeSpecial(subject);
    body = makeSpecial(body);

    string mess = "(send "+ token + " \"" + recipient + "\" \"" + subject + "\" \"" + body + "\")"; // concatening to create one beautiful message
    char buffer[1024] = {0};
    send(sock , mess.c_str(), mess.size(), 0); // TCP
    int valread = read(sock, buffer, 1023); // TCP
    if(valread<0)
    {
        cout << "client: error during accepting packets";
        return;
    }
    string succ = string(buffer);
    if(succ.substr(0, strlen("(ok ")) == "(ok ") // Answer parsing
    {
        cout << "SUCCESS: message sent" << endl;
    }
    else
    {
        if(succ.substr(0, strlen("(err \"unknown")) == "(err \"unknown")
        {
            cout << "ERROR: unknown recipient" << endl;
        }
        else
        {
            cout << "ERROR: incorrect login token" << endl;
        }
        return;
    }
}

void fetch(int sock, string id) 
{
    string token = loadToken();
    if(empty(token))
    {
        return;
    }
    char* p;
    string mess;
    strtol(id.c_str(), &p, 10);
    if (*p) {
        cout << "ERROR: id " + id + " is not a number" << endl;
        return;
    }
    else {
        mess = "(fetch "+ token + " " + id + ")"; // Creating message from keyword, token string and id of message to download
    }
    
    char buffer[1024] = {0};
    send(sock , mess.c_str(), mess.size(), 0); // TCP
    int valread;
    valread = read(sock, buffer, 1023); // TCp
    if(valread<0)
    {
        cout << "client: error during accepting packets";
        return;
    }
    string succ = string(buffer);
    if(succ.substr(0, strlen("(ok (")) == "(ok (")
    {
        succ.erase(0, strlen("(ok ("));
        succ = loadMessage(succ, sock); // Loading rest of message
    }
    else
    {
        if(succ.substr(0, strlen("(err \"message id")) == "(err \"message id")
        {
            cout << "ERROR: message id not found" << endl;
        }
        else if(succ.substr(0, strlen("(err \"wrong arguments")) == "(err \"wrong arguments")
        {
            cout << "ERROR: wrong arguments" << endl;
        }
        else
        {
            cout << "ERROR: incorrect login token" << endl;
        }
        return;
    }
    string message[3] = {"", "", ""};
    int i = 0;
    int j = 0;
    bool specialChar=false;
    bool inside=false;
    while(succ[i] != '\0') // Finite state machina
    {
        if(!inside and succ[i] == '\"') // state for start of source
        {
            inside = true;
            i++;
            continue;
        }
        if(inside and succ[i] == '\\' and !specialChar) // states for special characters
        {
            specialChar = true;
            i++;
            continue;
        }
        if(inside and specialChar)
        {
            specialChar = false;
            message[j] += toSpecialTranslator(succ[i]);
            i++;
            continue;
        }
        if(inside and succ [i] == '\"') // state for leaving of actual part of message state
        {
            j++;
            if(j > 2)
            {
                break;
            }
            inside = false;
            i++;
            continue;
        }
        if(inside) // State for adding character
        {
            message[j] += succ[i];
            i++;
            continue;
        }
        if(succ[i] == ')') // end of message
        {
            break;
        }
        i++; // some free spaces

    }
    cout << "SUCCESS:" << endl << endl; // Formating message
    cout << "From: " << message[0] << endl;
    cout << "Subject: " << message [1] << endl << endl;
    cout << message[2];
} 

void logout(int sock)
{
    ifstream myfile;
    myfile.open("login-token");
    if(!myfile.is_open())
    {
        cout << "Not logged in" << endl;
        return;
    }
    string token;
    getline(myfile, token);
    remove("login-token"); // Removing file with token = local unlogging
    myfile.close();
    string mess = "(logout " + token+ ")";
    char buffer[1024] = {0};
    send(sock , mess.c_str(), mess.size(), 0); // TCP
    int valread = read(sock, buffer, 1023); // TCP
    if(valread<0)
    {
        cout << "client: error during accepting packets";
        return;
    }
    string succ = string(buffer);
    if(succ.substr(0, strlen("(ok \"logged out")) == "(ok \"logged out")
    {
        cout << "SUCCESS: logged out" << endl;
    }
    else
    {
        cout << "ERROR: incorrect login token" << endl;
    }
}

void help()
{
    cout << "usage: client [ <option> ... ] <command> [<args>] ...";
    cout << "\n\n<option> is one of";
    cout << "\n\n  -a <addr>, --address <addr>";
    cout << "\n     Server hostname or address to connect to";
    cout << "\n  -p <port>, --port <port>";
    cout << "\n     Server port to connect to\n  --help, -h\n     Show this help\n  --";
    cout << "\n     Do not treat any remaining argument as a switch (at this level)";
    cout << "\n\n Multiple single-letter switches can be combined after";
    cout << "\n one `-`. For example, `-h-` is the same as `-h --`.";
    cout << "\n Supported commands:";
    cout << "\n   register <username> <password>";
    cout << "\n   login <username> <password>";
    cout << "\n   list";
    cout << "\n   send <recipient> <subject> <body>";
    cout << "\n   fetch <id>";
    cout << "\n   logout\n";
}

int main(int argc, char **argv)
{
    struct option long_options[] = { // Long options by zadání.
        {"address", required_argument, 0, 'a'},
        {"port", required_argument, 0, 'p'},
        {"help", no_argument, 0, 'h'},
        {0, 0,  0,  0} // Last line of longopts, it's mandatory.
    };
    struct addrinfo hints;
    struct addrinfo *result;

    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;

    int option_index; // Some useful declarations.
    int c;
    int ok = 0;
    char port[10] = "32323"; // default values
    char address[128] = "localhost";
    bool overflow = false;
    while((c = getopt_long(argc, argv, "+:a:p:h", long_options, &option_index)) != -1)
    {
        switch(c) // Long switch for all options. Please, read https://linux.die.net/man/3/getopt_long if you need more informations for how it works.
        {
        case 'p': // Port option
            strcpy(port, optarg);
            if(atol(optarg) < 0 or atol(optarg) > 65535) // You know, what I am doing, doesn't it? (Ports can be only values between <0, 65535>.)
            {
                cout << "client: Invalid port value\n" << endl;
                return EARGM;
            }
            break;
        case 'a': // TCP, UDP options.
            strcpy(address, optarg);
            break;
        case 'h':
            help();
            break;
        case ':':
            switch(optopt)
            {
                case 'a':
                    cout << "client: the \"-a\" option needs 1 argument, but 0 provided\n";
                    return EARG;
                case 'p':
                    cout << "client: the \"-p\" option needs 1 argument, but 0 provided\n";
                    return EARG;
            }
        default:
            cout << "client: unknown switch: " << argv[optind-1] << endl;
            return EARG;
        }
        if(overflow)
        {
            break;
        }
    }
    memset(&hints, 0, sizeof hints);
    ok = getaddrinfo(address, port, &hints, &result); // Converting informations to some understandable form
    //ok = inet_pton(AF_INET, optarg, &(serv_addr.sin_addr));
    if(ok!=0)
    {
        cout << gai_strerror(ok) << endl;
        return EARGM;
    }
    int sockfd;
    sockfd = socket(result->ai_family, result->ai_socktype, 0); // TCP, BSD socket
    if(sockfd<0)
    {
        printf("Error 404");
        return 404;
    }
    if(optind < argc)
    {
        string first(argv[optind]); // Parsing commands, actually nothing interesting
        if(first == "list" || first == "logout")
        {
            if(optind + 1 != argc)
            {
                cout << first + " " << endl;
                return ECOM;
            }
            if (makeConnection(sockfd, result, address, port) != OK) return ECON; // It is important to make connection right there!
            if(first == "list")
            {
                list(sockfd);
            }
            else
            {
                logout(sockfd);
            }
        }
        else if(first == "fetch")
        {
            if(optind + 2 != argc)
            {
                cout << first << " <id>" << endl;
                return ECOM;
            }
            if (makeConnection(sockfd, result, address, port) != OK) return ECON;
            fetch(sockfd, argv[optind+1]);
        }
        else if(first == "login" || first == "register")
        {
            if(optind + 3 != argc)
            {
                cout << first << " <username> <password>" << endl;
                return ECOM;
            }
            if (makeConnection(sockfd, result, address, port) != OK) return ECON;
            if(first == "login")
            {
                loginM(sockfd, argv[optind+1], argv[optind+2]);
            }
            else
            {
                registerM(sockfd, argv[optind+1], argv[optind+2]);
            }
        }
        else if(first == "send")
        {
            if(optind + 4 != argc)
            {
                cout << first << " <recipient> <subject> <body>" << endl;
                return ECOM;
            }
            if (makeConnection(sockfd, result, address, port) != OK) return ECON;
            send(sockfd, argv[optind+1], argv[optind+2], argv[optind+3]);
        }
        else
        {
            cout << "unknown command\n";
            return ECOM;
        }
        
    }
    else
    {
        cout << "client: expects <command> [<args>] ... on the command line, given 0 arguments\n";
        return ECOM; 
    }
    return 0;
}