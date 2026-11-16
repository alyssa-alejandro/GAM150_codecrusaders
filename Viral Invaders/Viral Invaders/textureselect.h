/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      textureselect.h
*@author(s)     Dalton Koh (Primary Author, 60%)
*               Chan Qi Ying (Secondary author, 10%)
*               Ang Yu Xuan (Secondary author, 10%)
*               Alyssa Nicole Alejandro (Secondary Author, 10%)
*               Ethan Ng (Secondary Author, 10%)
*@DP emails     d.koh@digipen.edu
*               q.chan@digipen.edu
*               alejandro.a@digipen.edu
*               yuxuan.ang@digipen.edu
*               n.ethanyongle@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file includes all requried externs so we can reuse textures!
******************************************************************************/

#pragma once
#include <AEEngine.h>
#include <fstream>

// All extern textures 
extern AEGfxTexture* grid_texture;
extern AEGfxTexture* path_texture;
extern AEGfxTexture* spawn;
extern AEGfxTexture* ebola_Texture;
extern AEGfxTexture* button_def;
extern AEGfxTexture* button_clicked;
extern AEGfxTexture* dengue_texture;
extern AEGfxTexture* main_Menu_Bg_Texture;
extern AEGfxTexture* bullets;
extern AEGfxTexture* Back_Button_Texture;
extern AEGfxTexture* credits_bg;
extern AEGfxTexture* digipen_logo;

extern AEGfxTexture* lymphocyte_Texture;
extern AEGfxTexture* macrophage_Texture;
extern AEGfxTexture* chickenpox_texture;
extern AEGfxTexture* shingles_texture;

extern AEGfxTexture* polio_texture;
extern AEGfxTexture* flu_texture;
extern AEGfxTexture* hfmd_texture;
extern AEGfxTexture* background_texture;
extern AEGfxTexture* arrow;
extern AEGfxTexture* arrow_2;
extern AEGfxTexture* dna;
extern AEGfxTexture* guide_1;
extern AEGfxTexture* guide_2;
extern AEGfxTexture* guide_3;
extern AEGfxTexture* guide_4;
extern AEGfxTexture* guide_5;
extern AEGfxTexture* boss_1_t;
extern AEGfxTexture* boss_1_active;
extern AEGfxTexture* boss_2_t;
extern AEGfxTexture* boss_1_inv;
extern AEGfxTexture* boss_2_inv;
extern s8 font;
extern s8 k_font;

// Init function
void load_data_textures();

// Free
void free_textures();

// Main load Json data function
 void load_data();