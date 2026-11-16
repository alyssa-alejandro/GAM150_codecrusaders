/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      paracetemol.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines and handles the paracetemol event. It includes
*               apply and update functions which temporarily boosts tower damage
*               by a certain multiplier for a set duration.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "events.h"
#include "level.h"
#include <fstream>
#include <AEEngine.h>
#include "towers.h"
#include "countdown.h"
#include "paracetemol.h"
#include <iostream>

#define MAXTOWER 20

// Tracks if the vaccine effect is applied
bool para_active{};   

// Tracks if the towers have received their boost
bool para_boosted{};  

// Stores when the vaccine was applied
double para_elapsed_time{};  

// Buff last for 10 sec
float PARACETEMOL_DURATION{}; 

// Increase by 50%
int DAMAGE_MULTIPLIER{};  

// Timer to control how long it should remain active
static float para_time{};

void load_data_para() {
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
    para_active = doc["para_active"].GetBool();
    para_boosted = doc["para_boosted"].GetBool();
    para_elapsed_time = doc["para_elapsed_time"].GetDouble();
    PARACETEMOL_DURATION = doc["PARACETEMOL_DURATION"].GetFloat();
    DAMAGE_MULTIPLIER = doc["DAMAGE_MULTIPLIER"].GetInt();
    para_time = doc["para_time"].GetFloat();
}

void para_apply() {

    // Apply DAMAGE_MULTIPLIER for all towers
    for (int i{}; i < MAXTOWER; ++i) {
        towered[i].atk *= DAMAGE_MULTIPLIER;
    }

    // Update para_active to true since its inside this function
    para_active = true;
}

void para_update() {

    // Get the frame time to calculate the elapsed time
    para_elapsed_time += AEFrameRateControllerGetFrameTime();

    if (para_elapsed_time >= para_time) // To continue running for 15 and above
    {
        // Apply the DAMAGE_MULTIPLIER for the towers
        for (int i{}; i < MAXTOWER; ++i) {
            towered[i].atk /= DAMAGE_MULTIPLIER;

            // Change to false since the event is completed
            para_active = false;

            // Update the elapsed time
            para_elapsed_time -= para_elapsed_time;
        }
    }
}


