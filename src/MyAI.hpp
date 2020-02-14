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
#include <utility>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define MAX_DIMENSION 7

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
	enum Direction { NORTH, SOUTH, EAST, WEST };

	struct Tile {
		bool discovered = false;
		bool safe = false;
		bool pit = false; 
		bool wumpus = false;
		bool p_pit = false;
		bool p_wumpus = false;
		bool is_breeze = false;
		bool visited = false;
	};

	struct World {
		Tile tiles[MAX_DIMENSION][MAX_DIMENSION];
	} world;

	int backtrack;
	vector<pair<int,int>> prev_tiles; 
	vector<pair<int, int>> desired_path;
	pair<int, int> curr_position;
	Direction curr_dir= EAST;
	Direction desired_dir = EAST;

	void mark_discovered();
	void mark_safe();
	void mark_p_wumpus();
	void mark_p_pit();
	void remove_p_wumpus();
	void remove_p_pit();
	void scan();
	pair<int, int> find_closest_tile();
	void make_path(pair<int, int> desired_tile);

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif