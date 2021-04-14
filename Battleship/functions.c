

#include "header.h"

#define _CRT_SECURE_NO_WARNINGS


extern int game_over;
extern int tournament_over;
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
		tournament_over = 1;
		return TRUE;
	default: return FALSE;
	}
}



