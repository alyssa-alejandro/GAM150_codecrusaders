/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      event.cpp
*@author(s)     Chan Qi Ying (Primary Author, 80%)
*               Alyssa Nicole Alejandro (Secondary Author, 20%)
*@DP emails     q.chan@digipen.edu
*               alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the event randomiser to call events at random intervals.
*               Also includes functions to assist with functions of events.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"

#include "events.h"
#include <ctime>        // std::time()
#include <cstdlib>      // std::rand()
#include <iostream>      
#include <fstream>      
#include <sstream>      
#include "pipagao.h"    
#include "platelets.h"
#include "paracetemol.h"
#include "vaccine.h"
#include "virus.h"    
#include "spritesheets.h"
#include "level.h"
#include "vector"


// Setting interval between each event
static int min_interval{};
static int max_interval{};
static int reset{};
static int countdown{};
static int coll_limit{};
static int row_limit{};
static int rand_modifier {};

// Aid in calculation
static float half{};

// To check which event is triggered 
int triggered_event = no_event;    

// To store triggered event
event_id event{ no_event };             

// To check if event has happened
bool event_happened = false;

// To store spritesheet assets
std::vector<std::string>ss_arr{};

// Load data from json file
void load_data_event() {
    std::fstream data("Assets/Data/event.json");
    if (!data) {
            gGameRunning = NULL;
            return;
    }

    rapidjson::Document doc;
    rapidjson::IStreamWrapper isw(data);

    doc.ParseStream(isw);
    if (doc.HasParseError()) {
            gGameRunning = NULL;
            return; // Exit the function on parse error
    }

    // Assigning data from file to variable
    min_interval = doc["min_interval"].GetInt();
    max_interval = doc["max_interval"].GetInt();
    reset = doc["reset"].GetInt();
    countdown = doc["countdown"].GetInt();
    coll_limit = doc["coll_limit"].GetInt();
    row_limit = doc["row_limit"].GetInt();
    rand_modifier = doc["rand_modifier"].GetInt();
    half = doc["half"].GetFloat();

    // Directly load the music array
    if (ss_arr.size() != 2ULL) {
        const rapidjson::Value& spritesheet = doc["spritesheets"];
        const rapidjson::GenericArray<true, rapidjson::Value>& ss_array = spritesheet.GetArray();
        for (const rapidjson::Value& ss : ss_array) {
            ss_arr.push_back(ss.GetString());
        }
    }

}

// Generate seed for random number 
void seed_generator() {      
    static bool is_seeded = false;
    if (!is_seeded) {
        srand((unsigned int)time(NULL));
        is_seeded = true;
    }
}

// Generate random coordinates for pipagao and platelet event
AEVec2 random_coor_generator(int square_columns, int square_rows, float cell_length) {         
    seed_generator();
    AEVec2 coor;
    
    // Different coordinates for normal and boss level
    if (current_State == STATE_LEVEL3) { // boss level 3
        coor.x = (-(square_columns * cell_length) / half + (cell_length / half) + cell_length) + (std::rand() % (square_columns - coll_limit)) * cell_length;
        float min = -(square_rows / half * cell_length) / half + (cell_length / half);
        float max = min + (static_cast<int>(square_rows / coll_limit)) * cell_length;
        do {
            coor.y = (-((square_rows - row_limit) * cell_length) / half + (cell_length)) + (std::rand() % static_cast<int>(square_rows - coll_limit)) * cell_length;
        } while (coor.y >= min && coor.y < max);
    }
    else { // notmal
        coor.x = (-(square_columns * cell_length) / half + (cell_length / half) + cell_length) + (std::rand() % (square_columns - coll_limit)) * cell_length;
        coor.y = (-(square_rows / coll_limit * cell_length) / half + (cell_length / half)) + (std::rand() % static_cast<int>(square_rows / coll_limit)) * cell_length;
    }

    return coor;
}

// Sets event to be triggered
void event_trigger(GameState lvl, std::vector<virus*>spawned_virus, int square_columns, int square_rows, float cell_length) {
    static int before_event_time = reset;

    if (lvl == STATE_GAME_OVER) {       // check if level is exited
        before_event_time = reset;
        return;
    }



    // Set events for each level
    std::vector<event_id> lvl_events{};
    switch (lvl) {
    case STATE_LEVEL1:
        lvl_events = { pipagao };
        break;
    case STATE_LEVEL2:
    case STATE_LEVEL3:
        lvl_events = { pipagao, paracetemol };
        break;
    case STATE_LEVEL4:
        lvl_events = { pipagao, vaccine, platelet };
        break;
    case STATE_LEVEL5:
    case STATE_LEVEL6:
        lvl_events = { pipagao, vaccine, platelet, paracetemol };
        break;
    }

    if (before_event_time == reset) {
        // Setting event
        event = event_randomizer(lvl_events);    
        if (ss_data.texture) AEGfxTextureUnload(ss_data.texture);
        switch (event) {
        case pipagao:
            ss_data.texture = AEGfxTextureLoad(ss_arr[0].c_str());
            break;
        case vaccine:
            ss_data.texture = AEGfxTextureLoad(ss_arr[1].c_str());
            break;
        case paracetemol:
            ss_data.texture = AEGfxTextureLoad(ss_arr[2].c_str());
            break;
        case platelet:
            ss_data.texture = AEGfxTextureLoad(ss_arr[3].c_str());
            break;
        default:
            ss_data.texture = nullptr;
            break;
        }
        
        // Set time before event starts
        before_event_time = min_interval + (std::rand() % (max_interval - min_interval + rand_modifier));
    }

    else if (before_event_time <= countdown && before_event_time > NULL) {       // visual for about 5 seconds

            if (level_end == norm) {
                before_event_time = static_cast<int>(before_event_time - AEFrameRateControllerGetFrameTime());
                ss_update(&ss_data, AEFrameRateControllerGetFrameTime());
            }
            ss_render(&ss_data);

        }
    

    else if (before_event_time > NULL) {   // Countdown
        if (level_end == norm) before_event_time = static_cast<int>(before_event_time - AEFrameRateControllerGetFrameTime());
    }
    else if (before_event_time <= NULL) {  // Trigger event


        switch (event) {
        case pipagao:
            triggered_event = pipagao;
            update_ppg(spawned_virus, square_columns, square_rows, cell_length);
            render_ppg();
            break;
        case vaccine:
            triggered_event = vaccine;
            vaccine_active = true;
            vaccine_apply();
            break;
        case paracetemol:
            triggered_event = paracetemol;
            para_active = true;
            para_apply();
            break;
        case platelet:
            triggered_event = platelet;
            generate_platelets(square_columns, square_rows, cell_length);
            update_platelets();
            render_platelets();
            collision_platelets_virus(spawned_virus);
            break;
        }
        before_event_time = reset;      // Reset before_event_time
        
        event_happened = true;
    }

}

// Choose an event to trigger
    event_id event_randomizer(std::vector<event_id> lvl) {
        seed_generator();

        // Determine which event to activate
        size_t num_of_events = lvl.size();
        int num = (std::rand() % num_of_events);

        // Ensure vaccine/paracetamol does not occur again when vaccine occured before or during paracetamol event
        if (vaccine_boosted == true || para_active == true) {
            while ((lvl[num] == vaccine || lvl[num] == paracetemol)) {
                num = (std::rand() % num_of_events);
            }
        }
        return lvl[num];
    }


