#include "hw1.h"
#include <math.h>

int get_src_addr(unsigned char packet[]) {
    return (packet[0] << 20) | (packet[1] << 12) | (packet[2] << 4) | ((packet[3] & 0xF0) >> 4);
}

int get_dest_addr(unsigned char packet[]) {
    return ((packet[3] & 0x0F) << 24) | (packet[4] << 16) | (packet[5] << 8) | packet[6];
}

int get_src_port(unsigned char packet[]) {
    return (packet[7] & 0xF0) >> 4;
}

int get_dest_port(unsigned char packet[]) {
    return packet[7] & 0x0F;
}

int get_fragment_offset(unsigned char packet[]) {
    return (packet[8] << 6) | ((packet[9] & 0xFC) >> 2);
}

int get_packet_len(unsigned char packet[]) {
    return ((packet[9] & 0x03) << 12) | (packet[10] << 4) | ((packet[11] & 0xF0) >> 4);
}

int get_max_hop_count(unsigned char packet[]) {
    return ((packet[11] & 0x0F) << 1) | ((packet[12] & 0x80) >> 7);
}

int get_checksum(unsigned char packet[]) {
    return ((packet[12] & 0x7F) << 16) | (packet[13] << 8) | packet[14];
}

int get_comp_scheme(unsigned char packet[]) {
    return (packet[15] & 0xC0) >> 6;
}

int get_traf_class(unsigned char packet[]) {
    return packet[15] & 0x3F;
}

void print_packet_sf(unsigned char packet[])
{
    printf("Source Address: %d\n", get_src_addr(packet));
    printf("Destination Address: %d\n", get_dest_addr(packet));
    printf("Source Port: %d\n", get_src_port(packet));
    printf("Destination Port: %d\n", get_dest_port(packet));
    printf("Fragment Offset: %d\n", get_fragment_offset(packet));
    printf("Packet Length: %d\n", get_packet_len(packet));
    printf("Maximum Hop Count: %d\n", get_max_hop_count(packet));
    printf("Checksum: %d\n", get_checksum(packet));
    printf("Compression Scheme: %d\n", get_comp_scheme(packet));
    printf("Traffic Class: %d\n", get_traf_class(packet));
    printf("Payload:");
    for(int i = 0; i < (get_packet_len(packet) - 16) / 4; i++) {
        printf(" %d", ((packet[i * 4 + 16] << 24) | (packet[i * 4 + 17] << 16) | (packet[i * 4 + 18] << 8) | (packet[i * 4 + 19])));
    }
    printf("\n");
}

unsigned int compute_checksum_sf(unsigned char packet[])
{
    unsigned int sum = get_src_addr(packet) + get_dest_addr(packet) + get_src_port(packet) + get_dest_port(packet) + get_fragment_offset(packet)
            + get_packet_len(packet) + get_max_hop_count(packet) + get_comp_scheme(packet) + get_traf_class(packet);

    for(unsigned int i = 0; i < (get_packet_len(packet) - 16) / 4; i++) {
        sum += abs((packet[i * 4 + 16] << 24) | (packet[i * 4 + 17] << 16) | (packet[i * 4 + 18] << 8) | (packet[i * 4 + 19]));
    }
    return sum % (unsigned int)(pow(2, 23) - 1);
}

unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len) {
    int count = 0;
    int packetsIndex = 0;
    int arrayIndex = 0;
    int payLoadLen;
    
    while(packetsIndex < packets_len) {
        unsigned char *currPacket = packets[packetsIndex];
        if(get_checksum(currPacket) != compute_checksum_sf(currPacket)) {
            packetsIndex++;
            continue;
        }

        payLoadLen = (get_packet_len(currPacket) - 16) / 4;
        int payLoad[payLoadLen];
        for(int i = 0; i < payLoadLen; i++) {
            payLoad[i] = (currPacket[i * 4 + 16] << 24) | (currPacket[i * 4 + 17] << 16) | (currPacket[i * 4 + 18] << 8) | (currPacket[i * 4 + 19]);
        }

        arrayIndex = get_fragment_offset(currPacket) / 4;
        for(int i = 0; i < payLoadLen; i++) {
            if(arrayIndex < array_len) {
                array[arrayIndex] = payLoad[i];
                arrayIndex++;
                count++;
            } else {
                break;
            }
        }
        packetsIndex++;
    }
    return count;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class)
{
    int arrayIndex = 0;
    int packetsIndex = 0;
    int intPerPayload = array_len / (max_payload / 4);
    int fragOffset = 0;
    int packetLen = 0;
    int checksum = 0;

    while(packetsIndex < packets_len && arrayIndex < array_len) {
        if(intPerPayload > (array_len - arrayIndex)) {
            intPerPayload = array_len - arrayIndex;
        }
        //printf("arrayIndex begin: %d\n", arrayIndex);
        //printf("intPerPayLoad: %d\n", intPerPayload);
        packetLen = 16 + (intPerPayload * 4);
        //printf("packetLen: %d\n", packetLen);
        packets[packetsIndex] = malloc(packetLen);

        //pack src_addr
        packets[packetsIndex][0] = src_addr >> 20;
        packets[packetsIndex][1] = src_addr >> 12;
        packets[packetsIndex][2] = src_addr >> 4;

        //shared byte for src and dest addr
        packets[packetsIndex][3] = ((src_addr << 4) & 0xF0) | ((dest_addr >> 24) & 0x0F);

        //pack dest_addr
        packets[packetsIndex][4] = dest_addr >> 16;
        packets[packetsIndex][5] = dest_addr >> 8;
        packets[packetsIndex][6] = dest_addr;

        //shared byte for src and dest port
        packets[packetsIndex][7] = ((src_port << 4) & 0xF0) | (dest_port & 0x0F);

        //pack fragment offset
        fragOffset = arrayIndex * 4;
        packets[packetsIndex][8] = fragOffset >> 6;

        //shared byte for fragment offset and packet length
        packets[packetsIndex][9] = ((fragOffset << 2) & 0xFC) | ((packetLen >> 12) & 0x03);

        //pack packet length
        packets[packetsIndex][10] = packetLen >> 4;

        //shared byte for packet length and max hop
        packets[packetsIndex][11] = ((packetLen << 4) & 0xF0) | ((maximum_hop_count >> 1) & 0x0F);

        //shared byte for compression scheme and traffic class
        packets[packetsIndex][15] = ((compression_scheme << 6) & 0xC0) | (traffic_class & 0x3F);

        //pack payload
        for(int i = 16; i < packetLen; i+=4) {
            packets[packetsIndex][i] = array[arrayIndex] >> 24;
            packets[packetsIndex][i+1] = array[arrayIndex] >> 16;
            packets[packetsIndex][i+2] = array[arrayIndex] >> 8;
            packets[packetsIndex][i+3] = array[arrayIndex];
            arrayIndex++;
            //printf("arrayIndex loop: %d\n", arrayIndex);
        }

        //shared byte for max hop and checksum
        checksum = compute_checksum_sf(packets[packetsIndex]);
        packets[packetsIndex][12] = ((maximum_hop_count << 7) & 0x80) | ((checksum >> 16) & 0x7F);

        //pack checksum
        packets[packetsIndex][13] = checksum >> 8;
        packets[packetsIndex][14] = checksum;
        
        //printf("arrayIndex end: %d\n", arrayIndex);
        packetsIndex++;
    }

    return packetsIndex;
}
