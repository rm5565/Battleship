

#include "header.h"


void updatePlayers(struct player_data* shooting_player, struct player_data* target_player, int firing_result, int target_row, int target_col, int ship_type) {

	char ch;

	if (firing_result == 0) {
		shooting_player->enemy_board[target_row][target_col] = '.';
		target_player->game_board[target_row][target_col] = 'm';
	}

	if (firing_result == 1) {

		shooting_player->enemy_board[target_row][target_col] = ship_type_to_CHAR(ship_type);
		target_player->game_board[target_row][target_col] = '*';

	}




}