/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      pausescreen.cpp
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains definitions for functions used to manage buttons 
*				and button clicks happening during pause state.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "AEEngine.h"
#include "rectangle.h"
#include "circle.h"
#include "confirmquit.h"
#include "countdown.h"
#include "guidepage.h"
#include "pipagao.h"
#include "events.h"
#include "virus.h"
#include "textureselect.h"
#include "gamestatemanager.h"
#include "game_setting.h"
#include "level.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "pausescreen.h"
#include <vector>


AEGfxVertexList* rect_mesh {};
AEMtx33 rect_transf_mtx{};
AEGfxVertexList* srect_mesh{};
AEMtx33 srect_transf_mtx{};
AEGfxTexture* bg_texture = nullptr;

float button_left_coor{}, screen_width_scale{}, screen_height_scale{};
float button_y_offset{}, title_height{}, space{}, text_scale{}, text_offset_scale{}, text_offset_y{}, scale{}, color{}, alpha{}, texture_offset{};
float srect_width_scale{}, srect_height_scale{};
unsigned int bg_color{}, button_color{};
static float half{};
std::string pText_1, bg_texture_file{};

std::vector<float> button_pos{};
int button_num{};
std::vector<std::string> buttons;


// Normalize world coordinates
float normalize_coor(float value, float min, float max) {
	return 2 * ((value - min) / (max - min)) -1;
}


// Check if area is clicked
int is_area_clicked(float area_left_x, float area_bottom_y, float area_width, float area_height, float click_x, float click_y)
{
	float boxx_mini = area_left_x;
	float boxx_max = area_left_x + area_width;
	float boxy_mini = area_bottom_y;
	float boxy_max = area_bottom_y + area_height;


	if ((boxx_mini < click_x) && (click_x < boxx_max) && (boxy_mini < click_y) && (click_y < boxy_max))
	{
		return 1;
	}
	return 0;
}

// Load data
void load_data_pausescreen() {
	std::fstream data("Assets/Data/pausescreen.json");
	if (!data) {
		gGameRunning = NULL;
		return;
	}

	rapidjson::IStreamWrapper isw(data);
	rapidjson::Document doc;

	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		return; // Exit the function on parse error
	}

	// Assigning data from file to variable
	half = doc["half"].GetFloat();
	button_left_coor = doc["button_left_coor"].GetFloat();
	screen_width_scale = doc["screen_width_scale"].GetFloat();
	screen_height_scale = doc["screen_height_scale"].GetFloat();
	button_y_offset = doc["button_y_offset"].GetFloat();
	space = doc["space"].GetFloat();
	title_height = doc["game_pause_height"].GetFloat();
	srect_width_scale = doc["srect_width_scale"].GetFloat();
	srect_height_scale = doc["srect_height_scale"].GetFloat();
	scale = doc["scale"].GetFloat();
	text_scale = doc["text_scale"].GetFloat();
	text_offset_scale = doc["text_offset_scale"].GetFloat();
	text_offset_y = doc["text_offset_y"].GetFloat();
	color = doc["color"].GetFloat();
	alpha = doc["alpha"].GetFloat();
	texture_offset = doc["texture_offset"].GetFloat();
	button_num = doc["button_num"].GetInt();
	bg_texture_file = doc["bg_texture_file"].GetString();

	const rapidjson::Value& buttonArray = doc["button_text"];
	for (int i{}; i < static_cast<int>(buttonArray.Size()) && i < button_num; ++i) {
		buttons.push_back(buttonArray[i].GetString());
	}

	pText_1 = doc["game_pause"].GetString();
	bg_color = std::stoul(doc["bg_color"].GetString(), nullptr, 16);
	button_color = std::stoul(doc["button_color"].GetString(), nullptr, 16);
}

void init_pausescreen(int win_width, int win_height) {
	float screen_width = static_cast<float>(win_width) - screen_width_scale;
	float screen_height = static_cast<float>(win_height) - screen_height_scale;
	float srect_width = screen_width * srect_width_scale;
	float srect_height = screen_height * srect_height_scale;

	// background
	if (rect_mesh == nullptr)
		Create_Rectangle(&rect_mesh, screen_width, screen_height, bg_color, &rect_transf_mtx, scale, scale, {}, {});
	if (bg_texture == nullptr)
		bg_texture = AEGfxTextureLoad(bg_texture_file.c_str());

	// buttons
	if (srect_mesh == nullptr)
		Create_Rectangle(&srect_mesh, srect_width, srect_height, button_color, &srect_transf_mtx, scale, scale, {}, {});
}

