// -----------------------------------------------
// File: parse.h
// Purpose: Parse a formatted score or auth file and
// return its content in a struct.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#ifndef PARSE_H_
#define PARSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define LINEBUF 124
#define FIELDBUF 32

typedef linked_list scores_db;
typedef linked_list auths_db;

typedef struct player_stats_s
{
    char *name;
    char *country;
    int innings;
    int runs;
    int not_out;
    int highscore;
    int highscore_not_out;
} *player_stats;

typedef struct client_details_s
{
    char *user;
    char *pass;
} *client_details;

/*
Returns a filled scores_db of a formatted scores text file.

PRE: 
POST: Returns a scores_db of information parsed
      If parsing failed, returns NULL
 */
scores_db parse_scores(FILE *file);

/*
Returns a filled auths_db of a formatted authentication text file.

PRE:
POST: Returns a auths_db of information parsed
      If parsing failed, returns NULL
 */
auths_db parse_auths(FILE *file);

/*
Intializes a player_stats variable.

PRE: Declared variable
POST: Returns an intialized player_stats with members nulled or zeroed
      Returns NULL if error occured
 */
player_stats create_player_stats(void);

/*
Frees memory that was allocated to an instance of player_stats.

PRE: player_stats must have been initialized
POST: player_stats allocated memory and pointer freed
 */
void destroy_player_stats(player_stats stats);

/*
Intializes a client_details variable.

PRE: Declared variable
POST: Returns an intialized client_details with members nulled or zeroed
      Returns NULL if error occured
 */
client_details create_client_details(void);

/*
Frees memory that was allocated to an instance of client_details.

PRE: client_details must have been initialized
POST: client_details allocated memory and pointer freed
 */
void destroy_client_details(client_details details);

/*
Searches score_db for a player_stats instance with matching member name

PRE: scores_db must be initialized
POST: Returns the matching player_stats, or a NULL if not found
 */
player_stats search_player(scores_db scores, char *name);

/*
Matches authentication details to details in the auths_db.

PRE: auths_db must be initialized
POST: Returns 0 if user and pass match an auths_db entry,
      else returns 1 if not found.
      Returns -1 if error occured.
 */
int auth_match(auths_db auths, char *user, char *pass);

// --------------
// Util Functions
//
void print_scores_db(scores_db scores);
void print_player_stats(player_stats stats);

#endif