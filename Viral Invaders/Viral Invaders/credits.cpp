/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      credits.cpp
*@author(s)     Dalton Koh
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file includes all function definitions to initialize, render
*               and use logic to make the credits appear as pages for users to
*               screen through!
******************************************************************************/

#include "textureselect.h"
#include "credits.h"
#include "rectangle.h"
#include "collision.h"
#include <iostream>
#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <sstream>
#include "level.h"


// buttons for credit page
Press_Button next_Button;
Press_Button exit_Button;

// bg mesh
AEGfxVertexList* credits_Bg_Mesh = nullptr;
AEMtx33 credits_Bg_Transform;


//most scale and translations
static float scale{};
static const float translate{};

//bool and setters
bool is_Credits_Visible{};
int credits_Page{};

//colour
static unsigned int bg_color{};
static unsigned int button_color{};

//sizing
static float bg_Width{};
static float bg_Height{};
static float button_width {};
static float button_height {};
static float credits_font_scale {};
static int dimension_modifier{};

//positions
static float X_Pos{};  // X position (normalized from -1.0 to 1.0)
static float Y_Pos{};   // Y position (normalized from -1.0 to 1.0)
static float button_x{};
static float button_y{};
static float offset_1{};//0.1f
static float offset_2{};//0.25f
static float offset_3{};//1.6f
static float offset_4{};//1.68f
static float offset_5{};// 0.6f
static float offset_6{};//0.2f
static float offset_7{};//1.5f
static float offset_8{};//0.05f
static float offset_9{};//0.15f
static float offset_10{};//0.165f
static float offset_11{};//1.20f
static float offset_12{};//0.125f
static float offset_13{};//0.27f
static float offset_14{};//0.33f
static float offset_15{};//0.39f
static float offset_16{};//0.45f
static float offset_17{};//0.225f
static float offset_18{};//0.55f
static float offset_19{};//0.17f
static float offset_20{};//0.63f
static float offset_21{};//0.75f
static float offset_22{};//0.84f
static float offset_23{};//0.90f
static float offset_24{};//0.95f
static float offset_25{};//0.8f
static float offset_26{};//1.05f
static float offset_27{};//1.13f
static float offset_28{};//0.85f
static float offset_29{};//0.40f
static float offset_30{};//0.70f
static float offset_31{};//0.40f
static float offset_32{};//0.70f



//required strings for display
std::string rights_reserve_1;
std::string rights_reserve_2;
std::string credits_title;
std::string team_name;
std::string instructors_title;
std::string instructor_1;
std::string instructor_2;
std::string instructor_3;
std::string instructor_4;
std::string created_at;
std::string institute;
std::string president_title;
std::string president_name;
std::string executives_title;
std::string executives_1;
std::string executives_2;
std::string music_credit;
std::string ui_credit;
std::string font_credit;
std::string next_text;
std::string exit_text;


