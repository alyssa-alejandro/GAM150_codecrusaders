/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      platelets.h
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains declarations for functions to create, update, render and free
*				platelets event related items.
******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "virus.h"

struct Platelets {
    AEVec2 xy;
    float width, height; 
    float curr_health;
};

// Load data
void load_data_platelets();

// Initialise platelets mesh and coordinates
void init_platelets();
// Update countdown for platelets
void update_platelets();
// Render platelets
void render_platelets();
// Free platelets meshes
void free_platelets_mesh();
// Check collision between platelets and virus
void collision_platelets_virus(std::vector<virus*> spawned_viruses);
// Assign coordinates for platelets
void generate_platelets(int square_columns, int square_rows, float cell_length);