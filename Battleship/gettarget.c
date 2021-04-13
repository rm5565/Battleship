

#include "header.h"



void randmonly_pick_a_target(struct player_data* shooting_player, int* target_row, int* target_col) {
	int row, col;
	int target_ok = 0;

	while (target_ok == 0) {
		row = rand() % 10;
		col = rand() % 10;
		if ((row >= 0) && (row <= 9) && (col >= 0) && (col <= 9)) {
			if (shooting_player->enemy_board[row][col] == ' ')   // check if that cell was already targetted by looking in the history
				target_ok = 1;
		}
	}
	*target_row = row;
	*target_col = col;
}



void getTargetRecommendation(struct player_data* shooting_player, int* target_row, int* target_col, int round) {
	
	switch (shooting_player->strategy) {

	case 1:
		randmonly_pick_a_target(shooting_player, target_row, target_col);
		break;

	case 2:
		break;

	case 3:
	case 4:
	case 5:
		if (shooting_player->target_queue.read_ptr != shooting_player->target_queue.write_ptr) { // there's a possible target in the queue
			*target_row = shooting_player->target_queue.queue[shooting_player->target_queue.read_ptr][0];
			*target_col = shooting_player->target_queue.queue[shooting_player->target_queue.read_ptr][1];
			shooting_player->target_queue.read_ptr = (shooting_player->target_queue.read_ptr + 1) % target_queue_size_max;
		}
		else
			randmonly_pick_a_target(shooting_player, target_row, target_col);
		break;
	}

}