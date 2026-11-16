/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      spawncontroller.cpp
*@author(s)     Ethan Ng (Primary Author, 100%)
*@DP emails     n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the logic for the spawning, selection and rendering of
*               virus options and spawnpoints in the maps for the game
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
#include "spawncontroller.h"

static int hfmd_cost{};
static int dengue_cost{};
static int chicken_pox_cost{};
static int polio_cost{};
static int ebola_cost{};
static int def_cost{};
static float ebola_x{};
static float ebola_y{};
static float f_spawnpoint_x {};
static float f_spawnpoint_y {};
static float f_spawnpoint_y2 {};
static float l3_spawnpoint_y{};
static float l3_spawnpoint_y2{};
static float fspawnpoint_height{};
static float selection_x {};
static float selection_y {};
static float selectrect_height{};
static float default_scale{};
static unsigned int orange{};
static unsigned int red{};
static unsigned int brown{};
static unsigned int pink{};
static unsigned int blue{};
static unsigned int green{};

AEGfxVertexList* spawn_mesh{};
AEGfxVertexList* selection_mesh{};
AEGfxVertexList* selection_mesh1{};
AEGfxVertexList* selection_mesh2{};
AEGfxVertexList* selection_mesh3{};
AEGfxVertexList* selection_mesh4{};
AEGfxVertexList* selection_POLIO_mesh{}; //Selection for polio

AEMtx33 spawn_transform[2]{};
AEMtx33 spawn_scale,  spawn_rotate,  spawn_translate;

AEMtx33 selection_transform[6] = {};
AEMtx33 selection_scale,  selection_rotate,  selection_translate;

// Load data from json file
void load_data_spawn() {
    std::fstream data("Assets/Data/spawncontroller.json");
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
    hfmd_cost = doc["hfmd_cost"].GetInt();
    dengue_cost = doc["dengue_cost"].GetInt();
    chicken_pox_cost = doc["chickenpox_cost"].GetInt();
    polio_cost = doc["polio_cost"].GetInt();
    ebola_cost = doc["ebola_cost"].GetInt();
    ebola_x = doc["ebola_x"].GetFloat();
    ebola_y = doc["ebola_y"].GetFloat();
    f_spawnpoint_x = doc["f_spawnpoint_x"].GetFloat();
    f_spawnpoint_y = doc["f_spawnpoint_y"].GetFloat();
    f_spawnpoint_y2 = doc["f_spawnpoint_y2"].GetFloat();
    l3_spawnpoint_y = doc["l3_spawnpoint_y"].GetFloat();
    l3_spawnpoint_y2 = doc["l3_spawnpoint_y2"].GetFloat();
    selection_x = doc["selection_x"].GetFloat();
    selection_y = doc["selection_y"].GetFloat();
    selectrect_height = doc["selectrect_height"].GetFloat();
    fspawnpoint_height = doc["fspawnpoint_height"].GetFloat();
    default_scale = doc["default_scale"].GetFloat();
    orange = static_cast<unsigned int>(doc["orange"].GetUint64());
    red = static_cast<unsigned int>(doc["red"].GetUint64());
    brown = static_cast<unsigned int>(doc["brown"].GetUint64());
    pink = static_cast<unsigned int>(doc["pink"].GetUint64());
    blue = static_cast<unsigned int>(doc["blue"].GetUint64());
    green = static_cast<unsigned int>(doc["green"].GetUint64());
}

//Spawnpoint initialization
void spawnpoint_init() {
    Create_Rectangle(&spawn_mesh, fspawnpoint_height, fspawnpoint_height, green, spawn_transform, fspawnpoint_height, fspawnpoint_height, f_spawnpoint_x, f_spawnpoint_y);
}

//Spawnpoint rendering
void spawnpoint_render() {
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(spawn, {}, {});
    render(&spawn_scale, &spawn_rotate, &spawn_translate, &spawn_transform[0], default_scale, default_scale, f_spawnpoint_x, f_spawnpoint_y);
    AEGfxSetTransform(spawn_transform[select_0].m);
    AEGfxMeshDraw(spawn_mesh, AE_GFX_MDM_TRIANGLES);

    render(&spawn_scale, &spawn_rotate, &spawn_translate, &spawn_transform[1], default_scale, default_scale, f_spawnpoint_x, f_spawnpoint_y2);
    AEGfxSetTransform(spawn_transform[select_1].m);
    AEGfxMeshDraw(spawn_mesh, AE_GFX_MDM_TRIANGLES);
}

//Spawnpoint initialization for lvl3 
void spawnpoint_lvl3_init() {
    Create_Rectangle(&spawn_mesh, fspawnpoint_height, fspawnpoint_height, green, spawn_transform, fspawnpoint_height, fspawnpoint_height, f_spawnpoint_x, f_spawnpoint_y);
}

