/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      virus.cpp
*@author(s)     Ethan Ng (Primary Author, 60%)
*               Alyssa Nicole Alejandro (Secondary Author, 20%)
*               Dalton Koh (Secondary Author, 15%)
*               Ang Yu Xuan (Secondary Author, 5%)
*@DP emails     n.ethanyongle@digipen.edu
*               alejandro.a@digipen.edu
*               d.koh@digipen.edu
*               yuxuan.ang@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file defines the logic for the virus, consisting of movement, initialization,
*               deletion, and unique mechanics
******************************************************************************/

#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <sstream>

#include "circle.h" 
#include "AEEngine.h"
#include "collision.h"
#include "virus.h"
#include "towers.h"
#include <math.h> // For cosf and sinf
#include <vector>
#include <iostream>
#include "healthbar.h"
#include "Shooting_System.h"
#include "Polio.h"
#include "setting.h"
#include "level.h"
#include "textureselect.h"
#include "rectangle.h"
#include "gamestatemanager.h"



static float default_offset{};
static float default_colour_multiplier{};
static float virus_default_radius{};
static float default_bullet_speed{};
static float default_bullet_radius{};
static float start_atk_cd{};
static float start_ability_cd{};
static float virus_scale{};
static unsigned int brown{};
static unsigned int purple{};
static unsigned int red{};
static unsigned int light_purple{};
static unsigned int yellow{};
static unsigned int green{};
static unsigned int dark_purple{};
static unsigned int gray{};
static float default_attack_radius{};
static float ebola_attack_radius{};
static float polio_attack_radius{};
static float polio_spawn_interval{};
static float polio_generate_interval{};
static float ebola_radius{};
static int polio_resource_generated{};
static float mini_bar_pos_adjustment{};
static float mini_bar_scale{};
static int minion_hp{};
static int minion_dmg{};
static float minion_ms{};
static float minion_generate_interval{};
static int minion_resource_generated{};
static int death_threshold{};
static int hfmd_resource_generated{};
static float hfmd_generate_interval{};
static int meter_adjustmet{};
static int victory_condition{};
bool chickenpox_Defeated = false;
static int global_index;
int upgrade_points;
struct Saved_Virus_Stats saved_Virus_Stats[10];
static int preallocated_resource{};
static int hp_value_100{};
static int hp_value_200{};
static int hp_value_120{};
static int hp_value_150{};
static int hp_value_700{};
static int damage_10{};
static int damage_25{};
static float movement_100{};
static float movement_50{};
static float movement_110{};
static float movement_75{};
static float movement_30{};
static float movement_40{};
static long long base_timer_set{};