// Loading all required data from credits.json
void load_data_credits() {

    // File stream
    std::fstream file("Assets/Data/credits.json");

    // If files fails to open
    if (!file) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(file); // wrap file stream into something rapid json can read from 
    rapidjson::Document doc; // Create json object
    doc.ParseStream(isw); // Parses contents from the isw stream into doc. so you can access it later on

    // If parse error or error getting the right data type
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return;
    }

    

    // Required data to be retrieved 

    bg_color = doc["bg_color"].GetUint();
    button_color = doc["button_color"].GetUint();
    bg_Width = doc["bg_Width"].GetFloat();
    bg_Height = doc["bg_Height"].GetFloat();
    button_x = doc["button_x"].GetFloat();
    button_y = doc["button_y"].GetFloat();
    button_width = doc["button_width"].GetFloat();
    button_height = doc["button_height"].GetFloat();
    dimension_modifier = doc["dimension_modifier"].GetInt();
    credits_font_scale = doc["credits_font_scale"].GetFloat();
    credits_Page = doc["Credits_Page"].GetInt();
    X_Pos = doc["x_Pos"].GetFloat();
    Y_Pos = doc["y_Pos"].GetFloat();
    is_Credits_Visible = doc["is_Credits_Visible"].GetBool();
    offset_1 = doc["offset_1"].GetFloat();
    offset_2 = doc["offset_2"].GetFloat();
    offset_3 = doc["offset_3"].GetFloat();
    offset_4 = doc["offset_4"].GetFloat();
    offset_5 = doc["offset_5"].GetFloat();
    offset_6 = doc["offset_6"].GetFloat();
    offset_7 = doc["offset_7"].GetFloat();
    offset_8 = doc["offset_8"].GetFloat();
    offset_9 = doc["offset_9"].GetFloat();
    offset_10 = doc["offset_10"].GetFloat();
    offset_11 = doc["offset_11"].GetFloat();
    offset_12 = doc["offset_12"].GetFloat();
    offset_13 = doc["offset_13"].GetFloat();
    offset_14 = doc["offset_14"].GetFloat();
    offset_15 = doc["offset_15"].GetFloat();
    offset_16 = doc["offset_16"].GetFloat();
    offset_17 = doc["offset_17"].GetFloat();
    offset_18 = doc["offset_18"].GetFloat();
    offset_19 = doc["offset_19"].GetFloat();
    offset_20 = doc["offset_20"].GetFloat();
    offset_21 = doc["offset_21"].GetFloat();
    offset_22 = doc["offset_22"].GetFloat();
    offset_23 = doc["offset_23"].GetFloat();
    offset_24 = doc["offset_24"].GetFloat();
    offset_25 = doc["offset_25"].GetFloat();
    offset_26 = doc["offset_26"].GetFloat();
    offset_27 = doc["offset_27"].GetFloat();
    offset_28 = doc["offset_28"].GetFloat();
    offset_29 = doc["offset_29"].GetFloat();
    offset_30 = doc["offset_30"].GetFloat();
    offset_31 = doc["offset_31"].GetFloat();
    offset_32 = doc["offset_32"].GetFloat();

    // Loading string literals 
    #define LOAD_STRING(key, var) if (doc.HasMember(key) && doc[key].IsString()) var = doc[key].GetString();

    LOAD_STRING("rights_reserve_1", rights_reserve_1);
    LOAD_STRING("rights_reserve_2", rights_reserve_2);
    LOAD_STRING("credits_title", credits_title);
    LOAD_STRING("team_name", team_name);
    LOAD_STRING("instructors_title", instructors_title);
    LOAD_STRING("instructor_1", instructor_1);
    LOAD_STRING("instructor_2", instructor_2);
    LOAD_STRING("instructor_3", instructor_3);
    LOAD_STRING("instructor_4", instructor_4);
    LOAD_STRING("created_at", created_at);
    LOAD_STRING("institute", institute);
    LOAD_STRING("president_title", president_title);
    LOAD_STRING("president_name", president_name);
    LOAD_STRING("executives_title", executives_title);
    LOAD_STRING("executives_1", executives_1);
    LOAD_STRING("executives_2", executives_2);
    LOAD_STRING("music_credit", music_credit);
    LOAD_STRING("ui_credit", ui_credit);
    LOAD_STRING("font_credit", font_credit);
    LOAD_STRING("next_text", next_text);
    LOAD_STRING("exit_text", exit_text);

    #undef LOAD_STRING
}


// Initialize the credits page
void init_credits_page() {

    // Background
    Create_Rectangle(&credits_Bg_Mesh, bg_Width, bg_Height, bg_color, &credits_Bg_Transform, credits_font_scale, credits_font_scale, translate, translate);

    // Next button
    next_Button.x = button_x;
    next_Button.y = button_y;
    next_Button.width = button_width;
    next_Button.height = button_height;
    next_Button.color = button_color;


    Create_Rectangle(&next_Button.mesh, next_Button.width, next_Button.height, next_Button.color, &next_Button.transform, credits_font_scale, credits_font_scale, next_Button.x, next_Button.y);

    // Exit Button (for second page)
    exit_Button.x = next_Button.x; 
    exit_Button.y = next_Button.y;
    exit_Button.width = next_Button.width;
    exit_Button.height = next_Button.height;
    exit_Button.color = next_Button.color;


    Create_Rectangle(&exit_Button.mesh, exit_Button.width, exit_Button.height, exit_Button.color, &exit_Button.transform, credits_font_scale, credits_font_scale, exit_Button.x, exit_Button.y);

}

