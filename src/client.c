// -----------------------------------------------
// File: client.c
// Purpose: Client implementation for the Batting Stats system.
// Uses BSD sockets.
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

#define PORT 42424
#define PACKET_SIZE 256
#define HOST "127.0.0.1"
#define YES 0
#define NO 1
#define FALSE 1
#define ERROR -1

/*
Client connects to a server will ask for user Authentication details,
that will be sent to and verified by the server. The connection will
be closed and the client will exit if authentication details are invalid.

Client then can query any number of players by entering their correct name.
Client can quit at this time by entering the character 'q'.
 */
int main(int argc, char const *argv[])
{
    msg_client("Started batting statistics client");

    // initialise socket objects
    int client_s, packet_bytes;
    struct hostent *he;
    struct sockaddr_in server_addr;
    if ((he = gethostbyname(HOST)) == NULL)
    {
        log_err("gethostbyname");
        exit(FALSE);
    }

    if ((client_s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        log_err("socket");
        exit(FALSE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *) he->h_addr);
    bzero(&(server_addr.sin_zero), 8);
    if (connect(client_s, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1)
    {
        if (errno == ECONNREFUSED)
        {
            msg_server("Connection refused, shutting down client");
            sleep(5);
            exit(FALSE);
        } else {
        log_err("connect");
        exit(FALSE);
        }
    }
    msg_client("Connected to server at %s", inet_ntoa(server_addr.sin_addr));

    // ----------------------------
    // Enter/check authentication details
    char input[PACKET_SIZE / 4];
    int boolean = ERROR;
    client_details *details;
    details = create_client_details();

    printf("\nEnter client authentication details.\n");
    printf("User: ");
    scanf("%s", input);
    strcpy(details->user, input);
    input[0] = '\0';
    printf("Pass: ");
    scanf("%s", input);
    printf("\n");
    strcpy(details->pass, input);
    input[0] = '\0';

    dbg_client("Sending client authentication details");
    if (send(client_s, details, sizeof(*details), 0) == -1)
    {
        log_err("send client details");
        exit(FALSE);
    }
    dbg_client("Size of sent packet: %d", (int)sizeof(details));
    destroy_client_details(details);
    
    
    if ((packet_bytes = recv(client_s, &boolean, PACKET_SIZE, 0)) == -1)
    {
        log_err("recv auth validity");
        exit(FALSE);
    }
    dbg_client("Size of recieved packet: %d", packet_bytes);

    if (boolean == NO)
    {
        msg_server("Invalid client authentication details");
        msg_client("Quitting client");
        close(client_s);
        return 0;
    } else if (boolean == YES)
    {
        msg_server("Valid client authentication details.");
    }
    boolean = ERROR;
    // END Enter/check authentication details
    // --------------------------------------

    // ----------------------
    // Query for player stats
    char *quit = "q";
    while(1)
    {
        input[0] = '\0';
        printf("\nEnter a players name, or q to quit: ");
        scanf("%s", input);

        if (send(client_s, &input, sizeof(input), 0) == -1)
        {
            log_err("send player name / quit input");
            exit(FALSE);
        }
        dbg_client("Size of sent packet: %d", (int)sizeof(input));

        // Close client connection and break to exit, if user quits
        if (strcmp(input, quit) == 0)
        {
            msg_client("User has chosen to quit");
            close(client_s);
            break;
        }
        if ((packet_bytes = recv(client_s, &boolean, PACKET_SIZE, 0)) == -1)
        {
            log_err("recv player name validity");
            exit(FALSE);
        }
        dbg_client("Size of recieved packet: %d", packet_bytes);

        // receive the server-found player stats and print them
        if (boolean == YES)
        {
            dbg_server("Player %s was found", input);

            player_stats *stats;
            stats = create_player_stats();

            // handshake with server BEFORE recieving player stats
            if (send(client_s, &boolean, sizeof(boolean), 0) == -1)
            {
                log_err("send player stats handshake");
                exit(FALSE);
            }
            dbg_client("Size of sent packet: %d", (int)sizeof(input));

            if ((packet_bytes = recv(client_s, stats, PACKET_SIZE, 0)) == -1)
            {
                log_err("recv player stats");
                exit(FALSE);
            }
            dbg_client("Size of recieved packet: %d", packet_bytes);

            print_player_stats(stats);
            destroy_player_stats(stats);
        } else {
            msg_server("Player %s was not found", input);
        }
    }
    // END Query for player stats
    // --------------------------
    
    msg_client("Closing client");
    return 0;
}
