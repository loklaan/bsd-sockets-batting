// -----------------------------------------------
// File: parse.h
// Purpose: Parse a formatted score or auth file and
// return its content in a struct.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "parse.h"

#define MAXBUF 64
#define DELIM '\t'
#define SCORE_NAME 0
#define SCORE_COUNTRY 12
#define SCORE_INNINGS 16
#define SCORE_RUNS 20
#define SCORE_NOT_OUT 24
#define SCORE_HIGHSCORE 28
#define SCORE_HIGHSCORE_NOT_OUT 32

/*
Returns a scores_db of a formatted scores text file.

PRE: 
 */
scores_db parse_scores(FILE *file)
{
	scores_db scores;
	init_list((linked_list) scores);
	char line[MAXBUF];

	while (fgets(line, sizeof(line), file) != NULL) // get line
	{
		player_stats stats;
		stats = init_player_stats();
		char *field;
		field = (char*) malloc(sizeof(char[64]));
		int field_count = 0;

		if (line[0] != '#') // skip commented lines
		{
			for (int i = 0; i < (strlen(line)+1); ++i)
			{
				if (((int)line[i] < 41) && ((int)strlen(field) != 0))
				{
					switch (field_count)
					{
						case 0 :
						stats->name = field;
						break;
						case 1 :
						stats->country = field;
						break;
						case 2 :
						stats->innings = atoi(field);
						break;
						case 3 :
						stats->runs = atoi(field);
						break;
						case 4 :
						stats->not_out = atoi(field);
						break;
						case 5 :
						stats->highscore = atoi(field);
						break;
						case 6 :
						stats->highscore_not_out = atoi(field);
						break;
						default :
						printf("Error: field_count for parse_scores is out of bounds\n");
						break;
					}
					free(field);
					field = (char*) malloc(sizeof(char[64]));
					field_count++;
				} else
				{
					field[strlen(field)] = line[i];
					field[strlen(field) + 1] = '\0';
				}

			}
		}
		if (append_var((linked_list)scores, (void*)stats) != 0)
		{
			printf("Error: could not append score stats.\n");
			return NULL;
		}
	}

	return scores;
}

auths_db parse_auths(FILE *file)
{
	auths_db auths;
	init_list((linked_list) auths);
	char line[MAXBUF];

	fgets(line, sizeof(line), file); // skip header line
	while (fgets(line, sizeof(line), file) != NULL)
	{
		client_details details;
		details = init_client_details();
		char *field;
		field = (char*) malloc(sizeof(char[64]));
		int field_count = 0;

		for (int i = 0; i < (strlen(line)+1); ++i)
		{
			if (((int)line[i] < 41) && ((int)strlen(field) != 0))
			{
				switch (field_count)
				{
					case 0 :
					details->user = field;
					break;
					case 1 :
					details->pass = field;
					break;
					default :
					printf("Error: field_count for parse_auths is out of bounds\n");
					break;
				}
				free(field);
				field = (char*) malloc(sizeof(char[64]));
				field_count++;
			} else
			{
				field[strlen(field)] = line[i];
				field[strlen(field) + 1] = '\0';
			}

		}
		if (append_var((linked_list)auths, (void*)details) != 0)
		{
			printf("Error: could not append auth details.\n");
			return NULL;
		}
	}
}


player_stats init_player_stats(void)
{
	player_stats stats;
	stats = (player_stats) malloc(sizeof(struct player_stats_s));
	stats->name = NULL;
	stats->country = NULL;
	stats->innings = 0;
	stats->runs = 0;
	stats->not_out = 0;
	stats->highscore = 0;
	stats->highscore_not_out = 0;

	return stats;
}

void destroy_player_stats(player_stats stats)
{
	free(stats);
}

client_details init_client_details(void)
{
	client_details details;
	details = (client_details) malloc(sizeof(struct client_details_s));
	details->user = NULL;
	details->pass = NULL;

	return details;
}

void destroy_client_details(client_details details)
{
	free(details);
}

player_stats search_player(scores_db scores, char *name)
{
	if (is_list_empty((linked_list)scores))
	{
		return NULL;
	}
	node current;
	current = scores->first;

	do
	{
		if (((player_stats)(current->var))->name == name)
		{
			return (player_stats)(current->var);
		}
		current = current->next;
	} while (current != NULL);
	return NULL;
}

int auth_match(auths_db auths, char *user, char *pass)
{
	if (is_list_empty((linked_list)auths))
	{
		return 1;
	}

	node current;
	current = auths->first;

	do
	{
		if ((((client_details)current->var)->user == user) && (((client_details)current->var)->pass == pass))
		{
			return 0;
		}
		current = current->next;
	} while (current != NULL);
}
