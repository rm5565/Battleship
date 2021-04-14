

#include "header.h"


// When a ship is sunk, update the target queue to remove nearby future targets
void updateTargetingQueue(struct player_data* shooting_player, int target_ship_type) {

	if ((shooting_player->strategy == 3) || (shooting_player->strategy == 4) || (shooting_player->strategy == 5)) { // this only applies to strategies 3 and 4
		if (shooting_player->target_queue.read_ptr == shooting_player->target_queue.write_ptr) {
			// no targets in queue, return without doing anything
			return;
		}

		// copy each element of the queue array back to starting from [0], skipping any that are of target_ship_type
		// this algo assumes the array size is so large that buffer wrap is not happening... 100 now.  May need to increase if 
		// bigger games are tried
		int write_ptr = 0;
		for (int i = shooting_player->target_queue.read_ptr; i < shooting_player->target_queue.write_ptr; i++) {
			if (shooting_player->target_queue.queue[i][2] != target_ship_type) { // copy the element
				shooting_player->target_queue.queue[write_ptr][0] = shooting_player->target_queue.queue[i][0];
				shooting_player->target_queue.queue[write_ptr][1] = shooting_player->target_queue.queue[i][1];
				shooting_player->target_queue.queue[write_ptr][2] = shooting_player->target_queue.queue[i][2];
				write_ptr++;
			}
		}
		shooting_player->target_queue.read_ptr = 0;
		shooting_player->target_queue.write_ptr = write_ptr;
	}
}


void add_new_hit_to_queue(struct player_data* shooting_player, int target_row, int target_col, int ship_type) {

	//printf("adding %d %d to target queue\n", target_row, target_col);

	//char answer;  scanf_s("%c", &answer, 1);

	// Before adding a new target to the queue, lets see if it is already in there.   If so, skip it.
	int found = 0;
	for (int i = shooting_player->target_queue.read_ptr; (i < shooting_player->target_queue.write_ptr) && (found == 0); i++) {
		if ((target_row == shooting_player->target_queue.queue[i][0]) && (target_col == shooting_player->target_queue.queue[i][1])) found = 1;
	}
	if (found == 1) return;


	if ((target_row >= 0) && (target_row <= 9) && (target_col >= 0) && (target_col <= 9)) {
		if (shooting_player->enemy_board[target_row][target_col] == ' ') {  // only update cells that are empty
			int write_ptr = shooting_player->target_queue.write_ptr;
			shooting_player->target_queue.queue[write_ptr][0] = target_row;
			shooting_player->target_queue.queue[write_ptr][1] = target_col;
			shooting_player->target_queue.queue[write_ptr][2] = ship_type;

			shooting_player->target_queue.write_ptr = (shooting_player->target_queue.write_ptr + 1) % target_queue_size_max;
		}
	}
}

void remove_veritcal_hits_from_queue(struct player_data* shooting_player, int target_col, int target_ship_type) {
// copy each element of the queue array back to starting from [0], skipping any that are to either side of a ship
// with known orientation.
// This algo assumes the array size is so large that buffer wrap is not happening... 100 now.  May need to increase if 
// bigger games are tried
	int write_ptr = 0;
	for (int i = shooting_player->target_queue.read_ptr; i < shooting_player->target_queue.write_ptr; i++) {
		if (shooting_player->target_queue.queue[i][2] != target_ship_type) { // copy the element
			shooting_player->target_queue.queue[write_ptr][0] = shooting_player->target_queue.queue[i][0];
			shooting_player->target_queue.queue[write_ptr][1] = shooting_player->target_queue.queue[i][1];
			shooting_player->target_queue.queue[write_ptr][2] = shooting_player->target_queue.queue[i][2];
			write_ptr++;
		}
		else {  // this is the case where we have found a future target of ship_type, now figure out it is on either side of known location
			if (shooting_player->target_queue.queue[i][1] == target_col) { // copy it over, it is at either end of the ship
				shooting_player->target_queue.queue[write_ptr][0] = shooting_player->target_queue.queue[i][0];
				shooting_player->target_queue.queue[write_ptr][1] = shooting_player->target_queue.queue[i][1];
				shooting_player->target_queue.queue[write_ptr][2] = shooting_player->target_queue.queue[i][2];
				write_ptr++;
			} // else we skip over it


		}
	}
	shooting_player->target_queue.read_ptr = 0;
	shooting_player->target_queue.write_ptr = write_ptr;
}


