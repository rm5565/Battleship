#include "header.h"


// Functions associated with targetting

void enter_a_target(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col) {
	int row, col;
	int input_ok = 0;

	while (input_ok == 0) {
		printf("%s, enter a target (row, col): ", shooting_player->name);
		scanf_s("%d%d", &row, &col);
		if ((row >= 0) && (row <= 9) && (col >= 0) && (col <= 9)) {

			// check if that cell was already used, ('*' or 'm')
			if ((target_player->game_board[row][col] == 'm')
				||
				(target_player->game_board[row][col] == '*')) {  // It's a miss
			// if yes, then print input error try again
				printf("That target has already been tried, shoot again.\n");
			}
			else
				input_ok = 1;
		}
		else printf("Error in input data, try again.\n");
	}
	*target_row = row;
	*target_col = col;
}








