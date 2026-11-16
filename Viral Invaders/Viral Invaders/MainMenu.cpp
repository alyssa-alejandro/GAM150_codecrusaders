/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      MainMenu.cpp
*@author        Ang Yu Xuan (Primary Author, 100%)
*               
*@DP emails     yuxuan.ang@digipen.edu
*               
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file is used to setup everything needed to display a main
*               menu.
******************************************************************************/

#include <iostream>
#include "AEEngine.h"
#include "confirmquit.h"
#include "MainMenu.h"
#include "mesh.h"
#include "GameStateManager.h"
#include "textureselect.h"
#include "level.h"
#include "game_setting.h"
#include "levelselector.h"
#include "bgmusic.h"
#include "credits.h"
#include "rectangle.h"
#include "upgradetree.h"
#include "levelselector.h"

#include "document.h"
#include "istreamwrapper.h"

std::vector<Button>main_menu_buttons; //main_menu_buttons is vector of type Button
std::vector<std::string> buttons_name; //in-game tooltips is vector of type Button
std::vector<Button>in_game_tooltips; //in-game tooltips is vector of type Button

bool isMainMenuActive{}; //check if Main Menu is activated, default is false
float DGPENlogo_timer;

AEGfxVertexList* _bg{}; //meshes for background texture
AEMtx33 BGTransform;
AEGfxVertexList* _logo{}; //meshes for logo texture
AEMtx33 logoTransform;

s8 tooltips_font;
AEGfxVertexList* mm_button_mesh{};
AEGfxVertexList* button_mesh{};
AEGfxVertexList* bg_mesh{};
AEGfxVertexList* polio_mesh{};
AEGfxVertexList* flu_mesh{};
AEGfxVertexList* hfmd_mesh{};
AEGfxVertexList* game_result_mesh{};

static AEGfxVertexList* tooltip;
u32 white_color;
u32 black_color;
u32 red_color;
u32 tooltips_color;
static float rendering_RGBA;
static float text_RGBA;
static int rewarded_upgrade_points;
static float scale_by_positive_1;
static float scale_by_negative_1;
static int tooltips_font_height;
static std::string tooltips_font_link;

static float game_result_main_menu_button_X;
static float game_result_main_menu_button_Y;
static std::string game_result_main_menu_button_name;
static float game_result_next_level_button_X;
static float game_result_next_level_button_Y;
static std::string game_result_next_level_button_name;
static float game_result_restart_level_button_X;
static float game_result_restart_level_button_Y;
static std::string game_result_restart_level_button_name;
static float game_result_button_width;
static float game_result_button_height;
static float tooltips_width;
static float tooltips_height;

static float first_tooltips_x;
static float first_tooltips_y;
static std::string first_tooltips_name;
static float second_tooltips_x;
static float second_tooltips_y;
static std::string second_tooltips_name;
static float third_tooltips_x;
static float third_tooltips_y;
static std::string third_tooltips_name;
static float fourth_tooltips_x;
static float fourth_tooltips_y;
static std::string fourth_tooltips_name;
static float fifth_tooltips_x;
static float fifth_tooltips_y;
static std::string fifth_tooltips_name;
static float sixth_tooltips_x;
static float sixth_tooltips_y;
static std::string sixth_tooltips_name;
static float last_tooltips_x;
static float last_tooltips_y;
static std::string last_tooltips_name;

static float logo_width;
static float logo_height;
static float first_main_menu_button_x;
static float first_main_menu_button_y;
static float first_main_menu_button_width;
static float first_main_menu_button_height;
static std::string first_main_menu_button_name;
static std::string second_main_menu_button_name;
static std::string third_main_menu_button_name;
static std::string fourth_main_menu_button_name;
static std::string fifth_main_menu_button_name;
static std::string last_main_menu_button_name;
static int main_menu_button_number;
static float main_menu_button_y_offset;

static float credit_text_RGBA;
static float credit_text_scale;
static std::string copyright_first_line;
static float credit_text_first_line_x;
static float credit_text_first_line_y;
static std::string copyright_second_line;
static float credit_text_second_line_x;
static float credit_text_second_line_y;
static float time_activate_main_menu;


