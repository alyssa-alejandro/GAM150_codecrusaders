/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      levelselector.cpp
*@author(s)     Dalton Koh (Primary Author, 75%)
*               Chan Qi Ying (Secondary Author, 10%)
*               Yu xuan Ang (Secondary Author, 5%)
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
*@brief:        This file defines the different functions - init, update, exit;
*               for the different levels in the game. The definitions in each
*               different function allow the levels to have its own specificities 
******************************************************************************/

#include "AEEngine.h"
#include <crtdbg.h> // To check for memory leaks
#include <iterator> // for std::size testing
#include "circle.h"
#include "rectangle.h"
#include "squaregrid.h"
#include "guidepage.h"
#include "pathing.h"
#include "textureselect.h"
#include "collision.h"
#include "healthbar.h"
#include <time.h>
#include <stdlib.h>
#include "polio.h"
#include <iterator>
#include "pipagao.h"
#include "paracetemol.h"
#include "vaccine.h"
#include <ctime>
#include "towers.h"
#include "upgradetree.h"
#include "countdown.h"
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
#include "levelselector.h"
#include "virulenecemeter.h"
#include "bgmusic.h"
#include "game_setting.h"
#include "spritesheets.h"

//dalton- 40% ethan - 20% yuxuan - 10% aly - 15% qy - 15%

//GRID START  
static float cell_length = 100.f; // Length/breadth of the square
int square_rows = 7; //square rows to be rendered
int square_columns = 16; //square columns to be rendered
SG_stats** grid;
//GRID END 

//PATHING START
int* path_Coords1 = nullptr;
int* path_Coords2 = nullptr;
//PATHING END

//Virulence meter
int meter{};

//Boss Timer
float bosstimer{};
float bossinvtimer{};

//Spawnpoint
float spawnpointx = -750.0f;
float spawnpointy = -100.0f;
float spawnpointy2 = 100.0f;

int num_polio{ 0 }, num_minion_polio{ 0 }, preallocated_resource{ 0 };

//Selection 
int selected = 0;
int cost = 0;
int virusToSpawn = chickenpox;
float spawnx = -600.0f;
float spawny = -400.0f;
float endx = 900.f;

//Virus init
virus* spawnedVirus = nullptr;

//Circle Mesh
float circlex = spawnx;
float circley = spawny;
float scalex = 1.0f;
float scaley = 1.0f;
AEGfxVertexList* circlemesh = 0;
AEMtx33 transforms[1] = { 0 };

//upgrade tree test 
bool inUpgradeTree = false;  // Tracks if the upgrade tree is active
bool mainmenu = false;

int x;
int y;
unsigned int pewcolour = 0XFFFFFFFF;
unsigned int pewcolour_2 = 0xFFFF0000;
//call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later

AEGfxVertexList* white_cMesh = 0;
AEGfxVertexList* red_cMesh = 0;

float bgmVolume = 0.1f;
AEAudio bgm, sound;
AEAudioGroup bgmGroup, soundGroup;

int game_condition = none; //3 game conditions: none, lost and win




void Level1_Init() {
    std::cout << "Initializing Level 1..." << std::endl;
    reset_all_towers();

    //countdown_timer init
    countdown_init(&timer, font);

    init_audio(bgm, bgmGroup);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    //init paths 
    path_start(grid, square_rows, square_columns);


    init_ppg();
    init_platelets();

    place_towers(grid, square_rows, square_columns, 1);
    spawnpoint_init();
    level1_selector_init();
    resourceinit();
    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);
    //CreateCircleMesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00, transforms, scalex, scaley, circlex, circley);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
     white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour, pewcolour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
     red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour_2, pewcolour_2);

     virulencemeter_init(&meter);

     //Initialize variables for in-game tooltips
     init_tooltips();

     Game_Setting_Init();
     
     //Starting income
     preallocated_resource = 500;
     

     //Virulence Meter
     meter = 0;

     //Chicken pox
     chickenpoxDefeated = false;

     //Init spritesheet
     ss_init(&ss_data);

     init_game_result();

     //Game win state set to norm
     levelend = norm;
}

