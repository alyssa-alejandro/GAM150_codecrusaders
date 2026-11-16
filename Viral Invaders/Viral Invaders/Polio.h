/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      Polio.h
*@author        Ang Yu Xuan (Primary Author, 100%)
*@DP emails     yuxuan.ang@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*@brief:        This file is used to declare variables and functions that will be defined
*               by polio.cpp later on.
******************************************************************************/

#pragma once
#include <vector>
#include "AEEngine.h"
#include "virus.h"

extern int gameplay_resources;

void polio_load_json_data();

void Polio_Behaviour(virus* polio_behaviour, f64 dt, int* num_resource);

void Minion_Polio_Behaviour(virus* minion_polio_behaviour, f64 dt, int* num_resource);

void resourceinit();

void display_playGAME_resource(s8 loaded_font, int num_resources);

void free_resource();