/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      towers.cpp
*@author(s)     Chan Qi Ying (Primary Author, 35%)
*               Dalton Koh (Secondary Author, 35%)
*               Ethan Ng (Secondary Author, 30%)
*@DP emails     q.chan@digipen.edu
*               d.koh@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains definitions for functions related to creating, updating,
*               rendering, and freeing of towers, including bosses and their respective
*               skills.
******************************************************************************/
#include "document.h"       // .json
#include "istreamwrapper.h" // .json
#include "AEEngine.h"
#include "collision.h"
#include "towers.h"
#include "circle.h"
#include "squaregrid.h"
#include "healthbar.h"
#include "iostream"
#include "rectangle.h"
#include "textureselect.h"
#include "level.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>


#define MAXTOWER 20
#define LVL1TOWER 8
#define LVL2TOWER 8
#define LVL3TOWER 11

// Skill variables
struct tower towered[MAXTOWER];
static float inv_timer{};
static int intersectors{};
static int boss_index{};
static int intersect_list{};
static float atk_spd_multiplier{};
static float boss1_buff_timer{};
static float boss2_revive_timer{};
static float boss_inv_duration{};
static float boss_inv_cd{};
static float default_offset{};
static unsigned int white{};
static float x;
static float y;
static float boss_width_modifier;
AEGfxVertexList* invmesh;

// Tower initial stats
static unsigned int tower_color{}, tower_coll_color{};
static float tower_coor{}, tower_scale{}, tower_coll_scale{};

// Tower specific stats
static int lymp_atk{}, lymp_hp{}, macro_atk{}, macro_hp{};
static int boss1_atk{}, boss1_hp{}, boss2_atk{}, boss2_hp{};
static float lymp_atk_spd{}, lymp_radius{}, lymp_coll_radius{};
static float macro_atk_spd{}, macro_radius{}, macro_coll_radius{};
static float boss1_atk_spd{}, boss1_radius{}, boss1_coll_radius{};
static float boss2_atk_spd{}, boss2_radius{}, boss2_coll_radius{};
static bool boss_state{};
static float boss_active_display{};
static float boss_active_display_time{};
//Tower reset stats
static int reset_type{}, reset_general_int{};
static float reset_general_float{}, reset_coor{};

// macrophage_skill
static int time_before_death1{}, time_before_death2{}, time_before_death3{}, time_before_death4{}, reset{};

