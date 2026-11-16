/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      collision.h
*@author(s)     Chan Qi Ying (Primary Author, 80%)
*               Ethan Ng (Secondary Author, 20%)
*@DP emails     q.chan@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares functions to determine intersection between circles,
*               circle and rectangle, and point to rectangle
******************************************************************************/
#pragma once

// Load data
void load_data_collision();

// Check for intersection between circle and rectangle
int is_circle_intersecting_rectangle(float circle_x, float circle_y, float radius,
    float rect_x, float rect_y, float rect_width, float rect_height);

// Check for intersection between circles
int are_circles_intersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

// Check for intersection between circle and point
int is_mouse_intersecting_rectangle_clicked(float mousex, float mousey, float rectx, float recty, float rectwidth, float rectheight, int mousetriggered);