void main_menu_load_json_data() {
	std::ifstream fs("Assets/Data/MainMenu.json");
	if (!fs) { //cannot open this json file
		gGameRunning = null;
		return;
	}

	rapidjson::IStreamWrapper isw(fs);
	rapidjson::Document doc;

	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		gGameRunning = null;
		return; // Exit the function on parse error
	}

	// Assigning data from json file to init variable in this function
	sscanf_s(doc["white_color"].GetString(), "%x", &white_color);
	sscanf_s(doc["black_color"].GetString(), "%x", &black_color);
	sscanf_s(doc["red_color"].GetString(), "%x", &red_color);
	sscanf_s(doc["tooltips_color"].GetString(), "%x", &tooltips_color);
	rendering_RGBA = doc["rendering_RGBA"].GetFloat();
	text_RGBA = doc["text_RGBA"].GetFloat();
	scale_by_positive_1 = doc["scale_by_positive_1"].GetFloat();
	scale_by_negative_1 = doc["scale_by_negative_1"].GetFloat();
	tooltips_font_height = doc["tooltips_font_height"].GetInt();
	tooltips_font_link = doc["tooltips_font_link"].GetString();

	//main menu button position on display game result screen for player to go back to main menu
	game_result_main_menu_button_X = doc["game_result_main_menu_button_X"].GetFloat();
	game_result_main_menu_button_Y = doc["game_result_main_menu_button_Y"].GetFloat();
	game_result_main_menu_button_name = doc["game_result_main_menu_button_name"].GetString();
	//next level button position on display game result screen for player to go to next level
	game_result_next_level_button_X = doc["game_result_next_level_button_X"].GetFloat();
	game_result_next_level_button_Y = doc["game_result_next_level_button_Y"].GetFloat();
	game_result_next_level_button_name = doc["game_result_next_level_button_name"].GetString();
	//restart level button position on display game result screen for player to restart current level
	game_result_restart_level_button_X = doc["game_result_restart_level_button_X"].GetFloat();
	game_result_restart_level_button_Y = doc["game_result_restart_level_button_Y"].GetFloat();
	game_result_restart_level_button_name = doc["game_result_restart_level_button_name"].GetString();
	//the size of button on display game result screen
	game_result_button_width = doc["game_result_button_width"].GetFloat();
	game_result_button_height = doc["game_result_button_height"].GetFloat();
	//the size of pop up tooltips
	tooltips_width = doc["tooltips_width"].GetFloat();
	tooltips_height = doc["tooltips_height"].GetFloat();

	rewarded_upgrade_points = doc["reward_upgrade_points_completed_level"].GetInt();
	//initialize first tooltips
	first_tooltips_x = doc["first_tooltips_x"].GetFloat();
	first_tooltips_y = doc["first_tooltips_y"].GetFloat();
	first_tooltips_name = doc["first_tooltips_name"].GetString();
	//initialize second tooltips
	second_tooltips_x = doc["second_tooltips_x"].GetFloat();
	second_tooltips_y = doc["second_tooltips_y"].GetFloat();
	second_tooltips_name = doc["second_tooltips_name"].GetString();
	//initialize third tooltips
	third_tooltips_x = doc["third_tooltips_x"].GetFloat();
	third_tooltips_y = doc["third_tooltips_y"].GetFloat();
	third_tooltips_name = doc["third_tooltips_name"].GetString();
	//initialize fourth tooltips
	fourth_tooltips_x = doc["fourth_tooltips_x"].GetFloat();
	fourth_tooltips_y = doc["fourth_tooltips_y"].GetFloat();
	fourth_tooltips_name = doc["fourth_tooltips_name"].GetString();
	//initialize fifth tooltips
	fifth_tooltips_x = doc["fifth_tooltips_x"].GetFloat();
	fifth_tooltips_y = doc["fifth_tooltips_y"].GetFloat();
	fifth_tooltips_name = doc["fifth_tooltips_name"].GetString();
	//initialize sixth tooltips
	sixth_tooltips_x = doc["sixth_tooltips_x"].GetFloat();
	sixth_tooltips_y = doc["sixth_tooltips_y"].GetFloat();
	sixth_tooltips_name = doc["sixth_tooltips_name"].GetString();
	//initialize last tooltips
	last_tooltips_x = doc["last_tooltips_x"].GetFloat();
	last_tooltips_y = doc["last_tooltips_y"].GetFloat();
	last_tooltips_name = doc["last_tooltips_name"].GetString();

	//initialize digipen logo
	logo_width = doc["logo_width"].GetFloat();
	logo_height = doc["logo_height"].GetFloat();

	//initialize for main menu button info
	first_main_menu_button_x = doc["first_main_menu_button_x"].GetFloat();
	first_main_menu_button_y = doc["first_main_menu_button_y"].GetFloat();
	first_main_menu_button_width = doc["first_main_menu_button_width"].GetFloat();
	first_main_menu_button_height = doc["first_main_menu_button_height"].GetFloat();
	first_main_menu_button_name = doc["first_main_menu_button_name"].GetString();
	second_main_menu_button_name = doc["second_main_menu_button_name"].GetString();
	third_main_menu_button_name = doc["third_main_menu_button_name"].GetString();
	fourth_main_menu_button_name = doc["fourth_main_menu_button_name"].GetString();
	fifth_main_menu_button_name = doc["fifth_main_menu_button_name"].GetString();
	last_main_menu_button_name = doc["last_main_menu_button_name"].GetString();
	main_menu_button_y_offset = doc["main_menu_button_y_offset"].GetFloat();
	main_menu_button_number = doc["main_menu_button_number"].GetInt();

	credit_text_RGBA = doc["credit_text_RGBA"].GetFloat();
	credit_text_scale = doc["credit_text_scale"].GetFloat();
	copyright_first_line = doc["copyright_first_line"].GetString();
	credit_text_first_line_x = doc["credit_text_first_line_x"].GetFloat();
	credit_text_first_line_y = doc["credit_text_first_line_y"].GetFloat();
	copyright_second_line = doc["copyright_second_line"].GetString();
	credit_text_second_line_x = doc["credit_text_second_line_x"].GetFloat();
	credit_text_second_line_y = doc["credit_text_second_line_y"].GetFloat();
	time_activate_main_menu = doc["time_activate_main_menu"].GetFloat();
}

