// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"

// *  stuff we added ourselves
#include <iostream>
#include <stack>
#include <utility>

using namespace std;
// *

class MyAI : public Agent
{
public:
	MyAI ( void );
	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);
	
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
private:
	enum Status { UNMARKED, SAFE, PIT, WUMPUS, POSSIBLE_DANGER };
	enum Direction { NORTH, SOUTH, EAST, WEST };

	struct Tile {
		Status status;
		Tile(): status(UNMARKED) {};
	};

	struct World {
		Tile tiles[7][7];
	} world;

	int tile_max = 7;
	int backtrack;
	stack <Tile> prev;
	stack <Tile> future;
	pair <int, int> curr_position;
	Direction direction = EAST;

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif