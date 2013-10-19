// -----------------------------------------------
// File: server.c
// Purpose: Server implementation for the Batting Stats system.
// Uses BSD sockets with pthreads for multithreaded performance.
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
#define PORT 42424
#define CLIENT_MAX 30
#define SERVER_SLEEP 5
#define PACKET_SIZE 256
#define FALSE 1
#define ERROR -1

scores_db *scores;
auths_db *auths;
int client_connects = 0;
int yes = 0, no = 1; // easier to read send/recv functions
pthread_mutex_t m_scores_db, m_connects;

// Function descriptions below
void server_listen(int sockfd);
void *client_thread(void *arg);
void exit_client_thread(void);

/*
Server will accept any client that, after connecting, sends correct
authentication details. The server will the dish out batting statistics
for players that clients query for.

The server has a maximum limit for concurrent client connections. Any client
that tries to connect after the limit is reached, will close itself.
 */
int main(int argc, char const *argv[])
{
    msg_server("Started batting statistics server");

    // initialise & parse files
    FILE *scores_f;
    FILE *auths_f;
    scores_f = fopen(SCORES_FILE, "r");
    auths_f = fopen(AUTHS_FILE, "r");
    scores = parse_scores(scores_f);
    auths = parse_auths(auths_f);
    fclose(scores_f);
    fclose(auths_f);

    // initialise thread infos
    pthread_attr_t attr;
    pthread_t threads;
    pthread_attr_init(&attr);
    pthread_mutex_init(&m_scores_db, NULL);
    pthread_mutex_init(&m_connects, NULL);

    // initialise socket objects
    int server_s, client_s;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len;

    if ((server_s = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
    {
        log_err("socket");
        exit(FALSE);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    int reuse = 1;
    if (setsockopt(server_s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0)
    {
        log_err("setsockopt");
        exit(FALSE);
    } else {
        if ((bind(server_s, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))) == -1)
        {
            log_err("bind");
            exit(FALSE);
        }
    }

    server_listen(server_s);
    while (1)
    {
        if (client_connects >= CLIENT_MAX)
        {
            // closes incoming reception to the once passive socket
            shutdown(server_s, SHUT_RD);
            while (client_connects >= CLIENT_MAX)
            {
                dbg_server("Max client connections, sleeping for %d seconds", (int)SERVER_SLEEP);
                sleep(SERVER_SLEEP);
            }
            // reopens passive socket for listening
            server_listen(server_s);
        } else {
            fflush(stdout); // prevents printing errors
            addr_len = sizeof(struct sockaddr_in);
            if ((client_s = accept(server_s, (struct sockaddr *) &client_addr, &addr_len)) == -1)
            {
                log_err("accept");
                exit(FALSE);
            } else {
                msg_server("Accepted client connection from %s", inet_ntoa(client_addr.sin_addr));
                pthread_mutex_lock(&m_connects);
                client_connects++;
                dbg_server("Number of connected clients: %d", client_connects);
                pthread_mutex_unlock(&m_connects);
                // moves off the per-client work to a new thread
                // each thread is responsible for closing itself
                pthread_create(&threads, &attr, client_thread, &client_s);
            }
        }
    }

    // will never actually close (not implemented)
    close(server_s);
    destroy_scores_db(scores);
    destroy_auths_db(auths);
    msg_server("Finished batting statistics server");
    return 0;

}

/*
Listens for connections on sockfd for the server.

PRE: sockfd must be initialised with appropriate socket functions,
     socket(), setsockopt(), bind()
POST: Marks sockfd as a passive socket to accept incoming connection requests
 */
void server_listen(int sockfd)
{
    if (listen(sockfd, 0) == -1)
    {
        log_err("listen");
        exit(FALSE);
    }
    msg_server("Listening on port %d", PORT);
}

/*
A thread function for handling queries that the client makes to the server.

PRE: The socket, that is the parameter, must be initialised and have
     successfully connected to some client with accept()
POST: Gracefully exits the thread once user inputs 'quit', or any error occurs
 */
void *client_thread(void *arg)
{
    int thread_s = *(int*) arg;
    pthread_mutex_lock(&m_connects);
    int client_id = client_connects;
    pthread_mutex_unlock(&m_connects);
    int packet_bytes = 0;

    dbg_client_id(client_id, "New client process");

    // -----------------------------
    // Authentication check [Task 3]
    client_details client;
    if ((packet_bytes = recv(thread_s, &client, PACKET_SIZE, 0)) == -1)
    {
        log_err("recv client details");
        exit_client_thread();
    } else if (packet_bytes == 0)
    {
        msg_client_id(client_id, "Received shutdown");
        exit_client_thread();
    }
    dbg_client_id(client_id, "Size of received packet: %d", packet_bytes);

    // auth is good, send approval and continue
    if (auth_match(auths, client.user, client.pass) == 0)
    {
        dbg_client_id(client_id, "Valid client details");
        dbg_client_id(client_id, "Sending authentication approval");
        if (send(thread_s, &yes, sizeof(yes), 0) == -1)
        {
            log_err("send good auth");
            exit_client_thread();
        }
        dbg_client_id(client_id, "Size of sent packet: %d", (int)sizeof(yes));
    // auth is bad, send disapproval and close socket
    } else {
        msg_client_id(client_id, "Invalid client details");
        dbg_client_id(client_id, "Sending authentication disapproval");
        if (send(thread_s, &no, sizeof(no), 0) == -1)
        {
            log_err("send auth disapproval");
            exit_client_thread();
        }
        dbg_client_id(client_id, "Size of sent packet: %d", (int)sizeof(no));
        msg_server("Closing connection from Client %d", client_id);
        close(thread_s);
        exit_client_thread();
    }
    // END Authentication check [Task 3]
    // ---------------------------------

    // --------------------------------
    // Player query check
    while(1)
    {
        player_stats *player;
        char input[PACKET_SIZE / 4]; // player names should be small

        if ((packet_bytes = recv(thread_s, input, PACKET_SIZE, 0)) == -1)
        {
            log_err("recv player name");
            exit_client_thread();
        } else if (packet_bytes == 0)
        {
            msg_client_id(client_id, "Received shutdown");
            exit_client_thread();
        }
        dbg_client_id(client_id, "Size of received packet: %d", packet_bytes);
        dbg_client_id(client_id, "Player name received: %s", input);

        // Closes connection if client inputs quit
        if (strcmp(input, "q") == 0)
        {
            msg_client_id(client_id, "User has chosen to quit");
            msg_server("Closing connection from Client %d", client_id);
            close(thread_s);
            exit_client_thread();
        }

        // Player name NOT found
        // [Task 2]
        // search_player updates query count on successful player find
        pthread_mutex_lock(&m_scores_db);
        if ((player = search_player(scores, input)) == NULL)
        {
            pthread_mutex_unlock(&m_scores_db);
            dbg_client_id(client_id, "Player '%s' was not found", input);

            if (send(thread_s, &no, sizeof(no), 0) == ERROR)
            {
                log_err("send player name disapproval");
                exit_client_thread();
            }
            dbg_client_id(client_id, "Size of sent packet: %d", (int)sizeof(no));
        // Player name FOUND
        } else {
            dbg_client_id(client_id, "Player '%s' was found", player->name);
            // print the precious player query count [Task 2]
            msg_server("Player %s queried %d times", player->name, player->queries);
            pthread_mutex_unlock(&m_scores_db);

            if (send(thread_s, &yes, sizeof(yes), 0) == ERROR)
            {
                log_err("send player name acceptance");
                exit_client_thread();
            }
            dbg_client_id(client_id, "Size of sent packet: %d", (int)sizeof(yes));

            // handshake with client BEFORE sending player stats.
            // prevents packets directly above and below being received
            // at same time by a client
            int handshake = ERROR;
            if ((packet_bytes = recv(thread_s, &handshake, PACKET_SIZE, 0)) == ERROR)
            {
                log_err("recv player stats handshake");
                exit_client_thread();
            } else if (packet_bytes == 0)
            {
                msg_client_id(client_id, "Received shutdown");
                exit_client_thread();
            }
            dbg_client_id(client_id, "Size of received packet: %d", packet_bytes);

            if (handshake == yes)
            {
                // sending the oh so preciously important player stats
                if (send(thread_s, player, sizeof(*player), 0) == ERROR)
                {
                    log_err("send player_stats");
                    exit_client_thread();
                }
                dbg_client_id(client_id, "Size of sent packet: %d", (int)sizeof(*player));
            }
        }
    }
    // END Player query check
    // ------------------------------------

    exit_client_thread();
}

void exit_client_thread(void)
{
    pthread_mutex_lock(&m_connects);
    dbg_server("Exiting client connection thread");
    client_connects--;
    dbg_server("Client connections now: %d", client_connects);
    pthread_mutex_unlock(&m_connects);
    pthread_exit(NULL);
}