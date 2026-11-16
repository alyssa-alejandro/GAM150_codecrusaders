/******************************************************************************
* All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
*
*@filename      upgradetree.cpp
*@author(s)     Dalton Koh (Primary Author, 100%)
*@DP emails     d.koh@digipen.edu
*@course        Software Engineering Project 2
*@teamname      Code Crusaders
*@gamename      Viral Invaders
*
*@brief:        This file includes the definitions to initiate, logify, render 
*               and free the upgradestree page! The file also includes the load
*               and save data function to load in required data!
******************************************************************************/

#include "document.h"
#include "istreamwrapper.h"
#include <fstream>
#include <sstream>
#include "level.h"

#include "AEEngine.h"
#include "rectangle.h"
#include "textureselect.h"
#include <stdio.h>
#include "upgradetree.h"
#include "countdown.h"
#include "virus.h"
#include "level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "levelselector.h"
#include <string>

// Define the background and buttons
struct UpgradeButton buttons[16];
AEGfxVertexList* bg_Mesh{};
AEMtx33 bg_Transform;
AEGfxVertexList* Exit_Mesh{};
AEMtx33 Exit_Transform;

// Buttons
static float btn_Width{}, btn_Height{};
static float start_X{}, start_Y = {};
static float start_Y2{};
static float start_Y3{};
static float spacing{};
static float Add_MS{};
static int Add_DMG{};
static int Add_HP{};
static int Upgrade_points_minus{};
static int num_5{};
static int num_10{};
static int num_15{};

// Base texts on buttons
static std::string HFMD{};
static std::string Ebola{};
static std::string Polio{};
static std::string Shingles{};
static std::string ChickenPox{};

static std::string upgrade_lvl1{};
static std::string upgrade_lvl2{};
static std::string Exit{};
static std::string upgrade_1_Des{};
static std::string upgrade_2_Des{};

// Initial setting coordinates
static float Set_x{};
static float Set_y{};
static float scale_1{};
static float Exit_x{};
static float Exit_y{};
static float X_offset{};
static float Text_width{};
static float button_5x_offset{};
static float button_4x_offset{};
static float Text_x{};
static float Text_y{};
static float Text_x_offset{};
static float Upgrade_1_text_x{};
static float Upgrade_2_text_x{};
static float Upgrade_1_text_y{};
static float Upgrade_2_text_y{};

void load_data_upgradetree_constants() {

    // File stream
    std::fstream file("Assets/Data/upgradetree_constants.json");

    // IF file fails to open
    if (!file) {
        gGameRunning = NULL;
        return;
    }

    rapidjson::IStreamWrapper isw(file); // wrap file stream into something rapid json can read from 
    rapidjson::Document doc; // Create json object
    doc.ParseStream(isw); // Parses contents from the isw stream into doc. so you can access it later on


    // If parse error or error getting the right data type
    if (doc.HasParseError()) {
        gGameRunning = NULL;
        return;
    }

    // Required data to be retrieved 
    btn_Width = doc["btn_Width"].GetFloat();
    btn_Height = doc["btn_Height"].GetFloat();
    start_X = doc["start_X"].GetFloat();
    start_Y = doc["start_Y"].GetFloat();
    start_Y2 = doc["start_Y2"].GetFloat();
    start_Y3 = doc["start_Y3"].GetFloat();
    spacing = doc["spacing"].GetFloat();
    Set_x = doc["Set_x"].GetFloat();
    Set_y = doc["Set_y"].GetFloat();
    Add_DMG = doc["Add_DMG"].GetInt();
    Add_HP = doc["Add_HP"].GetInt();
    Add_MS = doc["Add_MS"].GetFloat();
    Upgrade_points_minus = doc["Upgrade_points_minus"].GetInt();
    upgrade_lvl1 = doc["upgrade_lvl1"].GetString();
    upgrade_lvl2 = doc["upgrade_lvl2"].GetString();
    Exit = doc["Exit"].GetString();
    HFMD = doc["HFMD"].GetString();
    Ebola = doc["Ebola"].GetString();
    Polio = doc["Polio"].GetString();
    Shingles = doc["Shingles"].GetString();
    ChickenPox = doc["ChickenPox"].GetString();
    upgrade_1_Des = doc["upgrade_1_Des"].GetString();
    upgrade_2_Des = doc["upgrade_2_Des"].GetString();
    
    num_5 = doc["5"].GetInt();
    num_10 = doc["10"].GetInt();
    num_15 = doc["15"].GetInt();
    scale_1 = doc["scale_1"].GetFloat();
    Exit_x = doc["Exit_x"].GetFloat();
    Exit_y = doc["Exit_y"].GetFloat();
    X_offset = doc["X_offset"].GetFloat();
    Text_width = doc["Text_width"].GetFloat();
    button_5x_offset = doc["button_x_offset"].GetFloat();
    button_4x_offset = doc["button_4x_offset"].GetFloat();
    Text_x = doc["Text_x"].GetFloat();
    Text_y = doc["Text_y"].GetFloat();
    Text_x_offset = doc["Text_x_offset"].GetFloat();
    Upgrade_1_text_x = doc["Upgrade_1_text_x"].GetFloat();
    Upgrade_2_text_x = doc["Upgrade_2_text_x"].GetFloat();
    Upgrade_1_text_y = doc["Upgrade_1_text_y"].GetFloat();
    Upgrade_2_text_y = doc["Upgrade_2_text_y"].GetFloat();

}

