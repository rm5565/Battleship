

#include "header.h"




char ship_type_to_char(int ship_type) {
	switch (ship_type) {
		case Carrier:		return 'c';	break;
		case Battleship:	return 'b';	break;
		case Cruiser:		return 'r';	break;
		case Submarine:		return 's';	break;
		case Destroyer:		return 'd';	break;
		default:			return '?';	
	}
}

char ship_type_to_CHAR(int ship_type) {
	switch (ship_type) {
		case Carrier:		return 'C';	break;
		case Battleship:	return 'B';	break;
		case Cruiser:		return 'R';	break;
		case Submarine:		return 'S';	break;
		case Destroyer:		return 'D';	break;
		default:			return '?';	
	}
}
	


int char_to_ship(char ch) {
	switch (ch) {
		case 'c': return Carrier;		break;
		case 'b': return Battleship;	break;
		case 'r': return Cruiser;		break;
		case 's': return Submarine;		break;
		case 'd': return Destroyer;		break;
	}
	return UnknownShip;
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

char* char_to_ship_name(char ch) {
	switch (ch) {
	case 'c': return "Carrier"; break;
	case 'b': return "Battleship"; break;
	case 'r': return "Cruiser"; break;
	case 's': return "Submarine"; break;
	case 'd': return "Destroyer"; break;
	}
	return "Unknown Ship";
}

char* ship_type_to_ship_name(int ship_type) {
	switch (ship_type) {
	case Carrier:			return "Carrier"; break;
	case Battleship:		return "Battleship"; break;
	case Cruiser:			return "Cruiser"; break;
	case Submarine:			return "Submarine"; break;
	case Destroyer:			return "Destroyer"; break;
	}
	return "Unknown Ship";
}

