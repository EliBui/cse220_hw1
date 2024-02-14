#include "hw1.h"

void print_packet_sf(unsigned char packet[])
{
    int sourceAddress = (packet[0] << 20) | (packet[1] << 12) | (packet[2] << 4) | ((packet[3] & 0xF0) >> 4);
    int destinationAddress = ((packet[3] & 0x0F) << 24) | (packet[4] << 16) | (packet[5] << 8) | packet[6];
    int sourcePort = (packet[7] & 0xF0) >> 4;
    int destinationPort = packet[7] & 0x0F;
    int fragmentOffset = (packet[8] << 6) | ((packet[9] & 0xFC) >> 2);
    int packetLength = ((packet[9] & 0x03) << 12) | (packet[10] << 4) | ((packet[11] & 0xF0) >> 4);
    int maximumHopCount = ((packet[11] & 0x0F) << 1) | ((packet[12] & 0x80) >> 7);
    int checkSum = ((packet[12] & 0x7F) << 16) | (packet[13] << 8) | packet[14];
    int compressionScheme = (packet[15] & 0xC0) >> 6;
    int trafficClass = packet[15] & 0x3F;
    int payLoadSize = (packetLength - 16) / 4;
    
    printf("Source Address: %d\n", sourceAddress);
    printf("Destination Address: %d\n", destinationAddress);
    printf("Source Port: %d\n", sourcePort);
    printf("Destination Port: %d\n", destinationPort);
    printf("Fragment Offset: %d\n", fragmentOffset);
    printf("Packet Length: %d\n", packetLength);
    printf("Maximum Hop Count: %d\n", maximumHopCount);
    printf("Check Sum: %d\n", checkSum);
    printf("Compression Scheme: %d\n", compressionScheme);
    printf("Traffic Class: %d\n", trafficClass);
    printf("Payload:");
    for(int i = 0; i < payLoadSize; i++) {
        printf(" %d", ((packet[i * 4 + 16] << 24) | (packet[i * 4 + 17] << 16) | (packet[i * 4 + 18] << 8) | (packet[i * 4 + 19])));
    }
    printf("\n");
}

unsigned int compute_checksum_sf(unsigned char packet[])
{
    (void)packet;
    return -1;
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
