/******************************************************************************
* All content � 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      confirmquit.cpp
*@author(s)     Chan Qi Ying (Primary Author, 100%)
*@DP emails     q.chan@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines functions related to confirmation page pop up
*				when users are about to do a action that makes them lose progress.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "AEEngine.h"
#include "confirmquit.h"
#include "rectangle.h"
#include "level.h"
#include "pausescreen.h"
#include <iostream>
#include <fstream>
#include <string>

bool confirm_exit_page = false;
bool confirm_restart_page = false;
bool confirm_mm_page = false;

static AEGfxVertexList* rect_mesh{};
static AEGfxVertexList* confirm_rect_mesh {};
static AEGfxVertexList* cancel_rect_mesh {};
static AEMtx33 rect_transf_mtx {};
static AEMtx33 srect_transf_mtx {};
static AEGfxTexture* bg_texture = nullptr;


static float screen_width{}, screen_height{}, half{}, quarter{};
static float button_y_offset{}, title_height{}, text_scale{}, text_offset_scale{}, text_offset_y{}, long_text_scale{}, scale{}, color{}, alpha{}, texture_offset{};
static float rect_width_scale{}, srect_width_scale{}, srect_height_scale{};
static unsigned int bg_color{}, confirm_color{}, cancel_color{};
static std::string long_exit_text{}, long_restart_text{}, cancel{}, confirm{}, bg_texture_file{};

// Load data
void load_data_confirm_quit() {
	std::fstream data("Assets/Data/confirmquit.json");
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
	quarter = doc["quarter"].GetFloat();
	title_height = doc["long_text_height"].GetFloat();
	rect_width_scale = doc["rect_width_scale"].GetFloat();
	srect_width_scale = doc["srect_width_scale"].GetFloat();
	srect_height_scale = doc["srect_height_scale"].GetFloat();
	scale = doc["scale"].GetFloat();
	long_text_scale = doc["long_text_scale"].GetFloat();
	text_scale = doc["text_scale"].GetFloat();
	text_offset_scale = doc["text_offset_scale"].GetFloat();
	text_offset_y = doc["text_offset_y"].GetFloat();
	color = doc["color"].GetFloat();

	const rapidjson::Value& buttonArray = doc["button_text"];
	cancel = buttonArray[0].GetString();
	confirm = buttonArray[1].GetString();

	long_exit_text = doc["long_exit_text"].GetString();
	long_restart_text = doc["long_restart_text"].GetString();
	bg_color = std::stoul(doc["bg_color"].GetString(), nullptr, 16);
	confirm_color = std::stoul(doc["confirm_color"].GetString(), nullptr, 16);
	cancel_color = std::stoul(doc["cancel_color"].GetString(), nullptr, 16);

	color = doc["color"].GetFloat();
	alpha = doc["alpha"].GetFloat();
	texture_offset = doc["texture_offset"].GetFloat();
	bg_texture_file = doc["bg_texture_file"].GetString();

}

// Initializations for confirmation page
void init_confirm_page() {
	screen_width = AEGfxGetWinMaxX() * rect_width_scale;
	screen_height = AEGfxGetWinMaxY();
	if (rect_mesh == nullptr) {
		Create_Rectangle(&rect_mesh, screen_width, screen_height, bg_color, &rect_transf_mtx, scale, scale, texture_offset, texture_offset);
	}
	if (confirm_rect_mesh == nullptr)
		Create_Rectangle(&confirm_rect_mesh, screen_width * srect_width_scale, screen_height * srect_height_scale, confirm_color, &srect_transf_mtx, scale, scale, texture_offset, texture_offset);
	if (cancel_rect_mesh == nullptr)
		Create_Rectangle(&cancel_rect_mesh, screen_width * srect_width_scale, screen_height * srect_height_scale, cancel_color, &srect_transf_mtx, scale, scale, texture_offset, texture_offset);

	if (bg_texture == nullptr) bg_texture = AEGfxTextureLoad(bg_texture_file.c_str());
}

// Return value base on action made
int update_confirm_page() {

	float srect_width = screen_width * srect_width_scale;
	float srect_height = screen_height * srect_height_scale;
	float cancel_pos_x = -screen_width / quarter;
	float confirm_pos_x = screen_width / quarter;
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		int cursor_x, cursor_y;

		AEInputGetCursorPosition(&cursor_x, &cursor_y);
		// Check which button is clicked
		if (is_area_clicked(cancel_pos_x, -button_y_offset, srect_width, srect_height, static_cast<float>(cursor_x) - AEGfxGetWinMaxX() + srect_width / half, AEGfxGetWinMaxY() - static_cast<float>(cursor_y) + srect_height / half)) {		// cancel
			return cancel_reply;
		}
		if (is_area_clicked(confirm_pos_x, -button_y_offset, srect_width, srect_height, static_cast<float>(cursor_x) - AEGfxGetWinMaxX() + srect_width / half, AEGfxGetWinMaxY() - static_cast<float>(cursor_y) + srect_height / half)) {		// confirm
			return ok_reply;
		}
	}
	return no_reply;
}

// Render confirm page
void render_confirm_page(s8 loaded_font, action action) {
	float cancel_pos_x = -screen_width / quarter;
	float confirm_pos_x = screen_width / quarter;
	float width{}, button_x{}, button_y{};

	// Render background
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEMtx33Trans(&rect_transf_mtx, {}, {});
	AEGfxSetTransform(rect_transf_mtx.m);
	AEGfxTextureSet(bg_texture, texture_offset, texture_offset);
	AEGfxMeshDraw(rect_mesh, AE_GFX_MDM_TRIANGLES);

	// Print question text
	f32 w1, h1;
	if (action == restart) {
		AEGfxGetPrintSize(loaded_font, long_restart_text.c_str(), scale, &w1, &h1);
		AEGfxPrint(loaded_font, long_restart_text.c_str(), -w1 / half, title_height, long_text_scale, color, color, color, alpha);
	}
	else { // quit
		AEGfxGetPrintSize(loaded_font, long_exit_text.c_str(), scale, &w1, &h1);
		AEGfxPrint(loaded_font, long_exit_text.c_str(), -w1 / half, title_height, long_text_scale, color, color, color, alpha);
	}
	
	// Render button
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	button_y = normalize_coor(-button_y_offset - text_offset_y, static_cast<float>(AEGfxGetWinMinY()), static_cast<float>(AEGfxGetWinMaxY()));
	
	//render button - cancel
	// Rectangle
	AEMtx33Trans(&rect_transf_mtx, cancel_pos_x, button_y_offset);
	AEGfxSetTransform(rect_transf_mtx.m);
	AEGfxMeshDraw(cancel_rect_mesh, AE_GFX_MDM_TRIANGLES);
	// Text
	AEGfxGetPrintSize(loaded_font, cancel.c_str(), scale, &w1, &h1);
	width = static_cast<float>(w1 / half * AEGfxGetWindowHeight());
	button_x = normalize_coor(cancel_pos_x - width / text_offset_scale, static_cast<float>(AEGfxGetWinMinX()), static_cast<float>(AEGfxGetWinMaxX()));
	AEGfxPrint(loaded_font, cancel.c_str(), button_x, button_y, text_scale, color, color, color, alpha);

	//render button box - confirm
	// Rectangle
	AEMtx33Trans(&rect_transf_mtx, confirm_pos_x, button_y_offset);
	AEGfxSetTransform(rect_transf_mtx.m);
	AEGfxMeshDraw(confirm_rect_mesh, AE_GFX_MDM_TRIANGLES);
	// Text
	AEGfxGetPrintSize(loaded_font, cancel.c_str(), scale, &w1, &h1);
	width = static_cast<float>(w1 / half * AEGfxGetWindowHeight());
	button_x = normalize_coor(confirm_pos_x - width / text_offset_scale, static_cast<float>(AEGfxGetWinMinX()), static_cast<float>(AEGfxGetWinMaxX()));
	AEGfxPrint(loaded_font, confirm.c_str(), button_x, button_y, text_scale, color, color, color, alpha);
}

// Free memory assigned to confirm page objects
void free_confirm_page() {
	if (rect_mesh) AEGfxMeshFree(rect_mesh);
	rect_mesh = nullptr;
	if (cancel_rect_mesh) AEGfxMeshFree(cancel_rect_mesh);
	rect_mesh = nullptr;
	if (confirm_rect_mesh) AEGfxMeshFree(confirm_rect_mesh);
	confirm_rect_mesh = nullptr;
	if (bg_texture) AEGfxTextureUnload(bg_texture);
	bg_texture = nullptr;
	
}