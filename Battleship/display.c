

#include "header.h"

// Functions to display data on console



void  display_welcome_screen(void) {
	char answer;
	printf("Rules of the Game\n");
	printf("This is a two player game. Player1 is you and Player2 is the computer. \nPick a pair of numbers (0-9) to guess where the enemy's ships are\n ");
	printf("Hit enter to start the game.\n");
	scanf_s("%c", &answer, 1);
}



void display_target_queue(struct player_data* player) {
	if ( (player->strategy == 3) || (player->strategy == 4)){
		int items_in_queue = (player->target_queue.write_ptr - player->target_queue.read_ptr + player->target_queue.size) % 100;
		printf("%s%s's target queue (r=%d w=%d l=%d): ", ERASE_TO_EOL, player->name, player->target_queue.read_ptr, player->target_queue.write_ptr, items_in_queue);
		for (int i = 0; i < items_in_queue; i++) {
			int ptr = (i + player->target_queue.read_ptr) % 100;
			printf("  %d %d", player->target_queue.queue[ptr][0], player->target_queue.queue[ptr][1]);
		}
		printf("\n");
	}
}



void display_stats(struct player_data* player_1, struct player_data* player_2, int rounds_played) {
	double success_rate = 0.0;

	printf("%sRounds played: %d\n", ERASE_TO_EOL, rounds_played);
	if (player_1->total_hits + player_1->total_misses > 0) success_rate = (double)player_1->total_hits / (player_1->total_hits + player_1->total_misses) * 100;
	printf("%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %2.0f%%\n", player_1->name, player_1->strategy, player_1->total_hits, player_1->total_misses, success_rate);

	success_rate = 0.0;
	if (player_2->total_hits + player_2->total_misses > 0) success_rate = (double)player_2->total_hits / (player_2->total_hits + player_2->total_misses) * 100;
	printf("%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %2.0f%%\n", player_2->name, player_2->strategy, player_2->total_hits, player_2->total_misses, success_rate);
}



void display_boards(struct player_data* shooting_player, struct player_data* target_player, int hidden) {


	char radar[10][10];
	fill_in_radar(radar, shooting_player, target_player);

	/*
#define BG_sea				"\x1b[44m"
#define FG_sea_unknown		"\x1b[1;30m"
#define FG_sea_hit			"\x1b[1;32m"
#define FG_sea_missed		"\x1b[1;33m"
#define axis_label			"\x1b[46m\x1b[1;30m"
#define BG_black			"\x1b[40m"
*/

	printf("%s's board:       %s's history      %s's Radar\n", target_player->name, shooting_player->name, shooting_player->name);
	printf("%s  0 1 2 3 4 5 6 7 8 9 %s    %s  0 1 2 3 4 5 6 7 8 9 %s    %s  0 1 2 3 4 5 6 7 8 9 \n", axis_label, BG_black, axis_label, BG_black, axis_label);
	for (int i = 0; i < 10; i++) {
		printf("%s%d%s%s ", axis_label, i, BG_sea, FG_sea_unknown);
		for (int j = 0; j < 10; j++) {
			char ch = target_player->game_board[i][j];
			if (hidden == 0) {
				switch (ch) {
				case '*':  printf("%s%c%s ", FG_sea_hit, ch, FG_sea_unknown); break;
				case 'm':  printf("%s%c%s ", FG_sea_missed, '*', FG_sea_unknown); break;
				case 'b':
				case 'c':
				case 'r':
				case 's':
				case 'd':
				case '-': printf("%c ", ch); break;
				default :printf("%c ", ch); break;

				}

			}
			else {
				switch (ch) {

				case 'b':
				case 'c':
				case 'r':
				case 's':
				case 'd': printf("%c ", '-');
				default : printf("%c ", ch);
				}
			}
		}
		printf("%s    %s%d%s%s ", BG_black, axis_label, i, BG_sea, FG_sea_unknown);
		for (int j = 0; j < 10; j++) {
			printf("%c ", shooting_player->enemy_board[i][j]);
		}
		printf("%s    %s%d%s%s ", BG_black, axis_label, i, BG_sea, FG_sea_unknown);
		for (int j = 0; j < 10; j++) {
			char ch = radar[i][j];
			switch (ch) {
				case 'b':
				case 'c':
				case 'r':
				case 's':
				case 'd': printf("%s%c%s ", FG_sea_radar, ch, FG_sea_unknown); break;
				default: printf("%c ", ch);
			}
		}
		printf("%s\n", BG_black);
	}
}


