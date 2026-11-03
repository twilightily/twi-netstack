#ifndef NETREQ_H
#define NETREQ_H
#include <stdint.h>

struct eth_hdr
{
    unsigned char dmac[6]; //dmac and smac are dst and src mac addresses , each represented with 6 bits 
    unsigned char smac[6];
    uint16_t ethertype;   //if value >=1536 this contaisn type of payload, else it contains length of payload
    unsigned char payload[]; //pointer to the frame's payload 
	//ethernet frame tags are excluded from this implementation
} __attribute__((packed));


//struct eth_hdr *hdr = (struct eth_hdr *) buf; //buf is a pointer to the ethernet frame, which is casted to eth_hdr struct pointer

struct arp_hdr
{
    uint16_t hwtype;   //link layer type used in the network, for ethernet it is 1 and yes we're using ethernet
    uint16_t protype; //protocol type used in the network, for ipv4 it is 0x0800
    unsigned char hwsize; //size of both addresses in bytes, for ethernet it is 6 and for ipv4 it is 4
    unsigned char prosize;
    uint16_t opcode; //operation code, 1 for request and 2 for reply like we can see in captured packets on wireshark
    unsigned char data[];
} __attribute__((packed));


struct arp_ipv4
{
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed));

int tun_alloc(char *dev);
void handle_arp(struct eth_hdr *eth, unsigned char *arp_payload);

#endif