// Load Data
void load_data_towers() {
    std::fstream data("Assets/Data/towers.json");
    if (!data) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(data);
    rapidjson::Document doc;

    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return; // Exit the function on parse error
    }

    // Assigning data from file to variable
    tower_color = std::stoul(doc["tower_color"].GetString(), nullptr, 16);
    tower_coll_color = std::stoul(doc["tower_coll_color"].GetString(), nullptr, 16);
    tower_coor = doc["tower_coor"].GetFloat();
    tower_scale = doc["tower_scale"].GetFloat();
    tower_coll_scale = doc["tower_coll_scale"].GetFloat();

    lymp_atk = doc["lymp_atk"].GetInt();
    lymp_hp = doc["lymp_hp"].GetInt();
    macro_atk = doc["macro_atk"].GetInt();
    macro_hp = doc["macro_hp"].GetInt();
    boss1_atk = doc["boss1_atk"].GetInt();
    boss1_hp = doc["boss1_hp"].GetInt();
    boss2_atk = doc["boss2_atk"].GetInt();
    boss2_hp = doc["boss2_hp"].GetInt();

    lymp_atk_spd = doc["lymp_atk_spd"].GetFloat();
    lymp_radius = doc["lymp_radius"].GetFloat();
    lymp_coll_radius = doc["lymp_coll_radius"].GetFloat();
    macro_atk_spd = doc["macro_atk_spd"].GetFloat();
    macro_radius = doc["macro_radius"].GetFloat();
    macro_coll_radius = doc["macro_coll_radius"].GetFloat();
    boss1_atk_spd = doc["boss1_atk_spd"].GetFloat();
    boss1_radius = doc["boss1_radius"].GetFloat();
    boss1_coll_radius = doc["boss1_coll_radius"].GetFloat();
    boss_state = doc["boss_state"].GetBool();
    boss_active_display = doc["boss_active_display"].GetFloat();
    boss_active_display_time = doc["boss_active_display_time"].GetFloat();
    boss2_atk_spd = doc["boss2_atk_spd"].GetFloat();
    boss2_radius = doc["boss2_radius"].GetFloat();
    boss2_coll_radius = doc["boss2_coll_radius"].GetFloat();

    reset_type = doc["reset_type"].GetInt();
    reset_general_int = doc["reset_general_int"].GetInt();
    reset_general_float = doc["reset_general_float"].GetFloat();
    reset_coor = doc["reset_coor"].GetFloat();

    time_before_death1 = doc["time_before_death1"].GetInt();
    time_before_death2 = doc["time_before_death2"].GetInt();
    time_before_death3 = doc["time_before_death3"].GetInt();
    time_before_death4 = doc["time_before_death4"].GetInt();
    reset = doc["reset"].GetInt();
    inv_timer = doc["inv_timer"].GetFloat();
    atk_spd_multiplier = doc["atk_spd_multiplier"].GetFloat();
    boss1_buff_timer = doc["boss1_buff_timer"].GetFloat();
    boss2_revive_timer = doc["boss2_revive_timer"].GetFloat();
    boss_inv_duration = doc["boss_inv_duration"].GetFloat();
    boss_inv_cd = doc["boss_inv_cd"].GetFloat();
    white = static_cast<unsigned int>(doc["white"].GetUint64());
    default_offset = doc["default_offset"].GetFloat();
    boss_width_modifier = doc["boss_width_modifier"].GetFloat();
}

// Initialise tower stats
void init_tower_stats(int index, Tower_Type type) {
    switch (type) {
    case lymphocyte:
        towered[index].type = lymphocyte;
        towered[index].atk = lymp_atk;
        towered[index].atk_spd = lymp_atk_spd;
        towered[index].base_hp = lymp_hp;
        towered[index].current_hp = lymp_hp;
        towered[index].state = COOLDOWN_STATE;
        towered[index].size.xy = { tower_coor, tower_coor };
        towered[index].size.radius = lymp_radius;
        towered[index].size.coll_radius = lymp_coll_radius;
        towered[index].size.color = tower_color;
        towered[index].size.coll_color = tower_coll_color;
        towered[index].size.scalex = tower_scale;
        towered[index].size.scaley = tower_scale;
        towered[index].size.coll_scale_x = tower_coll_scale;
        towered[index].size.coll_scale_y = tower_coll_scale;
        break;

    case macrophage:
        towered[index].type = macrophage;
        towered[index].atk = macro_atk;
        towered[index].atk_spd = macro_atk_spd;
        towered[index].base_hp = macro_hp;
        towered[index].current_hp = macro_hp;
        towered[index].state = COOLDOWN_STATE;
        towered[index].size.xy = { tower_coor, tower_coor };
        towered[index].size.radius = macro_radius;
        towered[index].size.coll_radius = macro_coll_radius;
        towered[index].size.color = tower_color;
        towered[index].size.coll_color = tower_coll_color;
        towered[index].size.scalex = tower_scale;
        towered[index].size.scaley = tower_scale;
        towered[index].size.coll_scale_x = tower_coll_scale;
        towered[index].size.coll_scale_y = tower_coll_scale;
        break;
    case boss1:
        towered[index].type = boss1;
        towered[index].atk = boss1_atk;
        towered[index].atk_spd = boss1_atk_spd;
        towered[index].base_hp = boss1_hp;
        towered[index].current_hp = boss1_hp;
        towered[index].state = COOLDOWN_STATE;
        towered[index].size.xy = { tower_coor, tower_coor };
        towered[index].size.radius = boss1_radius;
        towered[index].size.coll_radius = boss1_coll_radius;
        towered[index].size.color = tower_color;  
        towered[index].size.coll_color = tower_coll_color;
        towered[index].size.scalex = tower_scale;
        towered[index].size.scaley = tower_scale;
        towered[index].size.coll_scale_x = tower_coll_scale;
        towered[index].size.coll_scale_y = tower_coll_scale;
        break;
    case boss2:
        towered[index].type = boss2;
        towered[index].atk = boss2_atk;
        towered[index].atk_spd = boss2_atk_spd;
        towered[index].base_hp = boss2_hp;
        towered[index].current_hp = boss2_hp;
        towered[index].state = COOLDOWN_STATE;
        towered[index].size.xy = { tower_coor, tower_coor };
        towered[index].size.radius = boss2_radius;
        towered[index].size.coll_radius = boss2_coll_radius;
        towered[index].size.color = tower_color;
        towered[index].size.coll_color = tower_coll_color;
        towered[index].size.scalex = tower_scale;
        towered[index].size.scaley = tower_scale;
        towered[index].size.coll_scale_x = tower_coll_scale;
        towered[index].size.coll_scale_y = tower_coll_scale;
        break;
    default:     
        break;
    }
}

