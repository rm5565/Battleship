

#include "header.h"

#define _CRT_SECURE_NO_WARNINGS


void  welcome_screen(void) {
	char answer;
	printf("Rules of the Game\n");
	printf("This is a two player game. Player1 is you and Player2 is the computer. \nPick a pair of numbers (0-9) to guess where the enemy's ships are\n ");
	printf("Hit enter to start the game.\n");
	scanf_s("%c", &answer, 1);
}


// Return 1 or 2 for player 1 or player 2
int select_who_starts_first(void) {
	return (rand() % 2) + 1;
}





// Return 1 if all the ships have been sunk
int is_winner(struct player_data* target_player) {

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





void display_boards(struct player_data* shooting_player, struct player_data* target_player, int hidden) {


	char radar[10][10];
	fill_in_radar(radar, shooting_player, target_player);



	printf("%s's board:		%s's history 				 %s's Radar\n", target_player->name, shooting_player->name, shooting_player->name);
	printf("  0 1 2 3 4 5 6 7 8 9		0 1 2 3 4 5 6 7 8 9			  0 1 2 3 4 5 6 7 8 9\n");
	for (int i = 0; i < 10; i++) {
		printf("%d ", i);
		for (int j = 0; j < 10; j++) {
			if (hidden == 0) {
				printf("%c ", target_player->game_board[i][j]);
			}
			else {
				if ((target_player->game_board[i][j] == 'c')
					||
					(target_player->game_board[i][j] == 'b')
					||
					(target_player->game_board[i][j] == 'r')
					||
					(target_player->game_board[i][j] == 's')
					||
					(target_player->game_board[i][j] == 'd'))
				{
					printf("%c ", '-');
				}
				else {
					printf("%c ", target_player->game_board[i][j]);
				}
			}
		}
		printf("		");
		for (int j = 0; j < 10; j++) {
			printf("%c ", shooting_player->enemy_board[i][j]);
		}
		printf("			");
		for (int j = 0; j < 10; j++) {
			printf("%c ", radar[i][j]);
		}
		printf("\n");
	}
}


void display_radar(struct player_data* shooting_player, struct player_data* target_player) {
	char radar[10][10];
	fill_in_radar(radar, shooting_player, target_player);

	printf("%s's radar:\n", shooting_player->name);
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	for (int i = 0; i < 10; i++) {
		printf("%d ", i);
		for (int j = 0; j < 10; j++) {
			printf("%c ", radar[i][j]);
		}
		printf("\n");
	}
}



// Return 1 if the ship was sunk
int check_if_sunk_ship(struct player_data* target_player, char target_ship_type) {
	int i = 0;
	int j = 0;
	int ship_still_floating = 0;

	while ((i < 10) && (ship_still_floating == 0)) {
		j = 0;
		while ((j < 10) && (ship_still_floating == 0)) {
			if (target_player->game_board[i][j] == target_ship_type) ship_still_floating = 1; // exit while loops as soon as we see another cell for that ship
			j = j + 1;
		}
		i = i + 1;
	}
	if (ship_still_floating == 1) return 0;
	return 1;
}


