/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      Shooting_System.cpp
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
*@brief:        This file is used to setup the shooting system for virus and tower.
*               
******************************************************************************/

#include <utility>  // For std::pair
#include "AEEngine.h"
#include <vector>
#include "virus.h"
#include "towers.h"
#include "collision.h"
#include "circle.h"
#include <iostream>
#include "rectangle.h"
#include "textureselect.h"
#include "level.h"

#include "document.h"
#include "istreamwrapper.h"

static float bullet_size;
static int number_circle_segments;
static float bullet_RGBA;
static int out_of_array_index;
static int DOT_damage;
static float damage_Per_Second_divider;
static float virus_attack_cooldown_CD;

void shooting_load_json_data() {
	std::ifstream fs("Assets/Data/Shooting.json");
	if (!fs) { //cannot open this json file
		gGameRunning = NULL;
		return;
	}

	rapidjson::IStreamWrapper isw(fs);
	rapidjson::Document doc;
	doc.ParseStream(isw);

	if (doc.HasParseError()) {
		gGameRunning = NULL;
		return; // Exit the function on parse error
	}

	// Assigning data from json file to init variable in this function
	bullet_size = doc["bullet_size"].GetFloat();
	number_circle_segments = doc["number_circle_segments"].GetInt();
	bullet_RGBA = doc["bullet_RGBA"].GetFloat();
	out_of_array_index = doc["out_of_array_index"].GetInt();
	DOT_damage = doc["DOT_damage"].GetInt();
	damage_Per_Second_divider = doc["damage_Per_Second_divider"].GetFloat();
	virus_attack_cooldown_CD = doc["virus_attack_cooldown_CD"].GetFloat();
}

//The function find the displacement between shooter and shooting target
//shooter_circle_x, shooter_circle_y, target_circle_x, target_circle_y
AEGfxVertexList* bullet_mesh = nullptr;
AEGfxVertexList* vbullet_mesh = nullptr;
std::pair<float, float> direction_vector(float& shooter_circle_x, float& shooter_circle_y, float& target_circle_x, float& target_circle_y)
{
	//finding direction vector between shooter and shooting target
	return std::make_pair(shooter_circle_x - target_circle_x, shooter_circle_y - target_circle_y);
}

void Free_Bullet() {
	if (bullet_mesh) {
		AEGfxMeshFree(bullet_mesh);
		bullet_mesh = nullptr; // Prevent dangling pointer
	}
}

void vFree_Bullet() {
	if (vbullet_mesh) {
		AEGfxMeshFree(vbullet_mesh);
		vbullet_mesh = nullptr; // Prevent dangling pointer
	}
}

void initBullet(float& bullet_X, float& bullet_Y, float bullet_radius, unsigned int color, bool is_Virus) {
	AEMtx33 bullettransform;
	AEMtx33 scale,  rotate,  translate;
	render(&scale, &rotate, &translate, &bullettransform, bullet_size, bullet_size, bullet_X, bullet_Y);
	AEGfxSetTransform(bullettransform.m);
	switch (is_Virus) {
	case false:
		Free_Bullet();
		if (bullet_mesh == nullptr) {
			create_circle_mesh(&bullet_mesh, bullet_radius, number_circle_segments, color, color);
		}

		//  Ensure Color Mode is enabled
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);


		AEGfxMeshDraw(bullet_mesh, AE_GFX_MDM_TRIANGLES);
		break;
	
	case true:
		vFree_Bullet();
		if (vbullet_mesh == nullptr) {
			create_circle_mesh(&vbullet_mesh, bullet_radius, number_circle_segments, color, color);

		}
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		AEGfxMeshDraw(vbullet_mesh, AE_GFX_MDM_TRIANGLES);
		break;
	}
	AEGfxSetColorToMultiply(bullet_RGBA, bullet_RGBA, bullet_RGBA, bullet_RGBA);
}