// Update function
void update_tower() {
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].current_hp <= NULL) {
            towered[i].state = DEAD_STATE;
            if (towered[i].mesh != NULL) {
                AEGfxMeshFree(towered[i].mesh);
                towered[i].mesh = NULL;
                --intersectors;
            }
        }

        // Ensure HP Bar Tracks Tower Position
        float towerHPPercentage = (float)towered[i].current_hp / towered[i].base_hp;
        float deltaTime = static_cast<float>(AEFrameRateControllerGetFrameTime());


        if (towered[i].current_hp >= place_0) {
            update_hp_T(i, towerHPPercentage, deltaTime, towered[i].size.xy.x, towered[i].size.xy.y);
        }
    }
}

// Init function to create towers
void create_towers(int* tower_Indices, int count) {
    for (int i{}; i < count; i++) {
        int towerID = tower_Indices[i];

        if (towerID == -place_1) {
            continue;
        }

        AEMtx33 scale, translate;
        AEMtx33Scale(&scale, towered[towerID].size.scalex, towered[towerID].size.scaley);
        AEMtx33Trans(&translate, towered[towerID].size.xy.x, towered[towerID].size.xy.y);
        AEMtx33Concat(&towered[towerID].transform, &translate, &scale);

        if (towered[towerID].type == boss1 || towered[towerID].type == boss2) {
            Create_Rectangle(
                &towered[towerID].mesh,
                towered[towerID].size.radius+ boss_width_modifier,
                towered[towerID].size.radius,
                towered[towerID].size.color,
                &towered[towerID].transform,
                towered[towerID].size.scalex, towered[towerID].size.scaley,
                towered[towerID].size.xy.x,
                towered[towerID].size.xy.y);
        }

        else {
            Create_Rectangle(
                &towered[towerID].mesh,
                towered[towerID].size.radius,
                towered[towerID].size.radius,
                towered[towerID].size.color,
                &towered[towerID].transform,
                towered[towerID].size.scalex, towered[towerID].size.scaley,
                towered[towerID].size.xy.x,
                towered[towerID].size.xy.y);
        }

        if (towered[towerID].mesh == NULL) {
            return; // Stop if mesh creation fails
        }

    }
}



