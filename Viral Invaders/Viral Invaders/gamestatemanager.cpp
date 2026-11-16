/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      gamestatemanager.cpp
*@author(s)     Dalton Koh
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines how function pointers are manipulated using 
*               the main switch case. The main switch case manipulates an enum
*               to switch out which functions the function pointers point to,
*               achieving a robust seemless level changing game state manager.
******************************************************************************/

#include "GameStateManager.h"
#include "pausescreen.h"
#include "AEEngine.h"
#include <iostream>
#include "levelselector.h"
#include "MainMenu.h"
#include "guidepage.h"
#include "events.h"



// Global GameState variable
GameState current_State = STATE_LEVEL1;
GameState previous_State = STATE_LEVEL1;

// Pointer to current game state functions
void (*GameState_Init)() = nullptr;
void (*GameState_Update)() = nullptr;
void (*GameState_Exit)() = nullptr;

// To start off the first state based on 
void GSM_Init(GameState startState) {
    current_State = startState;
    GSM_ChangeState(current_State);
}

// Main game state function
void GSM_ChangeState(GameState newState) {

    // To check for gamestate_exit to free anything that is not required 
    if (GameState_Exit) {
        GameState_Exit();
    }

    // To set current state to which state depending on whats called for 
    current_State = newState;

    // Store the previous state before changing for restart function
    if (newState != STATE_RESTART) {
        previous_State = current_State;
    }


    // Switch statement to control where function pointers point to which functions
    switch (current_State) {
    case STATE_MAINMENU:
         GameState_Init = MainMenu_Init;
         GameState_Update = MainMenu_Update;
         GameState_Exit = MainMenu_Exit;
         break;

    case STATE_LEVEL1:
        GameState_Init = Level1_Init;
        GameState_Update = Level1_Update;
        GameState_Exit = Level1_Exit;
        break;

    case STATE_LEVEL2:
        GameState_Init = Level2_Init;
        GameState_Update = Level2_Update;
        GameState_Exit = Level2_Exit;
        break;
    case STATE_LEVEL3:
        GameState_Init = Level3_Init;
        GameState_Update = Level3_Update;
        GameState_Exit = Level3_Exit;
        break;
    case STATE_LEVEL4:
        GameState_Init = Level4_Init;
        GameState_Update = Level4_Update;
        GameState_Exit = Level4_Exit;
        break;
    case STATE_LEVEL5:
        GameState_Init = Level5_Init;
        GameState_Update = Level5_Update;
        GameState_Exit = Level5_Exit;
        break;
    case STATE_LEVEL6:
        GameState_Init = Level6_Init;
        GameState_Update = Level6_Update;
        GameState_Exit = Level6_Exit;
        break;
    case STATE_GUIDEPAGE:
        GameState_Init = init_guidepage;
        GameState_Update = update_guidepage;
        GameState_Exit = free_guidepage;
        break;

    // In the case restart is pressed to revert to restart
    case STATE_RESTART:

        // To free 
        if (GameState_Exit) {
            GameState_Exit();  // Free everything from the current level
        }
        // To reinit anything required 
        GSM_ChangeState(previous_State);  // Reloads the current level after exit
        return;

    // Handle unknown states
    default:
        GameState_Init = nullptr;
        GameState_Update = nullptr;
        GameState_Exit = nullptr;
        break;
    }
    // If theres any required init else set state to null
    if (GameState_Init) {
        GameState_Init();
    }
}