//Loading data from json
void load_data_virus() {
    std::fstream data("Assets/Data/virus.json");
    if (!data) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(data);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return; // Exit the function on parse error
    }

    // Assigning data from file to variable
    preallocated_resource = doc["preallocated_resource"].GetInt();
    default_offset = doc["default_offset"].GetFloat();
    default_colour_multiplier = doc["default_colour_multiplier"].GetFloat();
    virus_default_radius = doc["virus_default_radius"].GetFloat();
    default_bullet_radius = doc["default_bullet_radius"].GetFloat();
    default_bullet_speed = doc["default_bullet_speed"].GetFloat();
    start_atk_cd = doc["start_atk_cd"].GetFloat();
    start_ability_cd = doc["start_ability_cd"].GetFloat();
    brown = static_cast<unsigned int>(doc["brown"].GetUint64());
    red = static_cast<unsigned int>(doc["red"].GetUint64());
    purple = static_cast<unsigned int>(doc["purple"].GetUint64());
    dark_purple = static_cast<unsigned int>(doc["dark_purple"].GetUint64());
    light_purple = static_cast<unsigned int>(doc["light_purple"].GetUint64());
    green = static_cast<unsigned int>(doc["green"].GetUint64());
    yellow = static_cast<unsigned int>(doc["yellow"].GetUint64());
    gray = static_cast<unsigned int>(doc["gray"].GetUint64());
    default_attack_radius = doc["default_attack_radius"].GetFloat();
    virus_scale = doc["virus_scale"].GetFloat();
    ebola_attack_radius = doc["ebola_attack_radius"].GetFloat();
    polio_attack_radius = doc["polio_attack_radius"].GetFloat();
    polio_spawn_interval = doc["polio_spawn_interval"].GetFloat();
    polio_generate_interval = doc["polio_generate_interval"].GetFloat();
    polio_resource_generated = doc["polio_resource_generated"].GetInt();
    ebola_radius = doc["ebola_radius"].GetFloat();
    mini_bar_pos_adjustment = doc["mini_bar_pos_adjustment"].GetFloat();
    global_index = doc["global_index"].GetInt();
    mini_bar_scale = doc["mini_bar_scale"].GetFloat();
    minion_hp = doc["minion_hp"].GetInt();
    minion_dmg = doc["minion_dmg"].GetInt();
    minion_ms = doc["minion_ms"].GetFloat();
    minion_generate_interval = doc["minion_generate_interval"].GetFloat();
    minion_resource_generated = doc["minion_resource_generated"].GetInt();
    death_threshold = doc["death_threshold"].GetInt();
    hfmd_resource_generated = doc["hfmd_resource_generated"].GetInt();
    hfmd_generate_interval = doc["hfmd_generate_interval"].GetFloat();
    meter_adjustmet = doc["meter_adjustmet"].GetInt();
    victory_condition = doc["victory_condition"].GetInt();
    hp_value_100 = doc["hp_value_100"].GetInt();
    hp_value_200 = doc["hp_value_200"].GetInt();
    hp_value_120 = doc["hp_value_120"].GetInt();
    hp_value_150 = doc["hp_value_150"].GetInt();
    hp_value_700 = doc["hp_value_700"].GetInt();
    damage_10 = doc["damage_10"].GetInt();
    damage_25 = doc["damage_25"].GetInt();
    movement_100 = doc["movement_100"].GetFloat();
    movement_50 = doc["movement_50"].GetFloat();
    movement_110 = doc["movement_110"].GetFloat();
    movement_75 = doc["movement_75"].GetFloat();
    movement_30 = doc["movement_30"].GetFloat();
    movement_40 = doc["movement_40"].GetFloat();
    base_timer_set = doc["base_timer_set"].GetInt();

}


//Initializing default virus stats
void setDefaultVirusStats() {
    saved_Virus_Stats[hfmdvirus] = { hp_value_100, {} , movement_100, {false, false} };
    saved_Virus_Stats[chickenpox] = { hp_value_200, {} , movement_50, {false, false} };
    saved_Virus_Stats[dengue] = { hp_value_120, damage_10, movement_110, {false, false} };
    saved_Virus_Stats[shingle] = { hp_value_150, damage_25, movement_75, {false, false} };
    saved_Virus_Stats[polio_virus] = { hp_value_200, {} , movement_30, {false, false} };
    saved_Virus_Stats[ebola] = { hp_value_700, damage_25, movement_40, {false, false} };
}