void Level1_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgmGroup); //bgm
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);

    // to check if countdown is completed
    // timer runs out means LOST GAME STATE
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
        levelend = fail;
    }

    //Resource controller
    resource_controller(&preallocated_resource);
    switch (is_paused) {
    case false:
        AEMtx33 scale,  rotate,  translate;

        if (levelend == norm) {
            //PPG platelets code
            update_ppg(spawnedViruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawnedViruses);

            if (event_happened) {
                if (vaccine_active == true) {
                    std::cout << "Vaccine updated called" << std::endl;
                    vaccine_update();
                }
                else if (para_active == true) {
                    std::cout << "Para updated called" << std::endl;
                    para_update();
                }
            }
            virus_Select(&selected, virusToSpawn, x, y, spawnx + 200.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawnx, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawnx + 400.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);

            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpointx, spawnpointy, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpointx, spawnpointy2, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            
            updateEnemies(spawnedViruses, dt, endx, spawnpointy, transforms, &scale, &rotate, &translate, scalex, scaley, meter, &preallocated_resource);
        
            Tower_Shooter_Logic(LVL1TOWER, towered, spawnedViruses.size(), spawnedViruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawnedViruses.size(), spawnedViruses, LVL1TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulencemeter_render(&meter);

        //Spawnpoint
        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        level1_selector_render();

        //Virus Selectors
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        virus_price(&cost, selected);
        
        tooltips_update();

        // Render HP bars
        render_hp(); 

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL1TOWER, towered, /*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL1TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);
        if (levelend == norm || event_happened == false) {
            event_trigger(STATE_LEVEL1, spawnedViruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if ( levelend == vic || AEInputCheckTriggered(AEVK_N)) {
            levelend = vic;     
            std::cout << "Level 1 Complete! Moving to Level 2..." << std::endl;
            upgradepoints += 5;  // Award upgrade points
            std::cout << upgradepoints << "\n";
            save_Upgrade_Data();    //Save progress immediately
            game_condition = level_completed;
        
        }
        
        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        if (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE)) {
            is_paused = true;
        }
        break;

        case true:
            
        render_ppg();
        render_platelets();

        render_tower();
        
        // Virulence meter 
        virulencemeter_render(&meter);
        spawnpoint_render();
        level1_selector_render();

        // Selection Square for shingles
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL1TOWER, towered, /*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL1TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;

    }
}

void Level1_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawnedViruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    FreeBullet();
    vFreeBullet();
    cleanupvirus(spawnedViruses);

    free_platelets_mesh();
    free_ppg_mesh();
    freeresource();
    tooltips_free();

    
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL;
    }

    free_spawner();
    freevirulencemeter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    if (circlemesh) {
        AEGfxMeshFree(circlemesh);
        circlemesh = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords1);
        path_Coords1 = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords2);
        path_Coords2 = NULL;
    }
    free_audio(bgm, bgmGroup);
    chickenpoxDefeated = false;
}

void Level2_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    //init paths 
    path_start(grid, square_rows, square_columns);
    init_audio(bgm, bgmGroup);


    init_ppg();
    init_platelets();
    resourceinit();
    place_towers(grid, square_rows, square_columns, 2);

    spawnpoint_init();
    level1_selector_init();

    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour, pewcolour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour_2, pewcolour_2);
    virulencemeter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    levelend = norm;

    event_happened = false;
   }

void Level2_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgmGroup); //bgm
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);


    //Resource controller
    resource_controller(&preallocated_resource);
    // to check if countdown is completed
    // timer runs out means LOST GAME STATE
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
        levelend = fail;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;
        
        if (levelend == norm ) {
            //PPG platelets code
            update_ppg(spawnedViruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawnedViruses);

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            virus_Select(&selected, virusToSpawn, x, y, spawnx + 200.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawnx, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawnx + 400.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            // virus_Select(&selected, ebola, x, y, spawnx + 600.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpointx, spawnpointy, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpointx, spawnpointy2, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            updateEnemies(spawnedViruses, dt, endx, spawnpointy, transforms, &scale, &rotate, &translate, scalex, scaley, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL2TOWER, towered, spawnedViruses.size(), spawnedViruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawnedViruses.size(), spawnedViruses, LVL2TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulencemeter_render(&meter);

        //Spawnpoint
        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);
        level1_selector_render();

        //Virus Selectors
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);
        
        virus_price(&cost, selected);
        
        render_hp();

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL2TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL2TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        if (levelend == norm || event_happened == false) {
            event_trigger(STATE_LEVEL2, spawnedViruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if (levelend == vic || AEInputCheckTriggered(AEVK_N)) {
            levelend = vic;
            std::cout << "Level 2 Complete! Moving to Level 3..." << std::endl;
            upgradepoints += 6;  // Award upgrade points
            save_Upgrade_Data();    // Save progress immediately
            std::cout << "Level completed! Upgrade points: " << upgradepoints << std::endl;
            game_condition = level_completed;
        }

       

        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        if (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE)) {
            is_paused = true;
        }
        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulencemeter_render(&meter);

        spawnpoint_render();
        level1_selector_render();

        // Selection Square for shingles
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL2TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL2TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;

    }
}

void Level2_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawnedViruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    FreeBullet();
    vFreeBullet();
    cleanupvirus(spawnedViruses);

    free_platelets_mesh();
    free_ppg_mesh();
    freeresource();
    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    freevirulencemeter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    if (circlemesh) {
        AEGfxMeshFree(circlemesh);
        circlemesh = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords1);
        path_Coords1 = NULL;
    }
    if (path_Coords2) {
        Free_Path_Coords(path_Coords2);
        path_Coords2 = NULL;
    }
    free_audio(bgm, bgmGroup);
    chickenpoxDefeated = false;
   }

void Level3_Init() {

    //countdown_timer init
    countdown_init(&timer, font);
    init_audio(bgm, bgmGroup);
    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);


    //init paths 
    path_start(grid, square_rows, square_columns);

    init_ppg();
    init_platelets();
    resourceinit();
    place_towers(grid, square_rows, square_columns, 3);
    spawnpoint_lvl3_init();
    level3_selector_init();
    
    searchintersecting();
    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour, pewcolour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour_2, pewcolour_2);
    virulencemeter_init(&meter);
    
    //Starting income
    preallocated_resource = 500;

    //boss invulnerability timer
    bossinvtimer = 0.0f;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    levelend = norm;
}
void Level3_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgmGroup);
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);
    //Resource controller
    resource_controller(&preallocated_resource);
    // to check if countdown is completed
    // timer runs out means LOST GAME STATE
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
        levelend = fail;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;

        if (levelend == norm) {
            //PPG platelets code
            update_ppg(spawnedViruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawnedViruses);

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            virus_Select(&selected, virusToSpawn, x, y, spawnx + 200.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawnx, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawnx + 400.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            // virus_Select(&selected, ebola, x, y, spawnx + 600.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);*/

            //Selection for polio
            virus_Select(&selected, polio_virus, x, y, spawnx + 800.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);

            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpointx, -200.f, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpointx, 200.f, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            updateEnemies(spawnedViruses, dt, endx, spawnpointy, transforms, &scale, &rotate, &translate, scalex, scaley, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL3TOWER, towered, spawnedViruses.size(), spawnedViruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawnedViruses.size(), spawnedViruses, LVL3TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulencemeter_render(&meter);

        //Spawnpoint

        spawnpoint_lvl3_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selectors
        level3_selector_render();
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);
        
        virus_price(&cost, selected);
        
        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL3TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL3TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        boss1_tower(&bosstimer);
        bossinvuln(&bossinvtimer);


        if (levelend == norm || event_happened == false) {
            event_trigger(STATE_LEVEL3, spawnedViruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if (levelend == vic || AEInputCheckTriggered(AEVK_N)) {
            levelend = vic;
            std::cout << "Level 3 Complete! Moving to Level 4..." << std::endl;
            upgradepoints += 6;  // Award upgrade points
            save_Upgrade_Data();    // Save progress immediately
            game_condition = level_completed;
        }


        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }


        // Check for pause
        if (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE)) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulencemeter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        level3_selector_render();
        // Selection Square for shingles
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL3TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL3TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}


void Level3_Exit() {
    std::cout << "Exiting Level 3..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawnedViruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    FreeBullet();
    vFreeBullet();
    cleanupvirus(spawnedViruses);

    free_platelets_mesh();
    free_ppg_mesh();
    freeresource();
    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    freevirulencemeter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    if (circlemesh) {
        AEGfxMeshFree(circlemesh);
        circlemesh = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords1);
        path_Coords1 = NULL;
    }
    if (path_Coords2) {
        Free_Path_Coords(path_Coords2);
        path_Coords2 = NULL;
    }
    free_audio(bgm, bgmGroup);
    chickenpoxDefeated = false;
}

void Level4_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);
    
    init_audio(bgm, bgmGroup);

    //init paths 
    path_start(grid, square_rows, square_columns);


    init_ppg();
    init_platelets();
    resourceinit();
    place_towers(grid, square_rows, square_columns, 4);
    spawnpoint_init();
    level3_selector_init();
    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour, pewcolour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour_2, pewcolour_2);
    virulencemeter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    levelend = norm;
}


