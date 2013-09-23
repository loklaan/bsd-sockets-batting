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

#define DELIMS "\t\n"

scores_db parse_scores(FILE *file)
{
	scores_db scores = create_list();
	player_stats stats;
	char line[LINEBUF];
	int count = 0;

	while (fgets(line, sizeof(line), file) != NULL)
	{
		if (line[0] != '#') // skip comments
		{
			stats = create_player_stats();
			char *field;
			int field_count = 0;

			field = strtok(line, DELIMS);
			while (field != NULL)
			{
				switch (field_count)
				{
					case 0 :
					stats->name = strdup(field);
					break;
					case 1 :
					stats->country = strdup(field);
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
				field = strtok(NULL, DELIMS);
				field_count++;
			}
			append_data(scores, stats);
		}
	}
	return scores;
}

auths_db parse_auths(FILE *file)
{
	auths_db auths = create_list();
	char line[LINEBUF];

	fgets(line, sizeof(line), file); // skip header
	while (fgets(line, sizeof(line), file) != NULL)
	{
		client_details details = create_client_details();
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
				printf("Error: field_count for parse_auths is out of bounds\n");
				break;
			}

			field = strtok(NULL, DELIMS);
			field_count++;
		}
		if (append_data(auths, details) != 0)
		{
			printf("Error: could not append auth details.\n");
			return NULL;
		}
	}
	return auths;
}

player_stats create_player_stats(void)
{
	player_stats stats;
	if ((stats = (player_stats) malloc(sizeof(struct player_stats_s))) == NULL)
	{
		printf("Error: malloc for create_player_stats\n");
		return NULL;
	}
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
	if (stats->name != NULL)
		free(stats->name);

	if (stats->country != NULL)
		free(stats->country);

	free(stats);
}

client_details create_client_details(void)
{
	client_details details;
	if ((details = (client_details) malloc(sizeof(struct client_details_s))) == NULL)
	{
		printf("Error: malloc for create_client_details\n");
		return NULL;
	}
	details->user = NULL;
	details->pass = NULL;

	return details;
}

void destroy_client_details(client_details details)
{
	if (details->user != NULL)
		free(details->user);

	if (details->pass != NULL)
		free(details->pass);

	free(details);
}

player_stats search_player(scores_db scores, char *name)
{
	if (is_list_empty(scores))
	{
		printf("Empty list for searching.\n");
		return NULL;
	}
	node current;
	current = scores->head;

	do
	{
		if (strcmp(((player_stats)(current->data))->name, name))
		{
			return (player_stats)(current->data);
		}
		current = current->next;
	} while (current != NULL);
	printf("Could not find %s\n", name);
	return NULL;
}

int auth_match(auths_db auths, char *user, char *pass)
{
	if (is_list_empty((linked_list)auths))
	{
		return 1;
	}

	node current;
	current = auths->head;

	do
	{
		if ((strcmp(((client_details)(current->data))->user, user)) && (strcmp(((client_details)(current->data))->pass, pass)))
		{
			return 0;
		}
		current = current->next;
	} while (current != NULL);

	return 1;
}

void print_scores_db(scores_db scores)
{
	node temp = scores->head;
	printf("Stats:\n");
	printf("Country\tInnings\tRuns\tN.Out\tHighscore\tHighscore N.Out\n");
	while (temp != NULL)
	{
		printf("%s\n", ((player_stats)((temp)->data))->name);
		printf("%s\t", ((player_stats)((temp)->data))->country);
		printf("%d\t", ((player_stats)((temp)->data))->innings);
		printf("%d\t", ((player_stats)((temp)->data))->runs);
		printf("%d\t", ((player_stats)((temp)->data))->not_out);
		printf("%d\t", ((player_stats)((temp)->data))->highscore);
		printf("%d\n", ((player_stats)((temp)->data))->highscore_not_out);
		temp = temp->next;
	}
}

void print_player_stats(player_stats stats)
{
	printf("%s scored a total of %d runs.\n", stats->name, stats->runs);
	printf("\tMore details about %s:\n", stats->name);
	printf("Country:\t%s\n", stats->country);
	printf("Average:\t%.3g\n", (double)stats->runs / (double)stats->innings);
	printf("Highscore:\t%d\n", stats->highscore);
}