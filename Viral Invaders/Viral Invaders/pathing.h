/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      pathing.h
*@author(s)     Dalton Koh (primary Author)
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines Path info struct as well as details the declarations
*               required for path creating!
******************************************************************************/

#pragma once

struct Path_Info {
    int start_Row;
    int start_Col;
    int end_Col;
    int path_Length;
    int* path_Coords; // Dynamic array for path coordinates
    unsigned int path_Color;
};

enum num_set {
    num_0 = 0,
    num_1,
    num_2
};

void load_data_path();
void Init_Path(SG_stats** grid, int rows, int cols, int* path_Coords, int path_Length, unsigned int path_Color);
void path_start(SG_stats** grid, int square_rows, int square_columns);
void Free_Path_Coords(int* path_Coords);