// Render pause menu
void render_pause_screen(s8 loaded_font, int win_width, int win_height, bool& is_pause) {
	float screen_width = static_cast<float>(win_width) - screen_width_scale;
	float screen_height = static_cast<float>(win_height) - screen_height_scale;
	float srect_width = screen_width * srect_width_scale;
	float srect_height = screen_height * srect_height_scale;

	
	//background
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(bg_texture, texture_offset, texture_offset);
	AEGfxSetTransform(rect_transf_mtx.m);
	AEGfxMeshDraw(rect_mesh, AE_GFX_MDM_TRIANGLES);

	// Render pause title
	f32 w1, h1;
	AEGfxGetPrintSize(loaded_font, pText_1.c_str(), scale, &w1, &h1);
	AEGfxPrint(loaded_font, pText_1.c_str(), -w1 / half, title_height, scale, color, color, color, alpha);
	
	// Render buttons
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	button_pos.clear();
	for (int i{}; i < button_num; ++i) {
		//render button box
		float y_offset = i * (srect_height + space);
		button_pos.push_back((screen_height / half) - screen_height_scale - y_offset);
		AEMtx33Trans(&srect_transf_mtx, {}, button_pos[i]);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(srect_transf_mtx.m);
		AEGfxMeshDraw(srect_mesh, AE_GFX_MDM_TRIANGLES);

		//render button text
		AEGfxGetPrintSize(loaded_font, buttons[i].c_str(), 1.f, &w1, &h1);
		float width = static_cast<float>(w1 / half * AEGfxGetWindowHeight());
		float buttons_x = normalize_coor(-width/ text_offset_scale, -win_width / half, win_width / half);
		float buttons_y = normalize_coor(button_pos[i] - text_offset_y, -win_height / half, win_height / half);
		AEGfxPrint(loaded_font, buttons[i].c_str(), buttons_x, buttons_y, text_scale, color, color, color, alpha);
	}

	if (game_setting_trigger) {		// show setting
		Game_Setting_Update();
	}

	else if (pause_guide_page) {	// show guide page
		set_guidepage();
	}
	else if (confirm_exit_page) {	// show confirmation page for exit
		if (update_confirm_page() == ok_reply) {
			gGameRunning = {};
		}
		else if (update_confirm_page() == cancel_reply) {
			confirm_exit_page = false;
		}
		else {}
		render_confirm_page(font, quit);
	}
	else if (confirm_restart_page) {	// show confirmation page for restart
		if (update_confirm_page() == ok_reply) {
			confirm_restart_page = false;
			GSM_ChangeState(STATE_RESTART);
		}
		else if (update_confirm_page() == cancel_reply) {
			confirm_restart_page = false;
		}
		else {}
		render_confirm_page(font, restart);
	}
	else if (confirm_mm_page) {			// show confirmation page for main menu
		if (update_confirm_page() == ok_reply) {
			confirm_mm_page = false;
			GSM_ChangeState(STATE_MAINMENU);
			return;
		}
		else if (update_confirm_page() == cancel_reply) {
			confirm_mm_page = false;
		}
		else {}
		render_confirm_page(font, quit);
	}


	// Check if cursor clicks button
	else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		int cursor_x, cursor_y;
		
		AEInputGetCursorPosition(&cursor_x, &cursor_y);	

		// Check which button is clicked

		if (is_area_clicked(button_left_coor, button_pos[but_0], srect_width, srect_height, static_cast<float>(cursor_x) - win_width/ half + srect_width / half, win_height / half - static_cast<float>(cursor_y)  + srect_height / half)) {		// resume
			is_pause = false;	
		}
		if (is_area_clicked(button_left_coor, button_pos[but_1], srect_width, srect_height, static_cast<float>(cursor_x) - win_width / half + srect_width / half, win_height / half - static_cast<float>(cursor_y) + srect_height / half)) {		// restart
			confirm_restart_page = true;
			init_confirm_page();
		}
		if (is_area_clicked(button_left_coor, button_pos[but_2], srect_width, srect_height, static_cast<float>(cursor_x) - win_width / half + srect_width / half, win_height / half - static_cast<float>(cursor_y) + srect_height / half)) {		// settings
			game_setting_trigger = true;
		}
		if (is_area_clicked(button_left_coor, button_pos[but_3], srect_width, srect_height, static_cast<float>(cursor_x) - win_width / half + srect_width / half, win_height / half - static_cast<float>(cursor_y) + srect_height / half)) {		// main menu
			confirm_mm_page = true;
			init_confirm_page();
		}
		if (is_area_clicked(button_left_coor, button_pos[but_4], srect_width, srect_height, static_cast<float>(cursor_x) - win_width / half + srect_width / half, win_height / half - static_cast<float>(cursor_y) + srect_height / half)) {		// guide page
			pause_guide_page = true;
			init_guidepage();
		}
		if (is_area_clicked({}, button_pos[but_5], srect_width, srect_height, static_cast<float>(cursor_x) - win_width / half + srect_width / half, win_height / half - static_cast<float>(cursor_y) + srect_height / half)) {		// exit

			confirm_exit_page = true;
			init_confirm_page();
		}

	}

	// Resume game
	if (AEInputCheckTriggered(AEVK_P)) {			
		is_pause = false;
	}
}

// Free pause menu objects
void free_pause_mesh() {
	if (rect_mesh) AEGfxMeshFree(rect_mesh);
	if (srect_mesh) AEGfxMeshFree(srect_mesh);
	rect_mesh = nullptr;
	srect_mesh = nullptr;
	if (bg_texture) AEGfxTextureUnload(bg_texture);
	bg_texture = nullptr;
	Game_Setting_Exit();
}