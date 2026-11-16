/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      event.h
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the event randomiser to call events at random intervals.
*               Also includes functions to assist with functions of events, such as 
*				the random coordinate generator.
******************************************************************************/
#pragma once
#include "AEEngine.h"
#include <vector>        // std::vector  
#include "virus.h"        // std::vector  
#include "gamestatemanager.h"     
      


enum event_id {
	no_event = 0,
	pipagao = 1,
	vaccine,
	paracetemol,
	platelet
};

extern int triggered_event;
extern event_id event;
extern bool event_happened;


// Load data from json file
void load_data_event();
// Generate seed for random number 
extern void seed_generator();
// Generate random coordinates on grid
extern AEVec2 random_coor_generator(int square_columns, int square_rows, float cell_length);

// Sets event to be triggered
void event_trigger(GameState lvl, std::vector<virus*>spawnedViruses, int square_columns, int square_rows, float cell_length);
// Choose event to trigger
event_id event_randomizer(std::vector<event_id> lvl);