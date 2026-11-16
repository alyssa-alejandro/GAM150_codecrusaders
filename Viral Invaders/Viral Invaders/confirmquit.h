/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      confirmquit.h
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares functions related to confirmation page pop up
*				when users are about to do a action that makes them lose progress.
******************************************************************************/
#pragma once

#include "AEEngine.h"

enum action {
	quit,
	restart
};


enum button_reply {
	no_reply = -1,
	cancel_reply,
	ok_reply
};

extern bool confirm_restart_page;
extern bool confirm_exit_page;
extern bool confirm_mm_page;

// Load data
void load_data_confirm_quit();
// Initializations for confirmation page
void init_confirm_page();
// Return value base on action made
int update_confirm_page();
// Render confirm page
void render_confirm_page(s8 loaded_font, action action);
// Free memory assigned to confirm page objects
void free_confirm_page();
