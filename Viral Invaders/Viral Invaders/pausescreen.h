/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      pausescreen.h
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains declarations for functions used to manage buttons
*				and button clicks happening during pause state.
******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "virus.h"



// Normalize world coordinates
float normalize_coor(float value, float min, float max);
// Check if area is clicked
int is_area_clicked(float area_left_x, float area_bottom_y, float area_width, float area_height, float click_x, float click_y);

// Load data
void load_data_pausescreen();
// Init Pause menu
 void init_pausescreen(int win_width, int win_height);
// Render pause menu
void render_pause_screen(s8 loaded_font, int win_width, int win_height, bool& is_paused);
// Free pause menu objects
void free_pause_mesh();

enum button_position {
	but_0 = 0,
	but_1,
	but_2,
	but_3,
	but_4,
	but_5
};