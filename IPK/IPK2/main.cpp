/***
 * Author: David Hudák
 * File: main.cpp
 * Project: Packet sniffer for subject IPK (Computer Communications and Networks)
 * Short description: This file contains almost all code for packet sniffer. Program prints packets to standard output.
***/

#include "main.h"
using namespace std;
#include <errno.h>
#define MAXLEN 65536 // Special constant for pcap_open_live (length of packet)
#define MAXMES 101
#include  <iomanip>

enum errors {EOK, EUNEX}; // Error codes - just two, because one is OK, one is UNEXPECTED (no errors are expected :) just sniffing packets.)

void help()
{
  cout << "Hello, my name is ipk-sniffer and my creator is David Hudak." << endl << endl;
}

void printPacket(const u_char *bytes, const struct pcap_pkthdr *h)
{
  int offset = 0; // Sets starting offset (0x0000)
  while(offset < (int)h->len) // printing while loop
  {
    cout << "0x" << setfill('0') << setw(4) << hex << offset << ": "; // prints actual offset in hexadecimal form.
    for(int j = 0; j<16; j++) // cycle for printing hexadecimal version of bytes (one line should have 16 bytes).
    {
      if(offset + j >= (int)h->len) // We are at the end of packet. Just print spaces ant it will be okay.
      {
        cout << "   ";
        continue;
      }
      cout << setfill('0') << setw(2) << hex << (int)bytes[offset+j] << " "; // printing hexadecimal form of bytes - notice setfill and setw which offers us filling with 0
    }
    cout << " "; // Printing additional space between hexadecimal form and ascii form
    for(int j = 0; j<16; j++) // cycle for printing ascii version of bytes (one line should have 16 bytes).
    {
      if(offset + j >= (int)h->len) // When we are on the edge, we don't have to print additional spaces like in cycle before.
      {
        break;
      }
      if(bytes[offset+j] >= 32 && bytes[offset+j] < 127) // byte is printable character.
      {
        cout << bytes[offset+j];
      }
      else // byte is not printable character
      {
        cout << ".";
      }
      
    }
    cout << endl; // formating stuff
    offset += 16; // Jumping with offset by number of printed bytes.
  } // end of printing while loop
  cout << endl; // formating stuff
} // End of data printing


void IPv4(const u_char *bytes)
{
  struct ip *IPHead = (struct ip *)(bytes + sizeof(struct ether_header)); // Using some magic trick to get ip header from packet.
  // Some useful declarations.
  struct tcphdr* TCPHeader; // For transmittion protocols
  struct udphdr* UDPHeader;
  char srcIP[MAXMES]; // For IPs
  char destIP[MAXMES];
  u_int16_t srcPort; // For ports
  u_int16_t destPort;
  inet_ntop(AF_INET, &(IPHead->ip_src), srcIP, 100); // Getting IP adress from IP headers to char * (srcIP)
  inet_ntop(AF_INET, &(IPHead->ip_dst), destIP, 100); // -||- (destIP)

  switch(IPHead->ip_p)
  {
    case IPPROTO_TCP: // Some magical constant, but hidden.
      TCPHeader = (struct tcphdr *)(bytes + sizeof(struct ether_header) + sizeof(struct ip)); // Some magical retyping to get TCP header with offset of previous headers.
      srcPort = ntohs(TCPHeader->source); // Converting information to actually useful port
      destPort = ntohs(TCPHeader->dest);
      cout << srcIP << " : " << srcPort << " > " << destIP << " : " << destPort; // Print correct form with srcIPv4 : srcPort > destIPv4 : destPort
      break;
    case IPPROTO_UDP: 
      UDPHeader = (struct udphdr *)(bytes + sizeof(struct ether_header) + sizeof(struct ip)); // Same magic as before with TCP
      srcPort = ntohs(UDPHeader->source);
      destPort = ntohs(UDPHeader->dest);  
      cout << srcIP << " : " << dec << srcPort << " > " << dec << destIP << " : " << destPort;
      break;
    default: // ICMP or some other stuff. No port needed. Maybe.
      cout << srcIP << " > " << destIP;
      break;
  }
} // End of IPv4 processing

void IPv6(const u_char *bytes)
{
  struct ip6_hdr *IPHead = (struct ip6_hdr *)(bytes + sizeof(struct ether_header)); // Similar to IPv4 processing, but with different header.
  struct tcphdr* TCPHeader; // Some useful declaration, same as in IPv4
  struct udphdr* UDPHeader;
  char srcIP[MAXMES];
  char destIP[MAXMES];
  u_int16_t srcPort;
  u_int16_t destPort;
  inet_ntop(AF_INET6, &(IPHead->ip6_src), srcIP, 100); // Getting IPv6 from header with correct char * form.
  inet_ntop(AF_INET6, &(IPHead->ip6_dst), destIP, 100);

  switch(IPHead->ip6_ctlun.ip6_un1.ip6_un1_nxt)
  {
    case IPPROTO_TCP: // Same as in IPv4
      TCPHeader = (struct tcphdr *)(bytes + sizeof(struct ether_header) + sizeof(struct ip6_hdr)); // Magical retyping similar to IPv4, but with ip6_hdr
      srcPort = ntohs(TCPHeader->source);
      destPort = ntohs(TCPHeader->dest);
      cout << srcIP << " : " << srcPort << " > " << destIP << " : " << destPort;
      break;
    case IPPROTO_UDP:
      UDPHeader = (struct udphdr *)(bytes + sizeof(struct ether_header) + sizeof(struct ip6_hdr)); // Magical retyping similar to IPv4, but with ip6_hdr
      srcPort = ntohs(UDPHeader->source);
      destPort = ntohs(UDPHeader->dest);  
      cout << srcIP << " : " << dec << srcPort << " > " << destIP << " : " << dec << destPort;
      break;
    default: // No ports needed (ICMP6)
      cout << srcIP << " > " << destIP;
      break;
  }
} // end of IPv6 processing

