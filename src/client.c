// -----------------------------------------------
// File: server.c
// Purpose: Server for the Batting Stats system.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include "resources.h"
#include "console.h"

#define PORT 42420
#define PACKET_SIZE 256
#define HOST "127.0.0.1"
#define YES 0
#define NO 1

int main(int argc, char const *argv[])
{
    msg("Started batting statistics client");

    // initialise socket objects
    int sockfd, packet_bytes;
    struct hostent *he;
    struct sockaddr_in their_addr;
    if ((he = gethostbyname(HOST)) == NULL)
    {
        log_err("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        log_err("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *) he->h_addr);
    bzero(&(their_addr.sin_zero), 8);
    if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == -1)
    {
        log_err("connect");
        exit(1);
    }
    msg_client("Connected to server at %s", inet_ntoa(their_addr.sin_addr));

    // ----- client logic -----
    // send entered client user / pass
    char input[PACKET_SIZE / 4];
    int boolean = NO;
    client_details details;
    printf("\nEnter client authentication details.\n");
    printf("User: ");
    scanf("%s", input);
    strcpy(details.user, input);
    input[0] = '\0';
    printf("Pass: ");
    scanf("%s", input);
    printf("\n");
    strcpy(details.pass, input);
    input[0] = '\0';
    msg_client("Sending client authentication details")
    if (send(sockfd, &details, sizeof(details), 0) == -1)
    {
        log_err("send client details");
        exit(1);
    }
    msg_client("Size of sent packet: %d", (int)sizeof(details));
    if ((packet_bytes = recv(sockfd, &boolean, PACKET_SIZE, 0)) == -1)
    {
        log_err("recv auth validity");
        exit(1);
    }
    msg_client("Size of recieved packet: %d", packet_bytes);
    // continue, or force close
    if (boolean == NO)
    {
        msg_server("Invalid client authentication details");
        msg("Quitting client");
        close(sockfd);
        return 0;
    } else if (boolean == YES)
    {
        msg_server("Valid client authentication details.");
    }
    // loop
    char *quit = "q";
    while(1)
    {
        //    enter q to quit at anytime
        //    enter player name
        printf("\nEnter a players name, or q to quit: ");
        scanf("%s", input);
        if (send(sockfd, input, sizeof(input), 0) == -1)
        {
            log_err("send player name / quit input");
            exit(1);
        }
        msg_client("Size of sent packet: %d", (int)sizeof(input));
        if (strcmp(input, quit) == 0)
        {
            msg_client("User has chosen to quit");
            close(sockfd);
            break;
        }
        if ((packet_bytes = recv(sockfd, &boolean, PACKET_SIZE, 0)) == -1)
        {
            log_err("recv player name validity");
            exit(1);
        }
        msg_client("Size of recieved packet: %d", packet_bytes);
        if (boolean == YES)
        {
            msg_server("Player %s was found", input);
            player_stats stats;
            if ((packet_bytes = recv(sockfd, &stats, PACKET_SIZE, 0)) == -1)
            { // TODO: screwing up sometimes....
                log_err("recv player stats");
                exit(1);
            }
            msg_client("Size of recieved packet: %d", packet_bytes);
            print_player_stats(&stats);
        } else
        {
            msg_server("Player %s was not found", input);
        }
        input[0] = '\0';
    }
    
    msg_client("Closing client");
    return 0;
}
