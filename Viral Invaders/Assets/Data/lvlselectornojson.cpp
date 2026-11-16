/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      levelselector.cpp
*@author(s)     Dalton Koh (Primary Author, 40%)
*               Chan Qi Ying (Secondary Author, 15%)
*               Ang Yu Xuan (Secondary Author, 10%)
*               Alyssa Nicole Alejandro (Secondary Author, 15%)
*               Ethan Ng (Secondary Author, 20%)
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
#include "document.h"
#include "istreamwrapper.h"
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
#include "virulencemeter.h"
#include "bgmusic.h"
#include "game_setting.h"
#include "spritesheets.h"



//GRID START  
static float cell_length = 100.f; // Length/breadth of the square
int square_rows = 7; //square rows to be rendered
int square_columns = 16; //square columns to be rendered
SG_stats** grid;
//GRID END 

//PATHING START
int* path_Coords1{};
int* path_Coords2{};
//PATHING END

//Virulence meter
static int meter{};

//Boss Timer
static float boss_timer{};
static float boss_inv_timer{};

//Spawnpoint
float spawnpoint_x{};
float spawnpoint_y{};
float spawnpoint_y2{};

//static int num_polio{ 0 }, num_minion_polio{ 0 };
static int preallocated_resource{};

//Selection 
static int selected{};
static int cost{};
static int virus_to_spawn{};//4
static float spawn_x{};
static float spawn_y{};
static float end_x{};

//Virus init
virus* spawnedVirus{};

//Circle Mesh
static float circle_x{};
static float circle_y{};
static float scale_x{};
static float scale_y{};
AEGfxVertexList* circlemesh{};
AEMtx33 transforms[1]{};

//upgrade tree test 
bool in_upgrade_tree;  // Tracks if the upgrade tree is active
bool main_menu;

static int x;
static int y;
static unsigned int pew_colour{};
static unsigned int pew_colour2{};
//call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later

AEGfxVertexList* white_cMesh{};
AEGfxVertexList* red_cMesh{};

static float bgm_volume{};
AEAudio bgm, sound;
AEAudioGroup bgm_Group;

int game_condition = none; //3 game conditions: none, lost and win

// Loading all required data from credits.json
void load_data_lvl_selector() {

    // File stream
    std::fstream data("Assets/Data/lvl_selector.json");

    // If files fails to open
    if (!data) {
        std::cerr << "Could not open Assets/Data/lvl_selector.json" << std::endl;
        gGameRunning = 0;
    }

    rapidjson::IStreamWrapper isw(data); // wrap file stream into something rapid json can read from 
    rapidjson::Document doc; // Create json object
    doc.ParseStream(isw); // Parses contents from the isw stream into doc. so you can access it later on

    // If parse error or error getting the right data type
    if (doc.HasParseError()) {
        std::cerr << "Error parsing JSON file!" << std::endl;
        gGameRunning = 0;
        return; 
    }

    // Required data to be retrieved 
    cell_length = doc["cell_length"].GetFloat();
    square_rows = doc["square_rows"].GetInt();
    square_columns = doc["square_columns"].GetInt();
    spawnpoint_x = doc["spawnpoint_x"].GetFloat();
    spawnpoint_y = doc["spawnpoint_y"].GetFloat();
    spawnpoint_y2 = doc["spawnpoint_y2"].GetFloat();
    virus_to_spawn = doc["virus_to_spawn"].GetInt();
    spawn_x = doc["spawn_x"].GetFloat();
    spawn_y = doc["spawn_y"].GetFloat();
    virus_to_spawn = doc["virus_to_spawn"].GetInt();
    end_x = doc["end_x"].GetFloat();
    circle_x = doc["circle_x"].GetFloat();
    circle_y = doc["circle_y"].GetFloat();
    scale_x = doc["scale_x"].GetFloat();
    scale_y = doc["scale_y"].GetFloat();
    in_upgrade_tree = doc["in_upgrade_tree"].GetBool();
    main_menu = doc["main_menu"].GetBool();
    pew_colour = static_cast<unsigned int>(doc["pew_colour"].GetUint64());
    pew_colour2 = static_cast<unsigned int>(doc["pew_colour2"].GetUint64());
    bgm_volume = doc["bgm_volume"].GetFloat();
}


