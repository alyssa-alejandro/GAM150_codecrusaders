/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      squaregrid.cpp
*@author(s)     Dalton Koh (Primary Author, 100%)
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the main grid code for every level in the
*               game. We have presetting code which helps set up each level's
*               grid. all viruses and towers exist on the grid with the 
*               functionality of the code!
******************************************************************************/

#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <sstream>
#include "level.h"

#include "squaregrid.h"
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf
#include "rectangle.h"
#include "textureselect.h"
#include <iostream>

// Simple variables for positioning and offset
static float grid_Width{};
static float grid_Height{};
static float offset_2{};
static float scale_1{};

// Json load function
void load_data_grid() {

    // File stream
    std::fstream file("Assets/Data/grid.json");

    // IF file fails to open
    if (!file) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(file); // wrap file stream into something rapid json can read from 
    rapidjson::Document doc; // Create json object
    doc.ParseStream(isw); // Parses contents from the isw stream into doc. so you can access it later on


    // If parse error or error getting the right data type
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return;
    }

    // Required data to be retrieved 
    offset_2 = doc["offset_2"].GetFloat();
    scale_1 = doc["scale_1"].GetFloat();

}


// Create the grid
SG_stats** Create_Grid(int rows, int cols, float cell_Width, float cell_Height, unsigned int color) {

    // Allocate memory for the grid
    SG_stats** grid_Create = (SG_stats**)malloc(rows * sizeof(SG_stats*));
    if (!grid_Create) {
        return NULL;
    }
   
    // Set grid width and height
     grid_Width = cols * cell_Width;
     grid_Height = rows * cell_Height;


    // Calculate the top-left corner of the grid to center it in the coordinate system (-800, 800) and (-450, 450)
    float start_X = -grid_Width / offset_2;
    float start_Y = -grid_Height / offset_2;

    // Create 2d array grid
    for (int i{}; i < rows; ++i) {
        grid_Create[i] = (SG_stats*)malloc(cols * sizeof(SG_stats));
        if (!grid_Create[i]) {
            return NULL;
        }
        for (int j{}; j < cols; ++j) {
            grid_Create[i][j].width = cell_Width;
            grid_Create[i][j].height = cell_Height;

            // Calculate the position for each cell
            grid_Create[i][j].x_cordinate = start_X + j * cell_Width + cell_Width / offset_2;
            grid_Create[i][j].y_cordinate = start_Y + i * cell_Height + cell_Height / offset_2;

            grid_Create[i][j].color = color;
            grid_Create[i][j].mesh = NULL;
            grid_Create[i][j].is_Occupied = {};
            grid_Create[i][j].pathing = false; // Default to non-pathing
            grid_Create[i][j].occupant = NULL;

            // Create the mesh for each cell
            AEGfxVertexList* cell_Mesh = NULL;
            AEMtx33 transform;
            Create_Rectangle(&cell_Mesh, cell_Width, cell_Height, color, &transform, scale_1, scale_1,
                grid_Create[i][j].x_cordinate, grid_Create[i][j].y_cordinate);
            grid_Create[i][j].mesh = cell_Mesh;
            grid_Create[i][j].transform = transform;

         
        }
    }
    return grid_Create;
}


// Render the grid
void Render_Grid(SG_stats** grid, int rows, int cols) {
    for (int row{}; row < rows; ++row) {
        for (int col{}; col < cols; ++col) {

            // Set the transformation matrix
            AEGfxSetTransform(grid[row][col].transform.m);

            // Bind the appropriate texture
            if (grid[row][col].pathing) {
                AEGfxTextureSet(path_texture, {}, {}); // Use pathing texture
             }
            else {
                AEGfxTextureSet(grid_texture, {}, {}); // Use grid texture
             }

            // Draw the mesh
            AEGfxMeshDraw(grid[row][col].mesh, AE_GFX_MDM_TRIANGLES);
        }
    }
}

// Free the grid
void Free_Grid(SG_stats** grid, int rows, int cols) {
    if (grid == NULL) {
        return;
    }

    for (int row{}; row < rows; ++row) {
        if (grid[row] == NULL) {
            continue;
        }

        for (int col{}; col < cols; ++col) {
            if (grid[row][col].mesh) {
                AEGfxMeshFree(grid[row][col].mesh);
                grid[row][col].mesh = NULL;
            }
        }

        free(grid[row]);
        grid[row] = NULL;
    }

    free(grid);
    grid = NULL;
   
}
