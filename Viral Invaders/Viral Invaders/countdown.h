/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      countdown.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the countdown timer's functions and variables 
*               for integration into the game. It includes the initializing, 
*               updating, and rendering the timer.
******************************************************************************/
#pragma once
#include "AEEngine.h"

typedef struct
{
    double time_remaining;   // Time remaining in seconds
    s8 font_handle;          // Text rendering
} countdown_timer;


extern countdown_timer timer;
extern bool is_paused;
extern double original_time;

// Load data to file
void load_data_countdown();

// Set values for countdown
void countdown_init(countdown_timer* timer, s8 font_handle);

// Where the countdown will decrement each frame
void countdown_update(countdown_timer* timer);

// Display countdown timer
void countdown_render(countdown_timer* timer);