//Spawned virus logic
virus* spawnVirus(int type, float x, float y, std::vector<virus*>& virus_List) {
    virus* new_Virus = new virus;
    if (!new_Virus) return nullptr;

    // Apply saved base stats + any previous upgrades from `saved_Virus_Stats`
    new_Virus->basehp = saved_Virus_Stats[type].basehp;
    new_Virus->currenthp = new_Virus->basehp; // Start at full HP
    new_Virus->dmg = saved_Virus_Stats[type].dmg;
    new_Virus->ms = saved_Virus_Stats[type].ms;
    new_Virus->original_ms = new_Virus->ms;
    new_Virus->currentms = new_Virus->ms;
    // Initialize general virus properties
    new_Virus->x = x;
    new_Virus->y = y;
    new_Virus->indexing = global_index++;
    new_Virus->radius = virus_default_radius;
    new_Virus->state = true;
    new_Virus->attackstate = false;
    new_Virus->atkcd = start_atk_cd;
    new_Virus->virus_bullet.bullet_movement_speed = default_bullet_speed;
    new_Virus->virus_bullet.bullet_radius = default_bullet_radius;
    new_Virus->virus_bullet.start_X = x;
    new_Virus->virus_bullet.start_Y = y;
    new_Virus->type = type;
    new_Virus->ppg_effect = false;
    new_Virus->start_time = base_timer_set;
    new_Virus->current_time = base_timer_set;
    new_Virus->elapsed_time = base_timer_set;
    new_Virus->time_before_death = base_timer_set;
    new_Virus->timer = NULL;
    new_Virus->texture = nullptr;
    new_Virus->attacker = false;
    new_Virus->attackradius = NULL;
    new_Virus->color = NULL;
    new_Virus->spawnx = NULL;
    new_Virus->spawny = NULL;
    new_Virus->mesh = nullptr;

    //  Set Type-Specific Properties (Base + Upgrades Applied)
    switch (type) {
    case hfmdvirus:
        new_Virus->timer = start_ability_cd;
        new_Virus->color = brown; // Brown
        new_Virus->attacker = false;
        new_Virus->type = hfmdvirus;
        break;
    case chickenpox:
        new_Virus->color = purple; // Purple
        new_Virus->attacker = false;
        new_Virus->type = chickenpox;
        break;
    case dengue:
        new_Virus->color = red; // Red
        new_Virus->attackradius = default_attack_radius;
        new_Virus->attacker = true;
        new_Virus->type = dengue;
        break;
    case shingle:
        new_Virus->color = light_purple; // Light Purple
        new_Virus->attackradius = default_attack_radius;
        new_Virus->attacker = true;
        new_Virus->type = shingle;
        break;
    case polio_virus:
        new_Virus->color = yellow; // Yellow for polio
        new_Virus->attackradius = polio_attack_radius;
        new_Virus->polio_virus.spawn_time_interval = polio_spawn_interval; //time interval for spawning minion polio
        new_Virus->polio_virus.generate_time_interval = polio_generate_interval; //time interval for generate gameplay resource
        //num gameplay resource generated by polio when resource generation timer == 0
        new_Virus->polio_virus.generate_resource = polio_resource_generated;
        new_Virus->attacker = false;
        new_Virus->type = polio_virus;
        break;
    case minion_polio_virus:
        new_Virus->basehp = minion_hp;
        new_Virus->currenthp = minion_hp;
        new_Virus->dmg = minion_dmg;
        new_Virus->ms = minion_ms;
        new_Virus->original_ms = minion_ms;
        new_Virus->currentms = minion_ms;
        new_Virus->color = green; // green Color for minion polio
        new_Virus->minion_polio_virus.generate_time_interval = minion_generate_interval; //time interval for generate gameplay resource
        //num gameplay resource generated by minion polio when resource generation timer == 0
        new_Virus->minion_polio_virus.generate_resource = minion_resource_generated;
        new_Virus->attacker = false;
        break;
    case ebola:
        new_Virus->color = light_purple; // Light Purple
        new_Virus->attackradius = ebola_attack_radius;
        new_Virus->radius = ebola_radius;
        new_Virus->attacker = true;
        new_Virus->type = ebola;
        break;
    default:
        new_Virus->color = gray; // Gray (Default for unknown types)
        break;
    }

    //Creating rectangle from virus mesh
    Create_Rectangle(
        &new_Virus->mesh,
        new_Virus->radius,
        new_Virus->radius,
        yellow,
        &new_Virus->transform,
        virus_scale, virus_scale,
        new_Virus->x,
        new_Virus->y);



    // Create HP bar only for newly spawned virus
    make_hp(virus_List.size(), new_Virus->x, new_Virus->y - mini_bar_pos_adjustment, mini_bar_scale, true);

    // Add new virus to the list
    virus_List.push_back(new_Virus);

    return new_Virus;
}



