

#include "header.h"

// Functions to display data on console


// DISPLAY OFFSETS
#define display_offset_tournament_line	1		//  Tournament data
#define display_offset_round_line		2		//  Round #
#define display_offset_dashes			3		//  ---------------------------------
#define display_offset_target_queue		4		//  Target queue
#define display_offset_recommended_target 5		//  Recommended Target
#define display_offset_firing_result	 6		//  
#define display_offset_boards			 7		//  Boards


void  display_welcome_screen(void) {
	char answer;
	printf("Rules of the Game\n");
	printf("This is a two player game. Player1 is you and Player2 is the computer. \nPick a pair of numbers (0-9) to guess where the enemy's ships are\n ");
	printf("Hit enter to start the game.\n");
	scanf_s("%c", &answer, 1);
}

void display_help(void) {
	printf("-t     run in strategy test mode, no animation\n");
	printf("-log   write log info for each game to battleship.log (overwrites each game)\n");
	printf("-csv   write strategy test info to a CSV file, battleship.csv.  (appends each game)\n");
	printf("-p     pause for user to press Enter at end of each round\n");
}

void build_tournament_line(char* s, struct player_data* player_1, struct player_data* player_2, int GAMES, int p1_wins, int p2_wins, int ties) {

	double p1winpercentage = 0.00; if (GAMES > 0) p1winpercentage = (double)p1_wins / (p1_wins + p2_wins + ties) * 100;
	double p2winpercentage = 0.00; if (GAMES > 0) p2winpercentage = (double)p2_wins / (p1_wins + p2_wins + ties) * 100;
	double tiepercentage = 0.00; if (GAMES > 0) tiepercentage = (double)ties / (p1_wins + p2_wins + ties) * 100;
	sprintf_s(s, 256, "GAMES: %d   %s wins:%d %2.1f%%    %s wins:%d %2.1f%%  ties:%d %2.1f%%", GAMES, player_1->name, p1_wins, p1winpercentage, player_2->name, p2_wins, p2winpercentage, ties, tiepercentage);

}
void display_tournament_line(struct player_data* player_1, struct player_data* player_2, int GAMES, int p1_wins, int p2_wins, int ties) {
	char line[256];
	char POSITION_CURSOR[20];
	strcpy_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[1;1H");

	build_tournament_line(line, player_1, player_2, GAMES, p1_wins, p2_wins, ties);
	printf("%s%s", POSITION_CURSOR, line);
}

void display_tournament_line_in_strategy_test_mode(struct player_data* player_1, struct player_data* player_2, int GAMES, int p1_wins, int p2_wins, int ties) {
	char line[256];

	build_tournament_line(line, player_1, player_2, GAMES, p1_wins, p2_wins, ties);
	printf("%s%s", RESTORE_CURSOR_POSN, line);
}


void display_divider_line(struct player_data* player) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", player->display_vertical_offset + display_offset_dashes);
	printf("%s-------------------------------------------", POSITION_CURSOR);
}

void display_round_line(int round) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", display_offset_round_line);
	printf("%sRound: %d", POSITION_CURSOR, round);
}

void display_recommended_target(struct player_data* shooting_player, int target_row, int target_col) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", shooting_player->display_vertical_offset + display_offset_recommended_target);
	printf("%sRecommended target: %d %d", POSITION_CURSOR, target_row, target_col);
}

void display_firing_result(struct player_data* player, int firing_result, int target_ship_type, int target_ship_sunk, int target_row, int target_col, int game_over) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", player->display_vertical_offset + display_offset_firing_result);

	if (firing_result == 0) {
		printf("%s%sBombardment of %d %d was a MISS", POSITION_CURSOR, ERASE_TO_EOL, target_row, target_col);
	}
	else {
		if (target_ship_sunk == 0) { //not sunk
			printf("%s%sBombardment of %d %d was a HIT!!%s", POSITION_CURSOR, ERASE_TO_EOL, target_row, target_col, NORMAL_TEXT);
			// fprintf(log_file, "HIT on a %s\n", ship_type_to_ship_name(target_ship_type)); // normally user doesn't know type until ship is sunk
		} else { // ship 
			printf("%s%sBombardment of %d %d was a %s%sHIT AND SUNK a %s%s!", POSITION_CURSOR, ERASE_TO_EOL, target_row, target_col, BG_sea, FG_sea_hit, ship_type_to_ship_name(target_ship_type), NORMAL_TEXT);
		}
	}
}


void display_target_queue(struct player_data* player) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", player->display_vertical_offset + display_offset_target_queue);
	int items_in_queue = (player->target_queue.write_ptr - player->target_queue.read_ptr + player->target_queue.size) % 100;
	printf("%s%s%s's target queue (r=%d w=%d l=%d): ", POSITION_CURSOR, ERASE_TO_EOL, player->name, player->target_queue.read_ptr, player->target_queue.write_ptr, items_in_queue);
	for (int i = 0; i < min (items_in_queue, 10); i++) {
		int ptr = (i + player->target_queue.read_ptr) % 100;
		printf("  %d %d", player->target_queue.queue[ptr][0], player->target_queue.queue[ptr][1]);
	}
	if (items_in_queue >= 10) printf("...");
	printf("\n");
}


void display_restore_screen() {
	printf("%s%s%s", NORMAL_TEXT, LINE40_CURSOR,SHOW_CURSOR);
}

void display_boards(struct player_data* shooting_player, struct player_data* target_player, int hidden) {
	char POSITION_CURSOR[20];
	sprintf_s(POSITION_CURSOR, sizeof(POSITION_CURSOR), "\x1b[%d;0H", shooting_player->display_vertical_offset + display_offset_boards);

	char radar[10][10];
	fill_in_radar(radar, shooting_player, target_player);

	printf("%s%s's board:       %s's history      %s's Radar\n", POSITION_CURSOR, target_player->name, shooting_player->name, shooting_player->name);
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



void display_stats(struct player_data* player_1, struct player_data* player_2, int rounds_played) {
	double success_rate = 0.0;
	/*
	printf("%sRounds played: %d\n", ERASE_TO_EOL, rounds_played);
	if (player_1->total_hits + player_1->total_misses > 0) success_rate = (double)player_1->total_hits / (player_1->total_hits + player_1->total_misses) * 100;
	printf("%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %2.0f%%\n", player_1->name, player_1->strategy, player_1->total_hits, player_1->total_misses, success_rate);

	success_rate = 0.0;
	if (player_2->total_hits + player_2->total_misses > 0) success_rate = (double)player_2->total_hits / (player_2->total_hits + player_2->total_misses) * 100;
	printf("%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %2.0f%%\n", player_2->name, player_2->strategy, player_2->total_hits, player_2->total_misses, success_rate);
	*/
	}
