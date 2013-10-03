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
#include "console.h"

#define SCORES_FILE "res/Batting.txt"
#define AUTHS_FILE "res/Authentication.txt"
#define PORT 42420
#define CLIENT_QUEUE 30
#define PACKET_SIZE 256
#define YES 0
#define NO 1

scores_db *scores;
auths_db *auths;
int *player_queries;

int listener(int socket_descr);

int main(int argc, char const *argv[])
{
    msg_server("Started batting statistics server");

    // initialise files / parsed objects
    FILE *scores_f;
    FILE *auths_f;
    scores_f = fopen(SCORES_FILE, "r");
    auths_f = fopen(AUTHS_FILE, "r");
    scores = parse_scores(scores_f);
    auths = parse_auths(auths_f);

    if ((player_queries = calloc(scores->size, sizeof(scores->last_found_index))) == NULL)
    {
        log_err("calloc player_queries");
        exit(1);
    }

    // initialise socket objects
    int sockfd, new_fd, packet_bytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        log_err("socket");
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
            log_err("bind");
            exit(1);
        }
    }

    // start listener
    if (listen(sockfd, CLIENT_QUEUE) == -1)
    {
        log_err("listen");
        exit(1);
    }

    msg_server("Listening on port %d", PORT);
    while (1)
    {
        fflush(stdout);
        char client_id[PACKET_SIZE / 4];

        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
        {
            log_err("accept");
            continue;
        }
        msg_server("Accepted client connection from %s", inet_ntoa(their_addr.sin_addr));
        int pid = -1;
        if ((pid = fork()) == 0)
        {
            msg_client_cpid("Forked client connection");
            sprintf(client_id, "%d", getpid());
            // --------------------------------------
            // Checking Authentication / handling
            int yes = 0;
            int no = 1;
            client_details client;
            if ((packet_bytes = recv(new_fd, &client, PACKET_SIZE, 0)) == -1)
            {
                log_err("recv client details");
                exit(1);
            }
            msg_client_cpid("Size of recieved packet: %d", packet_bytes);
            if (auth_match(auths, client.user, client.pass) == 0)
            { // all good, send good response and continue
                msg_client_cpid("Valid client details");
                msg_client_cpid("Sending authentication approval");
                if (send(new_fd, &yes, sizeof(yes), 0) == -1)
                {
                    log_err("send good auth");
                    exit(1);
                }
                msg_server_cpid("Size of sent packet: %d", (int)sizeof(yes))
            } else
            { // not good, send bad response and close socket
                msg_client_cpid("Invalid client details");
                msg_client_cpid("Sending authentication disaproval");
                if (send(new_fd, &no, sizeof(no), 0) == -1)
                { // client knows '1' is bad
                    log_err("send bad auth");
                }
                msg_server_cpid("Size of sent packet: %d", (int)sizeof(no))
                msg_client_cpid("Closing connection");
                close(new_fd);
                exit(0);
            } // continue with battings query...
            //
            // --------------------------------------

            // --------------------------------------
            // Checking for player name / handling valid request
            while(1)
            {
                player_stats *player;
                char input[PACKET_SIZE / 4]; // smaller size for name
                if ((packet_bytes = recv(new_fd, input, PACKET_SIZE, 0)) == -1)
                {
                    log_err("recv player name");
                    exit(1);
                }
                msg_client_cpid("Size of recieved packet: %d", packet_bytes);
                msg_client_cpid("Player name recieved: %s", input)
                // input[packet_bytes] = '\0';
                if (strcmp(input, "q") == 0)
                { // close connection if client quits
                    msg_client_cpid("User has chosen to quit");
                    msg_client_cpid("Closing connection");
                    close(new_fd);
                    exit(0);
                }
                if ((player = search_player(scores, input)) == NULL)
                { // player name not found
                    msg_server("Player %s was not found", input);
                    if (send(new_fd, &no, sizeof(no), 0) == -1)
                    { // client checks recv size, then if 1
                        log_err("send bad player name");
                        exit(1);
                    }
                    msg_server_cpid("Size of sent packet: %d", (int)sizeof(no))
                } else
                { // player name found
                    msg_server("Player %s was found", player->name);
                    // TODO: add task 2 from asgn spec
                    // I think this does it?1
                    msg_server("Player %s queried %d times", player->name, ++player_queries[scores->last_found_index]);
                    ;
                    if (send(new_fd, &yes, sizeof(yes), 0) == -1)
                    {
                        log_err("send good player name");
                        exit(1);
                    }
                    msg_server_cpid("Size of sent packet: %d", (int)sizeof(yes))
                    if (send(new_fd, player, sizeof(*player), 0) == 0)
                    {
                        log_err("send player_stats");
                        exit(1);
                    }
                    msg_server_cpid("Size of sent packet: %d", (int)sizeof(*player))
                }
            }
            //
            // --------------------------------------
        } else
        {
            sleep(1); // forced sequential prints. may prove problematic for further connection establishment
            msg_server("Cutting ties to client from %s", inet_ntoa(their_addr.sin_addr));
            // fflush(stdout);
            close(new_fd);
        }
    }


    msg_server("Finished batting statistics server");
    return 0;
}