void remove_horizontal_hits_from_queue(struct player_data* shooting_player, int target_row, int target_ship_type) {
	// copy each element of the queue array back to starting from [0], skipping any that are to either side of a ship
	// with known orientation.
	// This algo assumes the array size is so large that buffer wrap is not happening...   May need to increase if 
	// bigger games are tried
	int write_ptr = 0;
	for (int i = shooting_player->target_queue.read_ptr; i < shooting_player->target_queue.write_ptr; i++) {
		if (shooting_player->target_queue.queue[i][2] != target_ship_type) { // copy the element
			shooting_player->target_queue.queue[write_ptr][0] = shooting_player->target_queue.queue[i][0];
			shooting_player->target_queue.queue[write_ptr][1] = shooting_player->target_queue.queue[i][1];
			shooting_player->target_queue.queue[write_ptr][2] = shooting_player->target_queue.queue[i][2];
			write_ptr++;
		}
		else {  // this is the case where we have found a future target of ship_type, now figure out it is on either side of known location
			if (shooting_player->target_queue.queue[i][0] == target_row) { // copy it over, it is at either end of the ship
				shooting_player->target_queue.queue[write_ptr][0] = shooting_player->target_queue.queue[i][0];
				shooting_player->target_queue.queue[write_ptr][1] = shooting_player->target_queue.queue[i][1];
				shooting_player->target_queue.queue[write_ptr][2] = shooting_player->target_queue.queue[i][2];
				write_ptr++;
			} // else we skip over it
		}
	}
	shooting_player->target_queue.read_ptr = 0;
	shooting_player->target_queue.write_ptr = write_ptr;
}



void updatePlayers(struct player_data* shooting_player, struct player_data* target_player, int firing_result, int target_row, int target_col, int ship_type) {

	if (firing_result == 0) {
		shooting_player->enemy_board[target_row][target_col] = '.';
		target_player->game_board[target_row][target_col] = 'm';
	}

	if (firing_result == 1) {

		shooting_player->enemy_board[target_row][target_col] = ship_type_to_CHAR(ship_type);
		target_player->game_board[target_row][target_col] = '*';

		if (shooting_player->strategy == 3) {
			add_new_hit_to_queue(shooting_player, target_row - 1, target_col, ship_type);
			add_new_hit_to_queue(shooting_player, target_row + 1, target_col, ship_type);
			add_new_hit_to_queue(shooting_player, target_row, target_col - 1, ship_type);
			add_new_hit_to_queue(shooting_player, target_row, target_col + 1, ship_type);
		}

		if (shooting_player->strategy == 4 || shooting_player->strategy == 5) {

			int orientation = Unknown_ORIENTATION;
			int temp[5][2];  int temp_ptr = 0;

			// Look for at least 2 hits on ship_type so we can determine vertical or horizontal orientation
			// Skip destroyer since it is only 2 anyway
			if (ship_type != Destroyer) {
				// copy all known, confirmed hit locations of ship_type to a temp array by searching the shooting_player's history board
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						if (shooting_player->enemy_board[i][j] == ship_type_to_CHAR(ship_type)) {
							temp[temp_ptr][0] = i;
							temp[temp_ptr][1] = j;
							temp_ptr++;
						}
					}
				}
				//printf("Known enemy locations for %s : ", ship_type_to_ship_name(ship_type));
				//for (int k = 0; k < temp_ptr; k++) printf("   %d %d", temp[k][0], temp[k][1]);  printf("\n");
				if (temp_ptr > 1) {	// we have enough hits to know orientation
					if (temp[0][0] == temp[1][0]) {   // we only have to see if the row of the first two match
						orientation = Horizontal;
					}
					else {
						orientation = Vertical;
					}
				}
			}

			if (orientation == Unknown_ORIENTATION) {
				add_new_hit_to_queue(shooting_player, target_row - 1, target_col, ship_type);
				add_new_hit_to_queue(shooting_player, target_row + 1, target_col, ship_type);
				add_new_hit_to_queue(shooting_player, target_row, target_col - 1, ship_type);
				add_new_hit_to_queue(shooting_player, target_row, target_col + 1, ship_type);
			} else if (orientation == Horizontal) {
				add_new_hit_to_queue(shooting_player, target_row, target_col - 1, ship_type);
				add_new_hit_to_queue(shooting_player, target_row, target_col + 1, ship_type);
				// trim away any vertical oriented shots in the queue
				remove_horizontal_hits_from_queue(shooting_player, target_row, ship_type);
				// Strangely enough, trimming away prospective hits on either side of the vertical ship
				// reduces the effectiveness of strategy 4 vs strategy 3.  Strategy 3 will win more
				// matches that would have been ties if strategy 4 does this trimming.
				// This means that the prospective hits, while useless for the ship under attack
				// do expose nearby ships at a rate high enough to improve win rates.
			} else if (orientation == Vertical) {
				add_new_hit_to_queue(shooting_player, target_row - 1, target_col, ship_type);
				add_new_hit_to_queue(shooting_player, target_row + 1, target_col, ship_type);
				// trim away any vertical oriented shots in the queue
				remove_veritcal_hits_from_queue(shooting_player, target_col, ship_type);
			}
		}
	}
}