// Setting function to handle both update and render
void set_upgrade_tree() {
   

    // Render upgrade buttons and handle button click
    update_upgrade_tree();
    render_upgrade_tree();

    // Render texts for the page 
    render_text();
   

}

// Function to initialize the upgrade tree page
void init_upgrade_tree() {

    // Load upgrade data from file
    load_Upgrade_Data();

    {
        if (!bg_Mesh) Create_Rectangle(&bg_Mesh, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), {}, &bg_Transform, scale_1, scale_1, {}, {});
    }
   
    // Create Next Button (for first page)
    buttons[num_15].width = btn_Width;
    buttons[num_15].height = btn_Height;
    buttons[num_15].name = Exit.c_str();
    buttons[num_15].x = Set_x;
    buttons[num_15].y = Set_y;
   

    if (!buttons[15].mesh) Create_Rectangle(&buttons[15].mesh, btn_Width, btn_Height, buttons[num_15].color, &buttons[num_15].transform, scale_1, scale_1, buttons[num_15].x, buttons[num_15].y);

    // For loop for virus names 
    for (int i{}; i < num_5; ++i) {
        if (!buttons[i].mesh) { // Prevent recreating existing buttons
            buttons[i].x = start_X + (i * spacing);
            buttons[i].y = start_Y;
            buttons[i].width = btn_Width;
            buttons[i].height = btn_Height;
            buttons[i].name = HFMD;
            if (!buttons[i].mesh) Create_Rectangle(&buttons[i].mesh, btn_Width, btn_Height, buttons[i].color, &buttons[i].transform, scale_1, scale_1, buttons[i].x, buttons[i].y);
        }
    }


    // Upgrade 1 buttons
    for (int i = num_5; i < num_10; ++i) {
   
        if (!buttons[i].mesh) { // Prevent recreating existing buttons
            buttons[i].x = start_X + ((i - num_5) * spacing);
            buttons[i].y = start_Y2;
            buttons[i].width = btn_Width;
            buttons[i].height = btn_Height;
            buttons[i].name = upgrade_lvl1.c_str();

            buttons[i].is_Clicked;
            if (!buttons[i].mesh) Create_Rectangle(&buttons[i].mesh, btn_Width, btn_Height, buttons[i].color, &buttons[i].transform, scale_1, scale_1, buttons[i].x, buttons[i].y);
        }
    }



    // Upgrade 2 buttons
    for (int i = num_10; i < num_15; ++i) {
  

        if (!buttons[i].mesh) { // Prevent recreating existing buttons
            buttons[i].x = start_X + ((i - num_10) * spacing);
            buttons[i].y = start_Y3;
            buttons[i].width = btn_Width;
            buttons[i].height = btn_Height;
            buttons[i].name = upgrade_lvl2;

            buttons[i].is_Clicked;
            if (!buttons[i].mesh) Create_Rectangle(&buttons[i].mesh, btn_Width, btn_Height, buttons[i].color, &buttons[i].transform, scale_1, scale_1, buttons[i].x, buttons[i].y);
        }
    }

}