// Render the credits page
void render_credits_page() {

    if (!is_Credits_Visible) return;  // If credits page is not open, do nothing


    // Render Credits Text
     float x_Pos = X_Pos;  // X position (normalized from -1.0 to 1.0)
     float y_Pos = Y_Pos;   // Y position (normalized from -1.0 to 1.0)
  

    // Render Background Mesh
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(credits_bg, {}, {});
    AEGfxSetTransform(credits_Bg_Transform.m);
    AEGfxMeshDraw(credits_Bg_Mesh, AE_GFX_MDM_TRIANGLES);


    // Print bottom text
    AEGfxPrint(font, rights_reserve_1.c_str(), x_Pos - offset_1, y_Pos - offset_3, offset_5, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
    AEGfxPrint(font, rights_reserve_2.c_str(), x_Pos + offset_2, y_Pos - offset_4, offset_5, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);


    // Handle Input: Check if Exit Button is Clicked
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);
    float MouseX = static_cast<float>(mouseX) - (AEGfxGetWindowWidth()/2); 
    float MouseY = (AEGfxGetWindowHeight()/2) - static_cast<float>(mouseY);

    if (credits_Page == credit_1) {
        // If NEXT button is clicked, move to second page
        if (MouseX >= next_Button.x - next_Button.width / dimension_modifier && MouseX <= next_Button.x + next_Button.width / dimension_modifier &&
            MouseY >= next_Button.y - next_Button.height / dimension_modifier && MouseY <= next_Button.y + next_Button.height / dimension_modifier &&
            AEInputCheckTriggered(AEVK_LBUTTON)) {

            credits_Page = credit_2;// set page number to 2
            return;
        }
    }
    else if (credits_Page == credit_2) {
        // If EXIT button is clicked, close credits
        if (MouseX >= exit_Button.x - exit_Button.width / dimension_modifier && MouseX <= exit_Button.x + exit_Button.width / dimension_modifier &&
            MouseY >= exit_Button.y - exit_Button.height / dimension_modifier && MouseY <= exit_Button.y + exit_Button.height / dimension_modifier &&
            AEInputCheckTriggered(AEVK_LBUTTON)) {

            is_Credits_Visible = false;
            credits_Page = credit_1;  // Reset page number back to 1
            return;
        }
    }


    if (credits_Page == credit_1) {

        // Render Next Button
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(button_clicked, {}, {});
        AEGfxSetTransform(next_Button.transform.m);
        AEGfxMeshDraw(next_Button.mesh, AE_GFX_MDM_TRIANGLES);
         
        // Page 1 text print stub
        AEGfxPrint(font, credits_title.c_str(), x_Pos + offset_6, y_Pos, offset_7, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        y_Pos -= offset_1;
        AEGfxPrint(font, team_name.c_str(), x_Pos - offset_8, y_Pos - offset_9, offset_7, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        y_Pos -= offset_1;


        AEGfxPrint(font, instructors_title.c_str(), x_Pos + offset_10, y_Pos - offset_2, offset_11, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        y_Pos -= offset_1;
        AEGfxPrint(font, instructor_1.c_str(), x_Pos + offset_12, y_Pos - offset_13, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, instructor_2.c_str(), x_Pos + offset_12, y_Pos - offset_14, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, instructor_3.c_str(), x_Pos + offset_12, y_Pos - offset_15, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, instructor_4.c_str(), x_Pos + offset_12, y_Pos - offset_16, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);

        AEGfxPrint(font, created_at.c_str(), x_Pos + offset_17, y_Pos - offset_18, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, institute.c_str(), x_Pos - offset_6, y_Pos - offset_20, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, president_title.c_str(), x_Pos + offset_17, y_Pos - offset_21, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, president_name.c_str(), x_Pos + offset_19, y_Pos - offset_22, offset_23, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, executives_title.c_str(), x_Pos + offset_31, y_Pos - offset_24, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, executives_1.c_str(), x_Pos - offset_32, y_Pos - offset_26, offset_25, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, executives_2.c_str(), x_Pos - offset_2, y_Pos - offset_27, offset_25, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);


        AEGfxPrint(font, next_text.c_str(), -offset_28, -offset_25, credits_font_scale, {}, {}, {}, credits_font_scale);

    }
    else {

        // Render Exit Button 
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(button_clicked, {}, {});
        AEGfxSetTransform(exit_Button.transform.m);
        AEGfxMeshDraw(exit_Button.mesh, AE_GFX_MDM_TRIANGLES);

        // Page 2 text print stub 
        AEGfxPrint(font, credits_title.c_str(), x_Pos + offset_6, y_Pos, offset_7, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        y_Pos -= offset_1;
        AEGfxPrint(font, music_credit.c_str(), x_Pos - offset_6, y_Pos - offset_29, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, ui_credit.c_str(), x_Pos - offset_17, y_Pos - offset_18, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, font_credit.c_str(), x_Pos - offset_17, y_Pos - offset_30, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale, credits_font_scale);
        AEGfxPrint(font, exit_text.c_str(), -offset_28, -offset_25, credits_font_scale, {}, {}, {}, credits_font_scale);

       
    }
    
}

// Free resources related to the credits page
void free_credits_page() {
    
    if (exit_Button.mesh) {
        AEGfxMeshFree(exit_Button.mesh);
        exit_Button.mesh = nullptr;
    }
    if (credits_Bg_Mesh) {
        AEGfxMeshFree(credits_Bg_Mesh);
        credits_Bg_Mesh = nullptr;
    }
    if (next_Button.mesh) {
        AEGfxMeshFree(next_Button.mesh);
        next_Button.mesh = nullptr;
    }
}