void init_game_result() {
	if (!button_mesh) button_mesh = Draw_Square_Mesh(white_color); //meshes for button in display game result screen
	if (!game_result_mesh)game_result_mesh = Draw_Square_Mesh(red_color); //meshes for display game result screen
}

void free_game_result() {
	if (button_mesh) AEGfxMeshFree(button_mesh); button_mesh = nullptr;
	if (game_result_mesh) AEGfxMeshFree(game_result_mesh); game_result_mesh = nullptr;
}

void display_game_result(int check_game_condition, s8 loaded_font, AEGfxTexture* game_result_texture) {
	//handle input logic here
	if (confirm_mm_page) {
		if (update_confirm_page() == ok_reply) {
			free_game_result();
			confirm_mm_page = false;
			if (check_game_condition == level_completed) {
				upgrade_points += rewarded_upgrade_points;  // Award upgrade points
				save_Upgrade_Data();    //Save progress immediately
			}
			GSM_ChangeState(STATE_MAINMENU); //switch to main menu if main menu button is triggered
			return;
		}
		else if (update_confirm_page() == cancel_reply) {
			confirm_mm_page = false;
		}
		else {}
	}
	else {
		if (check_game_condition == level_completed) { //WIN GAME STATE
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				if (IsMouseInsideButton(game_result_main_menu_button_X, game_result_main_menu_button_Y,
					game_result_button_width, game_result_button_height)) {
					confirm_mm_page = true;
					init_confirm_page();
					return;
				}
			}
		}

		else { //LOST GAME STATE
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				if (IsMouseInsideButton(game_result_main_menu_button_X, game_result_main_menu_button_Y,
					game_result_button_width, game_result_button_height)) {
					confirm_mm_page = true;
					init_confirm_page();
					return;
				}

				//restart current level if the "restart level" button is clicked
				if (IsMouseInsideButton(game_result_restart_level_button_X, game_result_restart_level_button_Y,
					game_result_button_width, game_result_button_height)) {
					GSM_ChangeState(STATE_RESTART);
					return;
				}
			}
		}
	}
	

	//Render start here
	// Tell the engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(rendering_RGBA, rendering_RGBA, rendering_RGBA, rendering_RGBA);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd({}, {}, {}, {});
	AEGfxTextureSet(game_result_texture, {}, {});
	Draw_Rectangle_Shape(game_result_mesh, AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.0f, {}, {});
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	if (check_game_condition == level_completed) { //WIN GAME STATE
		//render main menu button, next level button here
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(button_clicked, {}, {});
		Draw_Rectangle_Shape(button_mesh, game_result_button_width, game_result_button_height, 
			game_result_main_menu_button_X, game_result_main_menu_button_Y);
		Render_Text(game_result_main_menu_button_name, loaded_font, game_result_main_menu_button_X / (AEGfxGetWindowWidth() / 2.0f), 
			game_result_main_menu_button_Y / (AEGfxGetWindowHeight() / 2.0f));

		if (current_State != STATE_LEVEL6) { //render next level button if player is not on last level
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(button_clicked, {}, {});
			Draw_Rectangle_Shape(button_mesh, game_result_button_width, game_result_button_height, 
				game_result_next_level_button_X, game_result_next_level_button_Y);
			Render_Text(game_result_next_level_button_name, loaded_font, game_result_next_level_button_X / (AEGfxGetWindowWidth() / 2.0f), 
				game_result_next_level_button_Y / (AEGfxGetWindowHeight() / 2.0f));
		}
		

		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (IsMouseInsideButton(game_result_main_menu_button_X, game_result_main_menu_button_Y,
				game_result_button_width, game_result_button_height)) {
				free_game_result();
				GSM_ChangeState(STATE_MAINMENU); //switch to main menu
				return;
			}

			//go to next level if the "next level" button is clicked
			if (IsMouseInsideButton(game_result_next_level_button_X, game_result_next_level_button_Y,
				game_result_button_width, game_result_button_height) && current_State != STATE_LEVEL6) {
				switch (current_State) {
				case STATE_LEVEL1:
					free_game_result();
					upgrade_points += rewarded_upgrade_points;  // Award upgrade points
					save_Upgrade_Data();    //Save progress immediately
					GSM_ChangeState(STATE_LEVEL2);
					break;
				case STATE_LEVEL2:
					free_game_result();
					upgrade_points += rewarded_upgrade_points;  // Award upgrade points
					save_Upgrade_Data();    //Save progress immediately
					GSM_ChangeState(STATE_LEVEL3); break;
				case STATE_LEVEL3:
					free_game_result();
					upgrade_points += rewarded_upgrade_points;  // Award upgrade points
					save_Upgrade_Data();    //Save progress immediately
					GSM_ChangeState(STATE_LEVEL4); break;
				case STATE_LEVEL4:
					free_game_result();
					upgrade_points += rewarded_upgrade_points;  // Award upgrade points
					save_Upgrade_Data();    //Save progress immediately
					GSM_ChangeState(STATE_LEVEL5); break;
				case STATE_LEVEL5: 
					free_game_result(); 
					upgrade_points += rewarded_upgrade_points;  // Award upgrade points
					save_Upgrade_Data();    //Save progress immediately
					GSM_ChangeState(STATE_LEVEL6); break;
				}
			}
		}
	}

	else { //LOST GAME STATE
		
		//render main menu button, restart button here
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(button_clicked, {}, {});
		Draw_Rectangle_Shape(button_mesh, game_result_button_width, game_result_button_height, 
			game_result_main_menu_button_X, game_result_main_menu_button_Y);
		Render_Text(game_result_main_menu_button_name, loaded_font, game_result_main_menu_button_X / (AEGfxGetWindowWidth() / 2.0f), 
			game_result_main_menu_button_Y / (AEGfxGetWindowHeight() / 2.0f));

		AEGfxTextureSet(button_clicked, {}, {});
		Draw_Rectangle_Shape(button_mesh, game_result_button_width, game_result_button_height, 
			game_result_restart_level_button_X, game_result_restart_level_button_Y);
		Render_Text(game_result_restart_level_button_name, loaded_font, game_result_restart_level_button_X / (AEGfxGetWindowWidth() / 2.0f), 
			game_result_restart_level_button_Y / (AEGfxGetWindowHeight() / 2.0f));
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}

	if (confirm_mm_page) render_confirm_page(font, quit);
	
}