// Function to check if a button is clicked
bool is_Button_Clicked(UpgradeButton& btn, float mouseX, float mouseY) {
    return (mouseX >= btn.x - btn.width / 2 && mouseX <= btn.x + btn.width / 2 &&
        mouseY >= btn.y - btn.height / 2 && mouseY <= btn.y + btn.height / 2);
}

// Function to handle button clicks
void handle_Button_Click() {
    s32 tempX, tempY;  // AEInputGetCursorPosition requires s32 (int)
    AEVec2 mousePos;    // Store converted float position

    // Get integer mouse position
    AEInputGetCursorPosition(&tempX, &tempY);

    // Convert to float (world coordinates)
    mousePos.x = static_cast<float>(tempX) - (AEGfxGetWindowWidth()/ 2);  // Center X
    mousePos.y = (AEGfxGetWindowHeight()/ 2) - static_cast<float>(tempY);  // Flip Y


    // Looping to control clicking save and Upgrades via clicking
    for (int i = num_5; i < num_15; ++i) {
        if (is_Button_Clicked(buttons[i], mousePos.x, mousePos.y) &&
            AEInputCheckTriggered(AEVK_LBUTTON) &&
            !buttons[i].is_Clicked) {

            //Check if the player has enough upgrade points (10 points per upgrade)
            if (upgrade_points >= Upgrade_points_minus) {
                upgrade_points -= Upgrade_points_minus; // per upgrade is -10
                buttons[i].is_Clicked = true;
                if (buttons[i].mesh) AEGfxMeshFree(buttons[i].mesh);
                    Create_Rectangle(&buttons[i].mesh, buttons[i].width, buttons[i].height,
                    buttons[i].color, &buttons[i].transform,
                    scale_1, scale_1, buttons[i].x, buttons[i].y);


                switch (i) {
                case button_5:  // Upgrade HFMD
                    saved_Virus_Stats[hfmdvirus].ms += Add_MS;
                    saved_Virus_Stats[hfmdvirus].basehp += Add_HP;
                    saved_Virus_Stats[hfmdvirus].upgradeCompleted[button_0] = true;
                    break;
                case button_6:  // Upgrade Ebola
                    saved_Virus_Stats[ebola].ms += Add_MS;
                    saved_Virus_Stats[ebola].basehp += Add_HP;
                    saved_Virus_Stats[ebola].upgradeCompleted[button_0] = true;
                    break;
                case button_7:  // Upgrade Polio
                    saved_Virus_Stats[polio_virus].ms += Add_MS;
                    saved_Virus_Stats[polio_virus].basehp += Add_HP;
                    saved_Virus_Stats[polio_virus].upgradeCompleted[button_0] = true;
                    break;
                case button_8:  // Upgrade Shingles
                    saved_Virus_Stats[ebola].ms += Add_MS;
                    saved_Virus_Stats[ebola].basehp += Add_HP;
                    saved_Virus_Stats[ebola].upgradeCompleted[button_0] = true;
                    break;
                case button_9:  // Upgrade Chickenpox
                    saved_Virus_Stats[chickenpox].ms += Add_MS;
                    saved_Virus_Stats[chickenpox].basehp += Add_HP;
                    saved_Virus_Stats[chickenpox].upgradeCompleted[button_0] = true;
                    break;
                case button_10: // HFMD - Level 2 Upgrade
                    saved_Virus_Stats[hfmdvirus].ms += Add_MS;
                    saved_Virus_Stats[hfmdvirus].basehp += Add_HP;
                    saved_Virus_Stats[hfmdvirus].upgradeCompleted[button_1] = true;
                    break;
                case button_11: // Ebola - Level 2 Upgrade
                    saved_Virus_Stats[ebola].dmg += Add_DMG;
                    saved_Virus_Stats[ebola].basehp += Add_HP;
                    saved_Virus_Stats[ebola].upgradeCompleted[button_1] = true;
                    break;
                case button_12: // Polio - Level 2 Upgrade
                    saved_Virus_Stats[polio_virus].ms += Add_MS;
                    saved_Virus_Stats[polio_virus].basehp += Add_HP;
                    saved_Virus_Stats[polio_virus].upgradeCompleted[button_1] = true;
                    break;
                case button_13: // Shingles - Level 2 Upgrade
                    saved_Virus_Stats[ebola].dmg += Add_DMG;
                    saved_Virus_Stats[ebola].basehp += Add_HP;
                    saved_Virus_Stats[ebola].upgradeCompleted[button_1] = true;
                    break;
                case button_14: // Chickenpox - Level 2 Upgrade
                    saved_Virus_Stats[chickenpox].ms += Add_MS;
                    saved_Virus_Stats[chickenpox].basehp += Add_HP;
                    saved_Virus_Stats[chickenpox].upgradeCompleted[button_1] = true;
                    break;
                default:
                    break;
                }
                //save button clicks
                save_Upgrade_Data();


            }
           
        }

    }
    if (is_Button_Clicked(buttons[15], mousePos.x, mousePos.y) &&
        AEInputCheckTriggered(AEVK_LBUTTON)) {
        in_upgrade_tree = false; // Exit the upgrade tree
    }
}

