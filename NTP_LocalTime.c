#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NTP_SERVER "167.248.62.201"
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define NTP_TIMESTAMP_DELTA 2208988800ull

typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_id;
    uint32_t ref_ts_sec;
    uint32_t ref_ts_frac;
    uint32_t orig_ts_sec;
    uint32_t orig_ts_frac;
    uint32_t recv_ts_sec;
    uint32_t recv_ts_frac;
    uint32_t trans_ts_sec;
    uint32_t trans_ts_frac;
} ntp_packet;

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    ntp_packet packet;

    // Create a socket for UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Setup address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NTP_PORT);
    if (inet_pton(AF_INET, NTP_SERVER, &serv_addr.sin_addr) <= 0)
        error("ERROR invalid address");

    // Prepare Packet
    memset(&packet, 0, sizeof(ntp_packet));
    packet.li_vn_mode = (0 << 6) | (4 << 3) | 3; // li = 0, vn = 4, mode = 3 (client)

    // Send NTP 
    if (sendto(sockfd, &packet, sizeof(ntp_packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR sending packet");

    //  response
    n = recv(sockfd, &packet, sizeof(ntp_packet), 0);
    if (n < 0)
        error("ERROR receiving response");
    close(sockfd);

    // Convert NTP time to UNIX epoch
    time_t ntp_time = ntohl(packet.trans_ts_sec) - NTP_TIMESTAMP_DELTA;

    // Get local time
    time_t local_time;
    time(&local_time);

    // Print both times
    printf("NTP time: %s", ctime(&ntp_time));
    printf("Local time: %s", ctime(&local_time));

    // Calculate difference
    double diff = difftime(local_time, ntp_time);
    printf("Time difference: %.f seconds\n", diff);

    return 0;
}



//I had to restructure this because I accidentally deleted some of it while I was working on it and had to organize and find what i broke
