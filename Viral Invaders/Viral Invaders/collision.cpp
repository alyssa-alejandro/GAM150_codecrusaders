/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      collision.cpp
*@author(s)     Chan Qi Ying (Primary Author, 80%)
*               Ethan Ng (Secondary Author, 20%)
*@DP emails     q.chan@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines functions to determine intersection between circles, 
*               circle and rectangle, and point to rectangle
******************************************************************************/
#include "document.h"
#include "istreamwrapper.h"
#include "level.h"
#include <cmath>
#include <iostream>      
#include <fstream>      
#include <sstream>  


static float offset_x{}, offset_y{}, dimension_modifier{};

// Load data 
void load_data_collision() {
    std::fstream data("Assets/Data/collision.json");
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
    offset_x = doc["offset_x"].GetFloat();
    offset_y = doc["offset_y"].GetFloat();
    dimension_modifier = doc["dimension_modifier"].GetFloat();

}

// Check for intersection between circle and rectangle
int is_circle_intersecting_rectangle(float circle_x, float circle_y, float radius,
    float rect_x, float rect_y, float rect_width, float rect_height)
{
    // Find the closest point on the rectangle to the circle's center
    float closest_x = fmax(rect_x, fmin(circle_x, rect_x + rect_width));
    float closest_y = fmax(rect_y, fmin(circle_y, rect_y + rect_height));

    // Calculate the distance from the circle's center to this closest point
    float dx = circle_x - closest_x;
    float dy = circle_y - closest_y;
    float distanceSquared = dx * dx + dy * dy;

    // Check if the distance is less than or equal to the circle's radius squared
    return distanceSquared <= radius * radius; // Returns 1 (true) if intersecting, 0 (false) otherwise
}

// Check for intersection between circles
int are_circles_intersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
    // Calculate the distance squared between the centers of the circles
    float dx = c1_x - c2_x;
    float dy = c1_y - c2_y;
    float distanceSquared = dx * dx + dy * dy;

    // Calculate the sum of the radii squared
    float radiusSum = r1 + r2;
    float radiusSumSquared = radiusSum * radiusSum;

    // Check if the circles intersect
    return distanceSquared <= radiusSumSquared;  // Returns 1 (true) if intersecting, 0 (false) otherwise
}

// Check for intersection between circle and point
int is_mouse_intersecting_rectangle_clicked(float mousex, float mousey, float rectx, float recty, float rectwidth, float rectheight, int mousetriggered) {
    float halfwidth = rectwidth / dimension_modifier;
    float halfheight = rectheight / dimension_modifier;
    rectx = rectx + offset_x;
    recty = -recty + offset_y;
    if (((mousex >= rectx - halfwidth/dimension_modifier && mousex <= rectx + halfwidth/dimension_modifier) &&
        ( mousey <= recty + halfheight/dimension_modifier && mousey >= recty - halfheight/dimension_modifier)) &&
        mousetriggered == 1) {
        return 1;
    }
    else return 0;
}

