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

bool DEBUG = true; 

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
	// if don't have gold yet after exhausting all safe spots and haven't killed wumpus, kill wumpus 

	if (scream) {
		world.tiles[wumpus_tile.first][wumpus_tile.second].safe = true;
		world.tiles[wumpus_tile.first][wumpus_tile.second].wumpus = false;
		world.tiles[wumpus_tile.first][wumpus_tile.second].p_wumpus = false;
		panic = false; 
		killed_wumpus = true;
	}

	if (panic && !killed_wumpus) {
		if ((curr_position.first + 1 == wumpus_tile.first && curr_position.second == wumpus_tile.second && curr_dir == WEST)
		|| (curr_position.first - 1 == wumpus_tile.first && curr_position.second == wumpus_tile.second && curr_dir == EAST)
		|| (curr_position.first == wumpus_tile.first && curr_position.second + 1 == wumpus_tile.second && curr_dir == SOUTH)
	 	|| (curr_position.first == wumpus_tile.first && curr_position.second - 1 == wumpus_tile.second && curr_dir == NORTH)) {
			return SHOOT
		}
		make_path(wumpus_tile);
		set_direction(); 
		return face_direction;
	}

	if (bump) {
		if (curr_dir == NORTH) {
			curr_position.second = curr_position.second - 1;
			MAX_Y = curr_position.second+1;
			for (int i = 0; i < world.tiles.size(); ++i)
			{
				world.tiles[i].resize(MAX_Y);
			}
		}
		else if (curr_dir == EAST) {
			curr_position.first = curr_position.first - 1;
			MAX_X = curr_position.first+1;
			world.tiles.resize(MAX_X);
		}

		else if (curr_dir == WEST)
			curr_position.first = curr_position.first + 1;
		else if (curr_dir == SOUTH)
			curr_position.second = curr_position.second + 1; 
	}
	
	world.tiles[curr_position.first][curr_position.second].safe = true;
	world.tiles[curr_position.first][curr_position.second].visited = true;
	world.tiles[curr_position.first][curr_position.second].discovered = true;
	if (prev_tiles.size() == 0)
		prev_tiles.push_back(curr_position);
	else if (prev_tiles.back().first != curr_position.first
		|| prev_tiles.back().second != curr_position.second)
		prev_tiles.push_back(curr_position);
	
	
	// Probability that will fall into a pit right away at spawn if breeze too high so climb
	if (curr_position.first == 0 && curr_position.second == 0 && (breeze || stench || has_gold || panic))	// Test how AI does without this line
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
	if (DEBUG)
		print_world();

	if ( glitter ) {
		has_gold = true;
		return GRAB;
	}

	
	
	if (has_gold || panic || killed_wumpus) {
		make_path(make_pair(0,0));
	} 
	else {
		make_path(find_closest_tile());
	}
	set_direction();
	
	return face_direction();

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
	if (curr_position.first < MAX_X-1)
	{
		world.tiles[curr_position.first+1][curr_position.second].discovered = true;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].discovered = true;
	}
	if (curr_position.second < MAX_Y-1)
	{
		world.tiles[curr_position.first][curr_position.second+1].discovered = true;
	}
}

void MyAI::mark_safe() {
	if (curr_position.first > 0) {
		world.tiles[curr_position.first-1][curr_position.second].safe = true;
		world.tiles[curr_position.first-1][curr_position.second].p_pit = false;
		world.tiles[curr_position.first-1][curr_position.second].p_wumpus = false;
	}
	if (curr_position.first < MAX_X-1) {
		world.tiles[curr_position.first+1][curr_position.second].safe = true;
		world.tiles[curr_position.first+1][curr_position.second].p_pit = false;
		world.tiles[curr_position.first+1][curr_position.second].p_wumpus = false;
	}
	if (curr_position.second > 0) {
		world.tiles[curr_position.first][curr_position.second-1].safe = true;
		world.tiles[curr_position.first][curr_position.second-1].p_pit = false;
		world.tiles[curr_position.first][curr_position.second-1].p_wumpus = false;
	}
	if (curr_position.second < MAX_Y-1) {
		world.tiles[curr_position.first][curr_position.second+1].safe = true;
		world.tiles[curr_position.first][curr_position.second+1].p_pit = false;
		world.tiles[curr_position.first][curr_position.second+1].p_wumpus = false;
	}
	mark_discovered();
}

