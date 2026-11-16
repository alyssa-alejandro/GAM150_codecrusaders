/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      circle.h
*@author(s)     Chan Qi Ying (Primary Author, 60%)
*               Ethan Ng (Secondary Author, 40%)
*@DP emails     q.chan@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares functions related to create circle mesh and calculate
*               transformation matrix.
******************************************************************************/
#pragma once

#include "AEEngine.h"

// Load data
void load_data_circle();

// Function to create a circle mesh and transformation matrix
void create_circle_mesh(
    AEGfxVertexList** mesh,     // Pointer to the mesh
    float radius,                // Radius of the circle
    int num_segments,             // Number of segments
    unsigned int center_color,    // Color of the center
    unsigned int edge_color      // Color of the edge
);


// Concatenate matrices
void render(AEMtx33* scale, AEMtx33* rotate, AEMtx33* translate, AEMtx33* transforms, float scalex, float scaley, float xcoord, float ycoord);