//Here starts callback function for pcap_loop, very important.
// user is useless information, pcap_pkthdr *h contains important informations about time and length of packet and bytes contains all bytes from packet.
void handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
  printTime(h->ts); // First part of every printed packet is it's time.
  arpHeader *arp; // Declaration for ARP header - only this transmittion protocol is not processed in separated function.
  auto *eptr = (struct ether_header *) bytes; // Ethernet header.
  cout << " ";
  char buffer[MAXMES];
  switch(ntohs(eptr->ether_type))
  {
    case ETHERTYPE_IP: // Ethernet type is IPv4
      IPv4(bytes);
      break;
    case ETHERTYPE_IPV6: // Ethernet type is IPv6
      IPv6(bytes);
      break;
    case ETHERTYPE_ARP:
      arp = (arpHeader *)(bytes + sizeof(struct ether_header)); // Getting header of arp packet
      cout << inet_ntop(AF_INET, arp->SPA, buffer, 100) << " > " << inet_ntop(AF_INET, arp->TPA, buffer, 100);
      break;
    
    default:
      cout << "Unsupported protocol" << endl; // Honestly, I don't know, if this situation ever occurs.
      break;
  }
  cout << ", length " << dec << h->len << " bytes" << endl << endl; // Printing length of packet, some format stuff.
  printPacket(bytes, h); // Every protocol has one same stuff - all would be printed with same function.
  //for (auto i = 0; i < h->) 
} // end of handler

void printTime(timeval time) // Worst function in my program. I could do this much easier.
{
  struct tm *actualTime = localtime(&time.tv_sec); // Geting time from received packet.
  string month = to_string(actualTime->tm_mon+1), day = to_string(actualTime->tm_mday); // Converting all time stuff to strings.
  string hour = to_string(actualTime->tm_hour), minute = to_string(actualTime->tm_min);
  string sec = to_string(actualTime->tm_sec);
  string timez = to_string(actualTime->tm_gmtoff/3600); // GMT is some kind of weird format which is needed to be divided by 3600.
  string plus = "+";
  if(actualTime->tm_mon + 1 < 10) // Filling empty spaces with zeros (for example month could be 12, 11, 01 etc.) 
  {
    month = "0" + month;
  }
  if(actualTime->tm_mday < 10) // Again
  {
    day = "0" + day;
  }
  if(actualTime->tm_hour < 10) // Again
  {
    hour = "0" + hour;
  }
  if(actualTime->tm_min < 10) // Again
  {
    minute = "0" + minute;
  }
  if(actualTime->tm_sec < 10) // Again
  {
    sec = "0" + sec;
  }
  if(actualTime->tm_gmtoff/3600 < 10 && actualTime->tm_gmtoff/3600 > - 10) // Again
  {
    if(actualTime->tm_gmtoff >= 0)
    {
      timez = "0" + timez;
    }
    else
    {
      plus = "";
      timez.insert(1, 1, '0');
    }
  }
  cout << dec << actualTime->tm_year + 1900 << "-" << month << "-" << day << "T"; // Print date
  cout << hour << ":" << minute << ":" << sec << "." << to_string(time.tv_usec).substr(0,3) << plus << timez << ":00"; // Print time of day with GMT
  // This part could be tricky in North Korea, because they use some kind of half of hour or dunno.

} // End of printing date.

int printAllDevices()
{
  pcap_if_t *allDevices; // Some kind of structure for devices. I only need their names.
  char errBuff[MAXMES];
  auto err = pcap_findalldevs(&allDevices, errBuff); // Function for getting all online devices for internet.
  if(err != 0) // Something bad happened.
  {
    cout << "Error with looking for devices." << endl;
    cout << errBuff << endl;
    return EUNEX;
  }
  while(allDevices != NULL) // Cycle which goes through all elements of list of allDevices.
  {
    cout << allDevices->name << endl;
    allDevices = allDevices->next;
  }
  return EOK;
} // End of getting all devices