void MyAI::mark_p_wumpus() {
	if (curr_position.first > 0 && world.tiles[curr_position.first-1][curr_position.second].discovered == false) {
		world.tiles[curr_position.first-1][curr_position.second].p_wumpus = true;
		world.tiles[curr_position.first-1][curr_position.second].safe = false;
	}
	if (curr_position.first < MAX_X-1 && world.tiles[curr_position.first+1][curr_position.second].discovered == false) {
		world.tiles[curr_position.first+1][curr_position.second].p_wumpus = true;
		world.tiles[curr_position.first+1][curr_position.second].safe = false;
	}
	if (curr_position.second > 0 && world.tiles[curr_position.first][curr_position.second-1].discovered == false) {
		world.tiles[curr_position.first][curr_position.second-1].p_wumpus = true;
		world.tiles[curr_position.first][curr_position.second-1].safe = false;
	}
	if (curr_position.second < MAX_Y-1 && world.tiles[curr_position.first][curr_position.second+1].discovered == false) {
		world.tiles[curr_position.first][curr_position.second+1].p_wumpus = true;
		world.tiles[curr_position.first][curr_position.second+1].safe = false;
	}
	mark_discovered();
}

void MyAI::mark_p_pit() {
	if (curr_position.first > 0 && world.tiles[curr_position.first-1][curr_position.second].discovered == false)
	{
		world.tiles[curr_position.first-1][curr_position.second].p_pit = true;
		world.tiles[curr_position.first-1][curr_position.second].safe = false;
	} 
	if (curr_position.first < MAX_X-1 && world.tiles[curr_position.first+1][curr_position.second].discovered == false)
	{
		world.tiles[curr_position.first+1][curr_position.second].p_pit = true;
		world.tiles[curr_position.first+1][curr_position.second].safe = false;
	}
	if (curr_position.second > 0 && world.tiles[curr_position.first][curr_position.second-1].discovered == false)
	{
		world.tiles[curr_position.first][curr_position.second-1].p_pit = true;
		world.tiles[curr_position.first][curr_position.second-1].safe = false;
	}
	if (curr_position.second < MAX_Y-1 && world.tiles[curr_position.first][curr_position.second+1].discovered == false)
	{
		world.tiles[curr_position.first][curr_position.second+1].p_pit = true;
		world.tiles[curr_position.first][curr_position.second+1].safe = false;
	}
	world.tiles[curr_position.first][curr_position.second].is_breeze = true;
	mark_discovered();
}

void MyAI::remove_p_wumpus() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].p_wumpus = false;
	} 
	if (curr_position.first < MAX_X-1)
	{
		world.tiles[curr_position.first+1][curr_position.second].p_wumpus = false;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].p_wumpus = false;
	}
	if (curr_position.second < MAX_Y-1)
	{
		world.tiles[curr_position.first][curr_position.second+1].p_wumpus = false;
	}
}

void MyAI::remove_p_pit() {
	if (curr_position.first > 0)
	{
		world.tiles[curr_position.first-1][curr_position.second].p_pit = false;
	} 
	if (curr_position.first < MAX_X-1)
	{
		world.tiles[curr_position.first+1][curr_position.second].p_pit = false;
	}
	if (curr_position.second > 0)
	{
		world.tiles[curr_position.first][curr_position.second-1].p_pit = false;
	}
	if (curr_position.second < MAX_Y-1)
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
				if (x < MAX_X-1) {
					if (world.tiles[x+1][y].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x+1, y);
				}
				if (y > 0) {
					if (world.tiles[x][y-1].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x, y-1);
				}
				if (y < MAX_Y-1) {
					if (world.tiles[x][y+1].p_pit == true)
						pp_count++;
						p_pit_coords = make_pair(x, y+1);
				}
			}
			if (world.tiles[x][y].p_pit == false && world.tiles[x][y].p_wumpus == false &&  world.tiles[x][y].discovered == true) {
				world.tiles[x][y].safe = true;
			}
			if (world.tiles[x][y].p_wumpus == true)
			{
				pw_count++;
				p_wumpus_coords = make_pair(x, y);
			}
		}
	}
	if (pp_count == 1) {
		world.tiles[p_pit_coords.first][p_pit_coords.second].pit = true;
		world.tiles[p_pit_coords.first][p_pit_coords.second].safe = false;

	}
	if (pw_count == 1) {
		world.tiles[p_wumpus_coords.first][p_wumpus_coords.second].wumpus = true;
		world.tiles[p_wumpus_coords.first][p_wumpus_coords.second].safe = false;
		wumpus_tile = p_wumpus_coords;
	}
}

// PATH FINDING CODE 
// Finds closest safe tile that is yet to be visited
pair<int, int> MyAI::find_closest_tile() {
	int minimum = 100; 
	pair<int, int> closest_tile = make_pair(0, 0); 
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

	// If no more non-visited safe tiles, have run out of options so panic
	if (closest_tile.first == 0 && closest_tile.second == 0) {
		panic = true;
	}

	if (DEBUG)
		cout << "CLOSEST TILE X: " << closest_tile.first << ", Y: " << closest_tile.second << endl;

	return closest_tile;
}

