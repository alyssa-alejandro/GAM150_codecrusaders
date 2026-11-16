#pragma once
/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      guidepage.cpp
*@author(s)     Ethan Ng (Primary Author, 100%)
*@DP emails     n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declarations the rendering and functionalities for the guidepage
******************************************************************************/
extern bool pause_guide_page;

enum page_numbers {
	page_0 = 0,
	page_1,
	page_2,
	page_3,
	page_4

};
void set_guidepage();
void init_guidepage();
void update_guidepage();
void free_guidepage();
void load_data_guide();