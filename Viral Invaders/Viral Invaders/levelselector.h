/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      levelselector.h
*@author(s)     Dalton Koh (Primary Author, 80%)
*               Alyssa Nicole Alejandro (Secondary author, 20%)
*@DP emails     d.koh@digipen.edu
*               alejandro.a@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file declares the different functions - init, update, exit;
*               for the different levels in the game. The file also includes 
*				other externed required variables.
******************************************************************************/

#pragma once

// Grid and path
extern SG_stats** grid;
extern int square_rows;
extern int square_columns;
extern int* path_Coords1;
extern int* path_Coords2;
extern bool in_upgrade_tree;

// audio
extern AEAudio bgm, sound;
extern AEAudioGroup bgm_Group;

// enum levels
enum level {
	nil =0,
	level_1,
	level_2,
	level_3,
	level_4,
	level_5,
	level_6
};

// Level 1
void load_data_lvl_selector();
void Level1_Init();
void Level1_Update();
void Level1_Exit();


// Level 2 
void Level2_Init();
void Level2_Update();
void Level2_Exit();

// Level 3
void Level3_Init();
void Level3_Update();
void Level3_Exit();

// Level 4
void Level4_Init();
void Level4_Update();
void Level4_Exit();

// Level 5
void Level5_Init();
void Level5_Update();
void Level5_Exit();

// Level 6
void Level6_Init();
void Level6_Update();
void Level6_Exit();
