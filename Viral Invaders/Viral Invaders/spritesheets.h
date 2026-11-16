/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      spritesheets.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the spritesheet animation for the events.
*			    It processes textures, calculates UV coordinates for animations and employs
*				a matrix for sprite transformations effectively.
******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "events.h"


typedef struct {
	AEGfxTexture* texture;		// Texture of the spritesheet
	AEGfxVertexList* mesh;		// Mesh for rendering
	int ss_rows;				// Number of rows in the spritesheet
	int ss_cols;				// Number of columns in the spritesheet
	int max_sprite;				// Total number of sprites in the spritesheet
	double width;               // Width of an individual sprite in the spritesheet
	double height;              // Height of an individual sprite in the spritesheet
	double animation_timer;     // Tracks time elapsed for frame-based animation updates
	double duration_ea_frame;   // Duration for each frame in the animation sequence
	int cur_index;				// Start from first sprite
	double cur_uv_x;			// Current horizontal (X) UV offset for the sprite in the texture
	double cur_uv_y;			// Current vertical (Y) UV offset for the sprite in the texture
	float scale_x;				// Scaling factor for the sprite along the X-axis
	float scale_y;				// Scaling factor for the sprite along the Y-axis
	float translation_x;		// X-axis position where the sprite will be rendered
	float translation_y;		// Y-axis position where the sprite will be rendered

} spritesheet_stats;

extern spritesheet_stats ss_data;

// Load data to file
void load_data_spritesheets();

// To create mesh for spritesheet
void ss_init(spritesheet_stats* event_ss);

// To create the animation effect
void ss_update(spritesheet_stats* event_ss, double dt);

// To display
void ss_render(spritesheet_stats* event_ss);

// To free the resource
void ss_free(spritesheet_stats* event_ss);