/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      healthbar.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 50%)
*               Dalton Koh (Secondary Author, 50%)
*@DP emails     alejandro.a@digipen.edu
*               d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the struct Hp_bar and all required function
*               declarations.
******************************************************************************/

#pragma once

#include <AEEngine.h>
#define MAXHPBARS 1500

// Main struct for hp bar
struct HP_Bar {
    AEGfxVertexList* bar_Mesh;  // Red HP bar mesh
    AEGfxVertexList* bg_Mesh;   // Background HP bar mesh
    AEMtx33 transform;          // HP bar transformation matrix
    AEMtx33 bg_Transform;       // Background transformation matrix
    float current_Scale;        // Current HP scale [0.0, 1.0]
    float target_Scale;         // Target HP scale
    float interp_Speed;         // Interpolation speed
    float pos_X, pos_Y;         // HP bar position
};


// Define an array to hold multiple HP bars
extern HP_Bar hp_Bars[MAXHPBARS];

// Load data 
void load_data_hpbar();

// Hp bar functions
void make_hp(size_t index, float x, float y, float scale, bool is_Virus);
void update_hp_V(int index, float hp_percentage, float dt, float x, float y);
void update_hp_T(int index, float hp_percentage, float dt, float x, float y);
void render_hp();
void free_hp_bars();

