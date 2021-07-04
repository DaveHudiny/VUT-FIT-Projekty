#pragma once
/***
 * Author: David Hudák
 * File: main.h
 * Project: Packet sniffer for subject IPK (Computer Communications and Networks)
 * Short description: This file contains headers for all used functions, #include's for almost every needed library and special ARP header structure,
 * which is needed for getting correct IP adress from ARP.
***/
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <iostream>
#include <pcap.h>
#include <time.h>
#include <math.h>

// Some libraries I copy pasted from internet because seemed useful for project. And actually was.
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header
#include<netinet/ip6.h>	

void printTime(timeval time); // Function which prints time.
int printAllDevices(); // Function which prints all available devices.
void handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes); // Callback for pcap_loop
void help(); // Prints help for user

void IPv4(const u_char *bytes); // Function which processes IPv4 header from const u_char *bytes
void IPv6(const u_char *bytes); // Function which processes IPv6 header from const u_char *bytes
void printData(const u_char *bytes, int limit); // Function which prints data in correct format, int limit should be obtain from packet header like header->len.
// void printZeroForm(int number); // Function which was removed during testing - I found better wat how to print 0010 (zeros on start)

/* Just copy of header of ARP packet. You can see it on https://en.wikipedia.org/wiki/Address_Resolution_Protocol */
typedef struct{
    u_char HTYPE[2];
    u_char PTYPE[2];
    u_char HLEN;
    u_char PLEN;
    u_char OPER[2];
    u_char SHA[6];
    u_char SPA[4];
    u_char THA[6];
    u_char TPA[4];
}arpHeader;