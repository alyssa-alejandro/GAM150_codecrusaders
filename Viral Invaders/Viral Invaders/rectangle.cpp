/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      rectangle.cpp
*@author(s)     Alyssa Nicole Alejandro (Primary Author, 100%)
*@DP emails     alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines functions related to create rectangle mesh and 
*               calculate transformation matrix.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "level.h"
#include <fstream>
#include <ostream>
#include "rectangle.h"
#include <stdio.h>
#include <iostream>

// Divides the width and height in the mesh by 2
static int half{};

// Range of 0.0 to 1.0
static float full_val{};  // 1.0
static float empty_val{}; // 0.0

// Load data from json file
void load_data_rectangle() {
    std::fstream data("Assets/Data/rectangle.json");
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
    half = doc["half"].GetInt();
    full_val = doc["full_val"].GetFloat();
    empty_val = doc["empty_val"].GetFloat();
}

void Create_Rectangle(AEGfxVertexList** pMesh, float width, float height, unsigned int color,
    AEMtx33* transform, float scaleX, float scaleY, float posX, float posY)
{
    // Start defining the mesh
    AEGfxMeshStart();

    // Add the center point as the first vertex (triangle fan origin)
    AEGfxTriAdd(
        -width / half, -height / half, color, empty_val, empty_val, // bottom left
        width / half, -height / half, color, full_val, empty_val,   // bottom right
        -width / half, height / half, color, empty_val, full_val);  // top left

    // Continue with the second half of the triangle
    AEGfxTriAdd(
        width / half, -height / half, color, full_val, empty_val,   // bottom right
        width / half, height / half, color, full_val, full_val,     // top right
        -width / half, height / half, color, empty_val, full_val);  // top left

    // Finalize the mesh and store in pMesh
    *pMesh = AEGfxMeshEnd();

    // Apply the scale and transform values that will be passed in
    AEMtx33Scale(transform, scaleX, scaleY);
    AEMtx33TransApply(transform, transform, posX, posY);
}


