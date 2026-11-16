/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      bgmusic.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines and manages the background music for the game.
*               It includes loading music assets, initialization of audio settings
*               and control of playback. It supports dynamic adjustments to volume
*               and pitch based on user settings and ensures proper clean of audio
*               resources.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include "level.h"
#include "AEEngine.h"
#include "bgmusic.h"
#include <crtdbg.h>
#include "gamestatemanager.h"
#include <iostream>
#include <vector>

// Volume variables
static float volume_bgm{};
static float volume_pitch{};

// Global variables for Settings
float current_volume{};
float current_pitch{};

// Loop -1
static int loop{};

//const char* bgm_arr[2] = {};
std::vector<const char*>bgm_arr{};

// For spritesheet init
rapidjson::Document doc;

static bool bgm_loaded = false;
static bool bgmGroup_loaded = false;

// Load data from json file
void load_data_bgmusic() {
    std::ifstream data("Assets/Data/bgmusic.json");
    if (!data) {
        gGameRunning = NULL;
        return; // Exit if the file couldn't be opened
    }

    rapidjson::IStreamWrapper isw(data);

    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return; // Exit on parsing error
    }

    volume_bgm = doc["volume_bgm"].GetFloat();
    volume_pitch = doc["volume_pitch"].GetFloat();
    current_volume = doc["current_volume"].GetFloat();
    current_pitch = doc["current_pitch"].GetFloat();
    loop = doc["loop"].GetInt();
}

void init_audio( AEAudio& bgm, AEAudioGroup& bgmGroup) {

    // Directly load the music array
    if (bgm_arr.size() != 2ULL) {

    
        const rapidjson::Value& music = doc["music"];
        const rapidjson::GenericArray<true, rapidjson::Value>& music_array = music.GetArray();
        for (const rapidjson::Value& m : music_array) {
            bgm_arr.push_back(m.GetString());
        }
    }
    switch (current_State)
    {
    case STATE_MAINMENU: 
            bgm = AEAudioLoadMusic(bgm_arr[0]);
            bgm_loaded = true;
            break;
    case STATE_LEVEL1: 
    case STATE_LEVEL2: 
    case STATE_LEVEL3: 
    case STATE_LEVEL4: 
    case STATE_LEVEL5: 
    case STATE_LEVEL6: 

            bgm = AEAudioLoadMusic(bgm_arr[1]);
            bgm_loaded = true;
            break;
    }



    // Create in level.cpp
    bgmGroup = AEAudioCreateGroup();     

    bgmGroup_loaded = true;

    AEAudioPlay(bgm, bgmGroup, volume_bgm, volume_pitch, loop); 
}

void input_audio(AEAudioGroup& bgmGroup) { 

    // User can control the current volume from game setting
    AEAudioSetGroupVolume(bgmGroup, current_volume); 

    // User can control the current pitch from game setting
    AEAudioSetGroupPitch(bgmGroup, current_pitch);   
}

void free_audio(AEAudio& bgm, AEAudioGroup& bgmGroup) {
    if (bgm_loaded) {
        AEAudioUnloadAudio(bgm);
        bgm_loaded = false;
    }

    if (bgmGroup_loaded) {
        AEAudioUnloadAudioGroup(bgmGroup);
        bgmGroup_loaded = false;
    }
}