//Spawnpoint rendering for lvl3
void spawnpoint_lvl3_render() {
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(spawn, {}, {});
    render(&spawn_scale, &spawn_rotate, &spawn_translate, &spawn_transform[0], default_scale, default_scale, f_spawnpoint_x, l3_spawnpoint_y);
    AEGfxSetTransform(spawn_transform[select_0].m);
    AEGfxMeshDraw(spawn_mesh, AE_GFX_MDM_TRIANGLES);

    render(&spawn_scale, &spawn_rotate, &spawn_translate, &spawn_transform[1], default_scale, default_scale, f_spawnpoint_x, l3_spawnpoint_y2);
    AEGfxSetTransform(spawn_transform[select_1].m);
    AEGfxMeshDraw(spawn_mesh, AE_GFX_MDM_TRIANGLES);
}

//Level 1 selector initialization
void level1_selector_init() {
    Create_Rectangle(&selection_mesh, selectrect_height, selectrect_height, orange, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh1, selectrect_height, selectrect_height, red, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh2, selectrect_height, selectrect_height, brown, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh3, selectrect_height, selectrect_height, pink, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
}

//Level 1 selector rendering
void level1_selector_render() {
    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[0], default_scale, default_scale, selection_x, selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_0].m);
    AEGfxTextureSet(hfmd_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);


    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[2], default_scale, default_scale, selection_x + (-selection_y), selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_2].m);
    AEGfxTextureSet(dengue_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh2, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}

//Level 3 selector initialization
void level3_selector_init() {
    Create_Rectangle(&selection_mesh, selectrect_height, selectrect_height, orange, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh1, selectrect_height, selectrect_height, red, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh2, selectrect_height, selectrect_height, brown, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh3, selectrect_height, selectrect_height, pink, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_POLIO_mesh, selectrect_height, selectrect_height, blue, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh4, selectrect_height, selectrect_height, pink, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
}

//Level 3 selector rendering
void level3_selector_render() {
    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[0], default_scale, default_scale, selection_x, selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_0].m);
    AEGfxTextureSet(hfmd_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);


    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[2], default_scale, default_scale, selection_x + (-selection_y), selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_2].m);
    AEGfxTextureSet(dengue_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh2, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    //Render rectangle box(yellow) for spawning polio
    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[5], default_scale, default_scale, selection_x + (-selection_y*2), selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_5].m);
    AEGfxTextureSet(polio_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_POLIO_mesh, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    //Render rectangle box(yellow) for spawning polio
}

//Level 5 selector intialization
void level5_selector_init() {
    Create_Rectangle(&selection_mesh, selectrect_height, selectrect_height, orange, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh1, selectrect_height, selectrect_height, red, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh2, selectrect_height, selectrect_height, brown, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh3, selectrect_height, selectrect_height, pink, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    //Create Rectangle box for polio(yellow color)
    Create_Rectangle(&selection_POLIO_mesh, selectrect_height, selectrect_height, blue, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
    Create_Rectangle(&selection_mesh4, selectrect_height, selectrect_height, pink, selection_transform, selectrect_height, selectrect_height, selection_x, selection_y);
}

//Level 5 selector rendering
void level5_selector_render() {
    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[0], default_scale, default_scale, selection_x, selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_0].m);
    AEGfxTextureSet(hfmd_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);


    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[2], default_scale, default_scale, selection_x + (-selection_y), selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_2].m);
    AEGfxTextureSet(dengue_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh2, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    //Render rectangle box(yellow) for spawning polio
    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[5], default_scale, default_scale, selection_x + (-selection_y*2), selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_5].m);
    AEGfxTextureSet(polio_texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_POLIO_mesh, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[4], default_scale, default_scale, selection_x + (-selection_x), selection_y);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransform(selection_transform[select_4].m);
    AEGfxTextureSet(ebola_Texture, default_scale, default_scale);
    AEGfxMeshDraw(selection_mesh4, AE_GFX_MDM_TRIANGLES);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

}

//Virus selection logic
void virus_Select(int *selected, int virus_type, int mousex, int mousey, float selectionX, float selectionY, float height, float width, int is_clicked, std::vector<virus*>& spawned_viruses, int* resource, int* cost) {
    if (is_mouse_intersecting_rectangle_clicked(static_cast<float>(mousex), static_cast<float>(mousey), selectionX, selectionY, height, width, is_clicked)) {
        *selected = virus_type;
        if (virus_type == ebola) {
            virus* new_Virus;
            new_Virus = spawnVirus(*selected, ebola_x, ebola_y, spawned_viruses);
            *resource -= *cost;
        }
    }
}

