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
	world.tiles[curr_position.first][curr_position.second].safe = true;
	world.tiles[curr_position.first][curr_position.second].visited = true;
	prev_tiles.push_back(curr_position);

	if (bump) {
		if (curr_dir == NORTH) {
			MAX_Y = curr_position.second+1;
			// world.tiles.resize(MAX_Y);
		}
		else if (curr_dir == EAST) {
			MAX_X = curr_position.first+1;
			/*
			for (int i = 0; i < world.tiles.size; ++i)
			{
				world.tiles[i].resize(MAX_X);
			}
			*/
		}
		if (curr_dir == WEST)
			curr_position.first++;
		else if (curr_dir == EAST)
			curr_position.first--;
		else if (curr_dir == SOUTH)
			curr_postion.second++; 
		else if (curr_dir == NORTH)
			curr_position.second--;
	}
	
	// Probability that will fall into a pit right away at spawn if breeze too high so climb
	if (curr_position.first == 0 && curr_position.second == 0 && (breeze || stench))	// Test how AI does without this line
		return CLIMB;

	// If tile no breeze/stench, adjacent tiles are safe
	if (!stench && !breeze) {
		mark_safe();
	}
	else {
		if (stench) {
			mark_p_wumpus();
		}
		else if (!stench) {
			remove_p_wumpus();
		}
		
		if (breeze) {
			mark_p_pit();
		}
		else if (!breeze) {
			remove_p_pit();
		}
	}

	scan();
	print_world();

	if ( glitter ) {
		has_gold = true;
		return GRAB;
	}
	if (has_gold) {
		make_path(make_pair(0,0));
	} 
	else {
		make_path(find_closest_tile());
	}

	set_direction();
	return face_direction();

	return CLIMB;
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
void MyAI::mark_discovered() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].discovered = true;
	} 
	if (curr_position.first < MAX_X)
	{
		world.tiles[curr_position.first+1][curr_position.second].discovered = true;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].discovered = true;
	}
	if (curr_position.second < MAX_Y)
	{
		world.tiles[curr_position.first][curr_position.second+1].discovered = true;
	}
}

void MyAI::mark_safe() {
	if (curr_position.first > 0)
		world.tiles[curr_position.first-1][curr_position.second].safe = true;		
	if (curr_position.first < MAX_X)
		world.tiles[curr_position.first+1][curr_position.second].safe = true;
	if (curr_position.second > 0)
		world.tiles[curr_position.first][curr_position.second-1].safe = true;
	if (curr_position.second < MAX_Y)
		world.tiles[curr_position.first][curr_position.second+1].safe = true;
	mark_discovered();
}

void MyAI::mark_p_wumpus() {
	if (curr_position.first > 0 && world.tiles[curr_position.first-1][curr_position.second].discovered == false)
		world.tiles[curr_position.first-1][curr_position.second].p_wumpus = true;
	if (curr_position.first < MAX_X && world.tiles[curr_position.first+1][curr_position.second].discovered == false)
		world.tiles[curr_position.first+1][curr_position.second].p_wumpus = true;
	if (curr_position.second > 0 && world.tiles[curr_position.first][curr_position.second-1].discovered == false)
		world.tiles[curr_position.first][curr_position.second-1].p_wumpus = true;
	if (curr_position.second < MAX_Y && world.tiles[curr_position.first][curr_position.second+1].discovered == false)
		world.tiles[curr_position.first][curr_position.second+1].p_wumpus = true;
	mark_discovered();
}

void MyAI::mark_p_pit() {
	if (curr_position.first > 0 && world.tiles[curr_position.first-1][curr_position.second].discovered == false)
	{
		world.tiles[curr_position.first-1][curr_position.second].p_pit = true;
	} 
	if (curr_position.first < MAX_X && world.tiles[curr_position.first+1][curr_position.second].discovered == false)
	{
		world.tiles[curr_position.first+1][curr_position.second].p_pit = true;
	}
	if (curr_position.second > 0 && world.tiles[curr_position.first][curr_position.second-1].discovered == false)
	{
		world.tiles[curr_position.first][curr_position.second-1].p_pit = true;
	}
	if (curr_position.second < MAX_Y && world.tiles[curr_position.first][curr_position.second+1].discovered == false)
	{
		world.tiles[curr_position.first][curr_position.second+1].p_pit = true;
	}
	world.tiles[curr_position.first][curr_position.second].is_breeze = true;
	mark_discovered();
}

void MyAI::remove_p_wumpus() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].p_wumpus = false;
	} 
	if (curr_position.first < MAX_X)
	{
		world.tiles[curr_position.first+1][curr_position.second].p_wumpus = false;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].p_wumpus = false;
	}
	if (curr_position.second < MAX_Y)
	{
		world.tiles[curr_position.first][curr_position.second+1].p_wumpus = false;
	}
}

void MyAI::remove_p_pit() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].p_pit = false;
	} 
	if (curr_position.first < MAX_X)
	{
		world.tiles[curr_position.first+1][curr_position.second].p_pit = false;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].p_pit = false;
	}
	if (curr_position.second < MAX_Y)
	{
		world.tiles[curr_position.first][curr_position.second+1].p_pit = false;
	}
}