void init_tooltips() {
	if (!tooltips_font) tooltips_font = AEGfxCreateFont(tooltips_font_link.c_str(), tooltips_font_height);
	tooltip = Draw_Square_Mesh(tooltips_color); //meshes for tooltips
	
	//init vector in_game_tooltips in the order from top to bottom 
	in_game_tooltips.push_back({ first_tooltips_x, first_tooltips_y, tooltips_width, tooltips_height, first_tooltips_name });
	in_game_tooltips.push_back({ second_tooltips_x, second_tooltips_y, tooltips_width, tooltips_height, second_tooltips_name });
	in_game_tooltips.push_back({ third_tooltips_x, third_tooltips_y, tooltips_width, tooltips_height, third_tooltips_name });
	in_game_tooltips.push_back({ fourth_tooltips_x, fourth_tooltips_y, tooltips_width, tooltips_height, fourth_tooltips_name });
	in_game_tooltips.push_back({ fifth_tooltips_x, fifth_tooltips_y, tooltips_width, tooltips_height, fifth_tooltips_name });
	in_game_tooltips.push_back({ sixth_tooltips_x, sixth_tooltips_y, tooltips_width, tooltips_height, sixth_tooltips_name });
	in_game_tooltips.push_back({ last_tooltips_x, last_tooltips_y, tooltips_width, tooltips_height, last_tooltips_name });
}

