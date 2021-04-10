

#include "header.h"


// Return 1 if col,row is a hit, 0 if a miss
int fireAtTarget(struct player_data* shooting_player, struct player_data* target_player, int target_row, int target_col, int* ship_type) {
	int result = 0;

	if (target_player->game_board[target_row][target_col] == '-') {  // It's a miss
		shooting_player->total_misses++;
		// target_player->game_board[target_row][target_col] = 'm';
	}
	else {
		//printf("hit\n");
		shooting_player->total_hits++;
		*ship_type = char_to_ship(target_player->game_board[target_row][target_col]);  // return the type of ship hit so we can check if it sunk
		// target_player->game_board[target_row][target_col] = '*';
		result = 1;
	}
	return result;
}