int main(int argc, char *argv[])
{
  struct option long_options[] = { // Long options by zadání.
    {"interface", optional_argument, 0, 'i'},
    {"tcp", no_argument, 0, 't'},
    {"udp", no_argument, 0, 'u'},
    {"arp", no_argument, 0, 0 },
    {"icmp", no_argument, 0, 0},
    {"help", no_argument, 0, 'h'},
    {0, 0,  0,  0} // Last line of longopts, it's mandatory.
  };
  int option_index; // Some useful declarations.
  bool tcp=false, udp=false, arp=false, icmp=false, portB = false; // Bools for occurencies of their name options.
  u_int16_t port; // Variable for saving port.
  char interface[MAXMES]; // Name of interface
  bool interfaceArg = false;
  pcap_t *handle;
  char errBuff[MAXMES]; // Buffer for errors.
  int packetNumber = 1;
  struct bpf_program fp; // some special variable for pcap filters.
  string filterExp = "";
  int c;
  while((c = getopt_long(argc, argv, ":i::p:tun:h", long_options, &option_index)) != -1)
  {
    switch(c) // Long switch for all options. Please, read https://linux.die.net/man/3/getopt_long if you need more informations for how it works.
    {
      case 0: // arp and icmp options.
        if(strcmp(long_options[option_index].name,"arp")==0)
        {
          arp = true;
        }
        else if(strcmp(long_options[option_index].name,"icmp")==0)
        {
          icmp = true;
        }
        else
        {
          cout << "you shouldnt be there" << endl;
          return EUNEX; 
        }
        break;
      case 'i': // Interface option
        interfaceArg = true;
        if(!optarg) // Argument wasn't found. Yet
        {
          int i = 0;
          while(i < argc && strcmp(argv[i], "-i")!=0) // Finding -i by force. Maybe there are better ways. But I really dont care.
          {
            i++;
          }
          if(i < argc - 1)
          {
            if(argv[i+1][0] != '-') // If next argument of program (after -i) is non -something, it is optional argument for -i.
            {
              strcpy(interface, argv[i+1]);
              break;
            }
          }
          cout << "You can choose from devices: " <<endl; // Really, no argument.
          return printAllDevices(); // Print all devices and end program, because without device specified program cannot track (sniff) packets
        }
        else // Argument found, just copy it.
        {
          strcpy(interface, optarg);
        }
        break;
      case 'p': // Port option
        portB = true;
        port = atol(optarg);
        if(port < 0 or port > 65535) // You know, what I am doing, doesn't it? (Ports can be only values between <0, 65535>.)
        {
          cout << "Invalid port value." << endl;
        }
        break;
      case 't': // TCP, UDP options.
        cout << "tcp" << endl;
        tcp = true;
        break;
      case 'u':
        udp = true;
        break;
      case 'n': // Number of packet option.
        packetNumber = atol(optarg);
        break;
      default: //I don't think that can actually happen. But if yes, I will be prepared.
        cout << "Unsupported arg" << endl;
        help();
        return 1;
    }
  }
  if(!interfaceArg) // No interface option found. Which is of course wrong.
  {
    help();
    return printAllDevices();
  }
  else
  {
    handle = pcap_open_live(interface, MAXLEN, 1, 10, errBuff); // Yay, open pcap for some chosen device.
  }

  if(handle == NULL) // Oh no, some error has occured.
  {
    cout << errBuff << endl;
    return EUNEX;
  }
  if(packetNumber < 1) // Okay, dunno why you called this program, but okay.
  {
    pcap_close(handle);
    return 0;
  }

  bool first = true; // Written option is first.

  if(arp) // Chosen option is arp
  {
    filterExp = filterExp + "arp";
    first = false; 
  }

  if(icmp) // Chosen option is icmp
  {
    if(first)
    {
      first = false;
    }
    else // It's not first option, you should add or
    {
      filterExp = filterExp + " or ";
    }
    filterExp = filterExp + "icmp or icmp6";
  }
  if(tcp)
  {
    if(first)
    {
      first = false;
    }
    else // It's not first option, you should add or
    {
      filterExp = filterExp + " or ";
    }
    filterExp = filterExp + "tcp";
  }
  if(udp)
  {
    if(first)
    {
      first = false;
    }
    else // It's not first option, you should add or
    {
      filterExp = filterExp + " or ";
    }
    filterExp = filterExp + "udp";
  }
  if(portB && (tcp || udp || (!icmp && !arp))) // Only tcp and udp have ports (or no argument).
  {
    if(first)
    {
      first = false;
    }
    else // port is not first option -> and option
    {
      filterExp = filterExp + " and ";
    }
    filterExp = filterExp + "port " + to_string(port);
  }
  else if(portB) // port was chosen, but also ICMP or ARP without TCP or UDP
  {
    cout << "Cannot filter port with ICMP or ARP" << endl;
    help();
    return EUNEX;
  }

  if (pcap_compile(handle, &fp, filterExp.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1) // Compile filters from string filterExp and make "program" fp.
  {
		cout << "Cannot compile filters, unexpected problem." << endl;
    cout << filterExp << endl;
    help();
		return(EUNEX);
	}
  if(pcap_setfilter(handle, &fp) == -1) // Add program fp to handle of our connection.
  {
		cout << "Cannot set filter.";
    help();
		return(EUNEX);
	}
  pcap_loop(handle, packetNumber, handler, NULL); // Sniff packets.
  pcap_close(handle); // Close connection. Goodbye


  return 0;

}