//Updating of viruses(movement, abilities, death)
void update_Enemies(std::vector<virus*>& spawned_Virus, f64 dt, float targetx,
 int& meter, int *resources)
{
    for (size_t i{}; i < spawned_Virus.size(); ++i) {
        virus* current_Virus = spawned_Virus[i];
    
        // virus
        if (current_Virus->state) {

            // **Movement Logic**
            if (current_Virus->attackstate == false) {
                current_Virus->x += static_cast<float>(current_Virus->ms * dt);
            }

            // hp bar
            float virusHPPercentage = static_cast<float>(current_Virus->currenthp) / static_cast<float>(current_Virus->basehp);

            if (current_Virus->currenthp >= death_threshold) {
                update_hp_V(current_Virus->indexing, virusHPPercentage, static_cast<float>(dt), spawned_Virus[i]->x, spawned_Virus[i]->y);

            }


            // **Check if Chickenpox Virus is Defeated to know when to switch to shingles**
            if (current_Virus->type == chickenpox && (current_Virus->currenthp <= death_threshold || current_Virus->state ==false)) {
                chickenpox_Defeated = true; // Flag that Chickenpox has been defeated
            }

            // if the current virus is polio, update the polio behaviour here
            if (current_Virus->type == polio_virus) {
                Polio_Behaviour(current_Virus, dt, resources);
            }

            // if the current virus is minion polio, update the minion polio behaviour here
            if (current_Virus->type == minion_polio_virus) {
                Minion_Polio_Behaviour(current_Virus, dt, resources);
            }

            //HFMD Resource generation
            if (current_Virus->type == hfmdvirus && current_Virus->state == true) {
                current_Virus->timer += static_cast<float>(dt);
                if (current_Virus->timer >= hfmd_generate_interval) {
                    *resources += hfmd_resource_generated;
                    current_Virus->timer = start_ability_cd;
                 
                }
                
            }

            // **Virus Reaches Target, damage the enemy (e.g., Player Base)**
            if (current_Virus->x >=  targetx ) {
                if (!current_Virus->dmg) meter += meter_adjustmet;
                meter += current_Virus->dmg/meter_adjustmet;
                current_Virus->currenthp = death_threshold;
                if (current_Virus->type == chickenpox) {
                    chickenpox_Defeated = true;
                }
                if (meter >= victory_condition) {
                    level_end = vic;
                }
            }
        }
    }
    //Erase dead viruses
    spawned_Viruses.erase(
        std::remove_if(spawned_Virus.begin(), spawned_Virus.end(),
            [](virus* v) {
                if (v->currenthp <= death_threshold  || v->state == false) {
                    AEGfxMeshFree(v->mesh);
                    delete v; // Free memory
                    return true;
                }
                return false;
            }),
        spawned_Virus.end()
    );
}



//Rendering of viruses
void render_Enemies(std::vector<virus*>& spawned_Virus, AEMtx33* transforms, AEMtx33* scale, AEMtx33* rotate, AEMtx33* translate,
     float scalex, float scaley)
{
    for (size_t i{}; i < spawned_Virus.size(); ++i) {
        virus* current_Virus = spawned_Virus[i];
        if (current_Virus->state) {
            // **Render Virus**
            render(scale, rotate, translate, &transforms[null], scalex, scaley, current_Virus->x, current_Virus->y);

            if (current_Virus->type == ebola) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(ebola_Texture, default_offset, default_offset);
            }
            if (current_Virus->type == hfmdvirus) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(hfmd_texture, default_offset, default_offset);
            }
            if (current_Virus->type == polio_virus) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(polio_texture, default_offset, default_offset);
            }
            if (current_Virus->type == minion_polio_virus) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(polio_texture, default_offset, default_offset);
            }
            if (current_Virus->type == dengue) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(dengue_texture, default_offset, default_offset);
            }
            if (current_Virus->type == shingle) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(shingles_texture, default_offset, default_offset);
            }
            if (current_Virus->type == chickenpox) {
                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxTextureSet(chickenpox_texture, default_offset, default_offset);
            }
            AEGfxSetColorToMultiply(default_colour_multiplier, default_colour_multiplier, default_colour_multiplier, default_colour_multiplier);

            AEGfxSetTransform(transforms->m);
            AEGfxMeshDraw(current_Virus->mesh, AE_GFX_MDM_TRIANGLES);

            AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        }
    }
}

void Viral_resource() {

    switch (current_State) {
    case null:
        preallocated_resource = preallocated_resource;
        break;
    case levelv_1:
        preallocated_resource = preallocated_resource;
        break;
    case levelv_2:
        preallocated_resource = preallocated_resource;
        break;
    case levelv_3:
        preallocated_resource = preallocated_resource;
        break;
    case levelv_4:
        preallocated_resource = preallocated_resource;
        break;
    case levelv_5:
        preallocated_resource = preallocated_resource;
        break;
    }

    
}

//Cleaning up of meshes
void cleanup_virus(std::vector<virus*>& spawned_Virus) {
    for (virus* i : spawned_Virus) {
        AEGfxMeshFree(i->mesh);
        delete i;
    }
    spawned_Virus.clear();
}