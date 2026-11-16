#pragma once
/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      virulencementer.h
*@author(s)     Ethan Ng 
*@DP emails     n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains the declarations for the functionalities and
*               rendering of the virulence meter in game
******************************************************************************/
void load_data_virulence_meter();
void virulence_meter_init(int* meter);
void virulence_meter_render(int* meter);
void free_virulence_meter();