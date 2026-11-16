/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      paracetemol.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the paracetemol event's function and variables.
*				It includes applying the event and updating its effects.
******************************************************************************/
#pragma once
#include "towers.h"
#include <vector>

extern bool para_active;
extern bool para_boosted;

// Load data to file
void load_data_para();

// Apply the event to the game
void para_apply();

// This will take effect after apply function is completed
void para_update();