/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      MainMenu.h
*@author        Ang Yu Xuan (Primary Author, 100%)
*
*@DP emails     yuxuan.ang@digipen.edu
*
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file is used to declare functions and variables that will be defined
*               later in MainMenu.cpp.
******************************************************************************/

#pragma once
#include "AEEngine.h"
#include <vector>
#include "string"

//Define a struct to hold button information
struct Button {
	float pos_x, pos_y;//position of the button
	float length, width;  //size of the button
	std::string button_name; //button name
};

extern std::vector<Button> main_menu_buttons; //extern vector to store main menu buttons variables
extern std::vector<std::string> buttons_name; //extern vector to store buttons name
extern std::vector<Button> in_game_tooltips; //extern vector store in-game tooltips
//extern s8 tooltips_font;

extern AEGfxTexture* flu_texture;
extern AEGfxTexture* background_texture;
extern AEGfxTexture* WIN_PAGE_texture;
extern AEGfxTexture* LOST_PAGE_texture;

extern AEGfxVertexList* bg_mesh;

//3 conditions for game result
enum {
	none, //game result is currently unknown
	game_lost, //game result is LOST
	level_completed //game result is WIN
};

void main_menu_load_json_data();
void init_game_result();
void free_game_result();
void display_game_result(int check_game_condition, s8 loaded_font, AEGfxTexture *game_result_texture);

bool IsMouseInsideButton(float buttonX, float buttonY, float buttonLength, float buttonWidth);
void Render_Text(std::string text, s8 loaded_font, float containerX, float containerY);
void MainMenu_Page(AEGfxVertexList* pMesh_Square_pointer, s8 pFont);
void free_Main_menu();

//in game tooltips
void init_tooltips();
void tooltips_update();
void tooltips_free();

//main menu
void MainMenu_Init();
void MainMenu_Update();
void MainMenu_Exit();
