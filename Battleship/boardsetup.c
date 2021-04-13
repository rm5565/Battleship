
#include "header.h"


//  BELOW HERE ARE PLAYER AND BOARD SETUP FUNCTIONS
// --------------------------------------------------

// Initialize a blank board
void initialize_player(struct player_data* player, int player_number, char* name, int strategy) {
	strncpy_s(player->name, 20, name, strlen(name));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			player->game_board[i][j] = '-';
			player->enemy_board[i][j] = ' ';
		}
	}
	player->total_hits = 0;
	player->total_misses = 0;
	player->strategy = strategy;
	player->player_number = player_number;
	player->display_vertical_offset = (player_number - 1) * 20;


	player->target_queue.read_ptr = 0;
	player->target_queue.write_ptr = 0;
	player->target_queue.size = 100;
	for (int i = 0; i < 100; i++) {
		player->target_queue.queue[i][0] = 0;
		player->target_queue.queue[i][1] = 0;
		player->target_queue.queue[i][2] = 0;
	}

	if (player->strategy == 5) {  // try pre-seeding shots into specific areas at the start of the game, then go random
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				 //if ((i % 3 == 0) && ( j % 3 == 0)) 
					 add_new_hit_to_queue(player, i, j, UnknownShip);
			}
		}

		display_target_queue(player);
		// char answer;  scanf_s("%c", &answer, 1);
	}
}


// Function to validate the position of the ship is ok,
// Confirm that a ship is not outside the boundaries of the board and that it
// is not on top of a ship that has already been placed.
// Return 0 if position is ok
// Return 1 if position is off the board
// Return 2 if position overlaps another ship
int verify_ship_position_ok(int ship_coordinates[5][2], int size_of_ship, struct player_data* player) {

	//for (int k = 0; k < size_of_ship; k++) printf("%d %d,  ", ship_coordinates[k][0], ship_coordinates[k][1]); printf("\n");

	// check boundaries first
	int boundaries_ok = 1;
	int index = 0;
	while ((boundaries_ok == 1) && (index < size_of_ship)) {
		if ((ship_coordinates[index][0] < 0) || (ship_coordinates[index][0] > 9) || (ship_coordinates[index][1] < 0) || (ship_coordinates[index][1] > 9))
			boundaries_ok = 0;
		index++;
	}
	if (boundaries_ok == 0) return 1;  // Off board 

	// check overlap with existing ships
	int no_overlapping = 1;
	index = 0;
	while ((no_overlapping == 1) && (index < size_of_ship)) {
		if (player->game_board[ship_coordinates[index][0]][ship_coordinates[index][1]] != '-')
			no_overlapping = 0;
		index++;
	}
	if (no_overlapping == 0) return 2;  // Overlap found

	return 0;  // All ok
}



// Get user input, validate the input, repeat input until ok, then update the gameboard for a single ship
void manually_place_one_ship(struct player_data* player, char* name_of_ship, char char_of_ship, int size_of_ship) {
	int x, y;
	int ship_coordinates[5][2];

	int input_ok = 0;
	while (input_ok != 1) {
		printf("Please enter the %d cells to place the %s across\n", size_of_ship, name_of_ship);
		for (int i = 0; i < size_of_ship; i++) {
			scanf_s("%d%d", &x, &y);
			ship_coordinates[i][0] = x;
			ship_coordinates[i][1] = y;
		}
		int input_result = verify_ship_position_ok(ship_coordinates, size_of_ship, player);
		if (input_result == 0) input_ok = 1;
		else if (input_result == 1) printf("*** Input error, ship positioned off board.  Try again.\n");
		else if (input_result == 2) printf("*** Input error, ship positioned over another ship.  Try again.\n");
	}

	// Ship position is ok, put it into the game board
	for (int i = 0; i < size_of_ship; i++) {
		player->game_board[ship_coordinates[i][0]][ship_coordinates[i][1]] = char_of_ship;
	}

	// print_game_board(player);
}



void manually_place_ships_on_board(struct player_data* player) {
	manually_place_one_ship(player, "Carrier", 'c', 5);
	manually_place_one_ship(player, "Battleship", 'b', 4);
	manually_place_one_ship(player, "Cruiser", 'r', 3);
	manually_place_one_ship(player, "Submarine", 's', 3);
	manually_place_one_ship(player, "Destroyer", 'd', 2);
}




void randomly_place_one_ship(struct player_data* player, char* name_of_ship, char char_of_ship, int size_of_ship) {
	int row, col;
	int ship_coordinates[5][2];
	int ship_is_vertical;
	int placement_ok = 0;

	while (placement_ok == 0) { // repeat until a valid placement is found
		ship_is_vertical = rand() % 2;
		col = rand() % (10 - size_of_ship);  // mod 10-size_of_ship ensures ship will fit on board
		row = rand() % (10 - size_of_ship);  // random starting place
		for (int i = 0; i < size_of_ship; i++) {
			if (ship_is_vertical == 0) {  // ship should be placed horizontally, const row, col changes
				ship_coordinates[i][0] = row;
				ship_coordinates[i][1] = col + i;
			}
			else { // ship is place vertically
				ship_coordinates[i][0] = row + i;
				ship_coordinates[i][1] = col;
			}
		}
		int placement_result = verify_ship_position_ok(ship_coordinates, size_of_ship, player);
		if (placement_result == 0) placement_ok = 1;
		// else if (placement_result == 1) printf("*** Placement error, %s positioned off board. (%d, %d) %d cells.  Trying again.\n", name_of_ship, col, row, size_of_ship);
		// else if (placement_result == 2) printf("*** Placement error, %s positioned over another ship.  Trying again.\n", name_of_ship);
	}

	// Ship position is ok, put it into the game board
	for (int i = 0; i < size_of_ship; i++) {
		player->game_board[ship_coordinates[i][0]][ship_coordinates[i][1]] = char_of_ship;
	}

	// print_game_board(player);
}



void randomly_place_ships_on_board(struct player_data* player) {
	
	randomly_place_one_ship(player, "Carrier", 'c', 5);
	randomly_place_one_ship(player, "Battleship", 'b', 4);
	randomly_place_one_ship(player, "Cruiser", 'r', 3);
	randomly_place_one_ship(player, "Submarine", 's', 3);
	randomly_place_one_ship(player, "Destroyer", 'd', 2);
}


void debug_place_one_ship(struct player_data* player, int ship_type, int row, int col, int orientation) {

	for (int i = 0; i < ship_type_to_ship_size(ship_type); i++) {

		if (orientation == Vertical) {

			player->game_board[row + i][col] = ship_to_char(ship_type);

		}
		else  // Horizontal
		{
			player->game_board[row][col + i] = ship_to_char(ship_type);
		}
	}
}


void debug_place_ships_on_board(struct player_data* player) {

	debug_place_one_ship(player, Carrier,  1, 1, Vertical);
	debug_place_one_ship(player, Battleship, 0, 5, Horizontal);
	debug_place_one_ship(player, Cruiser, 9, 1, Horizontal);
	debug_place_one_ship(player, Submarine, 7, 2, Horizontal);
	debug_place_one_ship(player, Destroyer, 3, 2, Vertical);
}
