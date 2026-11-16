/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      mesh.h
*@author(s)     Chan Qi Ying (Primary Author, 50%)
*               Ang Yu Xuan (Secondary Author, 50%)
*@DP emails     q.chan@digipen.edu
*               yuxuan.ang@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares functions related to create circle and rectangle mesh
******************************************************************************/
#pragma once
#include "AEEngine.h"

void load_data_mesh();

AEGfxVertexList* draw_circle_mesh(float circle_x, float circle_y, float radius, int segments, unsigned int color_center, unsigned int edge_color);

AEGfxVertexList* Draw_Square_Mesh(u32 colour);
void Draw_Rectangle_Shape(AEGfxVertexList* pVertex_List, f32 Rectangle_length, f32 Rectangle_width,
	f32 Rectangle_Xpos, f32 Rectangle_Ypos);




