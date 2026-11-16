/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      game_setting.cpp
*@author        Ang Yu Xuan (Primary Author, 95%)
*               Chan Qi Ying (Secondary Author, 5%)
*@DP emails     yuxuan.ang@digipen.edu
*               q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file is used to control the volume and pitch of the music that
*               is currently played.
******************************************************************************/

#include "AEEngine.h"
#include "vector"
#include "mesh.h"
#include "MainMenu.h"
#include "textureselect.h"
#include <iostream>
#include "bgmusic.h"
#include "textureselect.h"
#include "pausescreen.h"
#include "level.h"

#include "document.h"
#include "istreamwrapper.h"

AEGfxVertexList* setting_pop_up{}; //this is for Game_Setting_Init, meshes for pop up screen for game setting
AEGfxVertexList* setting_audio{}; //this is for Game_Setting_Init, meshes for volume and pitch adjusting bar
bool game_setting_trigger{}; //this will check whether the game setting button is triggered or not
std::vector<Button>game_setting; //game_setting is std::vector of type struct Button

bool toggle_volume{}; //this will check whether player is adjusting the volume or not
bool toggle_pitch{}; //this will check whether player is adjusting the pitch or not
float name_offset{};
static float dimension_modifer{};
void Game_Setting_Init() {
	float exit_button_w{}, exit_button_h{}, //the size of exit button for pop up screen
		audio_volume_w{}, audio_volume_h{}, //the size of audio volume bar for pop up screen
		volume_adjust_w{}, volume_adjust_h{}, //the size of audio volume adjustment toggle bar for pop up screen
		audio_pitch_w{}, audio_pitch_h{}, //the size of audio pitch bar for pop up screen
		pitch_adjust_w{}, pitch_adjust_h{}, //the size of audio volume adjustment toggle bar for pop up screen
		width_offset{}, height_offset{};

	u32 red_color{}, white_color{};
	std::string music_volume_string_name, music_pitch_string_name;

	std::ifstream fs("Assets/Data/game_setting.json");
	if (!fs) { //cannot open this json file
		gGameRunning = NULL;
		return;
	}

	rapidjson::IStreamWrapper isw(fs);
	rapidjson::Document doc;

	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		gGameRunning = NULL;
		return; // Exit the function on parse error
	}

	// Assigning data from json file to init variable in this function
	exit_button_w = doc["json_exit_button_size"].GetFloat();
	exit_button_h = doc["json_exit_button_size"].GetFloat();
	audio_volume_w = doc["json_audio_bar_width"].GetFloat();
	audio_volume_h = doc["json_audio_bar_height"].GetFloat();
	volume_adjust_w = doc["json_toggle_bar_width"].GetFloat();
	volume_adjust_h = doc["json_toggle_bar_height"].GetFloat();
	audio_pitch_w = doc["json_audio_bar_width"].GetFloat();
	audio_pitch_h = doc["json_audio_bar_height"].GetFloat();
	pitch_adjust_w = doc["json_toggle_bar_width"].GetFloat();
	pitch_adjust_h = doc["json_toggle_bar_height"].GetFloat();
	dimension_modifer = doc["dimension_modifer"].GetFloat();
	sscanf_s(doc["red_color"].GetString(), "%x", &red_color);
	sscanf_s(doc["white_color"].GetString(), "%x", &white_color);
	width_offset = doc["width_offset"].GetFloat();
	height_offset = doc["height_offset"].GetFloat();
	music_volume_string_name = doc["music_volume"].GetString();
	music_pitch_string_name = doc["music_pitch"].GetString();
	name_offset = doc["name_offset"].GetFloat();
	
	if (!setting_pop_up) setting_pop_up = Draw_Square_Mesh(red_color); //setting_pop_up now points to square mesh of red color
	if (!setting_audio) setting_audio = Draw_Square_Mesh(white_color); //setting_audio now points to square mesh of white color

	//Init setting container for pop up screen for game setting
	float setting_Container_x{ AEGfxGetWindowWidth() / dimension_modifer - AEGfxGetWinMaxX() }, //centralise pop up screen(x=0)
		setting_Container_y{ AEGfxGetWindowHeight() / dimension_modifer - AEGfxGetWinMaxY() }, //centralise pop up screen(y=0)
		setting_Container_w{ static_cast<float>(AEGfxGetWindowWidth() / dimension_modifer) }, //width of pop up screen(width=800)
		setting_Container_h{ static_cast<float>(AEGfxGetWindowHeight() / dimension_modifer) }; //height of pop up screen(height=450)

	//Init exit button for pop up screen for game setting
	//exit button is located at the bottom right of pop up screen
	float exit_button_x{ setting_Container_x + setting_Container_w / dimension_modifer - exit_button_w/ dimension_modifer },
		exit_button_y{ setting_Container_y - setting_Container_h / dimension_modifer + exit_button_h/ dimension_modifer };

	//Init audio volume bar for pop up screen for game setting
	//audio volume bar is located at the top left of pop up screen
	float audio_volume_x{ setting_Container_x - setting_Container_w / width_offset + audio_volume_w / dimension_modifer },
		audio_volume_y{ setting_Container_y + setting_Container_h / height_offset - audio_volume_h / dimension_modifer };

	//Init audio volume adjustment toggle bar for pop up screen for game setting
	//audio volume adjustment toggle bar is located at the rightmost side of audio volume bar
	float volume_adjust_x{ audio_volume_x + audio_volume_w / dimension_modifer },
		volume_adjust_y{ audio_volume_y};

	//Init audio pitch bar for pop up screen for game setting
	//audio pitch bar is located below audio volume bar
	float audio_pitch_x{ setting_Container_x - setting_Container_w / width_offset + audio_pitch_w / dimension_modifer },
		audio_pitch_y{ setting_Container_y - audio_pitch_h / dimension_modifer };

	//Init audio pitch adjustment for pop up screen for game setting
	//audio pitch adjustment toggle bar is located at the rightmost side of audio pitch bar
	float pitch_adjust_x{ audio_pitch_x + audio_pitch_w / dimension_modifer },
		pitch_adjust_y{ audio_pitch_y };

	//push variable for pop up screen and exit button into vector game_setting
	game_setting.push_back({ setting_Container_x, setting_Container_y, setting_Container_w, setting_Container_h});
	game_setting.push_back({ exit_button_x, exit_button_y, exit_button_w, exit_button_h});

	//push variable for audio volume bar and audio volume adjustment toggle bar into vector game_setting
	game_setting.push_back({ audio_volume_x, audio_volume_y, audio_volume_w, audio_volume_h, music_volume_string_name });
	game_setting.push_back({ volume_adjust_x, volume_adjust_y, volume_adjust_w, volume_adjust_h});

	//push variable for audio pitch bar and audio pitch adjustment toggle bar into vector game_setting
	game_setting.push_back({ audio_pitch_x, audio_pitch_y, audio_pitch_w, audio_pitch_h, music_pitch_string_name });
	game_setting.push_back({ pitch_adjust_x, pitch_adjust_y, pitch_adjust_w, pitch_adjust_h});
}

