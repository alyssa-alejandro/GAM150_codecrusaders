/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      upgradetree.h
*@author(s)     Dalton Koh (Primary Author, 100%)
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file includes declarations for the init, update, render
*               logic and free functions for upgradetrees. The file also includes
*               the defintion for struct UpgradeButton.
******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "collision.h"
#include "towers.h"
#include <stdio.h>
#include "squaregrid.h"


// Button struct for upgrade tree
struct UpgradeButton {
    float x, y, width, height;
    AEGfxVertexList* mesh;
    AEMtx33 transform;
    std::string name;
    bool is_Clicked;
    unsigned int color;
};

// Struct for button numbers
enum button_case {
    button_0 = 0,
    button_1,
    button_2,
    button_3,
    button_4,
    button_5,
    button_6,
    button_7,
    button_8,
    button_9,
    button_10,
    button_11,
    button_12,
    button_13,
    button_14,
    button_15
};

// Load Json file 
void load_data_upgradetree_constants();

// Init functions
void init_upgrade_tree();

// Update functions
void handle_Button_Click();
bool is_Button_Clicked(UpgradeButton& btn, float mouseX, float mouseY);
void render_upgrade_tree();
void update_upgrade_tree();
void render_text();
void set_upgrade_tree();

// Free
void free_upgrade_tree();

// Load upgrade data file - special 
void save_Upgrade_Data();
void load_Upgrade_Data();