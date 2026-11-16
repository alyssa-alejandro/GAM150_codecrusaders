/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      credits.h
*@author(s)     Dalton Koh
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file includes all function declaration to init, logify and
*               render the credits screen! Includes a Exit_button struct for buttons.
******************************************************************************/

#pragma once

// Boolean to toggle the credits screen
extern bool is_Credits_Visible;  

// Functions for Credits Page
void init_credits_page();
void render_credits_page();
void free_credits_page();

// Load json data
void load_data_credits();

// Struct for the Exit Button
struct Press_Button {
    float x, y, width, height;
    AEGfxVertexList* mesh;
    AEMtx33 transform;
    unsigned int color;
};

// Enum to set page number
enum credit_page {
    credit_nil =0,
    credit_1,
    credit_2
};