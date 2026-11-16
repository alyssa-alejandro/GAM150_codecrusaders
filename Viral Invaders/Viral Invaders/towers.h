/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      towers.h
*@author(s)     Chan Qi Ying (Primary Author, 35%)
*               Dalton Koh (Secondary Author, 35%)
*               Ethan Ng (Secondary Author, 30%)
*@DP emails     q.chan@digipen.edu
*               d.koh@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file contains declarations for functions related to creating, updating,
*               rendering, and freeing of towers, including bosses and their respective
*               skills.
******************************************************************************/
#pragma once
#include <AEEngine.h>
#ifndef MAXTOWER
#define MAXTOWER 20
#endif
#define LVL1TOWER 8
#define LVL2TOWER 8
#define LVL3TOWER 11
#define LVL4TOWER 13
#define LVL5TOWER 17
#define LVL6TOWER 19
#include "squaregrid.h"
#include "virus.h"
#include <vector>

struct tower_bullet {
	float start_X = -900.f;
	float start_Y = -900.f;
	float bullet_radius = 10.0f;
	float bullet_movement_speed = 20.f;
	int targeted_virus_index = -1;
};

enum Tower_Type {
	lymphocyte = 1,
	macrophage,
	boss1,
	boss2
};

enum Tower_Place {
	place_0 = 0,
	place_1,
	place_2,
	place_3,
	place_4,
	place_5,
	place_6,
	place_7,
	place_8,
	place_9,
	place_10,
	place_11,
	place_12,
	place_13,
	place_14,
	place_15,
	place_16,
	place_17
};

enum state {
	ATTACKING_STATE = 1,
	COOLDOWN_STATE,		//default state
	STOP_STATE,
	DEAD_STATE,
};

struct Tower_Size {			//size of wbc
	AEVec2 xy = { {}, {} };
	float radius{}, coll_radius {};
	unsigned int color{};
	unsigned int coll_color{};
	float scalex{};
	float scaley{};
	float coll_scale_x{};
	float coll_scale_y{};
};

struct tower {
	enum Tower_Type type = lymphocyte;
	enum state state = COOLDOWN_STATE;
	struct Tower_Size size;
	int atk{}, base_hp{}, current_hp{};
	bool inv_state = false;
	float atk_spd{};
	float cd_time{};
	AEVec2 target = { {} , {} };		//to be changed to virus struct
	AEGfxVertexList* mesh = nullptr;
	AEMtx33 transform{};
	struct tower_bullet tower_bullet;
};

//extern variables
extern struct tower towered[MAXTOWER];

// Json load
void load_data_towers();

// Init function
void init_tower_stats(int index, Tower_Type type);
void create_towers(int* tower_Indices, int count);
void place_towers(SG_stats** grid, int square_rows, int square_columns, int level);
void reset_tower_data(int index);
void reset_all_towers();

// Free function
void free_tower();

// Update functions
void update_tower();
void render_tower();
bool AreAllTowersDestroyed();
void searchintersecting();
void boss1_tower(float* timer);
void boss2_tower(float* timer);
void bossinvuln(float* timer);
void macrophage_skill(std::vector<virus*> spawned_viruses);