/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      gamestatemanager.h
*@author(s)     Dalton Koh
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the enum to be used for function pointer 
*               manipulation to provide transitions in levels/states. file 
*               also include required delcarations for gamestatemanager.cpp.
******************************************************************************/

#pragma once

// Enum to handle level states
enum GameState {
    STATE_LEVEL1,
    STATE_LEVEL2,
    STATE_LEVEL3,
    STATE_LEVEL4,
    STATE_LEVEL5,
    STATE_LEVEL6,
    STATE_GUIDEPAGE,
    STATE_MAINMENU, // tbd
    STATE_GAME_OVER, // used for events
    STATE_RESTART
};

// Global GameState variable
extern GameState current_State;
extern GameState previous_State;

// Function pointers for state handling
extern void (*GameState_Init)();
extern void (*GameState_Update)();
extern void (*GameState_Exit)();

// Game State Manager functions
void GSM_Init(GameState startState);
void GSM_ChangeState(GameState newState);