void place_towers(SG_stats** grid, int rows, int columns, int level) {
    struct TowerPlacement {
        int row;
        int col;
        Tower_Type type;
    };

    TowerPlacement towerPlacements[MAXTOWER];
    int numTowers = reset_general_int;

    // Select tower positions based on the level
    switch (level) {
    case 1: // Level 1 tower placements
    {
        TowerPlacement level1Towers[] = {
            {place_1, place_4, lymphocyte}, {place_1, place_6, lymphocyte}, {place_1, place_8, macrophage},
            {place_5, place_4, lymphocyte}, {place_5, place_6, lymphocyte}, {place_5, place_8, macrophage}
        };
        numTowers = sizeof(level1Towers) / sizeof(level1Towers[0]);
        memcpy(towerPlacements, level1Towers, sizeof(level1Towers));
    }
    break;

    case 2: // Level 2 tower placements
    {
        TowerPlacement level2Towers[] = {
            {place_1, place_3, lymphocyte}, {place_1, place_5, macrophage}, {place_1, place_7, macrophage},
            {place_3, place_4, lymphocyte}, {place_3, place_6, macrophage},
            {place_5, place_3, lymphocyte}, {place_5, place_5, macrophage}, {place_5, place_7, macrophage},
           
        };
        numTowers = sizeof(level2Towers) / sizeof(level2Towers[0]);
        memcpy(towerPlacements, level2Towers, sizeof(level2Towers));
    }
    break;

    case 3: // Level 3 tower placements 
    {
        TowerPlacement level3Towers[] = {
            {place_0, place_1, macrophage}, {place_0, place_3, macrophage}, {place_0, place_5, macrophage}, 
            {place_0, place_7, lymphocyte}, {place_0, place_9, lymphocyte}, {place_6, place_1, macrophage},
            {place_6, place_3, macrophage}, {place_6, place_5, macrophage}, {place_6, place_7, lymphocyte},
            {place_6, place_9, lymphocyte},
            {place_3, place_6, boss1}
        };
        numTowers = sizeof(level3Towers) / sizeof(level3Towers[0]);
        memcpy(towerPlacements, level3Towers, sizeof(level3Towers));
    }
    break;

    case 4: // Level 4 tower placements 
    {
        TowerPlacement level4Towers[] = {
            {place_1, place_1, macrophage}, {place_1, place_3, macrophage}, {place_1, place_5, macrophage}, {place_1, place_7, lymphocyte},
            {place_1, place_9, lymphocyte}, {place_3, place_4, lymphocyte}, {place_3, place_6, macrophage},  {place_3, place_8, macrophage},
            {place_5, place_1, macrophage}, {place_5, place_3, macrophage}, {place_5, place_5, macrophage}, {place_5, place_7, lymphocyte},
            {place_5, place_9, lymphocyte}
        };
        numTowers = sizeof(level4Towers) / sizeof(level4Towers[0]);
        memcpy(towerPlacements, level4Towers, sizeof(level4Towers));
    }
    break;
    case 5: // Level 5 tower placements
    {
        TowerPlacement level5Towers[] = {
            {place_1, place_1, macrophage}, {place_1, place_3, macrophage}, {place_1, place_5, macrophage}, {place_1, place_7, lymphocyte}, 
            {place_1, place_9, lymphocyte}, {place_1, place_11,lymphocyte}, {place_3, place_4, lymphocyte}, {place_3,place_5,lymphocyte},
            {place_3,place_7, macrophage}, {place_3, place_6, macrophage},  {place_3, place_8, macrophage}, {place_5, place_1, macrophage},
            {place_5, place_3, macrophage}, {place_5, place_5, macrophage}, {place_5, place_7, lymphocyte}, {place_5, place_9, lymphocyte},
            {place_5, place_11,lymphocyte}
        };
        numTowers = sizeof(level5Towers) / sizeof(level5Towers[0]);
        memcpy(towerPlacements, level5Towers, sizeof(level5Towers));
    }
    break;
    case 6: // Level 6 tower placements
    {
        TowerPlacement level6Towers[] = {
            {place_1, place_4, macrophage}, {place_1, place_6, macrophage}, {place_1, place_8, lymphocyte}, {place_1, place_10, lymphocyte},
            {place_1, place_12,lymphocyte}, {place_0, place_5, macrophage}, {place_0, place_7, macrophage}, {place_0, place_9, lymphocyte}, 
            {place_0, place_11, lymphocyte},{place_5, place_4, macrophage}, {place_5, place_6, macrophage}, {place_5, place_8, lymphocyte},
            {place_5, place_10, lymphocyte}, {place_5, place_12,lymphocyte},{place_6, place_5, macrophage}, {place_6, place_7, macrophage}, 
            {place_6, place_9, lymphocyte}, {place_6, place_11, lymphocyte},{place_3, place_8, boss2}
        };
        numTowers = sizeof(level6Towers) / sizeof(level6Towers[0]);
        memcpy(towerPlacements, level6Towers, sizeof(level6Towers));
    }
    break;

    default:
        return;
    }

    // Placement logic
    int towerIndex = reset_general_int;
    int createdTowerIndices[MAXTOWER];
    memset(createdTowerIndices, -1, sizeof(createdTowerIndices)); // Initialize to -1

    for (int i{}; i < numTowers; i++) {
        if (towerIndex >= MAXTOWER) {
            return;
        }

        int row = towerPlacements[i].row;
        int col = towerPlacements[i].col;

        if (row < place_0 || row >= rows || col < place_0 || col >= columns) {
            continue;
        }

        if (grid[row][col].is_Occupied) {
            continue;
        }

        // Initialize tower stats
        init_tower_stats(towerIndex, towerPlacements[i].type);
        towered[towerIndex].size.xy.x = grid[row][col].x_cordinate;
        towered[towerIndex].size.xy.y = grid[row][col].y_cordinate;

        grid[row][col].is_Occupied = place_1;
        createdTowerIndices[towerIndex] = towerIndex;
        towerIndex++;


    }
    for (int i = 0; i < MAXTOWER; ++i) {
        make_hp(i, towered[i].size.xy.x, towered[i].size.xy.y - towered[i].size.radius - boss_inv_duration, tower_coor, false);
    }

    // Create tower meshes
    create_towers(createdTowerIndices, towerIndex);
}

