// -----------------------------------------------
// File: parse.h
// Purpose: Parse a formatted score or auth file and
// return its content in a 'database' like object.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#ifndef PARSE_H_
#define PARSE_H_

#include <stdio.h>
#include "resources.h"

#define LINEBUF 124

/*
Returns a fixed size scores_db of a formatted scores
parsed from a text file.

PRE: file must be opened
POST: Returns a scores_db of information parsed
      If parsing failed, returns NULL
 */
scores_db *parse_scores(FILE *file);

/*
Returns a fixed size auths_db of formatted authentications
parsed from a text file.

PRE: file must be opened
POST: Returns a auths_db of information parsed
      If parsing failed, returns NULL
 */
auths_db *parse_auths(FILE *file);

/*
Searches score_db for a player_stats instance with matching member name

PRE: scores_db must be initialized
POST: Returns the matching player_stats, or a NULL if not found
 */
player_stats *search_player(scores_db *scores, char *name);

/*
Matches authentication details to details in the auths_db.

PRE: auths_db must be initialized
POST: Returns 0 if user and pass match an auths_db entry,
      else returns 1 if not found.
 */
int auth_match(auths_db *auths, char *user, char *pass);

// --------------
// Util Functions
//
/*
Counts the number of lines that have player stats.

PRE: file must be opened.
POST: the number of times a stats line occured is return
 */
int count_players(FILE *file);

/*
Counts the number of lines that have authentication details.

PRE: file must be opened.
POST: the number of times an authentication entry occured is return
 */
int count_auths(FILE *file);

#endif