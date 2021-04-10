

#include "header.h"




char ship_to_char(int ship_type) {
	if (ship_type == Carrier) return 'c';
	if (ship_type == Battleship) return 'b';
	if (ship_type == Cruiser) return 'r';
	if (ship_type == Submarine) return 's';
	return 'd';
}

ship_type_to_CHAR(int ship_type) {
	if (ship_type == Carrier) return 'C';
	if (ship_type == Battleship) return 'B';
	if (ship_type == Cruiser) return 'R';
	if (ship_type == Submarine) return 'S';
	return 'D';
}
	


int char_to_ship(char ch) {
	switch (ch) {
		case 'c': return Carrier;		break;
		case 'b': return Battleship;	break;
		case 'r': return Cruiser;		break;
		case 's': return Submarine;		break;
	}
	return Destroyer;
}


int char_to_ship_size(char ch) {
	switch (ch) {
	case 'c': return 5; break;
	case 'b': return 4; break;
	case 'r': return 3; break;
	case 's': return 3; break;
	}
	return 2;
}


int ship_type_to_ship_size(int ship_type) {

	switch (ship_type) {
	case Carrier:		return 5; break;
	case Battleship:	return 4; break;
	case Cruiser:		return 3; break;
	case Submarine:		return 3; break;
	}
	return 2;


}

char* get_ship_name(char ch) {
	switch (ch) {
	case 'c': return "Carrier"; break;
	case 'b': return "Battleship"; break;
	case 'r': return "Cruiser"; break;
	case 's': return "Submarine"; break;
	}
	return "Destroyer";
}

char* ship_type_to_ship_name(int ship_type) {
	switch (ship_type) {
	case Carrier: return "Carrier"; break;
	case Battleship: return "Battleship"; break;
	case Cruiser: return "Cruiser"; break;
	case Submarine: return "Submarine"; break;
	}
	return "Destroyer";
}

