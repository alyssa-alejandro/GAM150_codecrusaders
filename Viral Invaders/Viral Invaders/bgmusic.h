/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      bgmusic.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the background music for the game. It includes
*				loading, initializing, controlling and freeing of audio resources.
*				Also enabling dynamic adjustments to volume and pitch settings.
******************************************************************************/
#pragma once
#include "AEEngine.h"
#include <vector>

extern float current_volume;
extern float current_pitch;

// Load data to file
void load_data_bgmusic();

// To load music asset to the different levels
void init_audio(AEAudio& bgm, AEAudioGroup& bgmGroup);

// Audio controls
void input_audio(AEAudioGroup& bgmGroup);

// Free audio
void free_audio(AEAudio& bgm, AEAudioGroup& bgmGroup);


