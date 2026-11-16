/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      platelets.cpp
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains definitions for functions to create, update, render and free
*				platelets event related items.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "AEEngine.h"
#include "collision.h"
#include "rectangle.h"
#include "events.h"    // seedgenerator(), random_coor_generator()
#include "platelets.h"           
#include "level.h"      
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

static float platelets_width{}, platelets_height{};
static float hp_width{}, hp_height{};
static float base_health{};
static float reset_coor{}, scale{}, radius{}, half{};

static unsigned int platelet_color{}, fg_color{}, bg_color{};
static int frame_count1{}, frame_count2{}, frame_count3{}, deplete1{}, deplete2{}, deplete3{};

std::vector<Platelets> platelets;
static int sz{};

AEGfxVertexList* platelets_mesh{};
AEMtx33 platelets_transfmtx;

AEGfxVertexList* platelets_hp_bmesh {};
AEGfxVertexList* platelets_hp_fmesh {};
AEMtx33 platelets_hp_transfmtx;

// Load data
void load_data_platelets() {
    std::fstream data("Assets/Data/event.json");
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
    half = doc["half"].GetFloat();
    sz = doc["sz"].GetInt();
    platelets_width = doc["platelets_width"].GetFloat();
    platelets_height = doc["platelets_height"].GetFloat();
    hp_width = doc["hp_width"].GetFloat();
    hp_height = doc["hp_height"].GetFloat();
    base_health = doc["base_health"].GetFloat();
    reset_coor = doc["reset_coor"].GetFloat();
    scale = doc["scale"].GetFloat();
    radius = doc["radius"].GetFloat();
    
    platelet_color = std::stoul(doc["platelet_color"].GetString(), nullptr, 16);
    bg_color = std::stoul(doc["bg_color"].GetString(), nullptr, 16);
    fg_color = std::stoul(doc["fg_color"].GetString(), nullptr, 16);

    frame_count1 = doc["frame_count1"].GetInt();
    frame_count2 = doc["frame_count2"].GetInt();
    frame_count3 = doc["frame_count3"].GetInt();
    deplete1 = doc["deplete1"].GetInt();
    deplete2 = doc["deplete2"].GetInt();
    deplete3 = doc["deplete3"].GetInt();

}


// Initialise platelets mesh and coordinates
void init_platelets() {
    // platelets
    if (platelets_mesh == NULL) Create_Rectangle(&platelets_mesh, platelets_width, platelets_height, platelet_color, &platelets_transfmtx, scale, scale, reset_coor, reset_coor);      // platelets
    // Countdown bar
    if (platelets_hp_bmesh == NULL) Create_Rectangle(&platelets_hp_bmesh, hp_width, hp_height, bg_color, &platelets_hp_transfmtx, scale, scale, reset_coor, reset_coor);    //platelets hp bar
    if (platelets_hp_fmesh == NULL) Create_Rectangle(&platelets_hp_fmesh, hp_width, hp_height, fg_color, &platelets_hp_transfmtx, scale, scale, reset_coor, reset_coor);    //platelets hp bar

    // Coordinates
    platelets.clear();
    for (int i{}; i < sz; ++i) {
        Platelets plt;
        plt.xy.x = reset_coor;
        plt.xy.y = reset_coor;
        plt.curr_health = base_health;
        platelets.push_back(plt);
    }
}

// Assign coordinates for platelets
void generate_platelets(int square_columns, int square_rows, float cell_length) {
    if (triggered_event == platelet || AEInputCheckTriggered(AEVK_X)) {        // check for platelets event trigger
        for (int i{}; i < sz; ++i) {
            platelets[i].curr_health = base_health;
            AEVec2 pcoor = random_coor_generator(square_columns, square_rows, cell_length);
            for (int j{}; j < i; ++j)
                // make sure coordinates do not overlap
                while (pcoor.x == platelets[j].xy.x || pcoor.y == platelets[j].xy.y) pcoor = random_coor_generator(square_columns, square_rows, cell_length);

            platelets[i].xy = pcoor;
        }
        triggered_event = no_event;
    }


}