void Game_Setting_Update() {
	int mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY); //get current position of mouse

	float toggle_max = game_setting[2].pos_x + game_setting[2].length / dimension_modifer, //this is the maximum that player can toggle to the rightmost
		toggle_min = game_setting[2].pos_x - game_setting[2].length / dimension_modifer; //this is the minimum that player can toggle to the leftmost

	//exit game setting pop up if the "exit" button is clicked
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (IsMouseInsideButton(game_setting[1].pos_x, game_setting[1].pos_y,
			game_setting[1].length, game_setting[1].width)) {
			game_setting_trigger = false; //game setting is now not trigged
		}

		//if volume adjustment bar is activated
		if (IsMouseInsideButton(game_setting[3].pos_x, game_setting[3].pos_y,
			game_setting[3].length, game_setting[3].width)) {
			toggle_volume = true; //user now able to toggle volume bar
		}

		//if pitch adjustment bar is activated
		else if (IsMouseInsideButton(game_setting[5].pos_x, game_setting[5].pos_y,
			game_setting[5].length, game_setting[5].width)) {
			toggle_pitch = true; //user now able to toggle pitch bar
		}
	}

	if (toggle_volume) { //if user is adjusting the volume
		if (AEInputCheckReleased(AEVK_LBUTTON)) {
			toggle_volume = false; //user now lose control of adjusting volume if AEVK_LBUTTON is released
		}

		else {
			game_setting[3].pos_x = static_cast<float>(mouseX - AEGfxGetWindowWidth() / dimension_modifer);

			if (game_setting[3].pos_x >= toggle_max) {
				game_setting[3].pos_x = toggle_max;
			}

			if (game_setting[3].pos_x <= toggle_min) {
				game_setting[3].pos_x = toggle_min;
			}
			current_volume = (game_setting[3].pos_x - toggle_min) / (toggle_max - toggle_min); //formula for adjusting volume
		}
	}
	/*------------------------------------------------------------------------------------------*/

	else if (toggle_pitch) { //if user is adjusting the pitch
		if (AEInputCheckReleased(AEVK_LBUTTON)) {
			toggle_pitch = false; //user now lose control of adjusting pitch if AEVK_LBUTTON is released
		}

		else {
			game_setting[5].pos_x = static_cast<float>(mouseX - AEGfxGetWindowWidth() / dimension_modifer);

			if (game_setting[5].pos_x >= toggle_max) {
				game_setting[5].pos_x = toggle_max;
			}

			if (game_setting[5].pos_x <= toggle_min) {
				game_setting[5].pos_x = toggle_min;
			}
			current_pitch = (game_setting[5].pos_x - toggle_min) / (toggle_max - toggle_min); //formula for adjusting pitch
		}
	}
	/*------------------------------------------------------------------------------------------*/

	//render background
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(button_def, {}, {});
	Draw_Rectangle_Shape(setting_pop_up, game_setting[0].length, game_setting[0].width, game_setting[0].pos_x, game_setting[0].pos_y);

	// render exit button
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(Back_Button_Texture, {}, {});
	Draw_Rectangle_Shape(setting_pop_up, game_setting[1].length, game_setting[1].width, game_setting[1].pos_x, game_setting[1].pos_y);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//render everything needed for music volume control for game setting
	Render_Text(game_setting[2].button_name, font, game_setting[2].pos_x / (AEGfxGetWindowWidth() / dimension_modifer),
		game_setting[2].pos_y / (AEGfxGetWindowHeight() / dimension_modifer) + name_offset);
	Draw_Rectangle_Shape(setting_audio, game_setting[2].length, game_setting[2].width, game_setting[2].pos_x, game_setting[2].pos_y);
	Draw_Rectangle_Shape(setting_audio, game_setting[3].length, game_setting[3].width, game_setting[3].pos_x, game_setting[3].pos_y);


	//render everything needed for music pitch control for game setting
	Render_Text(game_setting[4].button_name, font, game_setting[4].pos_x / (AEGfxGetWindowWidth() / dimension_modifer),
		game_setting[4].pos_y / (AEGfxGetWindowHeight() / dimension_modifer) + name_offset);
	Draw_Rectangle_Shape(setting_audio, game_setting[4].length, game_setting[4].width, game_setting[4].pos_x, game_setting[4].pos_y);
	Draw_Rectangle_Shape(setting_audio, game_setting[5].length, game_setting[5].width, game_setting[5].pos_x, game_setting[5].pos_y);
}

void Game_Setting_Exit() {
	if (setting_pop_up) AEGfxMeshFree(setting_pop_up); setting_pop_up = nullptr;
	if (setting_audio) AEGfxMeshFree(setting_audio); setting_audio = nullptr;
}