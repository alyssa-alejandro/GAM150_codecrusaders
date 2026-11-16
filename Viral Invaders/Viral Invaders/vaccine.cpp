/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      vaccine.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines and manages the vaccine event. It includes apply 
*               and update functions to defuff towers and boost their attributes after 
*               a specific time.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include <AEEngine.h>
#include "towers.h"
#include "countdown.h"
#include "vaccine.h"
#include <vector>
#include <ctime>
#include <iostream>
#include "events.h"
#include "level.h"
#include <fstream>

// Tracks if the vaccine effect is applied
bool vaccine_active{};  

// Tracks if the towers have received their boost
bool vaccine_boosted{};  

// Stores when the vaccine was applied
double vaccine_elapsed_time{}; 

// Variables to be adjusted
static int vac_scale_atk_atkspd{};
static int vac_scale_hp{};
static int attack_up{};
static int attackspd_up{};
static int hp_up{};

// After a certain amount of time, it will boost the towers
static float after_time{};

void load_data_vaccine() {
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
    vaccine_active = doc["vaccine_active"].GetBool();
    vaccine_boosted = doc["vaccine_boosted"].GetBool();
    vaccine_elapsed_time = doc["vaccine_elapsed_time"].GetDouble();
    vac_scale_atk_atkspd = doc["vac_scale_atk_atkspd"].GetInt();
    vac_scale_hp = doc["vac_scale_hp"].GetInt();
    attack_up = doc["attack_up"].GetInt();
    attackspd_up = doc["attackspd_up"].GetInt();
    hp_up = doc["hp_up"].GetInt();
    after_time = doc["after_time"].GetFloat();
}

void vaccine_apply() {
    // Apply 50% debuff to all towers
    for (int i{}; i < MAXTOWER; ++i) {
        towered[i].atk /= vac_scale_atk_atkspd;
        towered[i].atk_spd /= vac_scale_atk_atkspd;
        towered[i].base_hp -= vac_scale_hp;
        towered[i].current_hp -= vac_scale_hp;
    }
}

void vaccine_update() {

    // Get the frame time to calculate the elapsed time
    vaccine_elapsed_time +=  AEFrameRateControllerGetFrameTime();

    if (vaccine_elapsed_time >= after_time) { // After 5 seconds, boost towers
        for (int i{}; i < MAXTOWER; ++i) {
            towered[i].atk *= vac_scale_atk_atkspd;
            towered[i].atk_spd *= vac_scale_atk_atkspd;
            towered[i].base_hp += vac_scale_hp;
            towered[i].current_hp += vac_scale_hp;

            towered[i].atk += attack_up;
            towered[i].atk_spd += attackspd_up;
            towered[i].base_hp += hp_up;
            towered[i].current_hp += hp_up;
        }
        vaccine_boosted = true; // Ensure boost is only applied once

        if (current_State == STATE_MAINMENU || level_end == vic) {

            // Loop through each element in MAXTOWER
            for (int i{}; i < MAXTOWER; ++i) {

                // Subtract the stats by the rvalue, which will decrease the stats
                towered[i].atk -= attack_up;
                towered[i].atk_spd -= attackspd_up;
                towered[i].base_hp -= hp_up;
                towered[i].current_hp -= hp_up;
            }
        }
    }
}

