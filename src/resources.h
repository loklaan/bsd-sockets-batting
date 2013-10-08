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

#define STR_BUFFER 64

#include <stdio.h>

// =======---------=======
//     Data Structures
//
typedef struct
{
    char name[STR_BUFFER];
    char country[STR_BUFFER];
    int innings;
    int runs;
    int not_out;
    int highscore;
    int highscore_not_out;
} player_stats;

typedef struct
{
    char user[STR_BUFFER];
    char pass[STR_BUFFER];
} client_details;

typedef struct
{
    player_stats **db;
    int last_found_index;
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
Allocates space for the 'databases' based on some fixed number
of elements

PRE:
POST: Returns a pointer to the allocated space.
If error occurs, then returns NULL
 */
scores_db *create_scores_db(int size);
auths_db *create_auths_db(int size);

/*
Frees space used by the 'databases'.

PRE: The database must exist / be initialised
POST: Database memory and pointer freed
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
//
// -------=========-------

// =======---------=======
//          Utils
//

/*
Searches score_db for a player_stats instance with matching member name

PRE: scores_db must be initialized
POST: Returns the pointer to the matching player_stats, or a NULL if not found
 */
player_stats *search_player(scores_db *scores, char *name);

/*
Matches authentication details to details in the auths_db.

PRE: auths_db must be initialized
POST: Returns 0 if user and pass match an auths_db entry,
      else returns 1 if not found.
 */
int auth_match(auths_db *auths, char *user, char *pass);

/*
Prints things.

PRE: Parameters must be initalised
POST: Prints relevant information from the parameter
 */
void print_player_stats(player_stats *stats);
void print_client_details(client_details *details);

//
// -------=========-------

#endif /* RESOURCES_H_ */