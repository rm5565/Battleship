

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



void output_tournament_line(struct player_data* player_1, struct player_data* player_2, struct game_data* game) {
	double p1winpercentage = 0.00; if (game->total_games > 0) p1winpercentage = (double)game->player_1_wins / game->total_games * 100;
	double p2winpercentage = 0.00; if (game->total_games > 0) p2winpercentage = (double)game->player_2_wins / game->total_games * 100;
	double tiepercentage = 0.00;   if (game->total_games > 0) tiepercentage = (double)game->ties / game->total_games * 100;
	fprintf(game->log_file, "GAMES: %d   %s wins:%d %2.1f%%    %s wins:%d %2.1f%%  ties:%d %2.1f%%", game->total_games, player_1->name, game->player_1_wins, p1winpercentage, player_2->name, game->player_2_wins, p2winpercentage, game->ties, tiepercentage);

}


void output_game_stats(struct player_data* player_1, struct player_data* player_2, struct game_data* game, int rounds_played) {
	double success_rate = 0.0;

	fprintf(game->log_file, "Rounds played: %d\n", rounds_played);
	if (rounds_played > 0) success_rate = (double)player_1->total_hits / (player_1->total_hits + player_1->total_misses) * 100;
	fprintf(game->log_file, "%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %f%%\n", player_1->name, player_1->strategy, player_1->total_hits, player_1->total_misses, success_rate);

	success_rate = 0.0;
	if (rounds_played > 0) success_rate = (double)player_2->total_hits / (player_2->total_hits + player_2->total_misses) * 100;
	fprintf(game->log_file, "%s stats using strategy %d:\nHits:  %d    Misses  %d     Success Rate = %f%%\n", player_2->name, player_2->strategy, player_2->total_hits, player_2->total_misses, success_rate);
}

void output_tournament_stats(struct player_data* player_1, struct player_data* player_2, struct game_data* game) {

	// printf("Hit accuracy for %s is %2.1f%", player_1->name, )
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

	if ((shooting_player->strategy == 3) || (shooting_player->strategy == 4) || (shooting_player->strategy == 5)) {
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

