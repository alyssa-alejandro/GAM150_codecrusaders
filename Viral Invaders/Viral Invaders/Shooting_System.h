/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      Shooting_System.h
*@author        Ang Yu Xuan (Primary Author, 70%)
*               Ethan Ng (Secondary Author, 30%)
*
*@DP emails     yuxuan.ang@digipen.edu
*               n.ethanyongle@digipen.edu
*
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file is used to declare struct and functions that are needed for 
*               bullet objects and basic shooting system.
******************************************************************************/

#pragma once
#include <utility>  // For std::pair
#include "AEEngine.h"
#include "virus.h"
#include <vector>

struct virus;

void shooting_load_json_data();

std::pair<float, float> direction_vector(float& shooter_circle_x, float& shooter_circle_y, float& target_circle_x, float& target_circle_y);

void initBullet(float& bullet_X, float& bullet_Y, float bullet_radius, unsigned int color, bool is_Virus);

void Virus_Shooter_Logic(size_t virus_array_size, std::vector<virus*> virus_array_pointer, size_t tower_array_size,
	struct tower* tower_array_pointer,
	f64& dt, unsigned int color);

void Tower_Shooter_Logic(size_t tower_array_size, struct tower* tower_array_pointer,
	size_t virus_array_size, std::vector<virus*> virus_array_pointer,
	f64& dt, unsigned int color);

void render_Virus_Bullet(size_t virus_array_size, std::vector<virus*>virus_array_pointer, size_t tower_array_size,
	struct tower* tower_array_pointer, unsigned int color);

void render_Tower_Bullet(size_t tower_array_size, struct tower* tower_array_pointer,
	size_t virus_array_size, unsigned int color);

void Free_Bullet();
void vFree_Bullet();

