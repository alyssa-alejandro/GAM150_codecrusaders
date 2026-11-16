/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      setting.cpp
*@author(s)     Ethan Ng (Primary Author, 100%)
*@DP emails     n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains the definition of some base settings the game uses
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

static float background_color_set{};
static float background_color_multiplier{};
static int max_frame_rate{};

// Load data from json file
void load_data_settings() {
    std::fstream data("Assets/Data/setting.json");
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
    background_color_set = doc["background_color_set"].GetFloat();
    background_color_multiplier = doc["background_color_multiplier"].GetFloat();
    max_frame_rate = doc["max_frame_rate"].GetInt();
}

void settings() {


    AESysFrameStart();
    AEFrameRateControllerInit(max_frame_rate);

    // Set background color
    AEGfxSetBackgroundColor(background_color_set, background_color_set, background_color_set);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    // Set the the color to multiply to white, so that the sprite can
    // display the full range of colors (default is black).
    AEGfxSetColorToMultiply(background_color_multiplier, background_color_multiplier, background_color_multiplier, background_color_multiplier);

    // Set the color to add to nothing, so that we don't alter the sprite's color
    AEGfxSetColorToAdd(background_color_set, background_color_set, background_color_set, background_color_set);

    // Set blend mode to AE_GFX_BM_BLEND, which will allow transparency.
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(background_color_multiplier);

}
