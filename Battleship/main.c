
#include "header.h"

#define _CRT_SECURE_NO_WARNINGS


char p1_game_board[10][10];
char p2_game_board[10][10];

int game_over = 0;  // 0 = game is still going,  1 = game over!  global var
int CTRL_C = 0;
boolean animate_display = 1;
int pause_between_shots_mode = 0;
int write_to_log_file = 0;
int write_to_csv_file = 0;

// Play a round 
// return 1 if game over
int play_a_round(struct player_data* shooting_player, struct player_data* target_player, FILE* log_file, FILE* csv_file, int round) {
	int target_row = 0, target_col = 0;
	int game_over = 0;
	int firing_result;
	int target_ship_type = 0;
	int target_ship_sunk = 0;
	int player_won_match = 0;

	getTargetRecommendation(shooting_player, &target_row, &target_col, round);

	if (animate_display) {
		display_target_queue(shooting_player);
		display_recommended_target(shooting_player, target_row, target_col);
	}

	if (write_to_log_file) {
		fprintf(log_file, "---------------------------------------------------------------------------------------------------------------\n");
		fprintf(log_file, "Round %d\n", round);
		output_target_queue(log_file, shooting_player);
		fprintf(log_file, "Recommended target is %d %d\n", target_row, target_col);
	}

	firing_result = fireAtTarget(shooting_player, target_player, target_row, target_col, &target_ship_type);

	updatePlayers(shooting_player, target_player, firing_result, target_row, target_col, target_ship_type);

	// check if the ship was sunk
	if (firing_result == 1) {
		target_ship_sunk = check_if_sunk_ship(target_player, ship_type_to_char(target_ship_type));
		if (target_ship_sunk == 1)
		{
			player_won_match = check_if_all_ships_sunk(target_player);
		}
	}


	if (animate_display) {
		display_firing_result(shooting_player, firing_result, target_ship_type, target_ship_sunk, target_row, target_col, player_won_match);
	}

	if (write_to_log_file == 0) {
		//output_firing_result(shooting_player, firing_result, target_ship_type, target_ship_sunk, target_row, target_col, player_won_match);
	}

	if (target_ship_sunk == 1) {
			updateTargetingQueue(shooting_player, target_ship_type);	// when a ship is sunk, update the target queue to remove nearby future targets
	}

	if (animate_display) {
		display_boards(shooting_player, target_player, 0);
		display_target_queue(shooting_player);
	}
	if (write_to_log_file) {
		output_strategy(log_file, shooting_player);
		output_boards(log_file, target_player, shooting_player);
		output_target_queue(log_file, shooting_player);
	}

	return(player_won_match);
}


int main(int argc, char *argv[]) {
	
	struct player_data player_1;
	struct player_data player_2;
	FILE* log_file = NULL, * csv_file = NULL;
	
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0) animate_display = 0;
		if (strcmp(argv[i], "-log") == 0) write_to_log_file = 1;
		if (strcmp(argv[i], "-csv") == 0) write_to_csv_file = 1;
		if (strcmp(argv[i], "-p") == 0) pause_between_shots_mode = 1;
		if (strcmp(argv[i], "-h") == 0) { display_help(); return; }
	}
	
	int strategy_test_mode = !animate_display;


	srand((unsigned int) time(0));


	if (write_to_log_file) fopen_s(&log_file, "battleship.log", "w");		// Rewrite open the log file	
	if (write_to_csv_file) fopen_s(&csv_file, "battleship.csv", "a+");		// Append open the cvs file

	// Set the CTRL-C handler so we can exit nicely
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	int tournament_ties = 0;
	int tournament_p1_wins = 0;
	int tournament_p2_wins = 0;
	int GAMES = 0;


	if (animate_display) system("cls");
	if (animate_display) printf(HIDE_CURSOR); 
	if (strategy_test_mode) printf(SAVE_CURSOR_POSN);


	for (GAMES = 0; (GAMES < 1000000) && (CTRL_C == 0); GAMES++) {

		// Strategies
		// 1: Random guess each turn
		// 2: User enters a target
		// 3: Random guessing until a hit, then fill target queue with surrounding coordinates
		// 4: Random guessing until a hit, then fill target queue with surrounding coordinates. Trim queue when orientation is known
		// 5: Fill queue at start of game with a set of targets, then random guessing

		initialize_player(&player_1, 1, "Strategy 4", 4);
		initialize_player(&player_2, 2, "Strategy 3", 3);
		//debug_place_ships_on_board(&player_1);  			add_new_hit_to_queue(&player_2, 5, 1, UnknownShip);
		//debug_place_ships_on_board(&player_2);			add_new_hit_to_queue(&player_1, 5, 1, UnknownShip);
		randomly_place_ships_on_board(&player_1);
		randomly_place_ships_on_board(&player_2);

		if (animate_display) 
			display_tournament_line(&player_1, &player_2, GAMES, tournament_p1_wins, tournament_p2_wins, tournament_ties);
		else if ((GAMES > 0) && (GAMES % 1000 == 0)) 
			display_tournament_line_in_strategy_test_mode(&player_1, &player_2, GAMES, tournament_p1_wins, tournament_p2_wins, tournament_ties);
			
		game_over = 0;


		int round = 1; // game round
		int p1_wins = 0, p2_wins = 0;

		while ((game_over == 0) && (round < 100)) {

			if (animate_display) {
				display_round_line(round);
				display_divider_line(&player_1);
				display_divider_line(&player_2);
			}

			p1_wins = play_a_round(&player_1, &player_2, log_file, csv_file, round);
			p2_wins = play_a_round(&player_2, &player_1, log_file, csv_file, round);
			
			if (animate_display) {
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
		//	if ((game_over == 0) && (round < 100) && (animate_display))
		//		  printf("%s%s", BG_black , HOME_CURSOR); // system("cls");//ESC [ <y> ; <x> H  //"\x1b[0G\x1[0d"
		}

		if ((p1_wins == 1) && (p2_wins == 1)) {
			tournament_ties++;
		}
		else if (p1_wins == 1) tournament_p1_wins++;
		else tournament_p2_wins++;

		
		//display_stats(&player_1, &player_2, round);
		if (write_to_log_file) output_stats(log_file, &player_1, &player_2, round);
		if (write_to_csv_file) output_CSV(csv_file, &player_1, &player_2, round);
	}

	if (animate_display)
		display_tournament_line(&player_1, &player_2, GAMES, tournament_p1_wins, tournament_p2_wins, tournament_ties);
	else if ((GAMES > 0) && (GAMES % 1000 == 0))
		display_tournament_line_in_strategy_test_mode(&player_1, &player_2, GAMES, tournament_p1_wins, tournament_p2_wins, tournament_ties);

	if (animate_display) display_restore_screen();
	if (write_to_log_file || write_to_csv_file) _fcloseall();
}