void tooltips_update() {
	//Logic Handle here
	if (in_game_tooltips.size() > NULL && game_condition == none) { //check how many in_game_tooltips that have not been clicked by player
		if (AEInputCheckTriggered(AEVK_LBUTTON)) { //we will always check whether current tooltip has been clicked
			if (IsMouseInsideButton(in_game_tooltips[0].pos_x, in_game_tooltips[0].pos_y,
				in_game_tooltips[0].length, in_game_tooltips[0].width)) {
				in_game_tooltips.erase(in_game_tooltips.begin()); //remove the current tooltip from vector in_game_tooltips
			}
		}
	}

	//Rendering here
	if (in_game_tooltips.size() > NULL && game_condition == none) {
		Draw_Rectangle_Shape(tooltip, in_game_tooltips[0].length, in_game_tooltips[0].width,
			in_game_tooltips[0].pos_x, in_game_tooltips[0].pos_y);
		Render_Text(in_game_tooltips[0].button_name, tooltips_font,
			in_game_tooltips[0].pos_x / (AEGfxGetWindowWidth() / 2.0f),
			in_game_tooltips[0].pos_y / (AEGfxGetWindowHeight() / 2.0f));
	}
}

void tooltips_free() {
	if (tooltips_font) {
		AEGfxDestroyFont(tooltips_font); //free the font
		tooltips_font = NULL; // Avoid dangling pointer
	}

	if (tooltip) AEGfxMeshFree(tooltip); tooltip = nullptr;

	in_game_tooltips.clear();
}

// Detect if mouse click is inside a button
bool IsMouseInsideButton(float buttonX, float buttonY, float buttonLength, float buttonWidth) {
	int mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	float worldMouseX = (float)mouseX - (AEGfxGetWindowWidth() / 2.0f); 
	float worldMouseY = (AEGfxGetWindowHeight() / 2.0f) - (float)mouseY;

	return (worldMouseX >= buttonX - buttonLength / 2 &&
		worldMouseX <= buttonX + buttonLength / 2 &&
		worldMouseY >= buttonY - buttonWidth / 2 &&
		worldMouseY <= buttonY + buttonWidth / 2);
}

