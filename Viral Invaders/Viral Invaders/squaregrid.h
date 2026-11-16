/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      squaregrid.h
*@author(s)     Dalton Koh (Primary Author, 100%)
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file includes all the function declarations for grid 
*               creation. The file also includes SG_stats which is the struct
*               to contain variables for our grid object!
******************************************************************************/

#pragma once

#include "AEEngine.h"

// Grid struct definition
struct SG_stats {
    float width;
    float height;
    float x_cordinate;
    float y_cordinate;
    unsigned int color;
    AEGfxVertexList* mesh;
    AEMtx33 transform;
    int is_Occupied;
    void* occupant;
    bool pathing;
};

// Init, render and free functions for the grid
SG_stats** Create_Grid(int rows, int cols, float cellWidth, float cellHeight, unsigned int color);
void Render_Grid(SG_stats** grid, int rows, int cols);
void Free_Grid(SG_stats** grid, int rows, int cols);

// Json grid file
void load_data_grid();
