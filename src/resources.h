// -----------------------------------------------
// File: resources.h
// Purpose: Data structures and corresponding
// functions for use in server/client batting
// statistics system.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>

// =======---------=======
//     Data Structures
//
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

typedef player_stats* scores_db;
typedef client_details* auths_db;
//
// -------=========-------

// =======---------=======
//        Functions
//

/*
Allocated space for the 'databases' based on some fixed size.
 */
void init_scores_db(scores_db *scores, int size);
void init_auths_db(scores_db *scores, int size);

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

//
// -------=========-------