void MyAI::scan() {
	int pw_count = 0;
	int pp_count;
	pair <int, int> p_wumpus_coords;
	pair <int, int> p_pit_coords;
	for (int x = 0; x < MAX_X; x++) {
		for (int y = 0; y < MAX_Y; y++)	{

			if (world.tiles[x][y].is_breeze) {
				pp_count = 0;
				if (x > 0) {
					if (world.tiles[x-1][y].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x-1, y);
				}
				if (x < MAX_X) {
					if (world.tiles[x+1][y].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x+1, y);
				}
				if (y > 0) {
					if (world.tiles[x][y-1].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x, y-1);
				}
				if (y < MAX_Y) {
					if (world.tiles[x][y+1].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x, y+1);
				}
			}
			if (world.tiles[x][y].p_pit == false && world.tiles[x][y].p_wumpus == false) {
				world.tiles[x][y].safe = true;;
			}
			if (world.tiles[x][y].p_wumpus = true)
			{
				pw_count++;
				p_wumpus_coords = make_pair(x, y);
			}
		}
	}
	if (pp_count == 1) {
		world.tiles[p_pit_coords.first][p_pit_coords.second].pit = true;
	}
	if (pw_count == 1) {
		world.tiles[p_wumpus_coords.first][p_wumpus_coords.second].wumpus = true;
	}
}

// Finds closest safe tile that is yet to be visited
pair<int, int> MyAI::find_closest_tile() {
	int minimum = 100; 
	pair<int, int> closest_tile; 
	for (int x = 0; x < MAX_X; x++) {
		for (int y = 0; y < MAX_Y; y++) {
			if (world.tiles[x][y].safe && !world.tiles[x][y].visited) { 
				int curr_distance = abs(curr_position.first - x) + abs(curr_position.second - y);
				if (curr_distance < minimum) {
					minimum = curr_distance;
					closest_tile = make_pair(x, y);
				}
			}
		}
	}
	return closest_tile;
}

// Finds path to desired tile from current position
void MyAI::make_path(pair<int, int> desired_tile) {
	for (auto tile = prev_tiles.rbegin(); tile != prev_tiles.rend(); ++tile) {
		if ((*tile).first == desired_tile.first + 1 && (*tile).second == desired_tile.second || 
			(*tile).first == desired_tile.first - 1 && (*tile).second == desired_tile.second ||
			(*tile).second == desired_tile.second + 1 && (*tile).first == desired_tile.first ||
			(*tile).second == desired_tile.second - 1 && (*tile).first == desired_tile.first) {
				desired_path.push_back(*tile);
				return;
			}
		else {
			// if tile is not found, find() return the end of the vector
			std::vector<pair<int,int>>::iterator it = find(desired_path.begin(), desired_path.end(), *tile);
			if (it == desired_path.end()) 
				desired_path.push_back(*tile);
			else {
				int new_size = it - desired_path.begin() + 1;
				desired_path.resize(new_size);
			}
		}
	}
}

void MyAI::set_direction() {
	pair<int, int> current_tile;
	pair<int, int> tile_to_move_to;
	if (curr_position == desired_path[0]) {
		current_tile = desired_path[0];
		desired_path.erase(desired_path.begin());
		tile_to_move_to = desired_path[1];
	}
	else {
		tile_to_move_to = desired_path[0];
	}
	
	if (current_tile.first+1 == tile_to_move_to.first)
	{
		desired_dir = EAST;
	}
	else if (current_tile.first-1 == tile_to_move_to.first)
	{
		desired_dir = WEST;
	}
	else if (current_tile.second+1 == tile_to_move_to.second)
	{
		desired_dir = NORTH;
	}
	else if (current_tile.second-1 == tile_to_move_to.second)
	{
		desired_dir = SOUTH;
	}
}

Agent::Action MyAI::face_direction() {
	switch(desired_dir) {
		case WEST:
			if (curr_dir != WEST)
				return face_west();
			break;
		case EAST:
			if (curr_dir != EAST)
				return face_east();
			break;
		case SOUTH:
			if (curr_dir != SOUTH)
				return face_south();
			break;
		case NORTH:
			if (curr_dir != NORTH)
				return face_north();
			break;
		default:
			if (curr_dir == WEST)
				curr_position.first--;
			else if (curr_dir == EAST)
				curr_position.first++;
			else if (curr_dir == SOUTH)
				curr_postion.second--;
			else if (curr_dir == NORTH)
				curr_position.second++;
			return FORWARD;
	}
}

Agent::Action MyAI::face_west() {
	switch (curr_dir) {
		case EAST:
			return TURN_LEFT;
		case SOUTH:
			return TURN_RIGHT;
		case NORTH:
			return TURN_LEFT;
	}

}

Agent::Action MyAI::face_east() {
	switch (curr_dir) {
		case WEST:
			return TURN_LEFT;
		case SOUTH:
			return TURN_LEFT;
		case NORTH:
			return TURN_RIGHT;
	}
}

Agent::Action MyAI::face_south() {
	switch (curr_dir) {
		case WEST:
			return TURN_LEFT;
		case EAST:
			return TURN_RIGHT;
		case NORTH:
			return TURN_LEFT;
	}
}

Agent::Action MyAI::face_north() {
	switch (curr_dir) {
		case WEST:
			return TURN_RIGHT;
		case EAST:
			return TURN_LEFT;
		case SOUTH:
			return TURN_LEFT;
	}
}

void MyAI::print_world() {
	for (int x = 0 ; x < MAX_X; ++x) {
		for (int y = MAX_Y; y >= 0; --y) {
			if (world.tiles[x][y].discovered)
				cout << "D"; 
			if (world.tiles[x][y].safe) 
				cout << "S";
			if (world.tiles[x][y].pit)
				cout << "P"; 
			if (world.tiles[x][y].wumpus)
				cout << "W"; 
			if (world.tiles[x][y].p_pit)
				cout << "P_P"; 
			if (world.tiles[x][y].p_wumpus)
				cout << "P_W"; 
			if (world.tiles[x][y].is_breeze) 
				cout << "B";
			if (world.tiles[x][y].visited)
				cout << "V"; 
			cout << "\t\t";
		}
		cout << endl;
	}
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================