//This function is about the shooting logic for virus as shooter shoot against the tower, DON'T USE for tower shooting virus
void Virus_Shooter_Logic(size_t virus_array_size, std::vector<virus*>virus_array_pointer, size_t tower_array_size,
	struct tower* tower_array_pointer, 
	f64& dt, unsigned int color)
{
	std::pair<float, float> displacement_counting; //variable for find displacement between tower and virus

	for (int virus_i{}; virus_i < virus_array_size; virus_i++) { //loop through the num of virus in array
		for (int tower_i{}; tower_i < tower_array_size; tower_i++) { //loop through the num of tower in array
			virus_array_pointer[virus_i]->atkcd += static_cast<float>(dt);
			//if (virus has not start shooting AND the attack zone of the virus collides with the tower)==TRUE
			//virus must have attack ability == TRUE && tower must be alive == TRUE
			if (virus_array_pointer[virus_i]->attackstate == false && virus_array_pointer[virus_i]->attacker == true &&
				(tower_array_pointer + tower_i)->state != DEAD_STATE &&
				are_circles_intersecting(virus_array_pointer[virus_i]->x,
					virus_array_pointer[virus_i]->y, virus_array_pointer[virus_i]->attackradius,
					(tower_array_pointer + tower_i)->size.xy.x,
					(tower_array_pointer + tower_i)->size.xy.y, (tower_array_pointer + tower_i)->size.radius)&& virus_array_pointer[virus_i]->atkcd >= virus_attack_cooldown_CD)
			{
				//initialize bullet Xpos&Ypos with current virus XPos&Ypos
				virus_array_pointer[virus_i]->virus_bullet.start_X = virus_array_pointer[virus_i]->x;
				virus_array_pointer[virus_i]->virus_bullet.start_Y = virus_array_pointer[virus_i]->y;

				virus_array_pointer[virus_i]->virus_bullet.targeted_virus_index = tower_i;
				virus_array_pointer[virus_i]->attackstate = true; //now the virus is in shooting mode
				virus_array_pointer[virus_i]->atkcd = NULL;
				virus_array_pointer[virus_i]->ms = NULL; //set the movement of virus to ZERO

				break; //Once the virus finds out there is 1 tower to attack, stop searching for other tower, end the inner loop
			}
		}
	}

	for (int virus_i{}; virus_i < virus_array_size; virus_i++) { //loop through the num of virus in array
		for (int tower_i{}; tower_i < tower_array_size; tower_i++) { //loop through the num of tower in array
			//virus must have attack ability == TRUE && tower must be alive == TRUE
			if (virus_array_pointer[virus_i]->attackstate == true && virus_array_pointer[virus_i]->attacker == true &&
				(tower_array_pointer + tower_i)->state != DEAD_STATE &&
				virus_array_pointer[virus_i]->virus_bullet.targeted_virus_index == tower_i && virus_array_pointer[virus_i]->type != ebola) //the virus is in shooting mode
			{

				//find displacement between virus(shooter) and tower(shooting target)
				displacement_counting = direction_vector(virus_array_pointer[virus_i]->virus_bullet.start_X,
					virus_array_pointer[virus_i]->virus_bullet.start_Y, (tower_array_pointer + tower_i)->size.xy.x,
					(tower_array_pointer + tower_i)->size.xy.y);

				//render the movement of bullet shooted by virus
				initBullet(virus_array_pointer[virus_i]->virus_bullet.start_X, virus_array_pointer[virus_i]->virus_bullet.start_Y,
					virus_array_pointer[virus_i]->virus_bullet.bullet_radius, color, true);

				virus_array_pointer[virus_i]->virus_bullet.start_X -=
					displacement_counting.first * ((virus_array_pointer[virus_i]->virus_bullet.bullet_movement_speed) * static_cast<float>(dt)); //update X-movement

				virus_array_pointer[virus_i]->virus_bullet.start_Y -=
					displacement_counting.second * ((virus_array_pointer[virus_i]->virus_bullet.bullet_movement_speed) * static_cast<float>(dt)); //update Y-movement

				//check if the bullet hits the tower
				float tower_collision_radius{}; //this is to ensure the bullet hit the center area of the tower
				if (are_circles_intersecting(virus_array_pointer[virus_i]->virus_bullet.start_X, virus_array_pointer[virus_i]->virus_bullet.start_Y,
					virus_array_pointer[virus_i]->virus_bullet.bullet_radius, (tower_array_pointer + tower_i)->size.xy.x,
					(tower_array_pointer + tower_i)->size.xy.y, tower_collision_radius))
				{
					if ((tower_array_pointer + tower_i)->inv_state == true) {
						(tower_array_pointer + tower_i)->current_hp -= virus_array_pointer[virus_i]->dmg*NULL;
					}
					else {
						//reduce tower's health by the total damage amount inside virus
						if (virus_array_pointer[virus_i]->dmg > (tower_array_pointer + tower_i)->current_hp) {
							(tower_array_pointer + tower_i)->current_hp -= (tower_array_pointer + tower_i)->current_hp;
						}
						else {
							(tower_array_pointer + tower_i)->current_hp -= virus_array_pointer[virus_i]->dmg;
						}
					}
					virus_array_pointer[virus_i]->attackstate = false;//the virus has finished the shooting process, RESET back to attackstate
				}

				if ((tower_array_pointer + tower_i)->current_hp <= NULL) {
					(tower_array_pointer + tower_i)->state = DEAD_STATE;
					for (int virus_index{}; virus_index < virus_array_size; virus_index++) {
						virus_array_pointer[virus_index]->attackstate = false;//the virus has finished the shooting process, RESET back to false attackstate
						virus_array_pointer[virus_index]->currentms = virus_array_pointer[virus_index]->original_ms; //reset back the movement of virus
						//set the virus_bullet.targeted_virus_index to be outside range of vector(-1 index cannot be reached at all)
						virus_array_pointer[virus_index]->virus_bullet.targeted_virus_index = out_of_array_index; 
					}
				}	
			}

			if (virus_array_pointer[virus_i]->attackstate == true && virus_array_pointer[virus_i]->attacker == true &&
				(tower_array_pointer + tower_i)->state != DEAD_STATE && virus_array_pointer[virus_i]->virus_bullet.targeted_virus_index == tower_i && virus_array_pointer[virus_i]->type == ebola) //ebola attack mode
			{
				virus_array_pointer[virus_i]->attackstate = false;//virus moving
				virus_array_pointer[virus_i]->ms = virus_array_pointer[virus_i]->currentms;
				virus_array_pointer[virus_i]->DOT.active = false ;

				// If DoT is not already active, start it
				if (!(virus_array_pointer[virus_i]->DOT.active)) {
					virus_array_pointer[virus_i]->DOT.active = true;
				
					virus_array_pointer[virus_i]->DOT.timeElapsed = {}; // Reset timer
					virus_array_pointer[virus_i]->DOT.totalDamage = DOT_damage; // Set total DoT damage
				}

				// Apply damage gradually over time using dt
				if (virus_array_pointer[virus_i]->DOT.active) {
					float damagePerSecond = static_cast<float > (virus_array_pointer[virus_i]->DOT.totalDamage )/ damage_Per_Second_divider ; // Spread over 3 seconds
					float damageThisFrame = damagePerSecond * static_cast<float>(dt);
					(tower_array_pointer + tower_i)->current_hp -= static_cast<int>(damageThisFrame); // Reduce HP gradually
					virus_array_pointer[virus_i]->DOT.timeElapsed += static_cast<float>(dt); // Update DoT timer
				}

				if ((tower_array_pointer + tower_i)->current_hp <= NULL) {
					(tower_array_pointer + tower_i)->state = DEAD_STATE;
					for (int virus_index{}; virus_index < virus_array_size; virus_index++) {
						virus_array_pointer[virus_index]->attackstate = false;//the virus has finished the shooting process, RESET back to false attackstate
					}
				}
			}
		}
	}
}

