#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //this gives access to POSIX OS API, allows applns to use system calls and interact with the OS
#include <fcntl.h>  //this gives access to file control options, allows applns to manipulate file descriptors and control file access modes
#include <sys/ioctl.h> //this gives access to ioctl system call, allows applns to perform device-specific input/output operations on file descriptors
#include <sys/socket.h>
#include <errno.h>
#include <net/if.h>
#include <arpa/inet.h> // For ntohs(), htons() , this header file provides functions for converting values between host and network byte order, which is important for network communication.
#include "netreq.h" //this is a custom header file that contains definitions and declarations related to network requests, including structures for Ethernet and ARP headers, as well as the tun_alloc function for creating TAP devices.

#define BUF_SIZE 1600
#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define print_error printf



int main() {
    char dev_name[IFNAMSIZ] = "tap0"; // Linux will name it tap0
    int tap_fd = tun_alloc(dev_name); //creating a TAP device named tap0
    
    if (tap_fd < 0) {
        printf("Failed to allocate TAP device\n");
        return 1;
    }
    
    printf("Successfully created TAP device: %s\n", dev_name);
    printf("Waiting for packets... (Run 'sudo ip link set dev %s up' in another terminal)\n", dev_name);

    unsigned char buf[BUF_SIZE]; //creating a buffer to hold incoming packets, with a size of 1600 bytes, which is sufficient for most Ethernet frames.
    
    // The Core Network Stack Loop
    while (1) {
        ssize_t nread = read(tap_fd, buf, sizeof(buf));  //reading from the fd into the buf array. The read function returns the number of bytes read, or -1 if an error occurred.
        
		if (nread < 0) {
            perror("Reading from interface failed");
            close(tap_fd);
            exit(1);
        }

        if (nread < sizeof(struct eth_hdr)) {
            continue; // Packet is too small to even contain an Ethernet header
        }

        // 1. Parse the Layer 2 (Ethernet) Header
        struct eth_hdr *hdr = (struct eth_hdr *) buf;
        
        // 2. Network Byte Order conversion (Big Endian to Little Endian)
        uint16_t ethertype = ntohs(hdr->ethertype);

        // 3. Route the packet based on EtherType
        switch (ethertype) {
            case 0x0806: // ARP Protocol (defined in linux/if_ether.h as ETH_P_ARP)
                printf("Received ARP packet (Size: %ld bytes)\n", nread);
                handle_arp(hdr, hdr->payload);
                break;
                
            case 0x0800: // IPv4 Protocol (defined as ETH_P_IP)
                printf("Received IPv4 packet (Size: %ld bytes) - Routing to IP handler...\n", nread);
                // TODO: struct ip_hdr *ip = (struct ip_hdr *)hdr->payload;
                break;
                
            default:
                // Ignore other protocols for now (like IPv6 0x86DD)
                break;
        }
    }

    close(tap_fd);
    return 0;
}