// Render towers
void render_tower() {


    //  Ensure Color Mode is enabled
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].mesh == NULL) {
 
            continue;
        }
            
           if (towered[i].type == lymphocyte ) {
               // Ensure texture mode is enabled
              AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
              AEGfxTextureSet(lymphocyte_Texture, default_offset, default_offset);
          }
           if (towered[i].type == macrophage) {
               AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
               AEGfxTextureSet(macrophage_Texture, default_offset, default_offset);
           }
           if (towered[i].type == boss1) {
               if (boss_state == false) {
                   AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                   AEGfxTextureSet(boss_1_t, default_offset, default_offset);
               }
               if (boss_state == true) {
                   AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                   AEGfxTextureSet(boss_1_active, default_offset, default_offset);
                   boss_active_display += dt;
                   if (boss_active_display >= boss_active_display_time) {
                       boss_active_display = reset_general_float;
                       boss_state = false;
                   }
                   
               }
           }
           if (towered[i].type == boss2) {
               //  Ensure texture mode is enabled
               AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
               AEGfxTextureSet(boss_2_t, default_offset, default_offset);
           }
        AEGfxSetTransform(towered[i].transform.m);
        AEGfxMeshDraw(towered[i].mesh, AE_GFX_MDM_TRIANGLES);

    }
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}

void reset_tower_data(int index) {
    towered[index].type = (Tower_Type)reset_type; // Invalid type
    towered[index].atk = reset_general_int;
    towered[index].atk_spd = reset_general_float;
    towered[index].base_hp = reset_general_int;
    towered[index].current_hp = reset_general_int;
    towered[index].state = DEAD_STATE;
    towered[index].size.xy.x = reset_coor; // Move far off-screen
    towered[index].size.xy.y = reset_coor;
    towered[index].size.radius = reset_general_float;
    towered[index].mesh = NULL;
}

// Call this in the beginning
void reset_all_towers() {
    for (int i{}; i < MAXTOWER; i++) {
        reset_tower_data(i);
    }
}

// Free tower meshes
void free_tower() {
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].mesh != NULL) {
            AEGfxMeshFree(towered[i].mesh);
            towered[i].mesh = NULL;
        }
    }
    if (invmesh) {
        AEGfxMeshFree(invmesh);
        invmesh = NULL;
    }
}


