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
#include <netinet/in.h>
#include <unistd.h>
#include "parse.h"
#include "resources.h"

#define SCORES_FILE "res/Batting.txt"
#define AUTHS_FILE "res/Authentication.txt"
#define PORT 42424
#define CLIENT_QUEUE 30
#define PACKET_SIZE 128

scores_db *scores;
auths_db *auths;

int listener(int socket_descr);

int main(int argc, char const *argv[])
{
    printf("...Started batting statistics server...\n");

    // initialise files / parsed objects
    FILE *score_file;
    FILE *auth_file;
    scores_f = fopen(SCORES_FILE);
    auths_f = fopen(AUTHS_FILE);
    scores parse_scores(scores_f);
    auths parse_auths(auths_f);

    // initialise socket objects
    int sockfd, new_fd, packet_bytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    // start listener
    if (listen(sockfd, CLIENT_QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    printf("...Listening on port %d...\n", PORT);
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }
        printf("Server: Accepted connection from %s\n", inet_ntoa(their_addr.sin_addr));
        if (!fork())
        {
            // --------------------------------------
            // Checking Authentication / handling
            client_details *client;
            if ((packet_bytes = recv(sockfd, client, PACKET_SIZE, 0)) == -1)
            {
                perror("recv");
                exit(1);
            }
            if (auth_match(auths, client->user, client->pass) != 0)
            { // not good, send bad response and close socket
                if (send(new_fd, 1, 1, 0) == -1)
                { // client knows '1' is bad
                    perror("send");
                }
                close(new_fd);
                exit(0);
            } else
            {
                if (send(new_fd, 0, 1, 0) == -1)
                { // all good, send good response and continue
                    perror("send");
                    exit(1);
                }
            } // continue
            //
            // --------------------------------------

            // --------------------------------------
            // Checking for player / handling
            while(1)
            {
                char input[PACKET_SIZE / 2]; // smaller size for name
                player_stats *player;
                if ((packet_bytes = recv(sockfd, input, PACKET_SIZE, 0)) == -1)
                {
                    perror("recv")
                    exit(1);
                }
                input[packet_bytes] = '\0';
                if (strcmp(input, "q"))
                { // close connection if client quits
                    close(new_fd);
                    exit(0);
                }
                if ((player = search_player(scores, input)) == NULL)
                { // player name not found
                    if (send(new_fd, 1, 1, 0) == -1)
                    { // client checks recv size, then if 1
                        perror("send");
                        exit(1);
                    }
                } else
                { // player name found
                    if (send(new_fd, *player, sizeof(*player), 0) == 0)
                    {
                        perror("send");
                        exit(1);
                    }
                }
            }
            //
            // --------------------------------------
        }
        close(new_fd);
    }


    printf("Finished batting statistics server...\n");
    return 0;
}