// Function to render the upgrade tree page
void render_upgrade_tree() {

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(credits_bg, {}, {});
    AEGfxSetTransform(bg_Transform.m);
    AEGfxMeshDraw(bg_Mesh, AE_GFX_MDM_TRIANGLES);
    
    AEGfxTextureSet(button_clicked, {}, {});
    AEGfxSetTransform(buttons[button_15].transform.m);
    AEGfxMeshDraw(buttons[num_15].mesh, AE_GFX_MDM_TRIANGLES);
 

    for (int i{}; i < num_15; ++i) {

        
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        
        // Button not clicked texture set
        if (!buttons[i].is_Clicked) { 
            AEGfxTextureSet(button_def, {}, {});
        }


        // Button clicked texture set
        if(buttons[i].is_Clicked) {
            AEGfxTextureSet(button_clicked, {}, {});
        }
       

        AEGfxSetTransform(buttons[i].transform.m);
        AEGfxMeshDraw(buttons[i].mesh, AE_GFX_MDM_TRIANGLES);
     

    }

}

// Render Texts 
void render_text() {

    AEGfxPrint(font, Exit.c_str(), Exit_x, Exit_y / (AEGfxGetWindowHeight()/2),
        scale_1, scale_1, scale_1, scale_1, scale_1);  // White text

  
    f32 textWidth, textHeight;
    for (int i {}; i < button_3; ++i) {
       
        if (i == button_0) {
            AEGfxGetPrintSize(font, HFMD.c_str(), Text_width, &textWidth, &textHeight);
            float textX = (buttons[i].x - X_offset) / (AEGfxGetWindowWidth() / 2);  // Normalize X (-800 to 800  -1 to 1)
            float textY = (buttons[i].y - num_10) / (AEGfxGetWindowHeight() / 2);  // Normalize Y (-450 to 450  -1 to 1)

            // Print text above the button
            AEGfxPrint(font, HFMD.c_str(), textX, textY,
                scale_1, scale_1, scale_1, scale_1, scale_1);  // White text
        }
        if (i == button_1) {
            AEGfxGetPrintSize(font, Ebola.c_str(), Text_width, &textWidth, &textHeight);
            float textX = (buttons[i].x - X_offset) / (AEGfxGetWindowWidth() / 2);  // Normalize X (-800 to 800  -1 to 1)
            float textY = (buttons[i].y - num_10) / (AEGfxGetWindowHeight() / 2);  // Normalize Y (-450 to 450  -1 to 1)

            // Print text above the button
            AEGfxPrint(font, Ebola.c_str(), textX, textY,
                scale_1, scale_1, scale_1, scale_1, scale_1);  // White text
        }
        if (i == button_2) {
            AEGfxGetPrintSize(font, Polio.c_str(), Text_width, &textWidth, &textHeight);
            float textX = (buttons[i].x - X_offset) / (AEGfxGetWindowWidth() / 2);  // Normalize X (-800 to 800  -1 to 1)
            float textY = (buttons[i].y - num_10) / (AEGfxGetWindowHeight() / 2);  // Normalize Y (-450 to 450  -1 to 1)

            // Print text above the button
            AEGfxPrint(font, Polio.c_str(), textX, textY,
                scale_1, scale_1, scale_1, scale_1, scale_1);  // White text
        }
       
    }
    AEGfxGetPrintSize(font, Shingles.c_str(), Text_width, &textWidth, &textHeight);

    AEGfxGetPrintSize(font, ChickenPox.c_str(), Text_width, &textWidth, &textHeight);

    float textX = (buttons[button_3].x - button_5x_offset) / (AEGfxGetWindowWidth() / 2);  // Normalize X (-800 to 800  -1 to 1)l
    float textY = (buttons[button_3].y - num_10) / (AEGfxGetWindowHeight() / 2);  // Normalize Y (-450 to 450  -1 to 1)

    // Print text above the button
    AEGfxPrint(font, Shingles.c_str(), textX, textY,
        scale_1, scale_1, scale_1, scale_1, scale_1);  // White text

    textX = (buttons[button_4].x - button_4x_offset) / (AEGfxGetWindowWidth() / 2);  // Normalize X (-800 to 800  -1 to 1)
    textY = (buttons[button_4].y - num_10) / (AEGfxGetWindowHeight() / 2);  // Normalize Y (-450 to 450  -1 to 1)

    AEGfxGetPrintSize(font, ChickenPox.c_str(), Text_width, &textWidth, &textHeight);

    // Print text above the button
    AEGfxPrint(font, ChickenPox.c_str(), textX, textY,
        scale_1, scale_1, scale_1, scale_1, scale_1);  // White text


    // Using file to display what is the saved points total
    std::stringstream ss;

    ss << "Upgrade Points: " << upgrade_points;

    std::string pointsText = ss.str();

    AEGfxGetPrintSize(font, pointsText.c_str(), Text_width, &textWidth, &textHeight);

    float text1 = (-Text_x) / (AEGfxGetWindowWidth() / 2);
    float text2 = (Text_y) / (AEGfxGetWindowHeight() / 2);

    AEGfxPrint(font, pointsText.c_str(), text1, text2, scale_1, scale_1, scale_1, scale_1, scale_1);

    //------------------------------------------------------------------------------------

    for (int i = num_5; i < num_10; ++i) {
        //f32 textWidth, textHeight;
        AEGfxGetPrintSize(font, upgrade_lvl1.c_str(), Text_width, &textWidth, &textHeight);
        
        float textx = (buttons[i].x - Text_x_offset) / (AEGfxGetWindowWidth() / 2);  // Normalize X (-800 to 800  -1 to 1)
        float texty = (buttons[i].y - num_10) / (AEGfxGetWindowHeight() / 2);  // Normalize Y (-450 to 450  -1 to 1)

        // Print text above the button
        AEGfxPrint(font, upgrade_lvl1.c_str(), textx, texty,
            scale_1, scale_1, scale_1, scale_1, scale_1);  // White text

       
    } 

    AEGfxPrint(font, upgrade_1_Des.c_str(), Upgrade_1_text_x, Upgrade_1_text_y,
        scale_1, scale_1, scale_1, scale_1, scale_1);  // White text

    //------------------------------------------------------------------------------------

    for (int i = num_10; i < num_15; ++i) {
        //f32 textWidth, textHeight;
        AEGfxGetPrintSize(font, upgrade_lvl2.c_str(), Text_width, &textWidth, &textHeight);

        float textx = (buttons[i].x - Text_x_offset) / (AEGfxGetWindowWidth()/2);  // Normalize X (-800 to 800  -1 to 1)
        float texty = (buttons[i].y - num_10) / (AEGfxGetWindowHeight()/2);  // Normalize Y (-450 to 450  -1 to 1)

        // Print text above the button
        AEGfxPrint(font, upgrade_lvl2.c_str(), textx, texty,
            scale_1, scale_1, scale_1, scale_1, scale_1);  // White text
    }

    AEGfxPrint(font, upgrade_2_Des.c_str(), Upgrade_2_text_x, Upgrade_2_text_y,
        scale_1, scale_1, scale_1, scale_1, scale_1);  // White text

}