// Macrophage skill function 
void macrophage_skill(std::vector<virus*> spawned_virus) {
    for (size_t i{}; i < spawned_virus.size(); ++i) {
        for (size_t j = 0; j < MAXTOWER; ++j) {    
            
            // Collision with skill 
            if (are_circles_intersecting(spawned_virus[i]->x, spawned_virus[i]->y, spawned_virus[i]->radius, towered[j].size.xy.x, towered[j].size.xy.y, towered[j].size.coll_radius) && towered[j].type == macrophage) {
                if (spawned_virus[i]->start_time != reset) {
                    time(&(spawned_virus[i]->current_time));

                    // If virus is within it overtime skill takes effect
                    spawned_virus[i]->elapsed_time = spawned_virus[i]->current_time - spawned_virus[i]->start_time;
                    if (spawned_virus[i]->elapsed_time >= spawned_virus[i]->time_before_death) {
                        spawned_virus[i]->currenthp -= spawned_virus[i]->currenthp;
                    }
                }
                else {
                    time(&(spawned_virus[i]->start_time));
                    switch (spawned_virus[i]->type) {
                    case hfmdvirus:
                    case hfmdspeed:
                    case minion_polio_virus:
                        spawned_virus[i]->time_before_death = time_before_death1;
                        break;
                    case hfmdtank:
                    case chickenpox:
                    case shingle:
                    case dengue:
                        spawned_virus[i]->time_before_death = time_before_death2;
                        break;
                    case polio_virus:
                        spawned_virus[i]->time_before_death = time_before_death3;
                        break;
                    case ebola:
                        spawned_virus[i]->time_before_death = time_before_death4;
                        break;
                    }
                }
                break;
            }
        
           
        }
         if (spawned_virus[i]->start_time == reset) {      // only if virus does not collide with any tower than it will reset
             // resetting variables
             spawned_virus[i]->start_time = reset;
             spawned_virus[i]->current_time = reset;
             spawned_virus[i]->elapsed_time = reset;
             spawned_virus[i]->time_before_death = reset;
            }
    }


}

// Search function to apply skills for boss
void searchintersecting() {
    boss_index = reset_general_int;
    intersectors = reset_general_int;
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].type == boss1 || towered[i].type == boss2) {
            boss_index = i;
            break;
        }
    }

    for (int i{}; i < MAXTOWER; i++) {
        if (are_circles_intersecting(towered[boss_index].size.xy.x, towered[boss_index].size.xy.y,
            towered[boss_index].size.coll_radius, towered[i].size.xy.x,
            towered[i].size.xy.y, towered[i].size.radius)) {
            intersectors++;
        }
    }
}

// Boss_1 function
void boss1_tower(float* timer) {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    intersect_list = reset_general_int;
    boss_index = reset_general_int;
    
    //Delta time
    *timer += dt;

    //Search for boss_index
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].type == boss1) {
            boss_index = i;
            break;
        }
    }

    //Check for intersetting towers
    for (int i{}; i < MAXTOWER; i++) {
        //Checks if tower is alive or dead
        if (are_circles_intersecting(towered[boss_index].size.xy.x, towered[boss_index].size.xy.y,
            towered[boss_index].size.coll_radius, towered[i].size.xy.x,
            towered[i].size.xy.y, towered[i].size.radius)
            && towered[boss_index].state != DEAD_STATE && towered[i].state != DEAD_STATE) {
                if (*timer >= boss1_buff_timer) {
                    //Buff attack speed of intersecting towers
                    boss_state = true;
                    towered[i].atk_spd *= atk_spd_multiplier;
                    intersect_list++;
                 }
                if (intersect_list == intersectors) {
                    //Only resets after all intersecting towers has been buffed
                    intersect_list = reset_general_int;
                    *timer = reset_general_float;
                }
                
        }
    }
}

