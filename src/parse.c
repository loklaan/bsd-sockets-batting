// -----------------------------------------------
// File: parse.c
// Purpose: Parse a formatted score or auth file and
// return its content in a 'database' like object.
// 
// Function details in the header.
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
#include "dbg.h"

#define DELIMS "\t\n"

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
					case 0 :
					player->name = strdup(field);
					break;
					case 1 :
					player->country = strdup(field);
					break;
					case 2 :
					player->innings = atoi(field);
					break;
					case 3 :
					player->runs = atoi(field);
					break;
					case 4 :
					player->not_out = atoi(field);
					break;
					case 5 :
					player->highscore = atoi(field);
					break;
					case 6 :
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
				details->user = strdup(field);
				break;
				case 1 :
				details->pass = strdup(field);
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

player_stats *search_player(scores_db *scores, char *name)
{
	// linear search
	for (int i = 0; i < scores->size; ++i)
	{
		if (strcmp((scores->db[i])->name, name))
		{
			return scores->db[i];
		}
	}

	return NULL;
}

int auth_match(auths_db *auths, char *user, char *pass)
{
	// linear search
	for (int i = 0; i < auths->size; ++i)
	{
		if (strcmp((auths->db[i])->user, user) && strcmp((auths->db[i])->pass, pass))
		{
			return 0;
		}
	}

	return 1;
}

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