float DistanceSquared(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return dx * dx + dy * dy;
}

//This function is about the shooting logic for tower as shooter shoot against the virus, DON'T USE for virus shooting tower
void Tower_Shooter_Logic(size_t tower_array_size, struct tower* tower_array_pointer,
	size_t virus_array_size, std::vector<virus*> virus_array_pointer,
	f64& dt, unsigned int color)
{
	std::pair<float, float> displacement_counting; //variable for find displacement between tower and virus

	for (int tower_i{}; tower_i < tower_array_size; tower_i++) { //loop through the num of tower in array
		for (int virus_i{}; virus_i < virus_array_size; virus_i++) { //loop through the num of virus in array
			//if (tower has not start shooting AND the attack zone of the tower collides with the virus)==TRUE
			(tower_array_pointer + tower_i)->cd_time += static_cast<float>(dt);
			
				if (((tower_array_pointer + tower_i)->state == COOLDOWN_STATE) &&
					are_circles_intersecting((tower_array_pointer + tower_i)->size.xy.x,
						(tower_array_pointer + tower_i)->size.xy.y,
						(tower_array_pointer + tower_i)->size.coll_radius, virus_array_pointer[virus_i]->x,
						virus_array_pointer[virus_i]->y, virus_array_pointer[virus_i]->radius))
				{
					if ((tower_array_pointer + tower_i)->cd_time >= (tower_array_pointer + tower_i)->atk_spd && (tower_array_pointer + tower_i)->type == lymphocyte) {
							//initialize bullet Xpos&Ypos with current tower XPos&Ypos
							(tower_array_pointer + tower_i)->tower_bullet.start_X = (tower_array_pointer + tower_i)->size.xy.x;
							(tower_array_pointer + tower_i)->tower_bullet.start_Y = (tower_array_pointer + tower_i)->size.xy.y;

							(tower_array_pointer + tower_i)->tower_bullet.targeted_virus_index = virus_i;
							(tower_array_pointer + tower_i)->state = ATTACKING_STATE; //now the tower is in shooting mode,
							(tower_array_pointer + tower_i)->cd_time = NULL;
							break;//Once the tower finds out there is 1 virus to attack, stop searching for other virus, end the inner loop
						
					}
					else if ((tower_array_pointer + tower_i)->type == macrophage && (tower_array_pointer + tower_i)->mesh != NULL) {
						macrophage_skill(virus_array_pointer);
					}
				
			}
		}
	}

	for (int tower_i{}; tower_i < tower_array_size; tower_i++) { //loop through the num of tower in array
		for (int virus_i{}; virus_i < virus_array_size; virus_i++) { //loop through the num of virus in array
			if ((tower_array_pointer + tower_i)->state == ATTACKING_STATE &&
				(tower_array_pointer + tower_i)->tower_bullet.targeted_virus_index == virus_i) //the tower is in shooting mode
			{
				//find displacement between tower(shooter) and virus(shooting target)
				displacement_counting = direction_vector((tower_array_pointer + tower_i)->tower_bullet.start_X,
					(tower_array_pointer + tower_i)->tower_bullet.start_Y, virus_array_pointer[virus_i]->x,
					virus_array_pointer[virus_i]->y);

				//render the movement of bullet shooted by tower
				initBullet((tower_array_pointer + tower_i)->tower_bullet.start_X,
					(tower_array_pointer + tower_i)->tower_bullet.start_Y, (tower_array_pointer + tower_i)->tower_bullet.bullet_radius,
					color, false);

				(tower_array_pointer + tower_i)->tower_bullet.start_X -=
					displacement_counting.first * ((tower_array_pointer + tower_i)->tower_bullet.bullet_movement_speed * static_cast<float>(dt)); //update X-movement


				(tower_array_pointer + tower_i)->tower_bullet.start_Y -=
					displacement_counting.second * ((tower_array_pointer + tower_i)->tower_bullet.bullet_movement_speed*static_cast<float>(dt)); //update Y-movement

				//check if the bullet hits the virus
				float virus_collision_radius{}; //this is to ensure the bullet hit the center area of the virus
				if (are_circles_intersecting((tower_array_pointer + tower_i)->tower_bullet.start_X, (tower_array_pointer + tower_i)->tower_bullet.start_Y,
					(tower_array_pointer + tower_i)->tower_bullet.bullet_radius, virus_array_pointer[virus_i]->x,
					virus_array_pointer[virus_i]->y, virus_collision_radius))

					if (DistanceSquared((tower_array_pointer + tower_i)->tower_bullet.start_X,
						(tower_array_pointer + tower_i)->tower_bullet.start_Y,
						virus_array_pointer[virus_i]->x,
						virus_array_pointer[virus_i]->y) <=
						((tower_array_pointer + tower_i)->tower_bullet.bullet_radius *
							(tower_array_pointer + tower_i)->tower_bullet.bullet_radius))
				{
						if ((tower_array_pointer + tower_i)->atk > virus_array_pointer[virus_i]->currenthp) {
							virus_array_pointer[virus_i]->currenthp -= virus_array_pointer[virus_i]->currenthp;
						}
						else {
							virus_array_pointer[virus_i]->currenthp -= (tower_array_pointer + tower_i)->atk;
						}
					
					(tower_array_pointer + tower_i)->state = COOLDOWN_STATE;//the tower has finished the shooting process, RESET back to COOLDOWN_STATE
				}

				//check if the virus is died
				if (virus_array_pointer[virus_i]->currenthp <= NULL) {
					for (int tower_index{}; tower_index < tower_array_size; tower_index++) {
						//set the tower_bullet.targeted_virus_index to be outside range of vector(-1 index cannot be reached at all)
						(tower_array_pointer + tower_index)->tower_bullet.targeted_virus_index = out_of_array_index;
						(tower_array_pointer + tower_index)->state = COOLDOWN_STATE;//the tower has finished the shooting process, RESET back to COOLDOWN_STATE

					}
				}

			}
		}
	}
}

