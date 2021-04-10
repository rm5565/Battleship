
#include "header.h"

#define _CRT_SECURE_NO_WARNINGS


char p1_game_board[10][10];
char p2_game_board[10][10];




int main(void) {
	
	struct player_data player_1;
	struct player_data player_2;
	FILE* log_file;

	srand(time(0));

	initialize_player(&player_1, "Richard");
	initialize_player(&player_2, "Computer");
	debug_place_ships_on_board(&player_1);  
	debug_place_ships_on_board(&player_2);

	//init_strategy3(&player_2);


	// Open the log file
	fopen_s(&log_file, "battleship.log", "w");

	int game_over = 0;  // 0 = game is still going,  1 = game over!
	int target_row = 0, target_col = 0;
	char answer;

	while (game_over == 0) {
		
		int firing_result;
		int ship_type = 0;

		getTargetRecommendation(&player_2,&target_row, &target_col);
		printf("Recommended target is %d %d\n", target_row, target_col);

		
		firing_result = fireAtTarget(&player_2, &player_1, target_row, target_col, &ship_type);
		printf("Result of firing at %d %d was ", target_row, target_col);
		if (firing_result == 0) printf("MISS\n"); else printf("HIT on a %s\n", ship_type_to_ship_name(ship_type));

		updatePlayers(&player_2, &player_1, firing_result, target_row, target_col, ship_type);
		

		display_boards(&player_2, &player_1, 0);
		printf("\n");

		output_strategy(log_file, &player_2);
		output_boards(log_file, &player_1, &player_2);

		printf("Exit game ?\n(Enter '1' for yes, '2' or just 'Enter' for no): ");
		scanf_s("%c", &answer, 1);
		if (answer == '1') game_over = 1;
		if (game_over == 0) system("cls");
	}


	fclose(log_file);
}




/*
* 
int OLDmain(void) {



	char answer;
	char str[10];
	char ship_type_hit;
	FILE* log_file;

	struct player_data player_1;
	struct player_data player_2;

	srand(time(0));

	initialize_game_board(&player_1, "Richard");
	initialize_game_board(&player_2, "Computer");

	//welcome_screen();

	printf("%s, do you want to setup your pieces (1) manually or (2) random or (3) debug [default] ?", player_1.name);
	scanf_s("%c", &answer, 1);
	if (answer == '1') { manually_place_ships_on_board(&player_1); randomly_place_ships_on_board(&player_2); }
	if (answer == '2') { randomly_place_ships_on_board(&player_1); randomly_place_ships_on_board(&player_2); }
	else { debug_place_ships_on_board(&player_1);  debug_place_ships_on_board(&player_2); }

	int current_player;
	if ((answer == '1') || (answer == '2')) {
		current_player = select_who_starts_first();  // returns '1' or '2'
		if (current_player == 1) printf("\n\n%s has been randomly selected to go first.\n\n", player_1.name);
		else printf("\n\n%s has been randomly selected to go first.\n\n", player_2.name);
	}
	else current_player = 2;

	
	int computer_plays_both_sides = 0;
	int hide_player_two = 1; 
	if ((answer == '1') || (answer == '2')) {
		printf("Do you want the computer to play both sides (fun to watch) ?\n(Enter '1' for yes, '2' or just 'Enter' for no): ");
		scanf_s("%c", &answer, 1);
		if (answer == '1') {
			computer_plays_both_sides = 1;
			hide_player_two = 0;
		}
	}
	else
	{
		computer_plays_both_sides = 1;
		hide_player_two = 0;
	}

	// Select a strategy for player 2
	player_2.strategy = 2;
	player_2.strategy1.strategy_stage= 0;
	init_strategy2(&player_2);

	// Open the log file
	fopen_s(&log_file, "battleship.log", "w");

	int game_over = 0;  // 0 = game is still going,  1 = game over!
	int hit_or_miss = 0;	// 0 = shot was a miss, 1 = shot was a hit
	int target_sunk = 0;
	int target_row, target_col;
	while (game_over == 0) {

		display_board(&player_1, 0);
		printf("\n");
		//display_board(&player_2, hide_player_two);  // show full board during debugging
		//printf("\n");
		display_radar(&player_2, &player_1);
		*/

		/*
		if (current_player == 1) {  // Player 1's turn
			hit_or_miss = 0;
			target_sunk = 0;
			if (computer_plays_both_sides == 0) {
				enter_a_target(&player_1, &player_2, &target_row, &target_col);  // comment out this line and enable line below for auto entry
			}
			else {
				pick_a_target(&player_1, &player_2, &target_row, &target_col);  // wow, fast for debugging
			}
			hit_or_miss = check_shot(&player_1, &player_2, target_row, target_col, &ship_type_hit);

			if (hit_or_miss == 1) {
				//printf("%s scored a Hit!\n\n", player_1.name);
				target_sunk = check_if_sunk_ship(&player_2, ship_type_hit);
				if (target_sunk == 1) {  // if this type of ship was sunk print a message
					//printf("\n**** SHIP LOST ****\n");

					// Ok ship was lost, now check to see if the game is over
					if (is_winner(&player_2) == 1) {  // there are no more ships floating for player2, player 1 wins
						//printf("********** %s WINS ************", player_1.name);

						
						print_game_board(&player_1);
						printf("\n");
						print_game_board(&player_2);  // show full board during debugging
						printf("is_winner = %d", is_winner(&player_2));
						
						game_over = 1;
					}
				}
			}

			
			//else printf("%s missed...\n\n", player_1.name);

			// output_current_move(log_file, &player_1, target_row, target_col, hit_or_miss, target_sunk, game_over);




		//}
		//else 
		{						// Player 2's turn


			hit_or_miss = 0;
			target_sunk = 0;
			pick_a_target(&player_2, &player_1, &target_row, &target_col);




			hit_or_miss = check_shot(&player_2, &player_1, target_row, target_col, &ship_type_hit);

			
			if (hit_or_miss == 1) {
				printf("%s scored a Hit !\n\n", player_2.name);
				target_sunk = check_if_sunk_ship(&player_1, ship_type_hit);
				if (target_sunk == 1) {  // if this type of ship was sunk print a message
					printf("\n**** SHIP LOST ****\n");

					// Ok ship was lost, now check to see if the game is over
					if (is_winner(&player_1) == 1) {  // there are no more ships floating for player1, player 2 wins
						printf("********** %s WINS ************", player_2.name);
						
						print_game_board(&player_1);
						printf("\n");
						print_game_board(&player_2);  // show full board during debugging
						printf("is_winner = %d", is_winner(&player_1));
						
						game_over = 1;
					}
				}
			}
			else printf("%s missed...\n\n", player_2.name);


			output_current_move(log_file, &player_2, target_row, target_col, hit_or_miss, target_sunk, game_over);
			output_strategy(log_file, &player_2);
			output_boards(log_file, &player_1, &player_2);



		}

		// Alternate the current player btwn 1 and 2
		if (current_player == 1) current_player = 2; 
		else
		{
			if (current_player == 2) current_player = 1;
			printf("Exit game ?\n(Enter '1' for yes, '2' or just 'Enter' for no): ");
			scanf_s("%c", &answer, 1);
			if (answer == '1') game_over = 1;
		}
		if (game_over == 0) system("cls");
	}


	// Open the log file
	// output_stats(log_file, &player_1);
	output_stats(log_file, &player_2);
	fclose(log_file);

}

*/