void Level4_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgmGroup /*sound, soundGroup, bgmVolume*/);
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);
    //Resource controller
    resource_controller(&preallocated_resource);
    // to check if countdown is completed
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;
        
        if (levelend == norm) {
            //PPG platelets code
            update_ppg(spawnedViruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawnedViruses);
            

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            // Select virus
            virus_Select(&selected, virusToSpawn, x, y, spawnx + 200.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawnx, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawnx + 400.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            // virus_Select(&selected, ebola, x, y, spawnx + 600.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);*/
            virus_Select(&selected, polio_virus, x, y, spawnx + 800.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            
            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpointx, spawnpointy, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpointx, spawnpointy2, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            updateEnemies(spawnedViruses, dt, endx, spawnpointy, transforms, &scale, &rotate, &translate, scalex, scaley, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL4TOWER, towered, spawnedViruses.size(), spawnedViruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawnedViruses.size(), spawnedViruses, LVL4TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulencemeter_render(&meter);

        //Spawnpoint
        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selector Rendering
        level3_selector_render();
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        virus_price(&cost, selected);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL4TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL4TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        if (levelend == norm || event_happened == false) {
            event_trigger(STATE_LEVEL4, spawnedViruses, square_columns, square_rows, cell_length);           // check for event trigger
        }


        if (levelend == vic || AEInputCheckTriggered(AEVK_N)) {
            std::cout << "Level 4 Complete! Moving to Level 5..." << std::endl;
            levelend = vic;
            upgradepoints += 6;  // Award upgrade points
            save_Upgrade_Data();    // Save progress immediately
            game_condition = level_completed;
        }


        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        // Check for pause
        if (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE)) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulencemeter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        // Selection Squares
        level3_selector_render();
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL4TOWER, towered, /*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL4TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}

void Level4_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawnedViruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    FreeBullet();
    vFreeBullet();
    cleanupvirus(spawnedViruses);
    freeresource();
    free_platelets_mesh();
    free_ppg_mesh();

    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    freevirulencemeter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    if (circlemesh) {
        AEGfxMeshFree(circlemesh);
        circlemesh = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords1);
        path_Coords1 = NULL;
    }
    if (path_Coords2) {
        Free_Path_Coords(path_Coords2);
        path_Coords2 = NULL;
    }
    free_audio(bgm, /*sound*/ bgmGroup);
    chickenpoxDefeated = false;
}


void Level5_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    init_audio(bgm, bgmGroup);

    //init paths 
    path_start(grid, square_rows, square_columns);


    init_ppg();
    init_platelets();
    resourceinit();
    place_towers(grid, square_rows, square_columns, 5);

    spawnpoint_init();

    level5_selector_init();

    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour, pewcolour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour_2, pewcolour_2);
    virulencemeter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    levelend = norm;
}

