#include <linux/if_ether.h>
#include "netreq.h"
#include <arpa/inet.h> // For ntohs(), htons() , this header file provides functions for converting values between host and network byte order, which is important for network communication.
#include <net/if.h>
#include <sys/ioctl.h> //this gives access to ioctl system call, allows applns to perform device-specific input/output operations on file descriptors
#include <stdint.h>

void handle_arp(struct eth_hdr *eth, unsigned char *arp_payload) { //this handles ARP pkts rcvd on the TAP device. It takes a pointer to the Ethernet header and a pointer to the ARP payload as arguments.
    struct arp_hdr *arp = (struct arp_hdr *)arp_payload;
    
    // Check if it is an IPv4 ARP packet over Ethernet
    if (ntohs(arp->hwtype) == 1 && ntohs(arp->protype) == 0x0800) { //checks if its ethernet and ipv4
        struct arp_ipv4 *arp_data = (struct arp_ipv4 *)arp->data;
        
        uint16_t op = ntohs(arp->opcode);
        if (op == 1) { // ARP Request
            printf("[ARP] Request received from IP: %d.%d.%d.%d\n", //decodes the source IP
                   (arp_data->sip >> 0) & 0xFF, (arp_data->sip >> 8) & 0xFF,
                   (arp_data->sip >> 16) & 0xFF, (arp_data->sip >> 24) & 0xFF);
            
            // TODO: Generate an ARP Reply here if the requested IP matches yours!
        } else if (op == 2) { // ARP Reply
            printf("[ARP] Reply received!\n");
        }
    }
}