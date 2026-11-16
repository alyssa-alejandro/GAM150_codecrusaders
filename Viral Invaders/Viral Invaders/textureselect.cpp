/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      textureselect.cpp
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
*@brief:        This file defines the setting and freeing of all textures to be
*               loaded into the game!
******************************************************************************/

#include "document.h"
#include "setting.h"
#include "virulencemeter.h"
#include "textureselect.h"
#include "bgmusic.h"
#include "circle.h"
#include "collision.h"
#include "confirmquit.h"
#include "countdown.h"
#include "rectangle.h"
#include "spritesheets.h"
#include "guidepage.h"
#include "events.h"
#include "mesh.h"
#include "pipagao.h"
#include "platelets.h"
#include "paracetemol.h"
#include "vaccine.h"
#include "pausescreen.h"
#include "spawncontroller.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "credits.h"
#include "document.h"
#include "istreamwrapper.h"
#include "level.h"
#include "string"
#include "healthbar.h"
#include "upgradetree.h"
#include "levelselector.h"
#include "pathing.h"
#include "MainMenu.h"
#include "Polio.h"

// Grid/path textures 
AEGfxTexture* grid_texture = nullptr;
AEGfxTexture* path_texture = nullptr;
AEGfxTexture* spawn = nullptr;

// Upgrade tree textures 
AEGfxTexture* button_def = nullptr;
AEGfxTexture* button_clicked = nullptr;

// Main menu/ credit scenes textures
AEGfxTexture* credits_bg = nullptr;
AEGfxTexture* digipen_logo = nullptr;
AEGfxTexture* main_Menu_Bg_Texture = nullptr;

// Win/lose textures
AEGfxTexture* WIN_PAGE_texture = nullptr;
AEGfxTexture* LOST_PAGE_texture = nullptr;

// Button textures
AEGfxTexture* Back_Button_Texture = nullptr;
AEGfxTexture* background_texture = nullptr;
AEGfxTexture* arrow = nullptr;
AEGfxTexture* arrow_2 = nullptr;

// Virus textures 
AEGfxTexture* dengue_texture = nullptr;
AEGfxTexture* shingles_texture = nullptr;
AEGfxTexture* chickenpox_texture = nullptr;
AEGfxTexture* polio_texture = nullptr;
AEGfxTexture* flu_texture = nullptr;
AEGfxTexture* hfmd_texture = nullptr;
AEGfxTexture* ebola_Texture = nullptr;


// Icons 
AEGfxTexture* dna = nullptr;

// Guide page 
AEGfxTexture* guide_1 = nullptr;
AEGfxTexture* guide_2 = nullptr;
AEGfxTexture* guide_3 = nullptr;
AEGfxTexture* guide_4 = nullptr;
AEGfxTexture* guide_5 = nullptr;

// White blood cell tower textures
AEGfxTexture* boss_1_t = nullptr;
AEGfxTexture* boss_1_active = nullptr;
AEGfxTexture* boss_1_inv = nullptr;
AEGfxTexture* boss_2_t = nullptr;
AEGfxTexture* boss_2_inv= nullptr;
AEGfxTexture* lymphocyte_Texture = nullptr;
AEGfxTexture* macrophage_Texture = nullptr;

// Fonts
s8 font;
s8 k_font;

// Load texture data straight to fonts/ textures
void load_data_textures() {

    std::ifstream file("Assets/Data/textures.json");
    if (!file) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(file);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return;
    }

#define LOAD_STRING(key, var) \
    if (doc.HasMember(key) && doc[key].IsString()) \
        var = AEGfxTextureLoad(doc[key].GetString());

    LOAD_STRING("grid_texture", grid_texture);
    LOAD_STRING("path_texture", path_texture);
    LOAD_STRING("spawn", spawn);
    LOAD_STRING("button_def", button_def);
    LOAD_STRING("button_clicked", button_clicked);
    LOAD_STRING("credits_bg", credits_bg);
    LOAD_STRING("digipen_logo", digipen_logo);
    LOAD_STRING("main_Menu_Bg_Texture", main_Menu_Bg_Texture);
    LOAD_STRING("WIN_PAGE_texture", WIN_PAGE_texture);
    LOAD_STRING("LOST_PAGE_texture", LOST_PAGE_texture);
    LOAD_STRING("Back_Button_Texture", Back_Button_Texture);
    LOAD_STRING("background_texture", background_texture);
    LOAD_STRING("arrow", arrow);
    LOAD_STRING("arrow_2", arrow_2);
    LOAD_STRING("dengue_texture", dengue_texture);
    LOAD_STRING("shingles_texture", shingles_texture);
    LOAD_STRING("chickenpox_texture", chickenpox_texture);
    LOAD_STRING("polio_texture", polio_texture);
    LOAD_STRING("flu_texture", flu_texture);
    LOAD_STRING("hfmd_texture", hfmd_texture);
    LOAD_STRING("ebola_Texture", ebola_Texture);
    LOAD_STRING("dna", dna);
    LOAD_STRING("guide_1", guide_1);
    LOAD_STRING("guide_2", guide_2);
    LOAD_STRING("guide_3", guide_3);
    LOAD_STRING("guide_4", guide_4);
    LOAD_STRING("guide_5", guide_5);
    LOAD_STRING("boss_1_active", boss_1_active);
    LOAD_STRING("boss_1_t", boss_1_t);
    LOAD_STRING("boss_1_inv", boss_1_inv);
    LOAD_STRING("boss_2_t", boss_2_t);
    LOAD_STRING("boss_2_inv", boss_2_inv);
    
    LOAD_STRING("lymphocyte_Texture", lymphocyte_Texture);
    LOAD_STRING("macrophage_Texture", macrophage_Texture);

    if (doc.HasMember("font") && doc["font"].IsArray()) {
        const auto& f = doc["font"];
        if (f.Size() == 2 && f[0].IsString() && f[1].IsInt())
            font = AEGfxCreateFont(f[0].GetString(), f[1].GetInt());
    }

    if (doc.HasMember("k_font") && doc["k_font"].IsArray()) {
        const auto& kf = doc["k_font"];
        if (kf.Size() == 2 && kf[0].IsString() && kf[1].IsInt())
            k_font = AEGfxCreateFont(kf[0].GetString(), kf[1].GetInt());
    }

    file.close();

