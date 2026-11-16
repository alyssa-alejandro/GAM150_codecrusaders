/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      spawncontroller.h
*@author(s)     Ethan Ng (Primary Author, 100%)
*@DP emails     n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the logic for the spawning, selection and rendering of
*               virus options and spawnpoints in the maps for the game
******************************************************************************/
#pragma once

enum select_num {
	select_0 = 0,
	select_1,
	select_2,
	select_3,
	select_4,
	select_5
};

//Yu Xuan's code for automatically spawning minion polio when countdown timer for spawning minion polio == 0
void load_data_spawn();
void Auto_spawner(float spawn_pointx, float spawn_pointy, std::vector<virus*>& spawned_Viruses);
void spawnpoint_init();
void spawnpoint_render();
void resource_controller(int* resource);
void level1_selector_init();
void level1_selector_render();
void spawnpoint_lvl3_init();
void spawnpoint_lvl3_render();
void level3_selector_init();
void level3_selector_render();
void level5_selector_init();
void level5_selector_render();
void virus_Select(int* selected, int virus_type, int mousex, int mousey, float selectionx, float selectiony, float height, float width, int is_clicked, std::vector<virus*>& spawned_Viruses, int* resource, int* cost);
void spawner_Select(int selected, int mousex, int mousey, float spawn_pointx, float spawn_pointy, float height, float width, std::vector<virus*>& spawned_Viruses, int is_clicked, int *resource, int *cost);
void render_shingle_select(int chickenpox_Defeated, int* selected, int mousex, int mousey,  std::vector<virus*>& spawned_Viruses, int* resource, int* cost);
int virus_price(int* cost, int selected);
void free_spawner();
void free_selectors();
void free_bullets(AEGfxVertexList** tbullet_mesh, AEGfxVertexList** vbullet_mesh);

