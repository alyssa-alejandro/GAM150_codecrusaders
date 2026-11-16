/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      mesh.cpp
*@author(s)     Chan Qi Ying (Primary Author, 50%)
*               Ang Yu Xuan (Secondary Author, 50%)
*@DP emails     q.chan@digipen.edu
*               yuxuan.ang@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines functions related to create circle and rectangle mesh
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "level.h"
#include <cmath>
#include "AEEngine.h"
#include <iostream>      
#include <fstream>      
#include <sstream> 


static float coor{}, pi{}, uv_min{}, uv_max{}, pi_modifier{};

void load_data_mesh() {
	std::fstream data("Assets/Data/mesh.json");
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
	coor = doc["coordinate"].GetFloat();
	pi = doc["pi"].GetFloat();
	uv_min = doc["uv_min"].GetFloat();
	uv_max = doc["uv_max"].GetFloat();
	pi_modifier = doc["pi_modifier"].GetFloat();

}

AEGfxVertexList* draw_circle_mesh(float circle_x, float circle_y, float radius, int segments, unsigned int color_center, unsigned int edge_color) {
	AEGfxMeshStart();

	float prev_x{}, prev_y = radius, angle = {};

	prev_x = radius * cosf(angle);				
	prev_y = radius * sinf(angle);
	for (int i{}; i <= segments; ++i) {
		angle = pi_modifier * pi * float(i) / float(segments);
		float new_x = radius * cosf(angle);		//calculate the x component
		float new_y = radius * sinf(angle);		//calculate the y component

		AEGfxTriAdd(
			circle_x, circle_y, edge_color, uv_max, uv_max,
			prev_x, prev_y, color_center, uv_max, uv_min,
			new_x, new_y, color_center, uv_min, uv_min);

		prev_x = new_x;
		prev_y = new_y;
	}

	return AEGfxMeshEnd();
}


AEGfxVertexList* Draw_Square_Mesh(u32 colour) {
	// We inform Alpha Engine that we are going to create a mesh
	AEGfxMeshStart();

	//using 2 triangles to draw a square
	AEGfxTriAdd(
		-coor, -coor, colour, uv_min, uv_max,
		coor, -coor, colour, uv_max, uv_max,
		-coor, coor, colour, uv_min, uv_min);

	AEGfxTriAdd(
		coor, -coor, colour, uv_max, uv_max,
		coor, coor, colour, uv_max, uv_min,
		-coor, coor, colour, uv_min, uv_min);

	// Returning the mesh (list of triangles) as a AEGfxVertexList pointer to the new mesh
	return AEGfxMeshEnd();
}

void Draw_Rectangle_Shape(AEGfxVertexList* pVertex_List, f32 Rectangle_length, f32 Rectangle_width,
	f32 Rectangle_Xpos, f32 Rectangle_Ypos)
{
	AEMtx33 transforms = {};

	AEMtx33 scale = {};
	AEMtx33Scale(&scale, Rectangle_length, Rectangle_width); //scaling

	AEMtx33 translate = {};
	AEMtx33Trans(&translate, Rectangle_Xpos, Rectangle_Ypos); //positioning

	AEMtx33Concat(&transforms, &translate, &scale); // Combine translation and scaling matrices
	AEGfxSetTransform(transforms.m); // Apply the transformation
	AEGfxMeshDraw(pVertex_List, AE_GFX_MDM_TRIANGLES);
}