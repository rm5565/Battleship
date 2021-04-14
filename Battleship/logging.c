

#include "header.h"


// Log file support
// --------------------------------------------------

void output_target_queue(FILE* log_file, struct player_data* player) {
	int items_in_queue = (player->target_queue.write_ptr - player->target_queue.read_ptr + player->target_queue.size) % 100;
	fprintf(log_file, "%s's target queue (r=%d w=%d l=%d): ", player->name, player->target_queue.read_ptr, player->target_queue.write_ptr, items_in_queue);
	for (int i = 0; i < items_in_queue; i++) {
		int ptr = (i + player->target_queue.read_ptr) % 100;
		fprintf(log_file, "  %d %d", player->target_queue.queue[ptr][0], player->target_queue.queue[ptr][1]);
	}
	fprintf(log_file, "\n");
}



void output_stats(FILE* log_file, struct player_data* player_1, struct player_data* player_2, int rounds_played) {
	double success_rate = 0.0;

	fprintf(log_file, "Rounds played: %d\n", rounds_played);
	if (player_1->total_hits + player_1->total_misses > 0) success_rate = (double)player_1->total_hits / (player_1->total_hits + player_1->total_misses) * 100;
	fprintf(log_file, "%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %f%%\n", player_1->name, player_1->strategy, player_1->total_hits, player_1->total_misses, success_rate);

	success_rate = 0.0;
	if (player_2->total_hits + player_2->total_misses > 0) success_rate = (double)player_2->total_hits / (player_2->total_hits + player_2->total_misses) * 100;
	fprintf(log_file, "%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %f%%\n", player_2->name, player_2->strategy, player_2->total_hits, player_2->total_misses, success_rate);
}


void output_CSV(FILE* csv_file, struct player_data* player_1, struct player_data* player_2, int rounds_played) {
	double success_rate = 0.0;
	/*
	fprintf(csv_file, "%d,", rounds_played);

	if (player_1->total_hits + player_1->total_misses > 0) success_rate = (double)player_1->total_hits / (player_1->total_hits + player_1->total_misses) * 100;
	fprintf(csv_file, "%s,", player_1->name);
	fprintf(csv_file, "%d,", player_1->strategy);
	fprintf(csv_file, "%d,", player_1->total_hits);
	fprintf(csv_file, "%d,", player_1->total_misses);
	fprintf(csv_file, "%2.0f,", success_rate);


	success_rate = 0.0;
	if (player_2->total_hits + player_2->total_misses > 0) success_rate = (double)player_2->total_hits / (player_2->total_hits + player_2->total_misses) * 100;
	fprintf(csv_file, "%s,", player_2->name);
	fprintf(csv_file, "%d,", player_2->strategy);
	fprintf(csv_file, "%d,", player_2->total_hits);
	fprintf(csv_file, "%d,", player_2->total_misses);
	fprintf(csv_file, "%2.0f", success_rate);;

	fprintf(csv_file, "%\n");
	*/
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
			radar[row][col] = shooting_player->enemy_board[row][col];
		}
	}

	if ((shooting_player->strategy == 3) || (shooting_player->strategy == 4)) {
		for (int i = shooting_player->target_queue.read_ptr; i < shooting_player->target_queue.write_ptr; i++) {

			int row = shooting_player->target_queue.queue[i][0];
			int col = shooting_player->target_queue.queue[i][1];
			int ship_char = ship_type_to_char(shooting_player->target_queue.queue[i][2]);

			radar[row][col] = ship_char;
		}
	}


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
			fprintf(log_file, "%c ", player_2->enemy_board[i][j]);
		}
		fprintf(log_file, "			%d ", i);
		for (int j = 0; j < 10; j++) {
			fprintf(log_file, "%c ", p2_radar[i][j]);
		}
		fprintf(log_file, "\n");
	}
}