void Level1_Init() {
    std::cout << "Initializing Level 1..." << std::endl;
    
    // Reset tower information 
    reset_all_towers();

    // Countdown_timer init
    countdown_init(&timer, font);

    // Init audio 
    init_audio(bgm, bgm_Group);

    // Init grid
    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    // Init paths 
    path_start(grid, square_rows, square_columns);

    // Init ppg
    init_ppg();

    // Init platelets
    init_platelets();

    // Init towers
    place_towers(grid, square_rows, square_columns, 1);

    // Spawnpoint init
    spawnpoint_init();

    // Virus selector init
    level1_selector_init();

    // Viral resource init
    resourceinit();

    // ??? ??? ??? idk
    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);
    

    // Call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
     white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour, pew_colour);

    // Call function DrawCircleMesh, pointer red cMesh is used for rendering later
     red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour2, pew_colour2);

     // Virulence meter init
     virulence_meter_init(&meter);

     // Initialize variables for in-game tooltips
     init_tooltips();

     // Game_settings init
     Game_Setting_Init();
     
     //Starting income
     preallocated_resource = 500;

     //Virulence Meter
     meter = 0;

     //Chicken pox
     chickenpox_Defeated = false;

     //Init spritesheet
     ss_init(&ss_data);

     init_game_result();

     //Game win state set to norm
     level_end = norm;
}

void Level1_Update() {

    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgm_Group); //bgm

    // DT for level 1
    double dt = AEFrameRateControllerGetFrameTime();

    //Countdown Timer
    countdown_update(&timer);
    countdown_render(&timer);

    // to check if countdown is completed
    // timer runs out means LOST GAME STATE
    if (timer.time_remaining <= 0.0) {
        game_condition = game_lost;
        level_end = fail;
    }

    //Resource controller
    resource_controller(&preallocated_resource);
    switch (is_paused) {
    case false:
        AEMtx33 scale,  rotate,  translate;

        if (level_end == norm) {
            //PPG platelets code
            update_ppg(spawned_Viruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawned_Viruses);

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
            virus_Select(&selected, virus_to_spawn, x, y, spawn_x + 200.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawn_x, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawn_x + 400.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);

            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y2, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            
            update_Enemies(spawned_Viruses, dt, end_x, spawnpoint_y, transforms, &scale, &rotate, &translate, scale_x, scale_y, meter, &preallocated_resource);
        
            Tower_Shooter_Logic(LVL1TOWER, towered, spawned_Viruses.size(), spawned_Viruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawned_Viruses.size(), spawned_Viruses, LVL1TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulence_meter_render(&meter);

        // Spawnpoint
        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        level1_selector_render();

        //Virus Selectors
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        virus_price(&cost, selected);
        
        tooltips_update();

        // Render HP bars
        render_hp(); 

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL1TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL1TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // If game is running and no event running -> event trigger for level 1
        if (level_end == norm || event_happened == false) {
            event_trigger(STATE_LEVEL1, spawned_Viruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        // If game is victory or key press N -> level is completed
        if ( level_end == vic || AEInputCheckTriggered(AEVK_N)) {
            level_end = vic;
            game_condition = level_completed;
        }
        
        // If game lost show level_lose screen, if won show level_completed screen
        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        // Handle pause game pop-up
        if (level_end == norm && (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE))) {
            is_paused = true;
        }
        break;

        case true:
            
        render_ppg();
        render_platelets();

        // Tower
        render_tower();
        
        // Virulence meter 
        virulence_meter_render(&meter);
        spawnpoint_render();
        level1_selector_render();

        // Selection Square for shingles
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL1TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL1TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;

    }
}

// level 1 free and exit functions
void Level1_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawned_Viruses, square_columns, square_rows, cell_length);
    vaccine_active = false;
    chickenpox_Defeated = false;

    ss_free(&ss_data);
    Free_Bullet();
    vFree_Bullet();
    cleanup_virus(spawned_Viruses);
    free_platelets_mesh();
    free_ppg_mesh();
    free_resource();
    tooltips_free();
    free_spawner();
    free_virulence_meter();
    free_selectors();
    free_bullets(&white_cMesh, &red_cMesh);
    free_hp_bars();
    free_tower();
    free_game_result();
    free_audio(bgm, bgm_Group);

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL;
    }
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
}

void Level2_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    //init paths 
    path_start(grid, square_rows, square_columns);
    init_audio(bgm, bgm_Group);


    init_ppg();
    init_platelets();
    resourceinit();
    place_towers(grid, square_rows, square_columns, 2);

    spawnpoint_init();
    level1_selector_init();

    create_circle_mesh(&circlemesh, 50, 36, 0xFFFF0000, 0xFF00FF00);

    //call function DrawCircleMesh, pointer WHITE cMesh is used for rendering later
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour, pew_colour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour2, pew_colour2);
    virulence_meter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    level_end = norm;

    event_happened = false;
   }