//FOR MAIN MENU implementation - a function to render text
void Render_Text(std::string text, s8 loaded_font, float containerX, float containerY) {
	f32 text_width, text_height;
	AEGfxGetPrintSize(loaded_font, text.c_str(), scale_by_positive_1, &text_width, &text_height);
	AEGfxPrint(loaded_font, text.c_str(), containerX - text_width / 2, containerY - text_height / 2,
		scale_by_positive_1, {}, {}, {}, text_RGBA);
}

//This function is used to display main menu page
void MainMenu_Page(AEGfxVertexList* pMesh_Square_pointer, s8 pFont) {
	// Draw main menu buttons in this order->start game, guide page, virus upgrade, gane setting, credit page, quit game
	for (int i{}; i < main_menu_buttons.size(); i++) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(button_clicked, {}, {});
		Draw_Rectangle_Shape(pMesh_Square_pointer, main_menu_buttons[i].length, main_menu_buttons[i].width, 
			main_menu_buttons[i].pos_x, main_menu_buttons[i].pos_y);
		//Render text for main menu buttons here in this order->start game, guide page, virus upgrade, game setting, credit page, quit game
		Render_Text(main_menu_buttons[i].button_name, pFont, 
			main_menu_buttons[i].pos_x / (AEGfxGetWindowWidth() / 2.0f), main_menu_buttons[i].pos_y / (AEGfxGetWindowHeight() / 2.0f));
	}
}

// "Init" function - Main Menu
void MainMenu_Init(void)
{
	float bg_width = static_cast<float>(AEGfxGetWindowWidth());
	float bg_height = static_cast<float>(AEGfxGetWindowHeight());
	DGPENlogo_timer = {};
	Create_Rectangle(&_bg, bg_width, bg_height, black_color, &BGTransform, scale_by_positive_1, scale_by_positive_1, {}, {});
	
	Create_Rectangle(&_logo, logo_width, logo_height, black_color, &logoTransform, scale_by_positive_1, scale_by_negative_1, {}, {});
    init_upgrade_tree();

	if (!mm_button_mesh) mm_button_mesh = Draw_Square_Mesh(red_color);
	if (!bg_mesh) bg_mesh = Draw_Square_Mesh(black_color);

	init_audio(bgm, bgm_Group); //init background music;
	
	//Init main menu buttons in this order->start game, guide page, virus upgrade, game setting, credit page, quit game
	buttons_name = { first_main_menu_button_name, second_main_menu_button_name, third_main_menu_button_name,
		fourth_main_menu_button_name, fifth_main_menu_button_name, last_main_menu_button_name}; //name for each needed buttons

	//Init main menu button names in this order->start game, guide page, virus upgrade, game setting, credit page, quit game
	for (int i{}; i < main_menu_button_number; i++) {
		main_menu_buttons.push_back({ first_main_menu_button_x, first_main_menu_button_y - main_menu_button_y_offset*i,
			first_main_menu_button_width, first_main_menu_button_height, buttons_name[i] });
	}
		
    Game_Setting_Init();
	init_credits_page();
}


