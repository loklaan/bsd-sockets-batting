// -----------------------------------------------
// File: resources.c
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
//        Functions
//

void init_scores_db(scores_db *scores, int size)
{
    scores_db temp;
    if ((temp = (scores_db) malloc(sizeof(struct player_stats_s) * size)) == NULL)
    {
        printf("Error: malloc in init_scores_db\n");
    }

    *scores = temp;
}

void init_auths_db(auths_db *auths, int size)
{
    auths_db temp;
    if ((temp = (auths_db) malloc(sizeof(struct client_details_s) * size)) == NULL)
    {
        printf("Error: malloc in init_auths_db\n");
    }

    *auths = temp;
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

//
// -------=========-------