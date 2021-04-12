#include "header.h"




void init_strategy2(struct player_data* shooting_player) {
	/*
	
	shooting_player->strategy2.strategy_stage = 0;
	for (int i = Carrier; i <= Destroyer; i++) {
		shooting_player->strategy2.ships[i].status = 0;
		shooting_player->strategy2.ships[i].vertical_count = 0;
		shooting_player->strategy2.ships[i].horizontal_count = 0;
		for (int j = 0; j < 10; j++) {
			shooting_player->strategy2.ships[i].vertical[j][2] = 0;
			shooting_player->strategy2.ships[i].horizontal[j][2] = 0;
		}
		shooting_player->strategy2.ships[i].orientation = Unknown;
		shooting_player->strategy2.ships[i].tried_top = 0;
		shooting_player->strategy2.ships[i].tried_bottom = 0;
		shooting_player->strategy2.ships[i].tried_left = 0;
		shooting_player->strategy2.ships[i].tried_right = 0;
	}
*/
}

/*
* 
* 	int status;  // 0 = not found, 1 = found still floating, 2 = sunk
	int vertical[10][2];
	int horizontal[10][2];
	int vertical_count, horizontal_count;  // how many in each array
	int first_hit_row;
	int first_hit_col;


struct strategy2_data {
	int original_hit_location_row, original_hit_location_col;
	int latest_hit_location_row, latest_hit_location_col;
	int strategy_stage;				// 0 = searching randomly, 1 =  2 =

	struct ship_info ships[5];  // carrier, battleship, cruiser, submarine, destroyer

};


struct ship_info { // once we have first hit, we can know all possible locations for that ship, left&right, up&down
	int status;  = // 0 = not found, 1 = found still floating, 2 = sunk
	int vertical[10][2];
	int horizontal[10][2];
	int vertical_count, horizontal_count;  // how many in each array
	int first_hit_row;
	int first_hit_col;
};

struct player_data {
	char name[20];
	char game_board[10][10];
	int total_hits;
	int total_misses;
	int strategy;
	struct strategy1_data strategy_data;
	struct strategy2_data strategy2;
};

*/

void add_location_to_vertical_list(struct player_data* shooting_player, int ship_type, int row, int col, int *array_count) {
	int index = *array_count;
	shooting_player->strategy2.ships[ship_type].vertical[index][0] = row;
	shooting_player->strategy2.ships[ship_type].vertical[index][1] = col;
	*array_count = (*array_count) + 1;
}

void add_location_to_horizontal_list(struct player_data* shooting_player, int ship_type, int row, int col, int* array_count) {
	int index = *array_count;
	shooting_player->strategy2.ships[ship_type].horizontal[index][0] = row;
	shooting_player->strategy2.ships[ship_type].horizontal[index][1] = col;
	*array_count = (*array_count) + 1;
}

void fill_in_ship_info(struct player_data* shooting_player, struct player_data* target_player, int ship_type, int target_row, int target_col) {
	shooting_player->strategy2.ships[ship_type].status = 1;
	shooting_player->strategy2.ships[ship_type].vertical_count = 0;
	shooting_player->strategy2.ships[ship_type].horizontal_count = 0;

	int orientation = shooting_player->strategy2.ships[ship_type].orientation;


	if ((orientation == Unknown) || (orientation == Vertical)) {

		// Fill in possible targets above the given location
		int obstruction = 0;  // use this to flag if a cell either has been shot at already or has another ship in it
		for (int i = 0; (i < ship_type_to_ship_size(ship_type) && (obstruction == 0) && (target_row - i >= 0)); i++)
		{
			if ((target_player->game_board[target_row - i][target_col] == 'm') || (target_player->game_board[target_row - i][target_col] == '*')) {
				obstruction = 1;
			}
			else {
				add_location_to_vertical_list(shooting_player, ship_type, target_row - i, target_col, &shooting_player->strategy2.ships[ship_type].vertical_count);
			}
		}

		// Fill in possible targets below the given location
		obstruction = 0;  // use this to flag if a cell either has been shot at already or has another ship in it
		for (int i = 0; (i < ship_size(ship_type) && (obstruction == 0) && (target_row + i <= 9)); i++)
		{
			if ((target_player->game_board[target_row + i][target_col] == 'm') || (target_player->game_board[target_row + i][target_col] == '*')) {
				obstruction = 1;
			}
			else {
				add_location_to_vertical_list(shooting_player, ship_type, target_row + i, target_col, &shooting_player->strategy2.ships[ship_type].vertical_count);
			}
		}

	}


	if ((orientation == Unknown) || (orientation == Horizontal)) {

		// Fill in possible targets left of the given location
		int obstruction = 0;  // use this to flag if a cell either has been shot at already or has another ship in it
		for (int i = 0; (i < ship_size(ship_type) && (obstruction == 0) && (target_col - i >= 0)); i++)
		{
			if ((target_player->game_board[target_row][target_col - i] == 'm') || (target_player->game_board[target_row][target_col - i] == '*')) {
				obstruction = 1;
			}
			else {
				add_location_to_horizontal_list(shooting_player, ship_type, target_row, target_col - i, &shooting_player->strategy2.ships[ship_type].horizontal_count);
			}
		}

		// Fill in possible targets right of the given location
		obstruction = 0;  // use this to flag if a cell either has been shot at already or has another ship in it
		for (int i = 0; (i < ship_size(ship_type) && (obstruction == 0) && (target_col + i <= 9)); i++)
		{
			if ((target_player->game_board[target_row][target_col + i] == 'm') || (target_player->game_board[target_row][target_col + i] == '*')) {
				obstruction = 1;
			}
			else {
				add_location_to_horizontal_list(shooting_player, ship_type, target_row, target_col + i, &shooting_player->strategy2.ships[ship_type].horizontal_count);
			}
		}

	}

}