// Function to update the upgrade tree page
void update_upgrade_tree() {
    handle_Button_Click();
}

void save_Upgrade_Data() {
    std::ofstream file("upgrades.txt");  // Save in text form
    if (!file) { 
        return;
    }

    file << upgrade_points << "\n";  // Save upgrade points 

    // Save button clicked states (index 5 to 14)
    for (int i = num_5; i < num_15; ++i) {
        file << buttons[i].is_Clicked << " ";
    }
    file << "\n";  // New line after all buttons

    //  Save full virus stats
    for (int i{}; i < num_10; ++i) {
        file << saved_Virus_Stats[i].basehp << " "
            << saved_Virus_Stats[i].dmg << " "
            << saved_Virus_Stats[i].ms << " ";
            
        for (int j{}; j < button_2; ++j) {
            file << saved_Virus_Stats[i].upgradeCompleted[j] << " ";
        }

        file << "\n";  // New line for next virus
    }


    file.close();
}

void load_Upgrade_Data() {
    std::ifstream file("upgrades.txt");  // Read from text file
    if (!file) {
        setDefaultVirusStats();  //  Set base stats
        save_Upgrade_Data();       //  Save them so they persist
        return;
    }
    

    file >> upgrade_points;  //upgrade points 

    // Load button clicked states (index 5 to 14)
    for (int i = num_5; i < num_15; ++i) {
        file >> buttons[i].is_Clicked;
    }
    // load saved stats
    for (int i{}; i < num_10; ++i) {
        file >> saved_Virus_Stats[i].basehp
            >> saved_Virus_Stats[i].dmg
            >> saved_Virus_Stats[i].ms;
        // If anyone edits file or accidentally creates a 0 value save to stats this would prevail
        if (saved_Virus_Stats[i].basehp == 0 || saved_Virus_Stats[i].dmg == 0 || saved_Virus_Stats[i].ms ==0) {
            setDefaultVirusStats();
        }

        for (int j{}; j < button_2; ++j) {
            file >> saved_Virus_Stats[i].upgradeCompleted[j];
        }
    }

    file.close();
}



// Function to free allocated resources
void free_upgrade_tree() {
    if (bg_Mesh) {
        AEGfxMeshFree(bg_Mesh);
        bg_Mesh = nullptr; // Ensure it doesn't get freed again
    }
    if (font) {
        AEGfxDestroyFont(font);
        font = NULL;  // Reset to avoid reuse of invalid pointer
    }
    if (Exit_Mesh) {
        AEGfxMeshFree(Exit_Mesh);
        Exit_Mesh = nullptr;  // Reset to avoid reuse of invalid pointer
    }

    for (int i {}; i <= num_15; ++i) {
        if (buttons[i].mesh) {
            AEGfxMeshFree(buttons[i].mesh);
            buttons[i].mesh = nullptr;  // Prevent double free
        }
    }
}
