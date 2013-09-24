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
#include "resources.h"
#include "dbg.h"

// =======---------=======
//        Functions
//

scores_db *create_scores_db(int size)
{
    scores_db *scores;
    if ((scores = malloc(sizeof(scores_db))) == NULL)
    {
        log_err("malloc");
        return NULL;
    }
    if ((scores->db = malloc(size * sizeof(player_stats *))) == NULL)
    {
        log_err("malloc");
        return NULL;
    }
    for (int i = 0; i < size; ++i)
    {
        scores->db[i] = NULL;
    }
    scores->size = size;

    return scores;
}

auths_db *create_auths_db(int size)
{
    auths_db *auths;
    if ((auths = malloc(sizeof(auths_db))) == NULL)
    {
        log_err("malloc");
        return NULL;
    }
    if ((auths->db = malloc(size * sizeof(client_details *))) == NULL)
    {
        log_err("malloc");
        return NULL;
    }
    for (int i = 0; i < size; ++i)
    {
        auths->db[i] = NULL;
    }
    auths->size = size;

    return auths;
}

void destroy_scores_db(scores_db *scores)
{
    for (int i = 0; i < scores->size; ++i)
    {
        destroy_player_stats(scores->db[i]);
    }
    free(scores->db);
    free(scores);
}

void destroy_auths_db(auths_db *auths)
{
    for (int i = 0; i < auths->size; ++i)
    {
        destroy_client_details(auths->db[i]);
    }
    free(auths->db);
    free(auths);
}

player_stats *create_player_stats(void)
{
    player_stats *stats;
    if ((stats = malloc(sizeof(player_stats))) == NULL)
    {
        log_err("malloc");
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

void destroy_player_stats(player_stats *stats)
{
    if (stats->name != NULL)
        free(stats->name);

    if (stats->country != NULL)
        free(stats->country);

    free(stats);
}

client_details *create_client_details(void)
{
    client_details *details;
    if ((details = malloc(sizeof(client_details))) == NULL)
    {
        log_err("malloc");
        return NULL;
    }
    details->user = NULL;
    details->pass = NULL;

    return details;
}

void destroy_client_details(client_details *details)
{
    if (details->user != NULL)
        free(details->user);

    if (details->pass != NULL)
        free(details->pass);

    free(details);
}

// === Utils ===
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

void print_player_stats(player_stats *stats)
{
    printf("\nPlayer stats for %s:\n", stats->name);
    printf("\tCountry: %s\n", stats->country);
    printf("\tInnings: %d\n", stats->innings);
    printf("\tAvg Runs: %d\n", (stats->runs / stats->innings));
    printf("\tBest Run: %d\n", stats->highscore);
    printf("\tTotal Runs: %d\n", stats->runs);
    printf("\tNot Outs: %d\n", stats->not_out);
    char *best_not_out = (stats->highscore_not_out == 1) ? "TRUE" : "FALSE";
    printf("\tBest Run was Not Out: %s\n", best_not_out);
}

void print_client_details(client_details *details)
{
    printf("\nValid authentication:\n");
    printf("\tUser: %s\n", details->user);
    printf("\tPass: %s\n", details->pass);
}

//
// -------=========-------