// Boss_2 function
void boss2_tower(float* timer) {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    intersect_list = reset_general_int;
    //Set index to 0
    boss_index = reset_general_int;
    //Delta time
    *timer += dt;

    //Search for boss index to exclude itself
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].type == boss2) {
            boss_index = i;
            break;
        }
    }

    for (int i{}; i < MAXTOWER; i++) {
        if (i == boss_index) continue; // Skip the boss itself
        //Checks if boss is alive and if tower is dead
        if (are_circles_intersecting(towered[boss_index].size.xy.x, towered[boss_index].size.xy.y,
            towered[boss_index].size.coll_radius, towered[i].size.xy.x,
            towered[i].size.xy.y, towered[i].size.radius)
            && towered[boss_index].state != DEAD_STATE && towered[i].state == DEAD_STATE) {
            x = towered[i].size.xy.x;
            y = towered[i].size.xy.y;
            //Reinitializing and rendering dead towers
            if (*timer >= boss2_revive_timer) {
                init_tower_stats(i, towered[i].type);
                towered[i].size.xy.x = x;
                towered[i].size.xy.y = y;
                AEMtx33 scale, translate;
                AEMtx33Scale(&scale, towered[i].size.scalex, towered[i].size.scaley);
                AEMtx33Trans(&translate, towered[i].size.xy.x, towered[i].size.xy.y);
                AEMtx33Concat(&towered[i].transform, &translate, &scale);

                Create_Rectangle(
                    &towered[i].mesh,
                    towered[i].size.radius,
                    towered[i].size.radius,
                    towered[i].size.color,
                    &towered[i].transform,
                    towered[i].size.scalex, towered[i].size.scaley,
                    towered[i].size.xy.x,
                    towered[i].size.xy.y);

                if (towered[i].type == lymphocyte) {
                    //  Ensure Texture Mode is enabled
                    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                    AEGfxTextureSet(lymphocyte_Texture, default_offset, default_offset);
                }
                if (towered[i].type == macrophage) {
                    //  Ensure Texture Mode is enabled
                    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                    AEGfxTextureSet(macrophage_Texture, default_offset, default_offset);
                }
                AEGfxMeshDraw(towered[i].mesh, AE_GFX_MDM_TRIANGLES);
                make_hp(i, towered[i].size.xy.x, towered[i].size.xy.y - towered[i].size.radius - boss_inv_duration, tower_coor, false);
                //set it from dead state back to cooldown state to trigger shooting function
                towered[i].state = COOLDOWN_STATE;
                intersect_list++;
                *timer = reset_general_float;
            }
            if (intersect_list == intersectors) {
                //Only reset timer after all dead towers are revived
                intersect_list = reset_general_int;
                *timer = reset_general_float;
            }

        }
    }
}

// Boss invulnerability state function
void bossinvuln(float *timer) {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    //Detla time
    *timer += dt;

    //Check for boss tower
    for (int i{}; i < MAXTOWER; i++) {
        //Rendering
        if ((towered[i].type == boss1 || towered[i].type == boss2) && towered[i].state != DEAD_STATE) {
            if (*timer >= boss_inv_cd) {
                towered[i].inv_state = true;
                inv_timer += dt;
                if (invmesh) AEGfxMeshFree(invmesh);
                Create_Rectangle(
                    &invmesh,
                    towered[i].size.radius + boss_width_modifier,
                    towered[i].size.radius,
                    white,
                    &towered[i].transform,
                    towered[i].size.scalex,
                    towered[i].size.scaley,
                    towered[i].size.xy.x,
                    towered[i].size.xy.y
                );
                if (inv_timer <= boss_inv_duration) {
                    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                    AEGfxSetTransform(towered[i].transform.m);
                    if (towered[i].type == boss1) {
                        AEGfxTextureSet(boss_1_inv, default_offset, default_offset);
                    }

                    if (towered[i].type == boss2) {
                        AEGfxTextureSet(boss_2_inv, default_offset, default_offset);
                    }
                    AEGfxMeshDraw(invmesh, AE_GFX_MDM_TRIANGLES);
                }
                else {
                    //Freeing mesh
                    AEGfxMeshFree(invmesh);
                    invmesh = NULL;
                    towered[i].inv_state = false;
                    //Reset timers
                    inv_timer = reset_general_float;
                    *timer = reset_general_float;
                }
            }
            break;
        }
    }
}

// free
bool AreAllTowersDestroyed() {
    for (int i{}; i < MAXTOWER; i++) {
        if (towered[i].state != DEAD_STATE) {
            return false; // At least one tower is still alive
        }
    }
    return true; // All towers are dead
}