void Level5_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgmGroup);
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);
    //Resource controller
    resource_controller(&preallocated_resource);
    // to check if countdown is completed
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;

        if (levelend == norm) {
            //PPG platelets code
            update_ppg(spawnedViruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawnedViruses);


            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            //Virus Selectors
            virus_Select(&selected, virusToSpawn, x, y, spawnx + 200.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawnx, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawnx + 400.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, ebola, x, y, spawnx + 600.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);

            //Selection for polio
            virus_Select(&selected, polio_virus, x, y, spawnx + 800.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            

            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpointx, spawnpointy, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpointx, spawnpointy2, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            updateEnemies(spawnedViruses, dt, endx, spawnpointy, transforms, &scale, &rotate, &translate, scalex, scaley, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL5TOWER, towered, spawnedViruses.size(), spawnedViruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawnedViruses.size(), spawnedViruses, LVL5TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();

        virulencemeter_render(&meter);

        //Spawnpoint

        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selector Rendering
        level5_selector_render();
        virus_price(&cost, selected);
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL5TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL5TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        if (levelend == norm || event_happened == false) {
            event_trigger(STATE_LEVEL5, spawnedViruses, square_columns, square_rows, cell_length);           // check for event trigger
        }


        if (levelend == vic || AEInputCheckTriggered(AEVK_N)) {
            levelend = vic;
            std::cout << "Level 5 Complete! Moving to Level 6..." << std::endl;
            upgradepoints += 7;  // Award upgrade points
            save_Upgrade_Data();    // Save progress immediately
            game_condition = level_completed;
        }

   

        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        if (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE)) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulencemeter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        // Selection Squares
        level5_selector_render();
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL5TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL5TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}

void Level5_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawnedViruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    FreeBullet();
    vFreeBullet();
    cleanupvirus(spawnedViruses);
    freeresource();
    free_platelets_mesh();
    free_ppg_mesh();

    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    freevirulencemeter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    if (circlemesh) {
        AEGfxMeshFree(circlemesh);
        circlemesh = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords1);
        path_Coords1 = NULL;
    }
    if (path_Coords2) {
        Free_Path_Coords(path_Coords2);
        path_Coords2 = NULL;
    }
    free_audio(bgm, bgmGroup);
    chickenpoxDefeated = false;
}


