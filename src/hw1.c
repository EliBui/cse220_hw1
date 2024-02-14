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
    int srcAddr = get_src_addr(packet);
    int destAddr = get_dest_addr(packet);
    int srcPort = get_src_port(packet);
    int destPort = get_dest_port(packet);
    int fragmentOffset = get_fragment_offset(packet);
    int packetLen = get_packet_len(packet);
    int maxHopCount = get_max_hop_count(packet);
    int checksum = get_checksum(packet);
    int compScheme = get_comp_scheme(packet);
    int trafClass = get_traf_class(packet);
    
    printf("Source Address: %d\n", srcAddr);
    printf("Destination Address: %d\n", destAddr);
    printf("Source Port: %d\n", srcPort);
    printf("Destination Port: %d\n", destPort);
    printf("Fragment Offset: %d\n", fragmentOffset);
    printf("Packet Length: %d\n", packetLen);
    printf("Maximum Hop Count: %d\n", maxHopCount);
    printf("Checksum: %d\n", checksum);
    printf("Compression Scheme: %d\n", compScheme);
    printf("Traffic Class: %d\n", trafClass);
    printf("Payload:");
    for(int i = 0; i < (packetLen - 16) / 4; i++) {
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
    (void)packets;
    (void)packets_len;
    (void)array;
    (void)array_len;
    return -1;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class)
{
    (void)array;
    (void)array_len;
    (void)packets;
    (void)packets_len;
    (void)max_payload;
    (void)src_addr;
    (void)dest_addr;
    (void)src_port;
    (void)dest_port;
    (void)maximum_hop_count;
    (void)compression_scheme;
    (void)traffic_class;
    return -1;
}