void render_Virus_Bullet(size_t virus_array_size, std::vector<virus*>virus_array_pointer, size_t tower_array_size,
	struct tower* tower_array_pointer, unsigned int color)
{
	for (int virus_i{}; virus_i < virus_array_size; virus_i++) { //loop through the num of virus in array
		for (int tower_i{}; tower_i < tower_array_size; tower_i++) { //loop through the num of tower in array
			//virus must have attack ability == TRUE && tower must be alive == TRUE
			if (virus_array_pointer[virus_i]->attackstate == true && virus_array_pointer[virus_i]->attacker == true &&
				(tower_array_pointer + tower_i)->state != DEAD_STATE &&
				virus_array_pointer[virus_i]->virus_bullet.targeted_virus_index == tower_i && virus_array_pointer[virus_i]->type != ebola) //the virus is in shooting mode
			{
				//render the movement of bullet shooted by virus
				initBullet(virus_array_pointer[virus_i]->virus_bullet.start_X, virus_array_pointer[virus_i]->virus_bullet.start_Y,
					virus_array_pointer[virus_i]->virus_bullet.bullet_radius, color, true);
			}
		}
	}
}

void render_Tower_Bullet(size_t tower_array_size, struct tower* tower_array_pointer,
	size_t virus_array_size, unsigned int color)
{
	for (int tower_i{}; tower_i < tower_array_size; tower_i++) { //loop through the num of tower in array
		for (int virus_i{}; virus_i < virus_array_size; virus_i++) { //loop through the num of virus in array
			if ((tower_array_pointer + tower_i)->state == ATTACKING_STATE &&
				(tower_array_pointer + tower_i)->tower_bullet.targeted_virus_index == virus_i) //the tower is in shooting mode
			{
				//render the movement of bullet shooted by tower
				initBullet((tower_array_pointer + tower_i)->tower_bullet.start_X,
					(tower_array_pointer + tower_i)->tower_bullet.start_Y, (tower_array_pointer + tower_i)->tower_bullet.bullet_radius,
					color, false);
			}

		}
	}
}
