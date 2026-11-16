/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      guidepage.cpp
*@author(s)     Ethan Ng (Primary Author, 70%)
*               Ang Yu Xuan (Secondary Author, 30%)
*@DP emails     n.ethanyongle@digipen.edu
*               yuxuan.ang@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the rendering and functionalities for the guidepage
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include <iostream>
#include "level.h"
#include "collision.h"
#include "AEEngine.h"
#include "rectangle.h"
#include "textureselect.h"
#include <stdio.h>
#include "guidepage.h"
#include "countdown.h"
#include "MainMenu.h"
#include "mesh.h"
#include "gamestatemanager.h"

// Define the background and buttons
AEGfxVertexList* gbg_Mesh = nullptr;
AEGfxVertexList* buttons_Mesh = nullptr;
AEGfxVertexList* buttons_Mesh2 = nullptr;
AEGfxVertexList* buttons_Mesh3 = nullptr;
AEMtx33 button_transform[3];
AEMtx33 gbg_Transform;
int mx{};
int my{};
static float box_width{};
static float box_height{};
static float exit_box_width{};
static float exit_box_height{};
static float default_set{};
static float background_width{};
static float background_height{};
static float left_button_pos_x{};
static float right_button_pos_x{};
static float exit_button_pos_x{};
static float button_pos_y{};
static float zero_color{};
static float zero_offset{};
static float default_center{};
static unsigned int white{};
static unsigned int pink{};
static std::string main_menu_text{};
static std::string return_text{};
static int page{};
static float float_half{};
bool pause_guide_page{};
float button_off_cordx{};
float button_off_cordy{};
unsigned int black{};

//Set guidepage(for gsm)
void set_guidepage() {
    update_guidepage();
}

// Load data from json file
void load_data_guide() {
    std::fstream data("Assets/Data/guidepage.json");
    if (!data) {
        gGameRunning = 0;
    }

    rapidjson::IStreamWrapper isw(data);
    rapidjson::Document doc;

    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        gGameRunning = 0;
        return; // Exit the function on parse error
    }

    // Assigning data from file to variable
    box_width = doc["box_width"].GetFloat();
    box_height = doc["box_height"].GetFloat();
    exit_box_width = doc["exit_box_width"].GetFloat();
    exit_box_height = doc["exit_box_height"].GetFloat();
    default_set = doc["default_set"].GetFloat();
    background_width = doc["background_width"].GetFloat();
    background_height = doc["background_height"].GetFloat();
    left_button_pos_x = doc["left_button_pos_x"].GetFloat();
    right_button_pos_x = doc["right_button_pos_x"].GetFloat();
    exit_button_pos_x = doc["exit_button_pos_x"].GetFloat();
    button_pos_y = doc["button_pos_y"].GetFloat();
    white = static_cast<unsigned int>(doc["white"].GetUint64());
    pink = static_cast<unsigned int>(doc["pink"].GetUint64());
    zero_color = doc["zero_color"].GetFloat();
    zero_offset = doc["zero_offset"].GetFloat();
    default_center = doc["default_center"].GetFloat();
    float_half = doc["float_half"].GetFloat();
    button_off_cordx = doc["button_off_cordx"].GetFloat();
    button_off_cordy = doc["button_off_cordy"].GetFloat();
    black = doc["black"].GetUint();
    main_menu_text = doc["main_menu_text"].GetString();
    return_text = doc["return_text"].GetString();
}

// Function to initialize the upgrade tree page
void init_guidepage() {
    // Background mesh

    if (!background_texture) background_texture = AEGfxTextureLoad("Assets/MENU.jpg");
    if (!bg_mesh) bg_mesh = Draw_Square_Mesh(black);

    //Page
    page = page_0;
    Create_Rectangle(&buttons_Mesh, box_width, box_height, white, &button_transform[0], default_set, default_set, right_button_pos_x, button_pos_y);
    Create_Rectangle(&buttons_Mesh2, box_width, box_height, white, &button_transform[1], default_set, default_set, left_button_pos_x, button_pos_y);
    Create_Rectangle(&buttons_Mesh3, exit_box_width, exit_box_height, white, &button_transform[2], default_set, default_set, exit_button_pos_x, button_pos_y);
    Create_Rectangle(&gbg_Mesh, background_width, background_height, pink, &gbg_Transform, default_set, default_set, default_center, default_center);
}

