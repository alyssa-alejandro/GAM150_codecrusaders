/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      rectangle.h
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares functions related to create rectangle mesh and 
*               calculate transformation matrix.
******************************************************************************/
#pragma once
#include "AEEngine.h"

// Function to create a rectangle
void Create_Rectangle(
    AEGfxVertexList** pMesh,   // Pointer to the mesh
    float width,               // Width of the rectangle
    float height,              // Height of the rectangle
    unsigned int color,        // Color of the rectangle (ARGB format)
    AEMtx33* transform,        // Output transformation matrix
    float scaleX,              // Scale factor in X
    float scaleY,              // Scale factor in Y
    float translateX,          // Translation in X
    float translateY           // Translation in Y
);

// Load data to file
void load_data_rectangle();

// To create a rectangle
void Create_Rectangle(AEGfxVertexList** pMesh, float width, float height, unsigned int color,
    AEMtx33* transform, float scaleX, float scaleY, float posX, float posY);