void Level6_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    init_audio(bgm, bgmGroup);

    //init paths 
    path_start(grid, square_rows, square_columns);


    init_ppg();
    init_platelets();
    resourceinit();
    place_towers(grid, square_rows, square_columns, 6);

    spawnpoint_init();

    level5_selector_init();

    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour, pewcolour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pewcolour_2, pewcolour_2);
    virulencemeter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Game win state set to norm
    levelend = norm;

    //Init spritesheet
    ss_init(&ss_data);
    init_game_result();
    searchintersecting();
    //Boss timer
    bosstimer = 0.0f;

    //boss invulnerability timer
    bossinvtimer = 0.0f;
}

void Level6_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgmGroup);
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);
    //Resource controller
    resource_controller(&preallocated_resource);
    // to check if countdown is completed
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;

        if (levelend == norm) {
            //PPG platelets 
            update_ppg(spawnedViruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawnedViruses);

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            virus_Select(&selected, virusToSpawn, x, y, spawnx + 200.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawnx, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawnx + 400.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, ebola, x, y, spawnx + 600.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);
            virus_Select(&selected, polio_virus, x, y, spawnx + 800.0f, spawny, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawnedViruses, &preallocated_resource, &cost);


            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpointx, spawnpointy, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpointx, spawnpointy2, 200.0f, 200.0f, spawnedViruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            updateEnemies(spawnedViruses, dt, endx, spawnpointy, transforms, &scale, &rotate, &translate, scalex, scaley, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL6TOWER, towered, spawnedViruses.size(), spawnedViruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawnedViruses.size(), spawnedViruses, LVL6TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulencemeter_render(&meter);

        //Spawnpoint

        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selector Rendering
        level5_selector_render();
        virus_price(&cost, selected);
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);
        
        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL6TOWER, towered,/*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL6TOWER, towered, spawnedViruses.size(), /*dt,*/ 0XFFFFFFFF);

        bossinvuln(&bossinvtimer);
        boss2_tower(&bosstimer);
        // Check if all enemies are defeated or another condition

        if (levelend == norm || event_happened == false) {
            event_trigger(STATE_LEVEL6, spawnedViruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if (levelend == vic || AEInputCheckTriggered(AEVK_N)) {
            std::cout << "Level 6 Complete! YOUR A CHAMP!..." << std::endl;
            levelend = vic;
            upgradepoints += 7;  // Award upgrade points
            save_Upgrade_Data();    // Save progress immediately
            game_condition = level_completed;
        }


        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        // Check for pause
        if (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE)) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulencemeter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        // Selection Squares
        level5_selector_render();
        render_shingle_select(chickenpoxDefeated, &selected, x, y, spawnedViruses, &preallocated_resource, &cost);

        // Virus
        renderEnemies(spawnedViruses, transforms, &scale, &rotate, &translate, scalex, scaley);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        renderVirusBullet(spawnedViruses.size(), spawnedViruses, LVL6TOWER, towered, /*dt,*/ 0xFFFF0000);
        renderTowerBullet(LVL6TOWER, towered, spawnedViruses.size(),/*dt,*/ 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}

void Level6_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawnedViruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    FreeBullet();
    vFreeBullet();
    cleanupvirus(spawnedViruses);
    freeresource();
    free_platelets_mesh();
    free_ppg_mesh();

    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    freevirulencemeter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    if (circlemesh) {
        AEGfxMeshFree(circlemesh);
        circlemesh = NULL;
    }
    if (path_Coords1) {
        Free_Path_Coords(path_Coords1);
        path_Coords1 = NULL;
    }
    if (path_Coords2) {
        Free_Path_Coords(path_Coords2);
        path_Coords2 = NULL;
    }
    free_audio(bgm, bgmGroup);
    chickenpoxDefeated = false;
}

void freemeshes() {
    free_bullets(&white_cMesh, &red_cMesh);
    if (circlemesh) AEGfxMeshFree(circlemesh);
}