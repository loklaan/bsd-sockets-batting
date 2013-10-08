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

// =======---------=======
//        Functions
//

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
//
// -------=========-------


// =======---------=======
//          Utils
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
//
// -------=========-------

#endif /* PARSE_H_ */