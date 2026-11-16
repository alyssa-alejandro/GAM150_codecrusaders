/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      virulencementer.cpp
*@author(s)     Ethan Ng (Primary Author, 90%)
*               Chan Qi Ying (Secondary Author, 10%)
*@DP emails     n.ethanyongle@digipen.edu
*               q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains the definitions for the functionalities and
*               rendering of the virulence meter in game
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "AEEngine.h"
#include "collision.h"
#include <iostream>
#include "virus.h"
#include "textureselect.h"
#include "circle.h"
#include "rectangle.h"
#include "level.h"


static float bar_x{};
static float bar_y{};
static float bar_height{};
static float bar_width{};
static float start_width{};
static float bar_scale_x{};
static float empty_scale_y{};
static float health_scale_y{};
static unsigned int red{};
static unsigned int gray{};
static float width{};
static float length_modifier{};
static float default_x_coord{};
static float render_scale_x{};
static float render_scale_y{};
static float health_x_modifier{};
AEGfxVertexList* health_mesh{};
AEGfxVertexList* empty_mesh{};
AEMtx33 empty_transform[1]{};
AEMtx33 health_transform[1]{};

AEMtx33 empty_scale,  empty_rotate,  empty_translate;
AEMtx33 meter_scale,  meter_rotate,  meter_translate;

// Load data from json file
void load_data_virulence_meter() {
    std::fstream data("Assets/Data/virulencemeter.json");
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
    bar_x = doc["bar_x"].GetFloat();
    bar_y= doc["bar_y"].GetFloat();
    bar_height = doc["bar_height"].GetFloat();
    bar_width = doc["bar_width"].GetFloat();
    start_width = doc["start_width"].GetFloat();
    bar_scale_x = doc["bar_scale_x"].GetFloat();
    empty_scale_y = doc["empty_scale_y"].GetFloat();
    health_scale_y = doc["health_scale_y"].GetFloat();
    length_modifier = doc["length_modifier"].GetFloat();
    red = static_cast<unsigned int>(doc["red"].GetUint64());
    gray = static_cast<unsigned int>(doc["gray"].GetUint64());
    default_x_coord = doc["default_x_coord"].GetFloat();
    render_scale_x = doc["render_scale_x"].GetFloat();
    render_scale_y = doc["render_scale_y"].GetFloat();
    health_x_modifier = doc["health_x_modifier"].GetFloat();
}

//Virulence Meter Initialization
void virulence_meter_init(int *meter) {
	Create_Rectangle(&empty_mesh, bar_width, bar_height, gray, empty_transform, bar_scale_x, empty_scale_y, bar_x, bar_y);
	Create_Rectangle(&health_mesh, start_width, bar_height, red, health_transform, bar_scale_x, health_scale_y, static_cast<float>(*meter), bar_y);
}

//Virulence Meter Rendering
void virulence_meter_render(int* meter) {
    render(&empty_scale, &empty_rotate, &empty_translate, &empty_transform[0], render_scale_x, render_scale_y, default_x_coord, bar_y);
    AEGfxSetTransform(empty_transform[0].m);
    AEGfxMeshDraw(empty_mesh, AE_GFX_MDM_TRIANGLES);
    
    width = static_cast<float>(*meter) / length_modifier * AEGfxGetWindowWidth();
    render(&meter_scale, &meter_rotate, &meter_translate, &health_transform[0], width, render_scale_y, (width / health_x_modifier) + AEGfxGetWinMinX(), bar_y);
    AEGfxSetTransform(health_transform[0].m);
    AEGfxMeshDraw(health_mesh, AE_GFX_MDM_TRIANGLES);
}

//Freeing of virulence meter meshes
void free_virulence_meter() {
    if (health_mesh) {
        AEGfxMeshFree(health_mesh);
        health_mesh = NULL;
    }
    if (empty_mesh) {
        AEGfxMeshFree(empty_mesh);
        empty_mesh = NULL;
    }
}