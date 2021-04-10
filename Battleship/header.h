#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS


struct ship_info { // once we have first hit, we can know all possible locations for that ship, left&right, up&down
	int status;									// 0 = not found, 1 = found still floating, 2 = sunk
	int vertical[10][3];						// 10 possible locations, each location has x, y, and confidence value
	int horizontal[10][3];						// confidence values:  0=moderate, 1=certain
	int vertical_count, horizontal_count;		// how many in each array
	int first_hit_row, first_hit_col;			// where the first hit landed
	int tried_top, tried_bottom;				// taking shots around original hit location to determine orientation
	int tried_left, tried_right;				// taking shots around original hit location to determine orientation
	int orientation;							// 0=unknown,  1=vertical,   2=horizontal
};


enum ship_enum { Carrier, Battleship, Cruiser, Submarine, Destroyer };
enum orientation_enum { Unknown, Vertical, Horizontal };


struct strategy3_data {

	int temp;
};


struct strategy2_data {
	// int original_hit_location_row, original_hit_location_col;
	int latest_hit_location_row, latest_hit_location_col;
	int strategy_stage;				// 0 = searching randomly, 1 =  2 =
	struct ship_info ships[5];  // carrier, battleship, cruiser, submarine, destroyer
	int ship_of_interest_type;
	char strategy_message[64];
};

struct player_data {
	char name[20];
	char game_board[10][10];
	char enemy_board[10][10];   // place to store results of our bombardments
	int total_hits;
	int total_misses;
	int strategy;
	struct strategy3_data strategy3;

	struct strategy2_data strategy2;
};



// These are implemented in functions.c
void  welcome_screen(void);
int select_who_starts_first(void);
void display_boards(struct player_data* player, int hidden);
void display_radar(struct player_data* shooting_player, struct player_data* target_player);
int check_shot(struct player_data* shooting_player, struct player_data* target_player, int target_row, int target_col, char* ship_type_hit);
int check_if_sunk_ship(struct player_data* target_player, char target_ship_type);
int is_winner(struct player_data* target_player);

// Implemented in targetting.c
void enter_a_target(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col);
void pick_a_target(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col);
void randmonly_pick_a_target(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col);

// Implemented in strategy2.c

void init_strategy2(struct player_data* shooting_player);
void pick_a_target_using_strategy2(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col);

// Implemented in boardsetup.c
void initialize_player(struct player_data* player, char* name);
void manually_place_ships_on_board(struct player_data* player);
void randomly_place_ships_on_board(struct player_data* player); 
void debug_place_ships_on_board(struct player_data* player);

// Implemented in logging.c
void output_current_move(FILE* log_file, struct player_data* player, int target_row, int target_col, int hit_or_miss, int target_sunk, int game_over);
void output_stats(FILE* log_file, struct player_data* player);
void output_strategy(FILE* log_file, struct player_data* shooting_player);
void output_boards(FILE* log_file, struct player_data* player_1, struct player_data* player_2);
void fill_in_radar(char radar[10][10], struct player_data* shooting_player, struct player_data* target_player);

// Implemented in conversions.c
char ship_to_char(int ship_type);
int char_to_ship_size(char ch);
char* get_ship_name(char ch);
char* ship_type_to_ship_name(int ship_type);
int ship_type_to_ship_size(int ship_type);
int char_to_ship(char ch);


// Implemented in gettarget.h
void getTargetRecommendation(struct player_data* shooting_player, int* target_row, int* target_col);

// Implemented in fireat.c
int fireAtTarget(struct player_data* shooting_player, struct player_data* target_player, int target_row, int target_col, int* ship_type);

// Implemented in update.c
void updatePlayers(struct player_data* shooting_player, struct player_data* target_player, int firing_result, int target_row, int target_col, int* ship_type);
