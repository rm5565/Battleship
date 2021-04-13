

#include "header.h"





// Return 1 if col,row is a hit, 0 if a miss
int fireAtTarget(struct player_data* shooting_player, struct player_data* target_player, int target_row, int target_col, int* ship_type, int* ship_sunk, int* game_over) {
	int result = 0;

	if ((target_player->game_board[target_row][target_col] == '-') 
							||
		(target_player->game_board[target_row][target_col] == 'm'))  // this case should never happen as it wouldn't make it into the targetting queue
	{  // It's a miss
		shooting_player->total_misses++;

	}
	else {
		// printf("hit a %c\n", char_to_ship(target_player->game_board[target_row][target_col]));
		shooting_player->total_hits++;
		*ship_type = char_to_ship(target_player->game_board[target_row][target_col]);  // return the type of ship hit 
		
		// check if the ship was sunk
		*ship_sunk = check_if_sunk_ship(target_player, target_player->game_board[target_row][target_col]);


		if (*ship_sunk == 1)
		{
			*game_over = check_if_all_ships_sunk(target_player);
		}

		result = 1;
	}
	return result;
}