//Spawner selection logic
void spawner_Select(int selected, int mousex, int mousey, float spawn_pointx, float spawn_pointy, float height, 
    float width, std::vector<virus*>& spawned_viruses, int is_clicked, int *resource, int *cost) {
    if (is_mouse_intersecting_rectangle_clicked(static_cast<float>(mousex), static_cast<float>(mousey), spawn_pointx, spawn_pointy, height, width, is_clicked) && selected != select_0 && *resource >= *cost) {
        if (chickenpox_Defeated == true && selected == chickenpox) {
            selected = shingle;
        }
        if (selected == chickenpox || selected == dengue || selected == shingle || selected == hfmdvirus || selected == polio_virus) {
            virus* new_Virus;
            new_Virus = spawnVirus(selected, spawn_pointx, spawn_pointy, spawned_viruses);
            *resource -= *cost;
        }
    }
}

//Virus price list
int virus_price(int* cost, int selected) {
    switch (selected) {
    case hfmdvirus:
        *cost = hfmd_cost;
        break;
    case dengue:
        *cost = dengue_cost;
        break;
    case chickenpox:
        *cost = chicken_pox_cost;
        break;
    case shingle:
        *cost = chicken_pox_cost;
        break;
    case polio_virus:
        *cost = polio_cost;
        break;
    case ebola:
        *cost = ebola_cost;
        break;
    default:
        *cost = select_0;
        break;
    }
    return *cost;
}

//Resource/viral load limiter
void resource_controller(int* resource) {
    if (*resource < select_0) {
        *resource = select_0;
    }
}

//Rendering for shingles option, due to chicken pox ability
void render_shingle_select(int chickenpox_Defeat, int* selected, int mousex, int mousey, std::vector<virus*>& spawned_viruses, int *resource, int *cost) {
    // If a chickenpox virus has been defeated, change to shingle
    if (chickenpox_Defeat) {
        render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[select_3], default_scale, default_scale, selection_x + l3_spawnpoint_y2, selection_y);
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetTransform(selection_transform[select_3].m);
        AEGfxTextureSet(shingles_texture, default_scale, default_scale);
        AEGfxMeshDraw(selection_mesh3, AE_GFX_MDM_TRIANGLES);
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        virus_Select(selected, shingle, mousex, mousey, selection_x + l3_spawnpoint_y2, selection_y, f_spawnpoint_y2, f_spawnpoint_y2, AEInputCheckTriggered(AEVK_LBUTTON), spawned_viruses, resource, cost);
    }
    else if (!chickenpox_Defeat) {
        render(&selection_scale, &selection_rotate, &selection_translate, &selection_transform[select_1], default_scale, default_scale, selection_x + l3_spawnpoint_y2, selection_y);
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetTransform(selection_transform[select_1].m);
        AEGfxTextureSet(chickenpox_texture, default_scale, default_scale);
        AEGfxMeshDraw(selection_mesh1, AE_GFX_MDM_TRIANGLES);
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        virus_Select(selected, chickenpox, mousex, mousey, selection_x + l3_spawnpoint_y2, selection_y, f_spawnpoint_y2, f_spawnpoint_y2, AEInputCheckTriggered(AEVK_LBUTTON), spawned_viruses, resource, cost);
    }
}

//Yu Xuan's code for automatically spawning minion polio when countdown timer for spawning minion polio == 0
void Auto_spawner(float spawn_pointx, float spawn_pointy, std::vector<virus*>& spawned_viruses) {
    virus* new_Virus;
    new_Virus = spawnVirus(minion_polio_virus, spawn_pointx, spawn_pointy, spawned_viruses);
}

//Freeing of spawner meshes
void free_spawner() {
    if (spawn_mesh) {
        AEGfxMeshFree(spawn_mesh);
        spawn_mesh = nullptr;
    }
}

//Freeing of selector meshes
void free_selectors() {
    if (selection_mesh) {
        AEGfxMeshFree(selection_mesh);
        selection_mesh = nullptr;
    }
    if (selection_mesh1) {
        AEGfxMeshFree(selection_mesh1);
        selection_mesh1 = nullptr;
    }
    if (selection_mesh2) {
        AEGfxMeshFree(selection_mesh2);
        selection_mesh2 = nullptr;
    }
    if (selection_mesh3) {
        AEGfxMeshFree(selection_mesh3);
        selection_mesh3 = nullptr;
    }
    if (selection_mesh4) {
        AEGfxMeshFree(selection_mesh4);
        selection_mesh4 = nullptr;
    }
    if (selection_POLIO_mesh) {
        AEGfxMeshFree(selection_POLIO_mesh);
        selection_POLIO_mesh = nullptr;
    }
}

void free_bullets(AEGfxVertexList** tbullet_mesh, AEGfxVertexList** vbullet_mesh) {
    if (*tbullet_mesh) {
        AEGfxMeshFree(*tbullet_mesh);
        *tbullet_mesh = nullptr;
    }
    if (*vbullet_mesh) {
        AEGfxMeshFree(*vbullet_mesh);
        *vbullet_mesh = nullptr;
    }
}