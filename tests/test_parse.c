// tests for parse.h (and resources.h)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parse.h"

#define SCORES_FILE "../res/Batting.txt"
#define AUTHS_FILE "../res/Authentication.txt"

scores_db *scores;
auths_db *auths;

void print_players(scores_db scores);

int main(int argc, char const *argv[])
{
    printf("\n...Opening files\n");
    FILE *scores_f;
    scores_f = fopen(SCORES_FILE, "r");
    FILE *auths_f;
    auths_f = fopen(AUTHS_FILE, "r");

    printf("...Parsing scores\n");
    scores = parse_scores(scores_f);
    printf("...Parsing auths\n");
    auths = parse_auths(auths_f);

    player_stats *stats;
    char *player = "Ponting";
    printf("\n...Searching for player: %s\n", player);
    if ((stats = search_player(scores, player)) == NULL)
    {
       printf("Player %s not found. Please check your spelling and try again.\n", player);
   } else
   {
    print_player_stats(stats);
    }

    client_details details;
    details.user = "Jason";
    details.pass = "222222";
    printf("\n...Matching authentication for: %s - %s\n", details.user, details.pass);
    if (auth_match(auths, details.user, details.pass) == 0)
    {
        print_client_details(&details);
    } else
    {
        printf("Authentication fail.\n");
    }

    printf("\n...Destroying 'databases'\n");
    destroy_scores_db(scores);
    destroy_auths_db(auths);

    fclose(scores_f);
    fclose(auths_f);
    printf("\n...Closing files");

    return 0;
}