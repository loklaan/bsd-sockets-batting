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
#include "parse.h"

#define SCORES_FILE "res/Batting.txt"
#define AUTHS_FILE "res/Authentication.txt"

scores_db scores;
auths_db auths;

int listener(int socket_descr);

int main(int argc, char const *argv[])
{
    printf("Started batting statistics server...\n");

    // initialise sockets
    // initialise files / parsed objects
    FILE *score_file;
    FILE *auth_file;
    score_file = fopen(SCORES_FILE);
    auth_file = fopen(AUTHS_FILE);
    scores parse_scores(score_file);
    // start listener
    if (listener == ERROR)
    {
        printf("error: listener function\n");
    }

    printf("Finished batting statistics server...\n");
    return 0;
}