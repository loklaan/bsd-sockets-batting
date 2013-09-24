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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include "resources.h"

#define PORT 42424
#define PACKET_SIZE 128
#define HOST "127.0.0.1"

int main(int argc, char const *argv[])
{
    printf("...Started batting statistics client...\n");

    // initialise socket objects
    int sockfd, packet_bytes;
    struct hostent *he;
    struct sockaddr_in their_addr;
    if ((he = gethosebyname(HOST)) == NULL)
    {
        herror("gethosebyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *) he->h_addr);
    bzero(&(their_addr.sin_zero), 8);
    if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect")
        error(1);
    }
    printf("Client: Connected to server at %s\n", inet_ntoa(their_addr.sin_addr));

    // ----- client logic -----
    // send entered client user / pass
    char input[PACKET_SIZE / 2];
    int boolean = -1;
    client_details details;
    printf("Enter server authentication details...\n");
    printf("User: ");
    scanf("%s", details->user);
    printf("Pass: ");
    scanf("%s", details->pass);
    if (send(sockfd, details, sizeof(details), 0) == -1)
    {
        perror("send");
        exit(1);
    }
    if ((packet_bytes = recv(sockfd, boolean, PACKET_SIZE, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }
    // continue, or force close
    if (boolean == 1)
    {
        printf("Invalid authentication details.\n...Closing client...\n");
        close(sockfd);
        return 0;
    } else if (boolean == 0)
    {
        printf("Valid authentication.\n");
    }
    // loop
    char *quit = "q";
    while(1)
    {
        //    enter q to quit at anytime
        //    enter player name
        printf("Enter a players name, or q to quit: ");
        scanf("%s", input);
        if (send(sockfd, input, sizeof(input), 0) == -1)
        {
            perror("send");
            exit(1);
        }
        if (strcmp(input, quit))
        {
            close(sockfd);
            break;
        }
        if ((packet_bytes = recv(sockfd, boolean, PACKET_SIZE, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        if (boolean == 0)
        {
            player_stats *stats;
            if ((packet_bytes = recv(sockfd, stats, PACKET_SIZE, 0)) == -1)
            {
                perror("recv");
                exit(1);
            }
            print_player_stats(stats);
        }
    }
    
    printf("...Closing client...\n");
    return 0;
}