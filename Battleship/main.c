
#include "header.h"

#define _CRT_SECURE_NO_WARNINGS


char p1_game_board[10][10];
char p2_game_board[10][10];

int game_over = 0;  // 0 = game is still going,  1 = game over!  global var
int CTRL_C = 0;
int strategy_test_mode = 0;
int pause_between_shots_mode = 0;
int write_to_log_file = 1;
int write_to_csv_file = 0;

// Play a round 
// return 1 if game over
int play_a_round(struct player_data* shooting_player, struct player_data* target_player, FILE* log_file, int round) {
	int target_row = 0, target_col = 0;
	int game_over = 0;
	int firing_result;
	int target_ship_type = 0;
	int target_ship_sunk = 0;

	getTargetRecommendation(shooting_player, &target_row, &target_col, round);

	if (strategy_test_mode == 0) {
		display_target_queue(shooting_player);
		display_recommended_target(shooting_player, target_row, target_col);
	}

	if (write_to_log_file) {
		fprintf(log_file, "---------------------------------------------------------------------------------------------------------------\n");
		fprintf(log_file, "Round %d\n", round);
		output_target_queue(log_file, shooting_player);
		fprintf(log_file, "Recommended target is %d %d\n", target_row, target_col);
	}

	firing_result = fireAtTarget(shooting_player, target_player, target_row, target_col, &target_ship_type, &target_ship_sunk, &game_over);

	updatePlayers(shooting_player, target_player, firing_result, target_row, target_col, target_ship_type);

	if (strategy_test_mode == 0) {
		display_firing_result(target_player, firing_result, target_ship_type, target_ship_sunk, target_row, target_col, game_over);
		// output_firing_result();
	}

	if (write_to_log_file == 0) {
		//output_firing_result(target_player, firing_result, target_ship_type, target_ship_sunk, target_row, target_col, game_over);
	}

	if (target_ship_sunk == 1) {
			updateTargetingQueue(shooting_player, target_ship_type);	// when a ship is sunk, update the target queue to remove nearby future targets
			if (strategy_test_mode == 0) {
			
				//fprintf(log_file, "HIT and SUNK !!!% s lost a % s\n", target_player->name, ship_type_to_ship_name(target_ship_type));
			}
	}

	if (strategy_test_mode == 0) {
		display_boards(shooting_player, target_player, 0);
		display_target_queue(shooting_player);
	}
	if (write_to_log_file) {
		output_strategy(log_file, shooting_player);
		output_boards(log_file, target_player, shooting_player);
		output_target_queue(log_file, shooting_player);
	}

	return(firing_result);
}


