/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      spritesheets.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)       
*@DP emails     alejandro.a@digipen.edu           
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines and handles the spritesheet animation for the events.
*			    It processes textures, calculates UV coordinates for animations and employs
*				a matrix for sprite transformations effectively.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"

#include "AEEngine.h"
#include "spritesheets.h"
#include "events.h"
#include <stdio.h>
#include <fstream>      
#include <sstream>  
#include <iostream>
#include "level.h"

spritesheet_stats ss_data;

// Variables for mesh
// XY coord (position)
static float coor_min{}; // Neg value
static float coor_max{}; // Pos value
// UV coord (texture), range of 0.0 to 1.0
static float uv_min{};	 // 0.0
static float uv_max{};	 // 1.0
// Color for mesh
static unsigned int color{};

// Load data from json file
void load_data_spritesheets() {
    std::fstream data("Assets/Data/spritesheets.json");
    if (!data) {
        gGameRunning = NULL;
        return; // Exit if the file couldn't be opened
    }

    rapidjson::IStreamWrapper isw(data);
    rapidjson::Document doc;

    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return; // Exit on parsing error
    }

		ss_data.ss_cols = doc["ss_cols"].GetInt();
		ss_data.ss_rows = doc["ss_rows"].GetInt();
		ss_data.max_sprite = doc["max_sprite"].GetInt();
		ss_data.width = doc["width"].GetDouble();
		ss_data.height = doc["height"].GetDouble();
		ss_data.animation_timer = doc["animation_timer"].GetDouble();
		ss_data.duration_ea_frame = doc["duration_ea_frame"].GetDouble();
		ss_data.cur_index = doc["cur_index"].GetInt();
		ss_data.cur_uv_x = doc["cur_uv_x"].GetDouble();
		ss_data.cur_uv_y = doc["cur_uv_y"].GetDouble();
		ss_data.scale_x = doc["scale_x"].GetFloat();
		ss_data.scale_y = doc["scale_y"].GetFloat();
		ss_data.translation_x = doc["translation_x"].GetFloat();
		ss_data.translation_y = doc["translation_y"].GetFloat();

		coor_min = doc["coor_min"].GetFloat();
		coor_max = doc["coor_max"].GetFloat();
		uv_min = doc["uv_min"].GetFloat();
		uv_max = doc["uv_max"].GetFloat();
		color = std::stoul(doc["color"].GetString(), nullptr, 16);
}

void ss_init(spritesheet_stats* event_ss) {

	// Set texture and mesh to nullptr first
	event_ss->texture = nullptr;
	event_ss->mesh = nullptr;

	// Start defining the mesh
	AEGfxMeshStart();

	// Add the center point as the first vertex (triangle fan origin)
	AEGfxTriAdd(
		coor_min, coor_min, color, uv_min, uv_max / event_ss->ss_cols,						// bottom left
		coor_max, coor_min, color, uv_max / event_ss->ss_rows, uv_max / event_ss->ss_cols,  // bottom right
		coor_min, coor_max, color, uv_min, uv_min											// top left
	);

	// Continue with the second half of the triangle
	AEGfxTriAdd(
		coor_max, coor_min, color, uv_max / event_ss->ss_rows, uv_max / event_ss->ss_cols,	// bottom right
		coor_max, coor_max, color, uv_max / event_ss->ss_rows, uv_min,						// top right
		coor_min, coor_max, color, uv_min, uv_min											// top left
	);

	// Finalize the mesh and store in mesh
	event_ss->mesh = AEGfxMeshEnd();
}

void ss_update(spritesheet_stats* event_ss, double dt) {

	// Update the animation timer in every frame time
	event_ss->animation_timer += dt;

	// Check if the animation timer has exceeded or reached the duration for a single frame
	if (event_ss->animation_timer >= event_ss->duration_ea_frame) {

		// Reset the animation timer by subtracting itself (effectively setting it to 0)
		event_ss->animation_timer -= event_ss->animation_timer;

		// Increment the current sprite index to move to the next frame
		// Use % to loop back to the first frame if the end of the spritesheet is reached
		event_ss->cur_index = (++event_ss->cur_index) % event_ss->max_sprite;

		// Calculate the row of the current sprite on the spritesheet
		double cur_sprite_row = event_ss->cur_index / event_ss->ss_cols;

		// Calculate the column of the current sprite on the spritesheet
		double cur_sprite_col = event_ss->cur_index % event_ss->ss_cols;

		// Update the UV X-coordinate for the current frame by multiplying the column by the sprite width
		event_ss->cur_uv_x = event_ss->width * cur_sprite_col;

		// Update the UV Y-coordinate for the current frame by multiplying the row by the sprite height
		event_ss->cur_uv_y = event_ss->height * cur_sprite_row;
	}
}

void ss_render(spritesheet_stats* event_ss) {

	// Check if the texture exists
	if (event_ss->texture) {

		// Calculate the row and column of the current sprite in the spritesheet
		int row = event_ss->cur_index / event_ss->ss_cols;
		int cols = event_ss->cur_index % event_ss->ss_rows;

		// Calculate the width and height of each sprite in the spritesheet
		float sprite_width =  uv_max/ event_ss->ss_cols;
		float sprite_height = uv_max / event_ss->ss_rows;

		// Calculate the width and height of each sprite in the spritesheet
		float uv_offset_x = cols * sprite_width;
		float uv_offset_y = row * sprite_height;

		// Set the rendering mode to texture
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		// Assign the texture and its UV offsets for rendering
		AEGfxTextureSet(event_ss->texture, uv_offset_x, uv_offset_y);

		AEMtx33 scale, translation, transform;

		// Apply scaling transformation based on the sprite's scale factors
		AEMtx33Scale(&scale, event_ss->scale_x, event_ss->scale_y);

		// Apply translation to position the sprite at the specified location
		AEMtx33Trans(&translation, event_ss->translation_x, event_ss->translation_y);

		// Combine the translation and scale transformations
		AEMtx33Concat(&transform, &translation, &scale);

		// Set the final transformation matrix for rendering
		AEGfxSetTransform(transform.m);

		// Draw the mesh for the sprite using triangles
		AEGfxMeshDraw(event_ss->mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void ss_free(spritesheet_stats* event_ss) {

	// Free the mesh memory if it exists, then set the pointer to null
	if (event_ss->mesh) AEGfxMeshFree(event_ss->mesh); event_ss->mesh = nullptr;

	// Unload the texture if it exists, then set the pointer to null
	if (event_ss->texture) AEGfxTextureUnload(event_ss->texture); event_ss->texture = nullptr;
}