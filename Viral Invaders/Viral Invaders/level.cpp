/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      level.cpp
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
*@brief:        This file defines the main game loop and game backbone.
******************************************************************************/


#include "AEEngine.h"
#include <iterator> 
#include "circle.h"
#include "rectangle.h"
#include "squaregrid.h"
#include "pathing.h"
#include "textureselect.h"
#include "collision.h"
#include "healthbar.h"
#include <time.h>
#include "pipagao.h"
#include "paracetemol.h"
#include "guidepage.h"
#include "vaccine.h"
#include <ctime>
#include "towers.h"
#include "upgradetree.h"
#include "countdown.h"
#include "polio.h"
#include "events.h"
#include "platelets.h"
#include "pausescreen.h"
#include "AEFrameRateController.h"
#include "MainMenu.h"
#include "mesh.h"
#include "Shooting_System.h"
#include "virus.h"
#include <iostream>
#include "spawncontroller.h"
#include "level.h"
#include <vector>
#include <sstream>
#include "gamestatemanager.h"
#include "setting.h"
#include "levelselector.h"
#include "virulencemeter.h"
#include "bgmusic.h"
#include "credits.h"
#include "confirmquit.h"
#include <fstream>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define NO_CONSOLE 0
#define FRAME_RATE_MIX 60
fail_vic level_end = norm;
int gGameRunning = Game_run;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize the game window
    AESysInit(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT, NO_CONSOLE, FRAME_RATE_MIX, true, NULL);


    // Changing the window title
    AESysSetWindowTitle("Viral Invaders");

    // Load all texture data
    load_data();

    // Initialize the Game State Manager with LEVEL1
    GSM_Init(STATE_MAINMENU);



    // Game loop
    while (gGameRunning) {

        // Preset background settings
        settings(); 

        // Main update function
        GameState_Update(); 

        AESysFrameEnd();
        
        if (!AESysDoesWindowExist())
            
        gGameRunning = Game_end;
    }
    GameState_Exit();

        //Free entities and textures
        free_pause_mesh();
        Free_Bullet();
        vFree_Bullet();
        cleanup_virus(spawned_Viruses);
        Free_Grid(grid, square_rows, square_columns);
        free_textures();
        free_hp_bars();
        free_platelets_mesh();
        free_ppg_mesh();
        free_virulence_meter();
        free_tower(); 
        free_spawner();
        free_selectors();
        free_upgrade_tree();
        free_guidepage();
        Free_Path_Coords(path_Coords1);
        Free_Path_Coords(path_Coords2);
        free_Main_menu();
        free_resource();
        free_game_result();
        free_confirm_page();

        // End frame
        AEFrameRateControllerEnd();
        AESysExit();
        return NULL;
    
}
