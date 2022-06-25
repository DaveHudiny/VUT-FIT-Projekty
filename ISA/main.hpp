/**
* File: main.hpp
* Author: David Hudak
* Description: header file for subject ISA project (option 3, reverse engineered client).
**/

#pragma once

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <string.h>
#include <fstream>
#include <string.h>

#include "base64.h" // Taken from https://github.com/ReneNyffenegger/cpp-base64

using namespace std;
/**
* universal description -- sockfd or sock means number of socket made by function socket()
**/
int makeConnection(int sockfd, struct addrinfo *serv_addr, char* addr, char* port); // Function creates connection on socket. serv_addr contains compiled information about wanted connection
// addr and port are used for error handling.

void registerM(int sock, string name, string pass);  // Function sends register message to server. register message contains name and password 
void loginM(int sock, string name, string pass); // Function sends login message to server. Saves login token to file "token-login"
void list(int sock); // Function downloads list of mails from server and prints it to std output
void send(int sock, string recipient, string subject, string body); // Function sends message to server
void fetch(int sock, string id); // Function downloads message by id from server (if logged in)
void logout(int sock); // Function logouts user and sends message of it to server 
void help(); // Function prints help...

string loadToken(); // Function returns token from login-token file.
string loadMessage(string succ, int sock); // Function loads long message from TCP connection.
string makeSpecial(string notSpecial); // Function recreates input string to string sendable by tcp connetion ('\n' to '\' and 'n' etc.)

