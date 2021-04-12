#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS

// https://docs.microsoft.com/en-us/windows/terminal/customize-settings/color-schemes
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#cursor-positioning
#define BG_sea				"\x1b[44m"
#define FG_sea_unknown		"\x1b[1;30m"
#define FG_sea_hit			"\x1b[31m"
#define FG_sea_radar		"\x1b[1;32m"
#define FG_sea_missed		"\x1b[1;37m"
#define axis_label			"\x1b[46m\x1b[1;34m"
#define BG_black			"\x1b[40m"
#define ERASE_TO_EOL		"\x1b[0K"			// Erase to end of line
#define ERASE_DISPLAY		"\x1b[0;0H\x1b[0J"   //   ESC [ <y> ; <x> H
#define SHOW_CURSOR			"\x1b[?25h"			// DECTCEM	Text Cursor Enable Mode Show
#define HIDE_CURSOR			"\x1b[?25l"			// DECTCEM	Text Cursor Enable Mode Hide
#define HOME_CURSOR			"\x1b[0;0H"
#define LINE25_CURSOR		"\x1b[40;0H"
#define SAVE_CURSOR_POSN	"\x1b[s"
#define RESTORE_CURSOR_POSN	"\x1b[u"


enum ship_enum { Carrier, Battleship, Cruiser, Submarine, Destroyer, UnknownShip };
enum orientation_enum { Unknown_ORIENTATION, Vertical, Horizontal };


struct target_queue_data {
	int read_ptr;
	int write_ptr;
	int size;
	int queue[100][3];  // upto 100 surrounding cells 
};


struct strategy3_data {
	int saved_space;

};


struct strategy4_data {
	int saved_space;

};


struct player_data {
	char name[20];
	char game_board[10][10];
	char enemy_board[10][10];   // place to store results of our bombardments
	int total_hits;
	int total_misses;
	int strategy;

	struct strategy3_data strategy3;
	struct strategy3_data strategy4;
	struct target_queue_data target_queue;

};



// These are implemented in functions.c
int select_who_starts_first(void);
int check_if_sunk_ship(struct player_data* target_player, char target_ship_type);
int check_if_all_ships_sunk(struct player_data* target_player);
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

// Implemented in targetting.c
void enter_a_target(struct player_data* shooting_player, struct player_data* target_player, int* target_row, int* target_col);

// Implemented in boardsetup.c
void initialize_player(struct player_data* player, char* name, int strategy);
void manually_place_ships_on_board(struct player_data* player);
void randomly_place_ships_on_board(struct player_data* player); 
void debug_place_ships_on_board(struct player_data* player);

// Implemented in logging.c
void output_current_move(FILE* log_file, struct player_data* player, int target_row, int target_col, int hit_or_miss, int target_sunk, int game_over);
void output_stats(FILE* log_file, struct player_data* player_1, struct player_data* player_2, int rounds_played);
void output_strategy(FILE* log_file, struct player_data* shooting_player);
void output_boards(FILE* log_file, struct player_data* player_1, struct player_data* player_2);
void fill_in_radar(char radar[10][10], struct player_data* shooting_player, struct player_data* target_player);
void output_CSV(FILE* csv_file, struct player_data* player_1, struct player_data* player_2, int rounds_played);
void output_target_queue(FILE* log_file, struct player_data* player);


// Implemented in conversions.c
char ship_to_char(int ship_type);
char ship_type_to_CHAR(int ship_type);
int char_to_ship_size(char ch);
char* char_to_ship_name(char ch);
char* ship_type_to_ship_name(int ship_type);
int ship_type_to_ship_size(int ship_type);
int char_to_ship(char ch);


// Implemented in gettarget.h
void getTargetRecommendation(struct player_data* shooting_player, int* target_row, int* target_col, int round);

// Implemented in fireat.c
int fireAtTarget(struct player_data* shooting_player, struct player_data* target_player, int target_row, int target_col, int* ship_type);

// Implemented in update.c
void updatePlayers(struct player_data* shooting_player, struct player_data* target_player, int firing_result, int target_row, int target_col, int ship_type);
void updateTargetingQueue(struct player_data* shooting_player, int target_ship_type);
void add_new_hit_to_queue(struct player_data* shooting_player, int target_row, int target_col, int ship_type);

// Implemented in display.c
void display_welcome_screen(void);
void display_target_queue(struct player_data* player);
void display_stats(struct player_data* player_1, struct player_data* player_2, int rounds_played);
void display_boards(struct player_data* shooting_player, struct player_data* target_player, int hidden);
void display_radar(struct player_data* shooting_player, struct player_data* target_player);
