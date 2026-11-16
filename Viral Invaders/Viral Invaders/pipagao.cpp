/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      pipagao.cpp
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains definitions for functions to create, update, render and free
*				pipagao event related items.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "AEEngine.h"
#include "pipagao.h"
#include "collision.h"
#include "virus.h"
#include "events.h"
#include "level.h"
#include "circle.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<PiPaGao> ppg;
static int sz{};

AEGfxVertexList* pipagao_mesh{};
AEMtx33 pipagao_transfmtx;

int circle_segments{}, duration{}, spd_multiplier{};
double end{};
bool ppg_event = false;       // pipagao event indicator  

static float reset_coor{}, radius{}, v_radius{};
unsigned int color{};

// Load data
void load_data_ppg() {
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
    sz = doc["sz"].GetInt();
    end = doc["end"].GetDouble();
    radius = doc["radius"].GetFloat();
    v_radius = doc["v_radius"].GetFloat();
    color = std::stoul(doc["ppg_color"].GetString(), nullptr, 16);
    reset_coor = doc["reset_coor"].GetFloat();
    circle_segments = doc["segment"].GetInt();
    duration = doc["max_time"].GetInt();
    spd_multiplier = doc["spd_multiplier"].GetInt();
}


// Initialise pipagao mesh and coordinates
void init_ppg() {
    if (pipagao_mesh == NULL)
        create_circle_mesh(&pipagao_mesh, radius, circle_segments, color, color);
    ppg.clear();
    for (int i{}; i < sz; ++i) {
        PiPaGao new_ppg;
        new_ppg.xy.x = reset_coor;
        new_ppg.xy.y = reset_coor;
        ppg.push_back(new_ppg);
    }
}

// Update pipagao and check for collision 
void update_ppg(std::vector<virus*> spawned_virus, int square_columns, int square_rows, float cell_length) {
    static double time_before_end{};
    if (triggered_event == pipagao || AEInputCheckTriggered(AEVK_Z)) {        // check for pipagao event trigger
        time_before_end = duration;
        coor_ppg(square_columns, square_rows, cell_length);     
        ppg_event = true;

    }

    //check collision with virus
    collision_ppg(spawned_virus);

    triggered_event = no_event;

    // Update pipagao timing
    switch (ppg_event) {        // check if pipagao event is happening currently
    case true:
        time_before_end -= AEFrameRateControllerGetFrameTime();
        if (time_before_end <= end) {
            time_before_end = duration;
            ppg_event = false;
        }
        break;
    case false: // reset pipagao coordinates
        for (int i{}; i < sz; ++i) {
            ppg[i].xy.x = reset_coor;
            ppg[i].xy.y = reset_coor;
        }
        break;
    }
}

// Render pipagao circles
void render_ppg() {
    // To render if pipagao evetn is currently occuring
    if (ppg_event) {
        for (int i{}; i < sz; ++i) {
            AEMtx33Trans(&pipagao_transfmtx, ppg[i].xy.x, ppg[i].xy.y);

            AEGfxSetRenderMode(AE_GFX_RM_COLOR);
            AEGfxSetTransform(pipagao_transfmtx.m);
            AEGfxMeshDraw(pipagao_mesh, AE_GFX_MDM_TRIANGLES);
        }
    }
}

// Assign coordinates to pipgagao
void coor_ppg(int square_columns, int square_rows, float cell_length) {  

    for (int i{}; i < sz; ++i) {
        AEVec2 pcoor = random_coor_generator(square_columns, square_rows, cell_length);
        for (int j{}; j < i; ++j)
            // make sure coordinates are not the same
            while (pcoor.x == ppg[j].xy.x || pcoor.y == ppg[j].xy.y) pcoor = random_coor_generator(square_columns, square_rows, cell_length);

        ppg[i].xy = pcoor;
    }
}

// Check for collision of virus with pipagao
void collision_ppg(std::vector<virus*> spawned_virus) {      // if virus collide with pipagao, slow them  
    for (size_t i{}; i < spawned_virus.size(); ++i) {        // virus
        bool colliding = false;
        for (size_t j{}; j < sz; ++j) {    // pipagao
            // if colliding with pipgao, slow virus
            if (are_circles_intersecting(spawned_virus[i]->x, spawned_virus[i]->y, v_radius, ppg[j].xy.x, ppg[j].xy.y, radius)) {
                colliding = true;
                if (!(spawned_virus[i]->ppg_effect)) { // set effect for the first time
                    spawned_virus[i]->ms /= spd_multiplier;
                    spawned_virus[i]->ppg_effect = true;
                    break;
                }
                if (spawned_virus[i]->ppg_effect) {   // set speed to slow after virus kills a tower
                    if (spawned_virus[i]->ms != spawned_virus[i]->original_ms && spawned_virus[i]->attackstate == false) {
                        spawned_virus[i]->ms = spawned_virus[i]->original_ms /spd_multiplier;
                    }
                }
            }
        }
        if (!colliding && spawned_virus[i]->ppg_effect) {  // return virus speed to normal
            spawned_virus[i]->ms *= spd_multiplier;
            spawned_virus[i]->ppg_effect = false;
        }
    }

}

// Free meshes for pipagao
void free_ppg_mesh() {
    if (pipagao_mesh)
        AEGfxMeshFree(pipagao_mesh);
    pipagao_mesh = nullptr;

}


