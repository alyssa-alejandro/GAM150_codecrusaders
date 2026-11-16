/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      vaccine.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the vaccine event's functions and variables. 
*				It includes apply and update functions.
******************************************************************************/
#pragma once
#include "towers.h"
#include <vector>

extern bool vaccine_active;
extern bool vaccine_boosted;

// Load data to file
void load_data_vaccine();

// When the event is called, this event will be applied
void vaccine_apply();

// This will take effect after apply function is completed
void vaccine_update();

