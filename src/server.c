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
#include <netinet/in.h>
#include <unistd.h>
#include "parse.h"
#include "resources.h"

#define SCORES_FILE "res/Batting.txt"
#define AUTHS_FILE "res/Authentication.txt"
#define PORT 42422
#define CLIENT_QUEUE 30
#define PACKET_SIZE 128

scores_db *scores;
auths_db *auths;

int listener(int socket_descr);

int main(int argc, char const *argv[])
{
    printf("...Started batting statistics server...\n");

    // initialise files / parsed objects
    FILE *scores_f;
    FILE *auths_f;
    scores_f = fopen(SCORES_FILE, "r");
    auths_f = fopen(AUTHS_FILE, "r");
    scores = parse_scores(scores_f);
    auths = parse_auths(auths_f);

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

    int reuse = 1;
    int err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (0 == err)
    {
        if ((err = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))) == -1)
        {
            perror("bind");
            exit(1);
        }
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
        char client_id[PACKET_SIZE / 2];
        sprintf(client_id, "%d", rand());

        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }
        printf("...Accepted client %s connection from %s...\n", client_id, inet_ntoa(their_addr.sin_addr));
        if (!fork())
        {
            // --------------------------------------
            // Checking Authentication / handling
            int yes = 0;
            int no = 1;
            client_details *client;
            if ((packet_bytes = recv(new_fd, client, PACKET_SIZE, 0)) == -1)
            {
                printf("FASJDN\n");
                perror("recv");
                exit(1);
            }
            if (auth_match(auths, client->user, client->pass) != 0)
            { // not good, send bad response and close socket
                printf("\t...Client %s had bad authentication.\n Closing connection...\n", client_id);
                if (send(new_fd, &no, sizeof(no), 0) == -1)
                { // client knows '1' is bad
                    perror("send");
                }
                close(new_fd);
                exit(0);
            } else
            {
                printf("\t...Client %s had good authentication.\n Sending approval...\n", client_id);
                if (send(new_fd, &yes, sizeof(yes), 0) == -1)
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
                if ((packet_bytes = recv(new_fd, input, PACKET_SIZE, 0)) == -1)
                {
                    perror("recv");
                    exit(1);
                }
                input[packet_bytes] = '\0';
                if (strcmp(input, "q"))
                { // close connection if client quits
                    printf("Client %s quit.\nClosing connection.\n", client_id);
                    close(new_fd);
                    exit(0);
                }
                // TODO: add task 2 from asgn spec
                if ((player = search_player(scores, input)) == NULL)
                { // player name not found
                    if (send(new_fd, &no, sizeof(no), 0) == -1)
                    { // client checks recv size, then if 1
                        perror("send");
                        exit(1);
                    }
                } else
                { // player name found
                    if (send(new_fd, &yes, sizeof(yes), 0) == -1)
                    {
                        perror("send");
                        exit(1);
                    }
                    if (send(new_fd, player, sizeof(*player), 0) == 0)
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