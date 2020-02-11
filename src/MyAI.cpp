// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}
	
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	// Mark current tile safe
	world.tiles[curr_position.first][curr_position.second].status[0] = SAFE;
	
	// Probability that will fall into a pit right away at spawn if breeze too high so climb
	if (curr_position.first == 0 & curr_position.second == 0 & (breeze || stench))	// Test how AI does without this line
		return CLIMB;

	// If tile no breeze/stench, adjacent tiles are safe
	if (!stench && !breeze) {
		mark_safe();
	}

	else if (stench || breeze) {
		if (stench) {
			mark_p_wumpus();
		}
		if (breeze) {
			mark_p_pit();
		}
	}

	else if (!stench || !breeze) {
		if (!stench) {
			remove_p_wumpus();
		}
		if (!breeze) {
			remove_p_pit();
		}
	}

	scan();

	
	if ( glitter )
		return GRAB;
	
	return CLIMB;
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
void MyAI::mark_safe() {
	if (curr_position.first > 0)
		world.tiles[curr_position.first-1][curr_position.second].status[0] = SAFE;
	if (curr_position.first < MAX_DIMENSION)
		world.tiles[curr_position.first+1][curr_position.second].status[0] = SAFE;
	if (curr_position.second > 0)
		world.tiles[curr_position.first][curr_position.second-1].status[0] = SAFE;
	if (curr_position.first < MAX_DIMENSION)
		world.tiles[curr_position.first][curr_position.second+1].status[0] = SAFE;
}

void MyAI::mark_p_wumpus() {
	if (curr_position.first > 0 && world.tiles[curr_position.first-1][curr_position.second].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first-1][curr_position.second].status[1] = P_WUMPUS;
		world.tiles[curr_position.first-1][curr_position.second].status[0] = MARKED;
	} 
	if (curr_position.first < MAX_DIMENSION && world.tiles[curr_position.first+1][curr_position.second].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first+1][curr_position.second].status[1] = P_WUMPUS;
		world.tiles[curr_position.first+1][curr_position.second].status[0] = MARKED;
	}
	if (curr_position.second > 0 && world.tiles[curr_position.first][curr_position.second-1].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first][curr_position.second-1].status[1] = P_WUMPUS;
		world.tiles[curr_position.first][curr_position.second-1].status[0] = MARKED;
	}
	if (curr_position.first < MAX_DIMENSION && world.tiles[curr_position.first][curr_position.second+1].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first][curr_position.second+1].status[1] = P_WUMPUS;
		world.tiles[curr_position.first][curr_position.second+1].status[0] = MARKED;
	}
}

void MyAI::mark_p_pit() {
	if (curr_position.first > 0 && world.tiles[curr_position.first-1][curr_position.second].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first-1][curr_position.second].status[0] = P_PIT;
	} 
	if (curr_position.first < MAX_DIMENSION && world.tiles[curr_position.first+1][curr_position.second].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first+1][curr_position.second].status[0] = P_PIT;
	}
	if (curr_position.second > 0 && world.tiles[curr_position.first][curr_position.second-1].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first][curr_position.second-1].status[0] = P_PIT;
	}
	if (curr_position.first < MAX_DIMENSION && world.tiles[curr_position.first][curr_position.second+1].status[0] == UNMARKED)
	{
		world.tiles[curr_position.first][curr_position.second+1].status[0] = P_PIT;
	}
	// breeze_arr.
}

void MyAI::remove_p_wumpus() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].status[1] = MARKED;
	} 
	if (curr_position.first < MAX_DIMENSION)
	{
		world.tiles[curr_position.first+1][curr_position.second].status[1] = MARKED;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].status[1] = MARKED;
	}
	if (curr_position.first < MAX_DIMENSION)
	{
		world.tiles[curr_position.first][curr_position.second+1].status[1] = MARKED;
	}
}

void MyAI::remove_p_pit() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].status[0] = MARKED;
	} 
	if (curr_position.first < MAX_DIMENSION)
	{
		world.tiles[curr_position.first+1][curr_position.second].status[0] = MARKED;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].status[0] = MARKED;
	}
	if (curr_position.first < MAX_DIMENSION)
	{
		world.tiles[curr_position.first][curr_position.second+1].status[0] = MARKED;
	}
}

void MyAI::scan() {
	int pw_count = 0;
	pair <int, int> p_wumpus_coords;
	for (int x = 0; x < MAX_DIMENSION; x++) {
		for (int y = 0; y < MAX_DIMENSION; y++)	{
			if (world.tiles[x][y].status[0] == MARKED && world.tiles[x][y].status[1] == MARKED) {
				world.tiles[x][y].status[0] = SAFE;
			}
			if (world.tiles[x][y].status[1] == P_WUMPUS)
			{
				pw_count++;
				p_wumpus_coords = make_pair(x, y);
			}

		}
	}
	if (pw_count == 1) {
		world.tiles[p_wumpus_coords.first][p_wumpus_coords.second].status[1] = WUMPUS;
	}
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================