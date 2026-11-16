/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      circle.cpp
*@author(s)     Chan Qi Ying (Primary Author, 60%)
*               Ethan Ng (Secondary Author, 40%)
*@DP emails     q.chan@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines functions related to create circle mesh and calculate
*               transformation matrix.
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "AEEngine.h"
#include "level.h"
#include <math.h> // For cosf and sinf
#include <iostream>      
#include <fstream>      
#include <sstream>  


static float coor{}, pi{}, pi_modifier{};
static int modifier;
// Load data
void load_data_circle() {
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
    modifier = doc["modifier"].GetInt();
    pi_modifier = doc["pi_modifier"].GetFloat();

}

// Create circle mesh
void create_circle_mesh(AEGfxVertexList** mesh, float radius, int num_segments, unsigned int center_color, unsigned int edge_color)
{

    // Start defining the mesh
    AEGfxMeshStart();

    // Center vertex of the circle
    float centerX{};
    float centerY{};

    // Add the center point as the first vertex (triangle fan origin)
    AEGfxTriAdd(
        centerX, centerY, center_color, coor, coor,  // Center of the circle
        centerX, centerY, center_color, coor, coor,  // Placeholder
        centerX, centerY, center_color, coor, coor   // Placeholder
    );

    // Add vertices for the circumference of the circle
    float angleStep = (pi_modifier * pi) / num_segments; // Angle between each segment
    for (int i {}; i < num_segments; ++i)
    {
        // Current vertex on the circumference
        float x1 = radius * cosf(i * angleStep);
        float y1 = radius * sinf(i * angleStep);

        // Next vertex on the circumference
        float x2 = radius * cosf((i + modifier) * angleStep);
        float y2 = radius * sinf((i + modifier) * angleStep);

        // Add the triangle for this segment
        AEGfxTriAdd(
            centerX, centerY, center_color, coor, coor,  // Center of the circle
            x1, y1, edge_color, coor + x1, coor + y1,   // Current vertex
            x2, y2, edge_color, coor + x2, coor + y2    // Next vertex
        );
        
    }



    // Finalize the mesh
    *mesh = AEGfxMeshEnd();
}

// Concatenate matrices
void render(AEMtx33* scale, AEMtx33* rotate, AEMtx33* translate, AEMtx33* transforms, float scalex, float scaley, float xcoord, float ycoord) {
    {
        //AEMtx33 scale, rotate, translate;
        AEMtx33Scale(scale, scalex, scaley);
        AEMtx33Trans(translate, xcoord, ycoord);
        AEMtx33Rot(rotate, {});

        AEMtx33Concat(transforms, rotate, scale);
        AEMtx33Concat(transforms, translate, &transforms[0]);

    }
}