void pick_a_target_using_strategy2(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col) {
	int found;
	int index;

	switch (shooting_player->strategy2.strategy_stage) {

	case 0 : // Looking for a new target.  Search internal list for possibilities, if none, generate a random location
		/*
		found = 0;
		int i = Carrier;
		while ((i <= Destroyer) && (found == 0)) {
			if (shooting_player->strategy2.ships[i].status == 1) {  // we found a ship still floating - for it to be here, we would have found it while
																	// trying to sink a different ship.

				
				found = 1;
			}
			i++;
		}

		if (found == 0) {	// No ships spotted, generate a random target location
			randmonly_pick_a_target(shooting_player, target_player, target_row, target_col);
		}
		*/

		*target_row = 5;
		*target_col = 5;

		if (target_player->game_board[*target_row][*target_col] != '-') {  // It's a HIT!  (other possibilities weeded out in randmonly_pick_a_target)
			int ship_type = char_to_ship_type(target_player->game_board[*target_row][*target_col]);

			// Fill out possible ship locations based on hit location, ship size, and other ships in the way
			// which will be used in next stage of the strategy
			
			shooting_player->strategy2.ship_of_interest_type = ship_type;
			shooting_player->strategy2.ships[ship_type].first_hit_row = *target_row;
			shooting_player->strategy2.ships[ship_type].first_hit_col = *target_col;
			fill_in_ship_info(shooting_player, target_player, ship_type, *target_row, *target_col);

			// move to next step of strategy
			shooting_player->strategy2.strategy_stage++;
			strncpy_s(shooting_player->strategy2.strategy_message, sizeof(shooting_player->strategy2.strategy_message), "First hit found, moving to 2nd strategy stage", 64);

		}


		break;

	case 1:  // Determine if the ship is oriented vertically or horizontally
		// Stay in this stage until all options are tried, or a second hit is made
		// Upon 2nd hit, update the possible target lists
		index = shooting_player->strategy2.ship_of_interest_type;
		int first_hit_row = shooting_player->strategy2.ships[index].first_hit_row;
		int first_hit_col = shooting_player->strategy2.ships[index].first_hit_col;

		int trial_row; 
		int trial_col;
		int trial_found = 0;  // Is trial location good to use for next shot
		char trial_contents;

		// Try above the ship first
		if ((shooting_player->strategy2.ships[index].tried_top == 0) && (trial_found == 0)) {
			trial_row = shooting_player->strategy2.ships[index].first_hit_row - 1;
			trial_col = shooting_player->strategy2.ships[index].first_hit_col;
			if (trial_row >= 0) {
				trial_contents = target_player->game_board[trial_row][trial_col];
				if ((trial_contents != '*') &&  (trial_contents != 'm'))			 {  // unknown contents
					shooting_player->strategy2.ships[index].tried_top = 1;				// this is our shot
					trial_found = 1;
					*target_row = trial_row;
					*target_col = trial_col;
					if (index == ch_to_ship_type(trial_contents)) { 					// a second hit on ship we are looking for
																						// confirmed vertical orientation, remove horizontal possibilities
						shooting_player->strategy2.ships[index].orientation = Vertical;
						fill_in_ship_info(shooting_player, target_player, index, first_hit_row, first_hit_col);

						// move to next step of strategy
						shooting_player->strategy2.strategy_stage++;
						strncpy_s(shooting_player->strategy2.strategy_message, sizeof(shooting_player->strategy2.strategy_message), "Orientation vertical, moving to 3rd strategy stage", 64);
					}
				}
			}
		}


		// Try below the ship next
		if ((shooting_player->strategy2.ships[index].tried_bottom == 0) && (trial_found == 0)) {
			trial_row = shooting_player->strategy2.ships[index].first_hit_row + 1;
			trial_col = shooting_player->strategy2.ships[index].first_hit_col;
			if (trial_row <= 9) {
				trial_contents = target_player->game_board[trial_row][trial_col];
				if ((trial_contents != '*') && (trial_contents != 'm')) {				// unknown contents
					shooting_player->strategy2.ships[index].tried_bottom = 1;			// this is our shot
					trial_found = 1;
					*target_row = trial_row;
					*target_col = trial_col;
					if (index == ch_to_ship_type(trial_contents)) { 					// a second hit on ship we are looking for
																						// confirmed vertical orientation, remove horizontal possibilities
						shooting_player->strategy2.ships[index].orientation = Vertical;
						fill_in_ship_info(shooting_player, target_player, index, first_hit_row, first_hit_col);
						// move to next step of strategy
						shooting_player->strategy2.strategy_stage++;
						strncpy_s(shooting_player->strategy2.strategy_message, sizeof(shooting_player->strategy2.strategy_message), "Orientation vertical, moving to 3rd strategy stage", 64);

					}
				}
			}
		}

		// Try left of the ship next
		if ((shooting_player->strategy2.ships[index].tried_left == 0) && (trial_found == 0)) {
			trial_row = shooting_player->strategy2.ships[index].first_hit_row;
			trial_col = shooting_player->strategy2.ships[index].first_hit_col-1;
			if (trial_row >= 0) {
				trial_contents = target_player->game_board[trial_row][trial_col];
				if ((trial_contents != '*') && (trial_contents != 'm')) {				// unknown contents
					shooting_player->strategy2.ships[index].tried_left = 1;			// this is our shot
					trial_found = 1;
					*target_row = trial_row;
					*target_col = trial_col;
					if (index == ch_to_ship_type(trial_contents)) { 					// a second hit on ship we are looking for
																						// confirmed vertical orientation, remove horizontal possibilities
						shooting_player->strategy2.ships[index].orientation = Horizontal;
						fill_in_ship_info(shooting_player, target_player, index, first_hit_row, first_hit_col);
						// move to next step of strategy
						shooting_player->strategy2.strategy_stage++;
						strncpy_s(shooting_player->strategy2.strategy_message, sizeof(shooting_player->strategy2.strategy_message), "Orientation horizontal, moving to 3rd strategy stage", 64);

					}
				}
			}
		}

		// Try right of the ship next
		if ((shooting_player->strategy2.ships[index].tried_right == 0) && (trial_found == 0)) {
			trial_row = shooting_player->strategy2.ships[index].first_hit_row;
			trial_col = shooting_player->strategy2.ships[index].first_hit_col + 1;
			if (trial_row >= 0) {
				trial_contents = target_player->game_board[trial_row][trial_col];
				if ((trial_contents != '*') && (trial_contents != 'm')) {				// unknown contents
					shooting_player->strategy2.ships[index].tried_right = 1;			// this is our shot
					trial_found = 1;
					*target_row = trial_row;
					*target_col = trial_col;
					if (index == ch_to_ship_type(trial_contents)) { 					// a second hit on ship we are looking for
																						// confirmed vertical orientation, remove horizontal possibilities
						shooting_player->strategy2.ships[index].orientation = Horizontal;
						fill_in_ship_info(shooting_player, target_player, index, first_hit_row, first_hit_col);
						// move to next step of strategy
						shooting_player->strategy2.strategy_stage++;
						strncpy_s(shooting_player->strategy2.strategy_message, sizeof(shooting_player->strategy2.strategy_message), "Orientation horizontal, moving to 3rd strategy stage", 64);

					}
				}
			}
		}
		break;
	}
}