// Finds path to desired tile from current position
void MyAI::make_path(pair<int, int> desired_tile) {		//NOTE: it isn't changing desired path??? 
	desired_path.clear();
	for (auto tile = prev_tiles.rbegin(); tile != prev_tiles.rend(); ++tile) {
		if (((*tile).first == desired_tile.first + 1 && (*tile).second == desired_tile.second) || 
			((*tile).first == desired_tile.first - 1 && (*tile).second == desired_tile.second) ||
			((*tile).second == desired_tile.second + 1 && (*tile).first == desired_tile.first) ||
			((*tile).second == desired_tile.second - 1 && (*tile).first == desired_tile.first)) {
				if (DEBUG)
					cout << "tile pushed back: " << (*tile).first << ", " << (*tile).second << endl; 
				desired_path.push_back(*tile);
				break;
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

	desired_path.push_back(desired_tile);
}

// MOVEMENT CODE 
void MyAI::set_direction() {
	pair<int, int> current_tile;
	pair<int, int> tile_to_move_to;
	if (curr_position == desired_path[0]) {
		current_tile = desired_path[0];
		desired_path.erase(desired_path.begin());
		tile_to_move_to = desired_path[0];
	}
	else {
		tile_to_move_to = desired_path[0];
	}
	
	if (DEBUG)
		cout << "TILE TO MOVE TO: " << tile_to_move_to.first << ", " << tile_to_move_to.second << endl;

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
	if (DEBUG) {
		cout << "prev Desired dir: " << desired_dir << endl;
		cout << "prev Current dir: " << curr_dir << endl;
	}
	
	Agent::Action action;
	if (desired_dir == WEST && curr_dir != WEST)
		action = face_west();
	else if (desired_dir == EAST && curr_dir != EAST)
		action = face_east();
	else if (desired_dir == SOUTH && curr_dir != SOUTH)
		action = face_south();
	else if (desired_dir == NORTH && curr_dir != NORTH)
		action = face_north();

	else {	// chayanne: i cant believe-
		if (curr_dir == WEST) {
			curr_position.first = curr_position.first - 1;
		}
		else if (curr_dir == EAST) {
			curr_position.first = curr_position.first + 1;
		}
		else if (curr_dir == SOUTH) {
			curr_position.second = curr_position.second - 1;
		}
		else if (curr_dir == NORTH) {
			curr_position.second = curr_position.second + 1;
		}
		if (DEBUG)
			cout << "AFTER curr pos " << curr_position.first << curr_position.second << endl;
		action =  FORWARD;
	}
	if (DEBUG) {
		cout << "after Desired dir: " << desired_dir << endl;
		cout << "after Current dir: " << curr_dir << endl;

		cout << "PREV_TILES PATH:" << endl;
		for (auto x: prev_tiles) {
			cout << "X: " << x.first << ", Y: " << x.second << endl;
		}

		cout << "DESIRED PATH:" << endl;
		for (auto x: desired_path) {
			cout << "X: " << x.first << ", Y: " << x.second << endl;
		}
	}
	
	return action;
}

Agent::Action MyAI::face_west() {
	switch (curr_dir) {
		case EAST:
			curr_dir = NORTH;
			return TURN_LEFT;
		case SOUTH:
			curr_dir = WEST; 
			return TURN_RIGHT;
		case NORTH:
			curr_dir = WEST;
			return TURN_LEFT;
	}

}

Agent::Action MyAI::face_east() {
	switch (curr_dir) {
		case WEST:
			curr_dir = SOUTH;
			return TURN_LEFT;
		case SOUTH:
			curr_dir = EAST;
			return TURN_LEFT;
		case NORTH:
			curr_dir = EAST;
			return TURN_RIGHT;
	}
}

Agent::Action MyAI::face_south() {
	switch (curr_dir) {
		case WEST:
			curr_dir = SOUTH;
			return TURN_LEFT;
		case EAST:
			curr_dir = SOUTH;
			return TURN_RIGHT;
		case NORTH:
			curr_dir = WEST;
			return TURN_LEFT;
	}
}

Agent::Action MyAI::face_north() {
	switch (curr_dir) {
		case WEST:
			curr_dir = NORTH;
			return TURN_RIGHT;
		case EAST:
			curr_dir = NORTH;
			return TURN_LEFT;
		case SOUTH:
			curr_dir = EAST;
			return TURN_LEFT;
	}
}

void MyAI::print_world() {
	for (int y = MAX_Y - 1; y >= 0 ; --y) {
		for (int x = 0; x < MAX_X; ++x) {
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
			cout << ".\t\t";
		}
		cout << endl;
	}
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================