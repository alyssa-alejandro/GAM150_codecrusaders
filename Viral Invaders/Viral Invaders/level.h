/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      level.h
*@author(s)     Dalton Koh (Primary Author, 75%)
*               Chan Qi Ying (Secondary Author, 10%)
*               Ang Yu Xuan (Secondary Author, 5%)
*               Alyssa Nicole Alejandro (Secondary Author, 5%)
*               Ethan Ng (Secondary Author, 5%)
*@DP emails     d.koh@digipen.edu
*               q.chan@digipen.edu
*               alejandro.a@digipen.edu
*               yuxuan.ang@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains required externed variables and an enum for 
*               fail_vic.
******************************************************************************/

#pragma once

#include <vector>
#include "virus.h"


// enum to handle fail or victory
enum fail_vic {
    norm,
    fail,
    vic
};

// Externs
extern int game_condition;
extern fail_vic level_end;
extern int gGameRunning;


// Game run state
enum game_run {
    Game_end = 0,
    Game_run = 1
};
