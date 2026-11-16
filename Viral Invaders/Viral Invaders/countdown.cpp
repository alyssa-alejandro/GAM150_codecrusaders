/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      countdown.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines and handles the countdown timer.
*               This includes initialization, updates, and rendering.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <iostream>
#include "countdown.h"
#include "AEEngine.h"
#include "MainMenu.h"
#include "level.h"

countdown_timer timer;

// Check is the game is paused
bool is_paused{};

// Stores the time for the level
double original_time{};

// Range of 0.0 to 1.0
static float full_val{};  // 1.0
static float empty_val{}; // 0.0

// Adjust the position of countdown timer
static float width_adjust{};
static float height_adjust{};

// Aid in calculation to count down
static int full_minutes{};
static int tens{};

// Aid in display
static int zero_ascii{};
static int colon_ascii{};
static int null_ascii{};

// Load data from json file
void load_data_countdown() {
    std::fstream data("Assets/Data/countdown.json");
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
    is_paused = doc["is_paused"].GetBool();
    original_time = doc["ori_time"].GetDouble();
    full_val = doc["full_val"].GetFloat();
    empty_val = doc["empty_val"].GetFloat();
    width_adjust = doc["width_adjust"].GetFloat();
    height_adjust = doc["height_adjust"].GetFloat();
    full_minutes = doc["full_minutes"].GetInt();
    tens = doc["tens"].GetInt();

    zero_ascii = doc["zero_ascii"].GetInt();
    colon_ascii = doc["colon_ascii"].GetInt();
    null_ascii = doc["null_ascii"].GetInt();
}

void countdown_init(countdown_timer* game_timer, s8 font_handle)
{
    // Stores the time for the level into time_remaining in the struct
    game_timer->time_remaining = original_time;

    // Stores the font in the struct
    game_timer->font_handle = font_handle;

    // set to false
    is_paused = false;
}

void countdown_update(countdown_timer* game_timer)
{
    // Countdown timer will loop every frame unless the game is paused or game_condition is fulfilled
    if (!is_paused && game_condition==none)
    {
        // Make dt to obtain each frame time
        double dt = AEFrameRateControllerGetFrameTime();

        // Subtract the time_remaining to each frame
        game_timer->time_remaining -= dt;

        // Ensure it doesn't go below 0
        if (game_timer->time_remaining < empty_val){ game_timer->time_remaining = empty_val;}
    }
}

void countdown_render(countdown_timer* game_timer)
{
    // Convert time to minutes and seconds
    int minutes = static_cast<int>(game_timer->time_remaining / full_minutes);
    int seconds = static_cast<int>(game_timer->time_remaining) % full_minutes;

    // DISPLAY 
    char time_string[6]; // "MM:SS" format requires 5 characters + null terminator
    time_string[0] = static_cast<char>(zero_ascii + minutes / tens); // tens place of minutes
    time_string[1] = static_cast<char>(zero_ascii + minutes % tens); // ones place of minutes
    time_string[2] = static_cast<char>(colon_ascii);                 // separator
    time_string[3] = static_cast<char>(zero_ascii + seconds / tens); // tens place of seconds
    time_string[4] = static_cast<char>(zero_ascii + seconds % tens); // ones place of seconds
    time_string[5] = static_cast<char>(null_ascii);                  // null terminator

    // Size of the text for positioning
    float width, height;
    AEGfxGetPrintSize(game_timer->font_handle, time_string, full_val, &width, &height);

    // Check font handle is valid and text can be rendered
    if (width > empty_val && height > empty_val) {

        // DISPLAY POSITION
        float x = full_val - width - width_adjust;
        float y = height_adjust - height;

        // Render the countdown timer
        AEGfxPrint(game_timer->font_handle, time_string,
            x, y,
            full_val, full_val, full_val, full_val, full_val);
    }
}