// Function to render the upgrade tree page
void update_guidepage() {
    // Set the background to white.
    AEGfxSetBackgroundColor(default_set, default_set, default_set);
    AEInputGetCursorPosition(&mx, &my);
    // Tell the engine to get ready to draw something with texture.
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(default_set, default_set, default_set, default_set);
    // Set the color to add to nothing, so that we don't alter the sprite's color
    AEGfxSetColorToAdd(zero_color, zero_color, zero_color, zero_color);

    //Page handling logic
    switch (page) {
    case page_0:
        AEGfxTextureSet(guide_1, zero_offset, zero_offset);
        break;
    case page_1:
        AEGfxTextureSet(guide_2, zero_offset, zero_offset);
        break;
    case page_2:
        AEGfxTextureSet(guide_3, zero_offset, zero_offset);
        break;
    case page_3:
        AEGfxTextureSet(guide_4, zero_offset, zero_offset);
        break;
    case page_4:
        AEGfxTextureSet(guide_5, zero_offset, zero_offset);
        break;
    default:
        page = page_0;
        AEGfxTextureSet(guide_1, zero_offset, zero_offset);
        break;
    }
    Draw_Rectangle_Shape(bg_mesh, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), {}, {});		// here

    if (page < page_4) {
        if (is_mouse_intersecting_rectangle_clicked(static_cast<float>(mx), static_cast<float>(my), right_button_pos_x, button_pos_y, box_width, box_height, AEInputCheckTriggered(AEVK_LBUTTON))) {
            page++;
        }
        AEGfxSetTransform(button_transform[page_0].m);
        AEGfxTextureSet(arrow, zero_offset, zero_offset); //1,1
        AEGfxMeshDraw(buttons_Mesh, AE_GFX_MDM_TRIANGLES);
    }

    AEGfxSetTransform(button_transform[page_1].m);
    AEGfxTextureSet(arrow_2, zero_offset, zero_offset);
    AEGfxMeshDraw(buttons_Mesh2, AE_GFX_MDM_TRIANGLES);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransform(button_transform[page_2].m);
    AEGfxMeshDraw(buttons_Mesh3, AE_GFX_MDM_TRIANGLES);
    if (previous_State == STATE_GUIDEPAGE)
        Render_Text(main_menu_text, font, {}, -button_off_cordy / (AEGfxGetWindowHeight() / static_cast<float>(page_2)));
    else
        Render_Text(return_text, font, {}, -button_off_cordy / (AEGfxGetWindowHeight() / static_cast<float>(page_2)));
    if (is_mouse_intersecting_rectangle_clicked(static_cast<float>(mx), static_cast<float>(my), -button_off_cordx, -button_off_cordy, box_width, box_height, AEInputCheckTriggered(AEVK_LBUTTON))) {
        if (page == page_0) {
            if (previous_State == STATE_GUIDEPAGE) GSM_ChangeState(STATE_MAINMENU);
            else { 
                pause_guide_page = false;  
                free_guidepage();
                return;
            }
        }
        else if(page>page_0) {
            page--;
        }
    }

    if (is_mouse_intersecting_rectangle_clicked(static_cast<float>(mx), static_cast<float>(my), {}, -button_off_cordy, exit_box_width, exit_box_height, AEInputCheckTriggered(AEVK_LBUTTON))) {
        if (previous_State == STATE_GUIDEPAGE) GSM_ChangeState(STATE_MAINMENU);
        else {
            pause_guide_page = false;
            free_guidepage();
            return;
        }
    }

}

// Function to free allocated resources
void free_guidepage() {
    if (gbg_Mesh) {
        AEGfxMeshFree(gbg_Mesh);
        gbg_Mesh = nullptr; // Ensure it doesn't get freed again
    }

    if (buttons_Mesh) {
        AEGfxMeshFree(buttons_Mesh);
        buttons_Mesh = nullptr; // Ensure it doesn't get freed again
    }

    if (buttons_Mesh2) {
        AEGfxMeshFree(buttons_Mesh2);
        buttons_Mesh2 = nullptr; // Ensure it doesn't get freed again
    }

    if (buttons_Mesh3) {
        AEGfxMeshFree(buttons_Mesh3);
        buttons_Mesh3 = nullptr; // Ensure it doesn't get freed again
    }
    

}
