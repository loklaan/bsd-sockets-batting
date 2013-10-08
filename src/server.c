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
#include <pthread.h>
#include "parse.h"
#include "resources.h"
#include "console.h"

#define SCORES_FILE "res/Batting.txt"
#define AUTHS_FILE "res/Authentication.txt"
#define PORT 42420
#define CLIENT_QUEUE 30
#define PACKET_SIZE 256
#define FALSE 1

scores_db *scores;
auths_db *auths;
int *player_queries;
pthread_mutex_t mutex;

void *client_thread(void *arg);

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
    fclose(scores_f);
    fclose(auths_f);
    if ((player_queries = calloc(scores->size, sizeof(scores->last_found_index))) == NULL)
    {
        log_err("calloc player_queries");
        exit(FALSE);
    }

    // initialise socket objects
    int server_s, client_s;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len;

    // initialise thread infos
    pthread_attr_t attr;
    pthread_t threads;
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);


    if ((server_s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        log_err("socket");
        exit(FALSE);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    int reuse = 1;
    int err = setsockopt(server_s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (0 == err)
    {
        if ((err = bind(server_s, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))) == -1)
        {
            log_err("bind");
            exit(FALSE);
        }
    }

    // start listener
    if (listen(server_s, CLIENT_QUEUE) == -1)
    {
        log_err("listen");
        exit(FALSE);
    }

    msg_server("Listening on port %d", PORT);
    while (1)
    {
        fflush(stdout);
        addr_len = sizeof(struct sockaddr_in);
        if ((client_s = accept(server_s, (struct sockaddr *) &client_addr, &addr_len)) == -1)
        {
            log_err("accept");
            exit(FALSE);
        } else {
            msg_server("Accepted client connection from %s", inet_ntoa(client_addr.sin_addr));

            pthread_create(&threads, &attr, client_thread, &client_s);
        }
    }

    msg_server("Finished batting statistics server");
    return 0;

}

void *client_thread(void *arg)
{
    int thread_s = *(int*) arg;
    int packet_bytes = 0;
    dbg_client_id("New client process");

    // --------------------------------------
    // Checking Authentication / handling
    int yes = 0;
    int no = 1;
    client_details client;
    if ((packet_bytes = recv(thread_s, &client, PACKET_SIZE, 0)) == -1)
    {
        log_err("recv client details");
        pthread_exit(NULL);
    }
    dbg_client_id("Size of recieved packet: %d", packet_bytes);
    if (auth_match(auths, client.user, client.pass) == 0)
    { // all good, send good response and continue
        dbg_client_id("Valid client details");
        dbg_client_id("Sending authentication approval");
        if (send(thread_s, &yes, sizeof(yes), 0) == -1)
        {
            log_err("send good auth");
            pthread_exit(NULL);
        }
        dbg_client_id("Size of sent packet: %d", (int)sizeof(yes));
    } else { // not good, send bad response and close socket
        msg_client_id("Invalid client details");
        dbg_client_id("Sending authentication disaproval");
        if (send(thread_s, &no, sizeof(no), 0) == -1)
        { // client knows '1' is bad
            log_err("send bad auth");
        }
        dbg_client_id("Size of sent packet: %d", (int)sizeof(no));
        msg_server("Closing connection from client %d", (int) pthread_self());
        close(thread_s);
        pthread_exit(NULL);
    } // continue with battings query...
    //
    // --------------------------------------

    // --------------------------------------
    // Checking for player name / handling valid request
    while(1)
    {
        player_stats *player;
        char input[PACKET_SIZE / 4]; // smaller size for name
        if ((packet_bytes = recv(thread_s, input, PACKET_SIZE, 0)) == -1)
        {
            log_err("recv player name");
            pthread_exit(NULL);
        }
        dbg_client_id("Size of recieved packet: %d", packet_bytes);
        dbg_client_id("Player name recieved: %s", input);
        // input[packet_bytes] = '\0';
        if (strcmp(input, "q") == 0)
        { // close connection if client quits
            msg_client_id("User has chosen to quit");
            msg_server("Closing connection from client %d", (int) pthread_self());
            close(thread_s);
            pthread_exit(NULL);
        }
        if ((player = search_player(scores, input)) == NULL)
        { // player name not found
            dbg_client_id("Player '%s' was not found", input);
            if (send(thread_s, &no, sizeof(no), 0) == -1)
            { // client checks recv size, then if 1
                log_err("send bad player name");
                pthread_exit(NULL);
            }
            dbg_client_id("Size of sent packet: %d", (int)sizeof(no));
        } else { // player name found
            dbg_client_id("Player '%s' was found", player->name);
            pthread_mutex_lock(&mutex);
            msg_server("Player %s queried %d times", player->name, ++player_queries[scores->last_found_index]);
            pthread_mutex_unlock(&mutex);
            ;
            if (send(thread_s, &yes, sizeof(yes), 0) == -1)
            {
                log_err("send good player name");
                pthread_exit(NULL);
            }
            dbg_client_id("Size of sent packet: %d", (int)sizeof(yes));
            if (send(thread_s, player, sizeof(*player), 0) == 0)
            {
                log_err("send player_stats");
                pthread_exit(NULL);
            }
            dbg_client_id("Size of sent packet: %d", (int)sizeof(*player));
        }
    }
    pthread_exit(NULL);
}