void Level2_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgm_Group); //bgm
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
        level_end = fail;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;
        
        if (level_end == norm ) {
            //PPG platelets code
            update_ppg(spawned_Viruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawned_Viruses);

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            virus_Select(&selected, virus_to_spawn, x, y, spawn_x + 200.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawn_x, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawn_x + 400.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y2, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            update_Enemies(spawned_Viruses, dt, end_x, spawnpoint_y, transforms, &scale, &rotate, &translate, scale_x, scale_y, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL2TOWER, towered, spawned_Viruses.size(), spawned_Viruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawned_Viruses.size(), spawned_Viruses, LVL2TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulence_meter_render(&meter);

        //Spawnpoint
        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);
        level1_selector_render();

        //Virus Selectors
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);
        
        virus_price(&cost, selected);
        
        render_hp();

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL2TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL2TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        if (level_end == norm || event_happened == false) {
            event_trigger(STATE_LEVEL2, spawned_Viruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if (level_end == vic || AEInputCheckTriggered(AEVK_N)) {
            level_end = vic;
            game_condition = level_completed;
        }

       

        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        if (level_end == norm && (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE))) {
            is_paused = true;
        }
        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulence_meter_render(&meter);

        spawnpoint_render();
        level1_selector_render();

        // Selection Square for shingles
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL2TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL2TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;

    }
}

void Level2_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawned_Viruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    Free_Bullet();
    vFree_Bullet();
    cleanup_virus(spawned_Viruses);

    free_platelets_mesh();
    free_ppg_mesh();
    free_resource();
    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    free_virulence_meter();
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
    free_audio(bgm, bgm_Group);
    chickenpox_Defeated = false;
   }

void Level3_Init() {

    //countdown_timer init
    countdown_init(&timer, font);
    init_audio(bgm, bgm_Group);
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
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour, pew_colour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour2, pew_colour2);
    virulence_meter_init(&meter);
    
    //Starting income
    preallocated_resource = 500;

    //boss invulnerability timer
    boss_inv_timer = 0.0f;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    level_end = norm;
}
void Level3_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgm_Group);
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
        level_end = fail;
    }

    switch (is_paused) {
    case false:
        AEMtx33 scale, rotate, translate;

        if (level_end == norm) {
            //PPG platelets code
            update_ppg(spawned_Viruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawned_Viruses);

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            virus_Select(&selected, virus_to_spawn, x, y, spawn_x + 200.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawn_x, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawn_x + 400.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);

            //Selection for polio
            virus_Select(&selected, polio_virus, x, y, spawn_x + 800.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);

            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpoint_x, -200.f, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpoint_x, 200.f, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            update_Enemies(spawned_Viruses, dt, end_x, spawnpoint_y, transforms, &scale, &rotate, &translate, scale_x, scale_y, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL3TOWER, towered, spawned_Viruses.size(), spawned_Viruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawned_Viruses.size(), spawned_Viruses, LVL3TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulence_meter_render(&meter);

        //Spawnpoint

        spawnpoint_lvl3_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selectors
        level3_selector_render();
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);
        
        virus_price(&cost, selected);
        
        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL3TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL3TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        boss1_tower(&boss_timer);
        bossinvuln(&boss_inv_timer);


        if (level_end == norm || event_happened == false) {
            event_trigger(STATE_LEVEL3, spawned_Viruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if (level_end == vic || AEInputCheckTriggered(AEVK_N)) {
            level_end = vic;
            game_condition = level_completed;
        }


        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }


        // Check for pause
        if (level_end == norm && (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE))) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulence_meter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        level3_selector_render();
        // Selection Square for shingles
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL3TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL3TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}


void Level3_Exit() {
    std::cout << "Exiting Level 3..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawned_Viruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    Free_Bullet();
    vFree_Bullet();
    cleanup_virus(spawned_Viruses);

    free_platelets_mesh();
    free_ppg_mesh();
    free_resource();
    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    free_virulence_meter();
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
    free_audio(bgm, bgm_Group);
    chickenpox_Defeated = false;
}

void Level4_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);
    
    init_audio(bgm, bgm_Group);

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
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour, pew_colour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour2, pew_colour2);
    virulence_meter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    level_end = norm;
}


