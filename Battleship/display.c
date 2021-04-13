

#include "header.h"

// Functions to display data on console


// DISPLAY OFFSETS
#define display_offset_tournament_line	0		// 0: Tournament data
#define display_offset_round_line		1		// 1: Round #
#define display_offset_dashes			2		// 2: ---------------------------------
#define display_offset_target_queue		3		// 3: Target queue
#define display_offset_reccomended_target 4		// 4: Recommended Target
#define display_offset_firing_result	 5		// 5: 
#define display_offset_boards			 6		// 6: Boards
#define display_offset_baoards			 6		// 6: Boards

void  display_welcome_screen(void) {
	char answer;
	printf("Rules of the Game\n");
	printf("This is a two player game. Player1 is you and Player2 is the computer. \nPick a pair of numbers (0-9) to guess where the enemy's ships are\n ");
	printf("Hit enter to start the game.\n");
	scanf_s("%c", &answer, 1);
}


void display_tournament_line(struct player_data* player_1, struct player_data* player_2, int GAMES, int p1_wins, int p2_wins, int ties) {
	char POSITION_CURSOR[20];
	strcpy_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%0;0H");

	double p1winpercentage = 0.00; if (GAMES>0) p1winpercentage = p1_wins / (p1_wins + p2_wins + ties) * 100;
	double p2winpercentage = 0.00; if (GAMES > 0) p2winpercentage = p2_wins / (p1_wins + p2_wins + ties) * 100;
	double tiepercentage = 0.00; if (GAMES > 0) tiepercentage = ties / (p1_wins + p2_wins + ties) * 100;
	printf("%sGAMES: %d   %s wins:%d %2.0f%%    %s wins:%d %2.0f%%  ties:%d %2.0f%%\n", POSITION_CURSOR, GAMES, player_1->name, p1_wins, p1winpercentage, player_2->name, p2_wins, p2winpercentage, ties, tiepercentage);

}






void display_round_line(int round) {

}


void display_target_queue(struct player_data* player) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", player->display_vertical_offset + 4);
	int items_in_queue = (player->target_queue.write_ptr - player->target_queue.read_ptr + player->target_queue.size) % 100;
	printf("%s%s%s's target queue (r=%d w=%d l=%d): ", POSITION_CURSOR, ERASE_TO_EOL, player->name, player->target_queue.read_ptr, player->target_queue.write_ptr, items_in_queue);
	for (int i = 0; i < min (items_in_queue, 10); i++) {
		int ptr = (i + player->target_queue.read_ptr) % 100;
		printf("  %d %d", player->target_queue.queue[ptr][0], player->target_queue.queue[ptr][1]);
	}
	if (items_in_queue >= 10) printf("...");
	printf("\n");
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
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", shooting_player->display_vertical_offset + 5);

	char radar[10][10];
	fill_in_radar(radar, shooting_player, target_player);

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