#undef LOAD_STRING
}


void free_textures() {

    // Free textures associated with GameObjects
    if (grid_texture) {
        AEGfxTextureUnload(grid_texture);
        grid_texture = NULL;
    }


    if (digipen_logo) {
        AEGfxTextureUnload(digipen_logo);
        digipen_logo = NULL;
    }

    if (credits_bg) {
        AEGfxTextureUnload(credits_bg);
        credits_bg = NULL;
    }

    if (spawn) {
        AEGfxTextureUnload(spawn);
        spawn = NULL;
    }

    if (path_texture) {
        AEGfxTextureUnload(path_texture);
        path_texture = NULL; 
    }

    if (ebola_Texture) {
        AEGfxTextureUnload(ebola_Texture);
        ebola_Texture = NULL; 
    }

    if (button_clicked) {
        AEGfxTextureUnload(button_clicked);
        button_clicked = NULL;
    }

    if (button_def) {
        AEGfxTextureUnload(button_def);
        button_def = NULL;
    }

    if (dengue_texture) {
        AEGfxTextureUnload(dengue_texture);
        dengue_texture = NULL;
    }

    if (chickenpox_texture) {
        AEGfxTextureUnload(chickenpox_texture);
        chickenpox_texture = NULL;
    }
    if (shingles_texture) {
        AEGfxTextureUnload(shingles_texture);
        shingles_texture = NULL;
    }
    if (main_Menu_Bg_Texture) {
        AEGfxTextureUnload(main_Menu_Bg_Texture);
        main_Menu_Bg_Texture = NULL;
    }
    if (Back_Button_Texture) {
        AEGfxTextureUnload(Back_Button_Texture);
        Back_Button_Texture = NULL;
    }

    if (arrow) {
        AEGfxTextureUnload(arrow);
        arrow = NULL;
    }
    if (arrow_2) {
        AEGfxTextureUnload(arrow_2);
        arrow_2 = NULL;
    }

    if (dna) {
        AEGfxTextureUnload(dna);
        dna = NULL;
    }

    if (guide_1) {
        AEGfxTextureUnload(guide_1);
        guide_1 = NULL;
    }

    if (guide_2) {
        AEGfxTextureUnload(guide_2);
        guide_2 = NULL;
    }

    if (guide_3) {
        AEGfxTextureUnload(guide_3);
        guide_3 = NULL;
    }

    if (guide_4) {
        AEGfxTextureUnload(guide_4);
        guide_4 = NULL;
    }

    if (guide_5) {
        AEGfxTextureUnload(guide_5);
        guide_5 = NULL;
    }

    if (boss_1_t) {
        AEGfxTextureUnload(boss_1_t);
        boss_1_t = NULL;
    }


    if (boss_1_active) {
        AEGfxTextureUnload(boss_1_active);
        boss_1_active = NULL;
    }

    if (boss_2_t) {
        AEGfxTextureUnload(boss_2_t);
        boss_2_t = NULL;
    }

    if (boss_1_inv) {
        AEGfxTextureUnload(boss_1_inv);
        boss_1_inv = NULL;
    }

    if (boss_2_inv) {
        AEGfxTextureUnload(boss_2_inv);
        boss_2_inv = NULL;
    }


  
    if (font) {
        AEGfxDestroyFont(font); 
        font = NULL; 
    }
    if (k_font) {
        AEGfxDestroyFont(k_font); 
        k_font = NULL; 
    }

    if (WIN_PAGE_texture) AEGfxTextureUnload(WIN_PAGE_texture); if (LOST_PAGE_texture) AEGfxTextureUnload(LOST_PAGE_texture);
    WIN_PAGE_texture = nullptr; LOST_PAGE_texture = nullptr;

    if (lymphocyte_Texture) AEGfxTextureUnload(lymphocyte_Texture); lymphocyte_Texture = nullptr;
    if (macrophage_Texture) AEGfxTextureUnload(macrophage_Texture); macrophage_Texture = nullptr;
    if (polio_texture) AEGfxTextureUnload(polio_texture); polio_texture = nullptr;
    if (flu_texture) AEGfxTextureUnload(flu_texture); flu_texture = nullptr;
    if (hfmd_texture) AEGfxTextureUnload(hfmd_texture); hfmd_texture = nullptr;
    if (background_texture) AEGfxTextureUnload(background_texture); background_texture = nullptr;
}


// Load Json data
void load_data() {
    main_menu_load_json_data();
    polio_load_json_data();
    load_Upgrade_Data();
    load_data_path();
    load_data_textures();
    shooting_load_json_data();
    load_data_confirm_quit();
    load_data_circle();
    load_data_rectangle();
    load_data_hpbar();
    load_data_bgmusic();;
    load_data_mesh();
    load_data_upgradetree_constants();
    load_data_countdown();
    load_data_collision();
    load_data_spawn();
    load_data_event();
    load_data_ppg();
    load_data_para();
    load_data_platelets();
    load_data_vaccine();
    load_data_pausescreen();
    load_data_towers();
    load_data_spritesheets();
    load_data_credits();
    load_data_guide();
    load_data_virulence_meter();
    load_data_settings();
    load_data_lvl_selector();
    load_data_virus();
    load_data_grid();
}