void Level4_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgm_Group);
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
        
        if (level_end == norm) {
            //PPG platelets code
            update_ppg(spawned_Viruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawned_Viruses);
            

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            // Select virus
            virus_Select(&selected, virus_to_spawn, x, y, spawn_x + 200.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawn_x, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawn_x + 400.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, polio_virus, x, y, spawn_x + 800.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            
            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y2, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            update_Enemies(spawned_Viruses, dt, end_x, spawnpoint_y, transforms, &scale, &rotate, &translate, scale_x, scale_y, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL4TOWER, towered, spawned_Viruses.size(), spawned_Viruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawned_Viruses.size(), spawned_Viruses, LVL4TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulence_meter_render(&meter);

        //Spawnpoint
        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selector Rendering
        level3_selector_render();
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        virus_price(&cost, selected);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL4TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL4TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        if (level_end == norm || event_happened == false) {
            event_trigger(STATE_LEVEL4, spawned_Viruses, square_columns, square_rows, cell_length);           // check for event trigger
        }


        if (level_end == vic || AEInputCheckTriggered(AEVK_N)) {
            level_end = vic;
            game_condition = level_completed;
        }


        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        // Check for pause
        if (level_end == norm && (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE))) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulence_meter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        // Selection Squares
        level3_selector_render();
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL4TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL4TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}

void Level4_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawned_Viruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    Free_Bullet();
    vFree_Bullet();
    cleanup_virus(spawned_Viruses);
    free_resource();
    free_platelets_mesh();
    free_ppg_mesh();

    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    free_virulence_meter();
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
    free_audio(bgm, bgm_Group);
    chickenpox_Defeated = false;
}


void Level5_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    init_audio(bgm, bgm_Group);

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
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour, pew_colour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour2, pew_colour2);
    virulence_meter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Init spritesheet
    ss_init(&ss_data);

    init_game_result();

    //Game win state set to norm
    level_end = norm;
}

void Level5_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgm_Group);
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

        if (level_end == norm) {
            //PPG platelets code
            update_ppg(spawned_Viruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawned_Viruses);


            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            //Virus Selectors
            virus_Select(&selected, virus_to_spawn, x, y, spawn_x + 200.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawn_x, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawn_x + 400.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, ebola, x, y, spawn_x + 600.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);

            //Selection for polio
            virus_Select(&selected, polio_virus, x, y, spawn_x + 800.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            

            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y2, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            update_Enemies(spawned_Viruses, dt, end_x, spawnpoint_y, transforms, &scale, &rotate, &translate, scale_x, scale_y, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL5TOWER, towered, spawned_Viruses.size(), spawned_Viruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawned_Viruses.size(), spawned_Viruses, LVL5TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();

        virulence_meter_render(&meter);

        //Spawnpoint

        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selector Rendering
        level5_selector_render();
        virus_price(&cost, selected);
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL5TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL5TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        if (level_end == norm || event_happened == false) {
            event_trigger(STATE_LEVEL5, spawned_Viruses, square_columns, square_rows, cell_length);           // check for event trigger
        }


        if (level_end == vic || AEInputCheckTriggered(AEVK_N)) {
            level_end = vic;
            game_condition = level_completed;
        }

   

        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        if (level_end == norm && (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE))) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulence_meter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        // Selection Squares
        level5_selector_render();
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL5TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL5TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}

void Level5_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawned_Viruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    Free_Bullet();
    vFree_Bullet();
    cleanup_virus(spawned_Viruses);
    free_resource();
    free_platelets_mesh();
    free_ppg_mesh();

    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    free_virulence_meter();
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
    free_audio(bgm, bgm_Group);
    chickenpox_Defeated = false;
}


void Level6_Init() {

    //countdown_timer init
    countdown_init(&timer, font);

    grid = Create_Grid(square_rows, square_columns, cell_length, cell_length, 0xFF00FF00);

    init_audio(bgm, bgm_Group);

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
    white_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour, pew_colour);
    //call function DrawCircleMesh, pointer red cMesh is used for rendering later
    red_cMesh = draw_circle_mesh(0.0f, 0.0f, 1.0f, 100, pew_colour2, pew_colour2);
    virulence_meter_init(&meter);

    //Starting income
    preallocated_resource = 500;

    //Virulence Meter
    meter = 0;

    //Game win state set to norm
    level_end = norm;

    //Init spritesheet
    ss_init(&ss_data);
    init_game_result();
    searchintersecting();
    //Boss timer
    boss_timer = 0.0f;

    //boss invulnerability timer
    boss_inv_timer = 0.0f;
}

