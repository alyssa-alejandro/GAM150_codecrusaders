/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      pathing.cpp
*@author(s)     Dalton Koh (primary Author)
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the main pathing code for every level in the 
*               game. We have presetting code which helps set up each level's
*               pathing. All viruses walk on the path, all towers avoid the 
*               paths!!!!
******************************************************************************/

#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <sstream>
#include "level.h"
#include "squaregrid.h"
#include "rectangle.h"
#include "AEEngine.h"
#include "textureselect.h"
#include <iostream>
#include "levelselector.h"
#include "pathing.h"
#include "gamestatemanager.h"

// Path values
static int path_value_1{}; 
static int path_value_2{}; 
static int path_value_3{}; 
static int path_value_4{}; 
static int path_value_5{}; 
static int path_value_6{}; 

// Path colours 
static unsigned int colour_1{};
static unsigned int colour_2{};

// Scale
static float scale{};

// Load Json Data
void load_data_path() {

    // File stream
    std::fstream data("Assets/Data/pathing.json");

    // If files fails to open
    if (!data) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(data); // wrap file stream into something rapid json can read from 
    rapidjson::Document doc; // Create json object
    doc.ParseStream(isw); // Parses contents from the isw stream into doc. so you can access it later on

    // If parse error or error getting the right data type
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return; 
    }

    // Required data to be retrieved 
    path_value_1 = doc["path_value_1"].GetInt();
    path_value_2 = doc["path_value_2"].GetInt();
    path_value_3 = doc["path_value_3"].GetInt();
    path_value_4 = doc["path_value_4"].GetInt();
    path_value_5 = doc["path_value_5"].GetInt();
    path_value_6 = doc["path_value_6"].GetInt();
    colour_1 = doc["colour_1"].GetUint();
    colour_2 = doc["colour_2"].GetUint();
    scale = doc["scale"].GetFloat();
}


// Path init
void Init_Path(SG_stats** grd, int rows, int cols, int* path_Coords, int path_Length, unsigned int path_Color) {
    
    // Ensure grid is not null
    if (!grd) {
        return;
    }
    

    
    // Iterate through the list of path coordinates
    for (int i{}; i < path_Length; ++i) {
        int row = path_Coords[i * num_2];      // Extract row from coordinate array
        int col = path_Coords[i * num_2 + num_1]; // Extract column from coordinate array

        // Validate row and column indices
        if (row >= NULL && row < rows&& col >= NULL && col < cols) {
            grid[row][col].pathing = true; // Set the pathing flag
            grid[row][col].color = path_Color; // Optionally change the color for visual feedback

            // Update the cell's mesh to reflect the new color
            if (grid[row][col].mesh) {
                AEGfxMeshFree(grid[row][col].mesh);
            }
           
            // Vertex list and transform
            AEGfxVertexList* Path_Mesh = NULL;
            AEMtx33 transform;

            // Create path
            Create_Rectangle(
                &Path_Mesh,
                grid[row][col].width,
                grid[row][col].height,
                path_Color,
                &transform,
                scale,
                scale,
                grid[row][col].x_cordinate,
                grid[row][col].y_cordinate
            );
            grid[row][col].mesh = Path_Mesh;
        }
    }
}

// To set the pathing of different levels
void path_start(SG_stats** game_grid, int square_row, int square_column) {
     
    Path_Info paths[num_2]; // Store path information for two paths

    switch (current_State) {
    case STATE_LEVEL1: // Level 1 Pathing
        paths[num_0] = { path_value_2, {}, path_value_5, {}, {}, colour_1 };
        paths[num_1] = { path_value_3, {}, path_value_6, {}, {}, colour_2 };
        break;

    case STATE_LEVEL2: // Level 2 Pathing
        paths[num_0] = { path_value_2, {}, path_value_5, {}, {}, colour_1 };
        paths[num_1] = { path_value_3, {}, path_value_6, {}, {}, colour_2 };
        break;

    case STATE_LEVEL3: // Level 3 Pathing
        paths[num_0] = { path_value_1, {}, path_value_5, {}, {}, colour_1};
        paths[num_1] = { path_value_4, {}, path_value_6, {}, {}, colour_2};
        break;
    case STATE_LEVEL4: // Level 4 Pathing
        paths[num_0] = { path_value_2, {}, path_value_5, {}, {}, colour_1 };
        paths[num_1] = { path_value_3, {}, path_value_6, {}, {}, colour_2 };
        break;
    case STATE_LEVEL5: // Level 5 Pathing
        paths[num_0] = { path_value_2, {}, path_value_5, {}, {}, colour_1 };
        paths[num_1] = { path_value_3, {}, path_value_6, {}, {}, colour_2 };
        break;
    case STATE_LEVEL6: // Level 6 Pathing
        paths[num_0] = { path_value_2, {}, path_value_5, {}, {}, colour_1 };
        paths[num_1] = { path_value_3, {}, path_value_6, {}, {}, colour_2 };
        break;

    default:
        return;
    }

    // Loop through both paths to set paths basically 2D array
    for (int i{}; i < num_2; ++i) {
        paths[i].path_Length = paths[i].end_Col - paths[i].start_Col + 1;
        paths[i].path_Coords = (int*)malloc(paths[i].path_Length * 2 * sizeof(int));

        if (!paths[i].path_Coords) { // NULL check
            return;
        }

        for (int j{}; j < paths[i].path_Length; ++j) {
            paths[i].path_Coords[j * num_2] = paths[i].start_Row;
            paths[i].path_Coords[j * num_2 + num_1] = paths[i].start_Col + j;
        }

        Init_Path(game_grid, square_row, square_column, paths[i].path_Coords, paths[i].path_Length, paths[i].path_Color);
    }

    // Store pointers for freeing later
    path_Coords1 = paths[num_0].path_Coords;
    path_Coords2 = paths[num_1].path_Coords;

}

// Free path
void Free_Path_Coords(int* path_Coords) {

    if (path_Coords != NULL) {
        free(path_Coords);
        path_Coords = nullptr; // Prevent double free errors
    }
}