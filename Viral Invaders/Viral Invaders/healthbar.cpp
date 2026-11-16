/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      healthbar.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 50%)
*               Dalton Koh (Secondary Author, 50%)
*@DP emails     alejandro.a@digipen.edu
*               d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines all Hp_bar related functions for both entities,
*               towers and viruses. All related initializing, logic, rendering
*               and update functions are defined here.
******************************************************************************/


#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <sstream>
#include "textureselect.h"

#include "AEEngine.h"
#include "healthbar.h"
#include "rectangle.h"
#include <algorithm> // For fmax and fmin
#include <iostream>
#include "level.h"
#define MAXHPBARS 1500


struct HP_Bar hp_Bars[MAXHPBARS];

// Health bar variables
static float left_Aligned_X{};
static float speed{};

// Scaling required to run scale matrix or function specific scaling
static float current_Bar_Scale {}; 
static bool scale_Left_To_Right {}; 

// static 
static float hp_Bar_Width {};     
static float hp_Bar_Height {};   

//offset to y cords
static float off_set_y {};

// Colour backgrounds 
static unsigned int grey_background {};
static unsigned int red_background {};

// Index offsets
static int index_offset {};
static int virus_HP_Index{};
static float cord_x_offset_1{};
static float cord_x_offset_2{};
static float cord_x_offset_3{};

// Loading all required data from credits.json
void load_data_hpbar() {

    // If file fails to open
    std::fstream file("Assets/Data/hpbar.json");
    if (!file) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(file); // Wrap file stream into something rapid json can read from 
    rapidjson::Document doc; // Create json object
    doc.ParseStream(isw); // Parses contents from the isw stream into doc. so you can access it later on

    // If parse error or error getting the right data type
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return;
    }
    
    // Required data to be retrieved 
    current_Bar_Scale = doc["current_Bar_Scale"].GetFloat();
    scale_Left_To_Right = doc["scale_Left_To_Right"].GetBool();
    hp_Bar_Width = doc["hp_Bar_Width"].GetFloat();
    hp_Bar_Height = doc["hp_Bar_Height"].GetFloat();
    off_set_y = doc["off_set_y"].GetFloat();
    grey_background = doc["grey_background"].GetUint();
    red_background = doc["red_background"].GetUint();
    virus_HP_Index = doc["virus_HP_Index"].GetInt();
    index_offset = doc["index_offset"].GetInt();
    cord_x_offset_1 = doc["cord_x_offset_1"].GetFloat();
    cord_x_offset_2 = doc["cord_x_offset_2"].GetFloat();
    cord_x_offset_3 = doc["cord_x_offset_3"].GetFloat();

}

// Initializing specific data to create rectangles for the hp bars
void make_hp(size_t index, float x, float y, float interp_Speed, bool is_Virus) {
    if (index < 0 || index >= MAXHPBARS) return;

    if (is_Virus) {
        index = virus_HP_Index++;    // Assign a separate index for virus HP bars
    }

    // Positions, scaling and interpolation speed 
    hp_Bars[index].pos_X = x;
    hp_Bars[index].pos_Y = y + off_set_y; // Ensure both bars start at the same Y position
    hp_Bars[index].current_Scale = current_Bar_Scale;
    hp_Bars[index].target_Scale = current_Bar_Scale;
    hp_Bars[index].interp_Speed = interp_Speed;

    // Create Background Bar First 
    Create_Rectangle(
        &hp_Bars[index].bg_Mesh,       // Background grey mesh
        hp_Bar_Width, hp_Bar_Height,     
        grey_background,                 
        &hp_Bars[index].bg_Transform,
        hp_Bars[index].current_Scale, hp_Bars[index].current_Scale,
        x, y                           
    );

    // Create Red HP Bar on Top
    Create_Rectangle(
        &hp_Bars[index].bar_Mesh,      // Front red bar
        hp_Bar_Width, hp_Bar_Height,    
        red_background,                 
        &hp_Bars[index].transform,
        hp_Bars[index].current_Scale, hp_Bars[index].current_Scale,
        x, y                        
    );
}