void Level6_Update() {
    // Render the grid
    Render_Grid(grid, square_rows, square_columns);
    display_playGAME_resource(font, preallocated_resource);
    input_audio(bgm_Group);
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

        if (level_end == norm) {
            //PPG platelets 
            update_ppg(spawned_Viruses, square_columns, square_rows, cell_length);
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            collision_platelets_virus(spawned_Viruses);

            if (event_happened == true && vaccine_boosted == false && vaccine_active == true) {
                std::cout << "Vaccine updated called" << std::endl;
                vaccine_update();
            }
            else if (event_happened == true && para_boosted == false && para_active == true) {
                std::cout << "Para updated called" << std::endl;
                para_update();
            }

            virus_Select(&selected, virus_to_spawn, x, y, spawn_x + 200.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, hfmdvirus, x, y, spawn_x, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, dengue, x, y, spawn_x + 400.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, ebola, x, y, spawn_x + 600.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);
            virus_Select(&selected, polio_virus, x, y, spawn_x + 800.0f, spawn_y, 100.0f, 100.0f, AEInputCheckTriggered(AEVK_LBUTTON), spawned_Viruses, &preallocated_resource, &cost);


            //Spawn Selectors/Spawnpoints
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);
            spawner_Select(selected, x, y, spawnpoint_x, spawnpoint_y2, 200.0f, 200.0f, spawned_Viruses, AEInputCheckTriggered(AEVK_LBUTTON), &preallocated_resource, &cost);

            update_Enemies(spawned_Viruses, dt, end_x, spawnpoint_y, transforms, &scale, &rotate, &translate, scale_x, scale_y, meter, &preallocated_resource);

            Tower_Shooter_Logic(LVL6TOWER, towered, spawned_Viruses.size(), spawned_Viruses, dt, 0XFFFFFFFF);
            Virus_Shooter_Logic(spawned_Viruses.size(), spawned_Viruses, LVL6TOWER, towered, dt, 0xFFFF0000);
        }
        render_ppg();
        render_platelets();

        // Render towers
        render_tower();
        update_tower();
        virulence_meter_render(&meter);

        //Spawnpoint

        spawnpoint_render();
        AEInputGetCursorPosition(&x, &y);

        //Virus Selector Rendering
        level5_selector_render();
        virus_price(&cost, selected);
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);
        
        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);
        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL6TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL6TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        bossinvuln(&boss_inv_timer);
        boss2_tower(&boss_timer);
        // Check if all enemies are defeated or another condition

        if (level_end == norm || event_happened == false) {
            event_trigger(STATE_LEVEL6, spawned_Viruses, square_columns, square_rows, cell_length);           // check for event trigger
        }

        if (level_end == vic || AEInputCheckTriggered(AEVK_N)) {
            level_end = vic;
            game_condition = level_completed;
        }


        switch (game_condition) {
        case game_lost: display_game_result(game_lost, font, LOST_PAGE_texture);
            break;
        case level_completed: display_game_result(level_completed, font, WIN_PAGE_texture);
        }

        // Check for pause
        if (level_end == norm && (AEInputCheckTriggered(AEVK_P) || AEInputCheckTriggered(AEVK_ESCAPE))) {
            is_paused = true;
        }


        break;

    case true:
        render_ppg();
        render_platelets();

        render_tower();

        // Virulence meter 
        virulence_meter_render(&meter);

        // Spawnpoints
        spawnpoint_render();
        // Selection Squares
        level5_selector_render();
        render_shingle_select(chickenpox_Defeated, &selected, x, y, spawned_Viruses, &preallocated_resource, &cost);

        // Virus
        render_Enemies(spawned_Viruses, transforms, &scale, &rotate, &translate, scale_x, scale_y);

        // HP
        render_hp(); // Render HP bars

        // Bullets
        render_Virus_Bullet(spawned_Viruses.size(), spawned_Viruses, LVL6TOWER, towered, 0xFFFF0000);
        render_Tower_Bullet(LVL6TOWER, towered, spawned_Viruses.size(), 0XFFFFFFFF);

        // Pause Menu
        render_pause_screen(font, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), is_paused);
        break;
    }
}

void Level6_Exit() {
    std::cout << "Exiting Level 1..." << std::endl;
    game_condition = none;

    event_trigger(STATE_GAME_OVER, spawned_Viruses, square_columns, square_rows, cell_length);

    ss_free(&ss_data);
    vaccine_active = false;

    Free_Bullet();
    vFree_Bullet();
    cleanup_virus(spawned_Viruses);
    free_resource();
    free_platelets_mesh();
    free_ppg_mesh();

    tooltips_free();
    free_game_result();

    if (grid) {
        Free_Grid(grid, square_rows, square_columns);
        grid = NULL; // Prevent re-accessing freed memory
    }
    free_spawner();
    free_virulence_meter();
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
    free_audio(bgm, bgm_Group);
    chickenpox_Defeated = false;
}

void freemeshes() {
    free_bullets(&white_cMesh, &red_cMesh);
    if (circlemesh) AEGfxMeshFree(circlemesh);
}