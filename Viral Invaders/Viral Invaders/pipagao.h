/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      pipagao.h
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains declarations for functions to create, update, render and free
*				pipagao event related items.
******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "virus.h"
#include <vector>

struct PiPaGao {
    AEVec2 xy;
    //float radius;
    //unsigned int color;
};


// Load data
void load_data_ppg();

// Initialise pipagao mesh and coordinates
void init_ppg();
// Set pipagao coordinates for first time and check for collision subsequently
void update_ppg(std::vector<virus*> spawned_viruses, int square_columns, int square_rows, float cell_length);
// Render pipagao circles
void render_ppg();
// Assign coordinates to pipgagao
void coor_ppg(int square_columns, int square_rows, float cell_length);
// Check for collision of virus with pipagao
void collision_ppg(std::vector<virus*> spawned_viruses);
// Free meshes for pipagao
void free_ppg_mesh();