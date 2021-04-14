

#include "header.h"


// Return 1 if all the ships have been sunk
int check_if_all_ships_sunk(struct player_data* target_player) {

	int i = 0;
	int j = 0;
	int ship_still_floating = 0;

	while ((i < 10) && (ship_still_floating == 0)) {
		j = 0;
		while ((j < 10) && (ship_still_floating == 0)) {
			if ((target_player->game_board[i][j] == 'c')
				||
				(target_player->game_board[i][j] == 'b')
				||
				(target_player->game_board[i][j] == 'r')
				||
				(target_player->game_board[i][j] == 's')
				||
				(target_player->game_board[i][j] == 'd')) ship_still_floating = 1; // exit while loops as soon as we see another cell with a ship part
			j = j + 1;
		}
		i = i + 1;
	}
	if (ship_still_floating) return 0;
	return 1;
}





// Return 1 if the ship was sunk
int check_if_sunk_ship(struct player_data* target_player, char target_ship_char) {
	int i = 0;
	int j = 0;
	int ship_still_floating = 0;

	while ((i < 10) && (ship_still_floating == 0)) {
		j = 0;
		while ((j < 10) && (ship_still_floating == 0)) {
			if (target_player->game_board[i][j] == target_ship_char) ship_still_floating = 1; // exit while loops as soon as we see another cell for that ship
			j = j + 1;
		}
		i = i + 1;
	}
	if (ship_still_floating == 1) return 0;
	return 1;
}




// Return 1 if col,row is a hit, 0 if a miss
int fireAtTarget(struct player_data* shooting_player, struct player_data* target_player, int target_row, int target_col, int* ship_type) {
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
		

		result = 1;
	}
	return result;
}

