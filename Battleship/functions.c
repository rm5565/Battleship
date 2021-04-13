

#include "header.h"

#define _CRT_SECURE_NO_WARNINGS


extern int game_over;
extern int CTRL_C;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal.
	case CTRL_C_EVENT:
		// Beep(750, 300);
		game_over = 1;
		CTRL_C = 1;
		return TRUE;
	default: return FALSE;
	}
}




// Return 1 or 2 for player 1 or player 2
int select_who_starts_first(void) {
	return (rand() % 2) + 1;
}





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


