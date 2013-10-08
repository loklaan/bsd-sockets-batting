// -----------------------------------------------
// File: parse.c
// Purpose: Parse a formatted score or auth file and
// return its content in a 'database' like object.
// 
// Function descriptions in the header.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "resources.h"
#include "console.h"

#define DELIMS " \t\r\n"
#define FIELD_NAME 0
#define FIELD_COUNTRY 1
#define FIELD_INNINGS 2
#define FIELD_RUNS 3
#define FIELD_NOT_OUT 4
#define FIELD_HIGHSCORE 5
#define FIELD_HIGHSCORE_N_OUT 6

// =======---------=======
//        Functions
//

scores_db *parse_scores(FILE *file)
{
	scores_db *scores = create_scores_db(count_players(file));
	player_stats *player;
	char line[LINEBUF];
	int player_count = 0;

	while (fgets(line, sizeof(line), file) != NULL)
	{
		if (line[0] != '#') // skip comments
		{
			player = create_player_stats();
			char *field;
			int field_count = 0;

			field = strtok(line, DELIMS);
			while (field != NULL)
			{
				switch (field_count)
				{
					case FIELD_NAME :
					strcpy(player->name, field);
					break;
					case FIELD_COUNTRY :
					strcpy(player->country, field);
					break;
					case FIELD_INNINGS :
					player->innings = atoi(field);
					break;
					case FIELD_RUNS :
					player->runs = atoi(field);
					break;
					case FIELD_NOT_OUT :
					player->not_out = atoi(field);
					break;
					case FIELD_HIGHSCORE :
					player->highscore = atoi(field);
					break;
					case FIELD_HIGHSCORE_N_OUT :
					player->highscore_not_out = (field[0] == 'T') ? 1 : 0;
					break;
					default :
					log_info("field_count for parse_scores is out of bounds");
					return NULL;
					break;
				}
				field = strtok(NULL, DELIMS);
				field_count++;
			}
			scores->db[player_count++] = player;
		}
	}
	rewind(file);
	return scores;
}

auths_db *parse_auths(FILE *file)
{
	auths_db *auths = create_auths_db(count_auths(file));
	client_details *details;
	char line[LINEBUF];
	int auth_count = 0;

	fgets(line, sizeof(line), file); // skip first line
	while (fgets(line, sizeof(line), file) != NULL)
	{
		details = create_client_details();
		char *field;
		int field_count = 0;

		field = strtok(line, DELIMS);
		while (field != NULL)
		{
			switch (field_count)
			{
				case 0 :
				strcpy(details->user, field);
				break;
				case 1 :
				strcpy(details->pass, field);
				break;
				default :
				log_info("field_count for parse_auths is out of bounds");
				return NULL;
				break;
			}

			field = strtok(NULL, DELIMS);
			field_count++;
		}
		auths->db[auth_count++] = details;
	}
	rewind(file);
	return auths;
}
//
// -------=========-------


// =======---------=======
//          Utils
//
int count_players(FILE *file)
{
	int count = 0;
	char line[LINEBUF];

	while (fgets(line, sizeof(line), file) != NULL)
	{
		count += ((line[0] != '#') && (line[0] != '\0')) ? 1 : 0;
	}
	rewind(file);
	return count;
}

int count_auths(FILE *file)
{
	int count = 0;
	char line[LINEBUF];

	fgets(line, sizeof(line), file); // skip first line
	while (fgets(line, sizeof(line), file) != NULL)
	{
		count ++;
	}
	rewind(file);
	return count;
}
//
// -------=========-------
