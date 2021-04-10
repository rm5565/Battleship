

#include "header.h"


// Log file support
// --------------------------------------------------
void output_stats(FILE* log_file, struct player_data* player) {
	fprintf(log_file, "\n\nPLAYER STATS\n");
	fprintf(log_file, "%s, hits=%d, misses=%d, ratio=%f\n ", player->name, player->total_hits, player->total_misses,  (double) (player->total_hits / (player->total_hits + player->total_misses)));

}

// Output the current move
void output_current_move(FILE* log_file, struct player_data* player, int target_row, int target_col, int hit_or_miss, int target_sunk, int game_over) {

	fprintf(log_file, "%s move #%d: shot at %d %d ", player->name, player->total_hits + player->total_misses, target_row, target_col);
	if (hit_or_miss == 0) fprintf(log_file, "and missed.");
	else {
		fprintf(log_file, "and hit.  ");
		if (target_sunk == 1) {
			fprintf(log_file, "Ship was sunk.");
			if (game_over == 1) fprintf(log_file, "Game over");
		}
	}
	fprintf(log_file, "\n");
}





// Output strategy info
void output_strategy(FILE* log_file, struct player_data* shooting_player) {
	/*
	fprintf(log_file, "%s strategy #%d, stage %d: ", shooting_player->name, shooting_player->strategy, shooting_player->strategy2.strategy_stage);

	if (shooting_player->strategy2.strategy_stage == 0)  fprintf(log_file, "   Searching for a target\n");

	if (shooting_player->strategy2.strategy_stage == 1) {
		int ship_type = shooting_player->strategy2.ship_of_interest_type;
		fprintf(log_file, "   \n%s found at %d %d, %s for 2nd hit. \n", ship_type_to_ship_name(ship_type), shooting_player->strategy2.ships[ship_type].first_hit_row, shooting_player->strategy2.ships[ship_type].first_hit_col, shooting_player->strategy2.strategy_message);
	}

	if (shooting_player->strategy2.strategy_stage == 2) {
		fprintf(log_file, "   %s\n", shooting_player->strategy2.strategy_message);
	}
	*/
}


void fill_in_radar(char radar[10][10], struct player_data* shooting_player, struct player_data* target_player) {
	
	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			if ((target_player->game_board[row][col] == 'm') || (target_player->game_board[row][col] == '*')) 	radar[row][col] = '.';
			else 	radar[row][col] = ' ';
		}
	}
	/*
	for (int i = Carrier; i <= Destroyer; i++) {
		if (shooting_player->strategy2.ships[i].status != 0) {
			for (int j = 0; j < shooting_player->strategy2.ships[i].vertical_count; j++)
			{

				int row = shooting_player->strategy2.ships[i].vertical[j][0];
				int col = shooting_player->strategy2.ships[i].vertical[j][1];
				//printf("row, col = %d %d\n ", row, col);
				radar[row][col] = ship_to_char(i);

			}
			for (int j = 0; j < shooting_player->strategy2.ships[i].horizontal_count; j++)
			{

				int row = shooting_player->strategy2.ships[i].horizontal[j][0];
				int col = shooting_player->strategy2.ships[i].horizontal[j][1];
				//printf("row, col = %d %d\n ", row, col);
				radar[row][col] = ship_to_char(i);
			}
		}
	}
	*/
}


// output the boards
void output_boards(FILE* log_file, struct player_data* player_1, struct player_data* player_2) {
	char p2_radar[10][10];

	fill_in_radar(p2_radar, player_2, player_1);

	fprintf(log_file, "%s's board:				%s's board:				%s's history:				%s's radar:\n", player_1->name, player_2->name, player_2->name, player_2->name);
	fprintf(log_file, "  0 1 2 3 4 5 6 7 8 9			  0 1 2 3 4 5 6 7 8 9			  0 1 2 3 4 5 6 7 8 9			  0 1 2 3 4 5 6 7 8 9\n");
	for (int i = 0; i < 10; i++) {
		fprintf(log_file, "%d ", i);
		for (int j = 0; j < 10; j++) {
			fprintf(log_file, "%c ", player_1->game_board[i][j]);
		}
		fprintf(log_file, "			%d ", i);
		for (int j = 0; j < 10; j++) {
			fprintf(log_file, "%c ", player_2->game_board[i][j]);
		}
		fprintf(log_file, "			%d ", i);
		for (int j = 0; j < 10; j++) {
			fprintf(log_file, "%c ", p2_radar[i][j]);
		}
		fprintf(log_file, "			%d ", i);
		for (int j = 0; j < 10; j++) {
			fprintf(log_file, "%c ", player_2->enemy_board[i][j]);
		}
		fprintf(log_file, "\n");
	}
}

