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
	world.tiles[curr_position.first][curr_position.second].status = SAFE;
	
	// Probability that will fall into a pit right away at spawn if breeze too high so climb
	if (curr_position.first == 0 & curr_position.second == 0 & (breeze || stench))	// Test how AI does without this line
		return CLIMB;

	// If tile no breeze/stench, adjacent tiles are safe
	if (!stench && !breeze) {
		if (curr_position.first > 0)
			world.tiles[curr_position.first-1][curr_position.second] = SAFE;
		if (curr_position.first < tile_max)
			world.tiles[curr_position.first+1][curr_position.second] = SAFE;
		if (curr_position.second > 0)
			world.tiles[curr_position.first][curr_position.second-1] = SAFE;
		if (curr_position.first < tile_max)
			world.tiles[curr_position.first][curr_position.second+1] = SAFE;
	}
	
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


// ======================================================================
// YOUR CODE ENDS
// ======================================================================