// "Update" function - Main Menu
void MainMenu_Update(void)
{
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime()); //delta time
	if (!isMainMenuActive) {
		//Render Credits Text
		// Continue showing DigiPen Logo
		AEGfxSetBackgroundColor({}, {}, {}); // Set the background to black
		AEGfxSetColorToMultiply(credit_text_RGBA, credit_text_RGBA, credit_text_RGBA, credit_text_RGBA); // Ensure full opacity


		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(BGTransform.m);
		AEGfxMeshDraw(_bg, AE_GFX_MDM_TRIANGLES);


		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(digipen_logo, {}, {});
		AEGfxSetTransform(logoTransform.m);
		AEGfxMeshDraw(_logo, AE_GFX_MDM_TRIANGLES);

		AEGfxPrint(font, copyright_first_line.c_str(), credit_text_first_line_x, credit_text_first_line_y, credit_text_scale, 
			credit_text_RGBA, credit_text_RGBA, credit_text_RGBA, credit_text_RGBA);
		AEGfxPrint(font, copyright_second_line.c_str(), credit_text_second_line_x, credit_text_second_line_y, credit_text_scale, 
			credit_text_RGBA, credit_text_RGBA, credit_text_RGBA, credit_text_RGBA);
		DGPENlogo_timer += dt;

		if (DGPENlogo_timer >= time_activate_main_menu || AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_RBUTTON) || AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_RETURN) || AEInputCheckTriggered(AEVK_ESCAPE)) {
			// Once transition is complete, activate main menu
			isMainMenuActive = true;
		}
		return;  // Exit early (prevents menu from rendering)
	}
	
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	// Your own rendering logic goes here
	// Set the background to white.
	AEGfxSetBackgroundColor(rendering_RGBA, rendering_RGBA, rendering_RGBA);

	// Tell the engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(rendering_RGBA, rendering_RGBA, rendering_RGBA, rendering_RGBA);
	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd({}, {}, {}, {});

	AEGfxTextureSet(main_Menu_Bg_Texture, {}, {});
	Draw_Rectangle_Shape(bg_mesh, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), {}, {});

	// Tell the engine to get ready to draw something with color.
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	MainMenu_Page(mm_button_mesh, font);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(rendering_RGBA);

	for (int i{}; i < main_menu_buttons.size(); i++) {
		
		if (confirm_exit_page) {
			if (update_confirm_page() == ok_reply) {
				gGameRunning = null;
			}
			else if (update_confirm_page() == cancel_reply) {
				confirm_exit_page = false;
			}
			else {}
			render_confirm_page(font, quit);
		}
		//Detect button clicks
		else if (AEInputCheckTriggered(AEVK_LBUTTON) && !(game_setting_trigger)) {
				if (IsMouseInsideButton(main_menu_buttons[i].pos_x, main_menu_buttons[i].pos_y,
					main_menu_buttons[i].length, main_menu_buttons[i].width)) {
					switch (i) {
					//check if start game button is clicked 
					case 0:  if (!in_upgrade_tree && !is_Credits_Visible)GSM_ChangeState(STATE_LEVEL1); break;
					//check if guide page button is clicked 
					case 1:  if (!in_upgrade_tree && !is_Credits_Visible)GSM_ChangeState(STATE_GUIDEPAGE); break;
					//check if virus upgrade button is clicked 
					case 2: if (!in_upgrade_tree && !is_Credits_Visible) {
						in_upgrade_tree = true;  // **Set flag to track that upgrade tree is open**
					}break;
					//check if game setting button is clicked
					case 3:  if (!in_upgrade_tree && !is_Credits_Visible) game_setting_trigger = true; break;
					//check if credit page button is clicked
					case 4:  if (!in_upgrade_tree) is_Credits_Visible = true; break;  // Show Credits
					//check if exit game button is clicked
					case 5:  
						if (!in_upgrade_tree && !is_Credits_Visible) {
							confirm_exit_page = true;
							init_confirm_page();
						}
						break;
					}
				}
		}
	}

	input_audio(bgm_Group);

	if (game_setting_trigger) {
		Game_Setting_Update();
	}
	if (in_upgrade_tree) {
		set_upgrade_tree();
	}
	// Handle and Render the Credits Page if it's Open
	render_credits_page();
}


void free_Main_menu() {

	if (mm_button_mesh) AEGfxMeshFree(mm_button_mesh); mm_button_mesh = nullptr; 
	if (bg_mesh) AEGfxMeshFree(bg_mesh); bg_mesh = nullptr;
	if (polio_mesh) AEGfxMeshFree(polio_mesh); polio_mesh = nullptr;
	if (flu_mesh) AEGfxMeshFree(flu_mesh); flu_mesh = nullptr;
	if (hfmd_mesh) AEGfxMeshFree(hfmd_mesh); hfmd_mesh = nullptr;
	if (_bg) AEGfxMeshFree(_bg); _bg = nullptr;
	if (_logo) AEGfxMeshFree(_logo); _logo = nullptr;

	free_audio(bgm, bgm_Group);
	Game_Setting_Exit();
}

void MainMenu_Exit() {
	free_Main_menu();
	free_credits_page();
	Game_Setting_Exit();
}