// Render platelets
void render_platelets() {
    for (int i{}; i < sz; ++i) {
        // Draw platelets
        AEMtx33Trans(&platelets_transfmtx, platelets[i].xy.x, platelets[i].xy.y);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetTransform(platelets_transfmtx.m);
        AEGfxMeshDraw(platelets_mesh, AE_GFX_MDM_TRIANGLES);

        // Draw platelets hp bar: background 
        AEMtx33Scale(&platelets_hp_transfmtx, base_health, hp_height);
        AEMtx33Trans(&platelets_hp_transfmtx, platelets[i].xy.x, platelets[i].xy.y + platelets_height/2.f + hp_height);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetTransform(platelets_hp_transfmtx.m);
        AEGfxMeshDraw(platelets_hp_bmesh, AE_GFX_MDM_TRIANGLES);

        // Draw platelets hp bar: foreground 
        AEMtx33Scale(&platelets_hp_transfmtx, (platelets[i].curr_health/base_health), scale);
        float new_x = platelets[i].xy.x - (hp_width - hp_width * (platelets[i].curr_health / base_health)) / half;
        AEMtx33TransApply(&platelets_hp_transfmtx, &platelets_hp_transfmtx, new_x, platelets[i].xy.y + platelets_height / half + hp_height);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetTransform(platelets_hp_transfmtx.m);
        AEGfxMeshDraw(platelets_hp_fmesh, AE_GFX_MDM_TRIANGLES);

    }
}

// Check collision between platelets and virus
void collision_platelets_virus(std::vector<virus*> spawnedVirus) {      // virus stays in place when collide with platelets
    for (size_t i{}; i < spawnedVirus.size(); ++i) {
        bool colliding = false;
        for (size_t j{}; j < sz; ++j) {
            // stop virus if colliding with platelets
            if (is_circle_intersecting_rectangle(spawnedVirus[i]->x, spawnedVirus[i]->y, radius, platelets[j].xy.x, platelets[j].xy.y, platelets[j].width, platelets[j].height)) {
                colliding = true;
                spawnedVirus[i]->ms -= spawnedVirus[i]->ms;
                break;
            }
        }
        if (!colliding && !spawnedVirus[i]->ppg_effect) {       // return speed to normal if not colliding with platelets
            spawnedVirus[i]->ms = spawnedVirus[i]->original_ms;
        }

    }

}

// Update countdown for platelets
void update_platelets() {        
    // Deplete health by frame, if health == 0, remove platelet
    static int frame_count{};
    for (int i{}; i < sz; ++i) {
        if (platelets[i].xy.x != reset_coor) {  // if platelets on map
            switch (i) {     
            // Different deplation rates for each platelet
            case 0:
                if (!(frame_count % frame_count1)) {
                    platelets[i].curr_health -= deplete1;
                }
                break;
            case 1:
                if (!(frame_count % frame_count2)) {
                    platelets[i].curr_health -= deplete2;
                }
                break;
            case 2:
                if (!(frame_count % frame_count3)) {
                    platelets[i].curr_health -= deplete3;
                }
                break;
            }
            if (platelets[i].curr_health < 0.f) platelets[i].curr_health -= platelets[i].curr_health;
        }

    }
    
    // Reset coordinates if health <= 0
    for (int i{}; i < sz; ++i) {
        if (platelets[i].curr_health <= NULL) {
            platelets[i].xy.x = reset_coor;
            platelets[i].xy.y = reset_coor;
        }
    }
    frame_count++;
}

// Free platelets meshes
void free_platelets_mesh() {
    if (platelets_mesh) AEGfxMeshFree(platelets_mesh); platelets_mesh = nullptr;
    if (platelets_hp_bmesh) AEGfxMeshFree(platelets_hp_bmesh); platelets_hp_bmesh = nullptr;
    if (platelets_hp_fmesh) AEGfxMeshFree(platelets_hp_fmesh); platelets_hp_fmesh = nullptr;
}


