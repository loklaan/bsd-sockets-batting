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
POST: NULL if error occured.
 */
scores_db parse_scores(FILE *file);

/*
Returns a filled auths_db of a formatted authentication text file.

PRE:
POST: Null if error occured.
 */
auths_db parse_auths(FILE *file);

/*
Returns an initialised player_stats type.
 */
player_stats init_player_stats(void);

/*
Frees memory that was allocated to an instance of player_stats.
 */
void destroy_player_stats(player_stats stats);

/*
Returns an initialised client_details type.
 */
client_details init_client_details(void);

/*
Frees memory that was allocated to an instance of client_details.
 */
void destroy_client_details(client_details details);

/*
Searchs a score_db for a player name.

Returns a player_stats if found, otherwise returns NULL.
 */
player_stats search_player(scores_db scores, char *name);

/*
Matches authentication details to details in the auths_db.

 */
int auth_match(auths_db auths, char *user, char *pass);

#endif