// To run hp logics for virus
void update_hp_V(int index, float new_Hp_Percentage, float delta_Time, float virus_X, float virus_Y) {
    if (index < NULL || index >= MAXHPBARS) return; // Safety check - prevent invalid indexes

    // Virus index differ 
    index += index_offset;

    // To free any dead virus hp bars
    for (int i = index; i < MAXHPBARS; i++) {
        if (new_Hp_Percentage <= NULL) {
            if (hp_Bars[index].bg_Mesh) {  // Check if background mesh exists5
                AEGfxMeshFree(hp_Bars[index].bg_Mesh);
                hp_Bars[index].bg_Mesh = nullptr;
            }
            if (hp_Bars[index].bar_Mesh) {  // Check if HP bar mesh exists
                AEGfxMeshFree(hp_Bars[index].bar_Mesh);
                hp_Bars[index].bar_Mesh = nullptr;
            }
        }
    }


    // Update HP bar and background position to follow virus
    if (index < MAXHPBARS) {
        hp_Bars[index].pos_X = virus_X;
        hp_Bars[index].pos_Y = virus_Y; // Keep both bars at the same Y offset

        // Clamp HP percentage
        hp_Bars[index].target_Scale = static_cast<float>(fmax( {}, fmin(new_Hp_Percentage, cord_x_offset_1)));

        // Smooth interpolation
        speed = hp_Bars[index].interp_Speed * delta_Time;
        hp_Bars[index].current_Scale += (hp_Bars[index].target_Scale - hp_Bars[index].current_Scale) * speed;



        // Apply Scaling for Red HP Bar
        AEMtx33 rect_Scale, rect_Translate;
        AEMtx33Scale(&rect_Scale, hp_Bars[index].current_Scale, current_Bar_Scale);

        // Move HP Bar Correctly to Align with Background 
        left_Aligned_X = hp_Bars[index].pos_X - (hp_Bar_Width * (cord_x_offset_1 - hp_Bars[index].current_Scale)) * cord_x_offset_2;
        AEMtx33Trans(&rect_Translate, left_Aligned_X, hp_Bars[index].pos_Y - cord_x_offset_3);
      
        // Apply Final Transformation
        AEMtx33Concat(&hp_Bars[index].transform, &rect_Translate, &rect_Scale);

        // Ensure Background Always Stays in the Same Position
        AEMtx33 bg_Translate;
        AEMtx33Trans(&bg_Translate, hp_Bars[index].pos_X, hp_Bars[index].pos_Y - cord_x_offset_3);
        hp_Bars[index].bg_Transform = bg_Translate;
    }
}

// To run hp logics for Towers (white blood cells)
void update_hp_T(int index, float new_Hp_Percentage, float delta_Time, float virus_X, float virus_Y) {
    if (index < NULL || index >= MAXHPBARS) return; // Safety check

    // To free any dead tower hp bars
    for (int i = index; i < MAXHPBARS; i++) {
        if (new_Hp_Percentage <= NULL) {
            if (hp_Bars[index].bg_Mesh) {  // Check if background mesh exists5
                AEGfxMeshFree(hp_Bars[index].bg_Mesh);
                hp_Bars[index].bg_Mesh = nullptr;
            }
            if (hp_Bars[index].bar_Mesh) {  // Check if HP bar mesh exists
                AEGfxMeshFree(hp_Bars[index].bar_Mesh);
                hp_Bars[index].bar_Mesh = nullptr;
            }
        }
    }

    if (index < NULL || index >= MAXHPBARS) return;
    // Update HP bar and background position to follow virus**
    hp_Bars[index].pos_X = virus_X;
    hp_Bars[index].pos_Y = virus_Y; // Keep both bars at the same Y offset

    // Clamp HP percentage
    hp_Bars[index].target_Scale = static_cast<float>(fmax({}, fmin(new_Hp_Percentage, cord_x_offset_1)));

    // Smooth interpolation
    speed = hp_Bars[index].interp_Speed * delta_Time;
    hp_Bars[index].current_Scale += (hp_Bars[index].target_Scale - hp_Bars[index].current_Scale) * speed;

    // Apply Scaling for Red HP Bar
    AEMtx33 rect_Scale, rect_Translate;
    AEMtx33Scale(&rect_Scale, hp_Bars[index].current_Scale, cord_x_offset_1);

    // Move HP Bar Correctly to Align with Background
    float left_AlignedX = hp_Bars[index].pos_X - (hp_Bar_Width * (cord_x_offset_1 - hp_Bars[index].current_Scale)) * cord_x_offset_2;
    AEMtx33Trans(&rect_Translate, left_AlignedX, hp_Bars[index].pos_Y - cord_x_offset_3);

    // Apply Final Transformation
    AEMtx33Concat(&hp_Bars[index].transform, &rect_Translate, &rect_Scale);

    // Ensure Background Always Stays in the Same Position
    AEMtx33 bg_Translate;
    AEMtx33Trans(&bg_Translate, hp_Bars[index].pos_X, hp_Bars[index].pos_Y - cord_x_offset_3);
    hp_Bars[index].bg_Transform = bg_Translate;
}

// render hp bars using the different transformations
void render_hp() {

    for (int i{}; i < MAXHPBARS; ++i) {
       if (!hp_Bars[i].bar_Mesh || !hp_Bars[i].bg_Mesh) continue;

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);       

        // Draw background
        AEGfxSetTransform(hp_Bars[i].bg_Transform.m);
        AEGfxMeshDraw(hp_Bars[i].bg_Mesh, AE_GFX_MDM_TRIANGLES);

        // Draw red bar
        AEGfxSetTransform(hp_Bars[i].transform.m);
        AEGfxMeshDraw(hp_Bars[i].bar_Mesh, AE_GFX_MDM_TRIANGLES);
    }
}

// Main free function if any dangling between levels!
void free_hp_bars() {
    for (int i{}; i < MAXHPBARS; ++i) {
        if (hp_Bars[i].bg_Mesh) {
            AEGfxMeshFree(hp_Bars[i].bg_Mesh);
            hp_Bars[i].bg_Mesh = NULL;
        }
        if (hp_Bars[i].bar_Mesh) {
            AEGfxMeshFree(hp_Bars[i].bar_Mesh);
            hp_Bars[i].bar_Mesh = NULL;
        }
    }
}