int main(void) {
	
	struct player_data player_1;
	struct player_data player_2;
	FILE* log_file = NULL, * csv_file = NULL;

	srand((unsigned int) time(0));


	if (write_to_log_file) {
		fopen_s(&log_file, "battleship.log", "w");						// Open the log file	
		errno_t result = fopen_s(&csv_file, "battleship.csv", "a+");	// Open the cvs file
	}

	// Set the CTRL-C handler so we can exit nicely
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	int tournament_ties = 0;
	int tournament_p1_wins = 0;
	int tournament_p2_wins = 0;
	int GAMES = 0;


	if (strategy_test_mode == 0) system("cls");
	if (strategy_test_mode == 0) printf("%s", HIDE_CURSOR); else printf(SAVE_CURSOR_POSN);

	for (GAMES = 0; (GAMES < 1000000) && (CTRL_C==0); GAMES++) {
		
		//Strategies
		// 1: Random guess each turn
		// 2: User enters a target
		// 3: Random guessing until a hit, then fill target queue with surrounding coordinates
		// 4: Random guessing until a hit, then fill target queue with surrounding coordinates. trim queue when orientation is known
		// 5: Fill queue at start of game with a set of targets, then random guessing


		initialize_player(&player_1, 1, "Strategy 3", 3);
		initialize_player(&player_2, 2, "Strategy 4", 4);
		// debug_place_ships_on_board(&player_1);  			add_new_hit_to_queue(&player_2, 5, 1, UnknownShip);
		// debug_place_ships_on_board(&player_2);			add_new_hit_to_queue(&player_1, 5, 1, UnknownShip);
		randomly_place_ships_on_board(&player_1);
		randomly_place_ships_on_board(&player_2);

		if (strategy_test_mode == 0) {
			display_tournament_line(&player_1, &player_2, GAMES, tournament_p1_wins, tournament_p2_wins, tournament_ties);
		} 
		else {
			if ((GAMES>0) && (GAMES % 1000 == 0)) {
				double p1winpercentage = (double)tournament_p1_wins / (tournament_p1_wins + tournament_p2_wins + tournament_ties) * 100;
				double p2winpercentage = (double)tournament_p2_wins / (tournament_p1_wins + tournament_p2_wins + tournament_ties) * 100;
				double tiepercentage = (double)tournament_ties / (tournament_p1_wins + tournament_p2_wins + tournament_ties) * 100;
				//printf("%sGAMES: %d   %s wins:%d %2.0f%%    %s wins:%d %2.0f%%  ties:%d %2.0f%%   total:%d\n", RESTORE_CURSOR_POSN, GAMES, player_1.name, tournament_p1_wins, p1winpercentage, player_2.name, tournament_p2_wins, p2winpercentage, tournament_ties, tiepercentage, (tournament_p1_wins + tournament_p2_wins + tournament_ties));
			}
		}




		game_over = 0;


		int round = 1; // game round
		int p1_wins = 0, p2_wins = 0;

		while ((game_over == 0) && (round < 100)) {

			// if (strategy_test_mode == 0) printf("GAME: %d   %s wins:%d    %s wins:%d   ties:%d\n", GAMES, player_1.name, tournament_p1_wins, player_2.name, tournament_p2_wins, tournament_ties);
			display_round_line(round);
			display_divider_line(&player_1);
			display_divider_line(&player_2);

			p1_wins = play_a_round(&player_1, &player_2, log_file, round);

			p2_wins = play_a_round(&player_2, &player_1, log_file, round);

			
			if (strategy_test_mode == 0) {
				if (pause_between_shots_mode == 1) {
					if (game_over != 1) {
						char answer;
						printf("%sExit game ?  (Enter '1' for yes, '2' or just 'Enter' for no): ", LINE40_CURSOR);
						scanf_s("%c", &answer, 1);
						if (answer == '1') game_over = 1;
					}
				}
			}
			
			if ((p1_wins == 1) || (p2_wins == 1)) game_over = 1;
			round++;
		//	if ((game_over == 0) && (round < 100) && (strategy_test_mode == 0))
		//		  printf("%s%s", BG_black , HOME_CURSOR); // system("cls");//ESC [ <y> ; <x> H  //"\x1b[0G\x1[0d"
		}

		if ((p1_wins == 1) && (p2_wins == 1)) {
			tournament_ties++;
		}
		else if (p1_wins == 1) tournament_p1_wins++;
		else tournament_p2_wins++;

		
		//display_stats(&player_1, &player_2, round);
		if (write_to_log_file) {
			output_CSV(csv_file, &player_1, &player_2, round);
			output_stats(log_file, &player_1, &player_2, round);
		}

	}


	if (strategy_test_mode == 0) display_restore_screen();
	/*
	double p1winpercentage = (double)tournament_p1_wins / (tournament_p1_wins + tournament_p2_wins + tournament_ties) * 100;
	double p2winpercentage = (double)tournament_p2_wins / (tournament_p1_wins + tournament_p2_wins + tournament_ties) * 100;
	double tiepercentage = (double)tournament_ties / (tournament_p1_wins + tournament_p2_wins + tournament_ties) * 100;
	printf("\nGAMES: %d   %s wins:%d %2.0f%%    %s wins:%d %2.0f%%  ties:%d %2.0f%%   total:%d\n", GAMES, player_1.name, tournament_p1_wins, p1winpercentage, player_2.name, tournament_p2_wins, p2winpercentage, tournament_ties, tiepercentage, (tournament_p1_wins + tournament_p2_wins + tournament_ties));
	*/
	if (write_to_log_file) _fcloseall();
}



