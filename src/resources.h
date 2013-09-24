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

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <stdio.h>

// =======---------=======
//     Data Structures
//
typedef struct
{
    char *name;
    char *country;
    int innings;
    int runs;
    int not_out;
    int highscore;
    int highscore_not_out;
} player_stats;

typedef struct
{
    char *user;
    char *pass;
} client_details;

typedef struct
{
    player_stats **db;
    int size;
} scores_db;

typedef struct
{
    client_details **db;
    int size;
} auths_db;
//
// -------=========-------

// =======---------=======
//        Functions
//

/*
Allocated space for the 'databases' based on some fixed size.
 */
scores_db *create_scores_db(int size);
auths_db *create_auths_db(int size);

/*
Frees space used by the 'databases'.
 */
void destroy_scores_db(scores_db *scores);
void destroy_auths_db(auths_db *auths);

/*
Intializes a player_stats variable.

PRE: Declared variable
POST: Returns an intialized player_stats with members nulled or zeroed
      Returns NULL if error occured
 */
player_stats *create_player_stats(void);

/*
Frees memory that was allocated to an instance of player_stats.

PRE: player_stats must have been initialized
POST: player_stats allocated memory and pointer freed
 */
void destroy_player_stats(player_stats *stats);

/*
Intializes a client_details variable.

PRE: Declared variable
POST: Returns an intialized client_details with members nulled or zeroed
      Returns NULL if error occured
 */
client_details *create_client_details(void);

/*
Frees memory that was allocated to an instance of client_details.

PRE: client_details must have been initialized
POST: client_details allocated memory and pointer freed
 */
void destroy_client_details(client_details *details);

// === Utils ===
void print_player_stats(player_stats *stats);
void print_client_details(client_details *details);

//
// -------=========-------

#endif /* RESOURCES_H_ */