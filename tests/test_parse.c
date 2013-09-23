// tests for the parse.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parse.h"

#define SCORES_FILE "../res/Batting.txt"
#define AUTHS_FILE "../res/Authentication.txt"

scores_db scores;
auths_db auths;

void print_players(scores_db scores);

int main(int argc, char const *argv[])
{
    FILE *scores_f;
    scores_f = fopen(SCORES_FILE, "r");
    FILE *auths_f;
    auths_f = fopen(AUTHS_FILE, "r");

    scores = parse_scores(scores_f);
    auths = parse_auths(auths_f);

    player_stats stats;
    char *player = "Ponting";
    if ((stats = search_player(scores, player)) == NULL)
     {
         printf("Player %s not found. Please check your spelling and try again.\n", player);
     } else
     {
        printf("%s scored a total of %d runs.\n", player, stats->runs);
        printf("\tMore details about %s:\n", player);
        printf("Country:\t%s\n", stats->country);
        printf("Average:\t%.3g\n", (double)stats->runs / (double)stats->innings);
        printf("Highscore:\t%d\n", stats->highscore);
     }

     char *user = "Jason";
     char *pass = "222222";
     if (auth_match(auths, user, pass) == 0)
     {
         printf("Authentication for:\nUser: %s\nPass: %s\n", user, pass);
     } else
     {
        printf("Authentication fail.\n");
     }
     print_scores_db(scores);

    return 0;
}