// Includes
#include "template.h"
#include "level.h"
#include "Mage.h"
#include <cmath>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Namespace
using namespace AGK;

app App;

// Lazy boi fix
#define mainPlayer (*mainPlayero)

// Change Vector to list vector for easier deletes?

// Main Menu -> Class Select -> Game.    Create mainCharacter at Class Select with Player * mainPlayero = new Class();
// Game States:
// 0 = Main Menu
// 1 = Game
// 2 = Pause
// 3 = Life Overview
// 4 = Class Select

// Sprite Groups: 1 Player, 2 Tiles, 3 Enemies, 4 Ladders, 5 Doors

// Had to use C style b/c StringStream performance issues and project not configured for C++ 11 so cannot to_string.
std::string app::intToString(int value) {
    char buffer [50];
    int n, a=value;
    n=sprintf (buffer, "%d", a);
    return buffer;
}

void app::Begin(void)
{
    int xRes = 1920;
    int yRes = 1080;
    
	agk::SetVirtualResolution (xRes, yRes);
    agk::SetClearColor(0,0,0); // Dark blue 0,0,30
    agk::SetSyncRate(0,120);
    agk::SetVSync(1);
    agk::SetScissor(0,0,0,0);
    // Set up Physics
    agk::SetPhysicsScale(.05);
    agk::SetPhysicsGravity(0, 1200);
    agk::SetPhysicsWallBottom(0);
    agk::SetViewZoom(.60);
    
//    agk::SetPhysicsDebugOn();
    
    
    // Ground Sensor
    
    // When loading images, if performance issues delete after usage? SPEND A DAY ORGANIZING?
    
    // Load Music/Sound
    agk::LoadMusicOGG(1,"media/ilikethis.ogg"); // Mooncatcher temporary music.
    agk::PlayMusicOGG(1,1);
    agk::LoadSound(2, "media/fireball.wav");
    agk::LoadSound(3, "media/coinsound.wav");
    agk::LoadSound(4, "media/damage.wav");
    
    // Load Tile
     agk::LoadImage(2, "Tiles/castleHalf.png");
    
    // Load Projectiles
    agk::LoadImage(3, "mainCharacter/laserPurpleDot.png");
    // Fireball
    agk::LoadImage(7, "fireball/1.png");
    agk::LoadImage(8, "fireball/2.png");
    agk::LoadImage(9, "fireball/3.png");
    agk::LoadImage(10, "fireball/4.png");
    agk::LoadImage(11, "fireball/5.png");
    agk::LoadImage(12, "fireball/6.png");
    
    // Load Enemy Images
    agk::LoadImage(4, "Enemy sprites/slime.png");
    agk::LoadImage(5, "Enemy sprites/slime_hit.png");
    agk::LoadImage(6, "Enemy sprites/slime_dead.png");
    
    // Load Torch
    agk::LoadImage(13, "Tiles/tochLit.png");
    agk::LoadImage(14, "Tiles/tochLit2.png");
    
    // Load HealthBars
    agk::LoadImage(15, "media/BarEmptyTest.png");
    agk::LoadImage(16, "media/HealthBarGreenSquare.png");
    
    // Load Background
    agk::LoadImage(17, "media/StarBackground.png");
    
    // Load ExperienceBar
    agk::LoadImage(18, "media/ExperienceBar.png");
    
    // Load CastleBlocks
    agk::LoadImage(19, "Tiles/castle.png");
    agk::LoadImage(20, "Tiles/castleCenter.png");
    agk::LoadImage(21, "Tiles/castleCliffLeft.png");
    agk::LoadImage(22, "Tiles/castleCliffRight.png");
    
    // Load SlimeDead
    agk::LoadImage(23, "Enemy sprites/slime_dead.png");
    
    // Load BlueBat
    agk::LoadImage(24, "bat/blueBat-121x89.png");
    
    // Load Ladder
    agk::LoadImage(25, "Tiles/ladder_top.png");
    agk::LoadImage(26, "Tiles/ladder_mid.png");
    
    // Load Cursor
    agk::LoadImage(27, "circle/circle-01.png");
    
    // Load Door
    agk::LoadImage(28, "Tiles/door_openTop.png");
    agk::LoadImage(29, "Tiles/door_openMid.png");
    
    // Load Slash Attack
    agk::LoadImage(30, "fx/5a.png");
    agk::LoadImage(31, "fx/5b.png");
    agk::LoadImage(32, "fx/5c.png");
    agk::LoadImage(33, "fx/5d.png");
    agk::LoadImage(34, "fx/5e.png");
    
    // Load Training Dummy
    agk::LoadImage(35, "Enemy sprites/yeti.png");
    
    // Load CastleBackground Level 2
    agk::LoadImage(36, "media/CastleBackgroundNight.png");
    
    // Load HealthBarRed
    agk::LoadImage(37, "media/red.png");
    agk::LoadImage(38, "media/HealthBarEmptyBlue.png");
    agk::LoadImage(39, "Enemy sprites/slime.png");
    agk::LoadImage(40, "media/coin_gold.png");
    
    
    // Load Attack Animations
    
    // Load Dragon
    
    
    // Load UI
    userInterface.initUI();
    
    // Load Health Numbers
    std::string currentHealth = intToString(mainPlayer.getHealth()) + "/";
    agk::CreateText(1,  currentHealth.c_str());
    agk::SetTextPosition(1, agk::GetSpriteX(userInterface.getHealthBarGreen())+agk::GetSpriteWidth(userInterface.getHealthBarGreen())/2-50 , agk::GetSpriteY(userInterface.getHealthBarGreen())-2.5);
    agk::SetTextSize(1, 20);
    agk::SetTextDepth(1, 1);
    agk::FixTextToScreen(1, 1);
    
    // Load Start Screen Stuff
    
    // Change GameState to 0 for mainScreen    Flow = 0 -> 4 -> 1
    gameState = 4;
    if (gameState == 0) {
        levelOne.loadStartScreen();
        userInterface.hideUI();
        agk::SetTextVisible(1, 0);
        // Title Text
        agk::CreateText(100, "OverThroned");
        agk::SetTextPosition(100, 1600, 400);
        agk::SetTextAlignment(100, 1);
        agk::SetTextSize(100, 90);
        
        // Start Screen Buttonos
        agk::AddVirtualButton(1, 960, 500, 100);
        agk::SetVirtualButtonText(1, "Start");
        agk::SetVirtualButtonAlpha(1, 0);
        
        agk::AddVirtualButton(2, 960, 600, 100);
        agk::SetVirtualButtonText(2, "How to Play");
        agk::SetVirtualButtonAlpha(2, 0);
        
        agk::AddVirtualButton(3, 960, 700, 100);
        agk::SetVirtualButtonText(3, "Achievements");
        agk::SetVirtualButtonAlpha(3, 0);
    } else {
        levelOne.loadLevel(1);
//        levelOne.loadLevelTmx("media/Test.tmx", "media/newTileSet.tsx");
    }
    
    
    
    // Cursor invisible
//    agk::SetRawMouseVisible(0);
    
    
    // Load Player Animations
    
    
    // Highest Image = 40.
}


// Game Logic
int app::Loop (void)
{
    float timeSinceLastFrame = agk::GetFrameTime();
    
    if (gameState == 1) {
        
    agk::Print(agk::GetSpriteCurrentFrame(mainPlayer.getID()));
    
    //Player Movement
        if ((agk::GetRawKeyState(37) || agk::GetRawKeyState(65)) && !mainPlayer.touchingLeftWall()) {
            mainPlayer.movementLeft();
        } else if ((agk::GetRawKeyState(37) || agk::GetRawKeyState(65)) && mainPlayer.touchingLeftWall()) {
            // If I want to slide down walls add here
            mainPlayer.setStateWallSlide();
            agk::SetSpritePhysicsVelocity(mainPlayer.getID(), 0, agk::GetSpritePhysicsVelocityY(mainPlayer.getID()));
        }
        
        if((agk::GetRawKeyState(39) || agk::GetRawKeyState(68)) && !mainPlayer.touchingRightWall()) {
            mainPlayer.movementRight();
        } else if ((agk::GetRawKeyState(39) || agk::GetRawKeyState(68)) && mainPlayer.touchingRightWall()) {
            // Same thing
            mainPlayer.setStateWallSlide();
            agk::SetSpritePhysicsVelocity(mainPlayer.getID(), 0, agk::GetSpritePhysicsVelocityY(mainPlayer.getID()));
        }
        
    if (agk::GetSpritePhysicsVelocityX(mainPlayer.getID()) == 0 && agk::GetSpritePhysicsVelocityY(mainPlayer.getID()) == 0){
    mainPlayer.stopMovement();
    }
//        agk::SetSpritePhysicsFriction(mainPlayer.getID(), 0);
//        if (agk::GetRawKeyState(37) == 0 && agk::GetRawKeyState(39) == 0) {
//            agk::SetSpritePhysicsFriction(mainPlayer.getID(), 2);
//        }
       

    // Restrict Jumps to maxJumps
        if (mainPlayer.touchingLeftWall() && (agk::GetRawKeyState(87) && agk::GetRawKeyState(68))) {
            mainPlayer.movementJump();
        } else if (mainPlayer.touchingRightWall() && (agk::GetRawKeyState(87) && agk::GetRawKeyState(65))){
            mainPlayer.movementJump();
        } else if (mainPlayer.getRecentlyJumped() < mainPlayer.getMaxJumps()) {
                if (agk::GetRawKeyPressed(38) || agk::GetRawKeyPressed(87)) {
                    mainPlayer.movementJump();
                }
        }
    
    // When landing, reset RecentlyJumped to 0
    if (agk::GetSpriteFirstContact(mainPlayer.getGroundSensor()) == 1 && agk::GetSpritePhysicsVelocityY(mainPlayer.getID()) <= 0){
        mainPlayer.resetJump();
    }
    
    // Move Character Sensors (For some reason if moved before, GetSpriteFirstContact is changed to 0)
    agk::SetSpritePosition(mainPlayer.getGroundSensor(), agk::GetSpriteX(mainPlayer.getID())+agk::GetSpriteWidth(mainPlayer.getID())/2,agk::GetSpriteY(mainPlayer.getID())+agk::GetSpriteHeight(mainPlayer.getID())-100);
        
        agk::SetSpritePosition(mainPlayer.getLeftSensor(), agk::GetSpriteX(mainPlayer.getID())+agk::GetSpriteWidth(mainPlayer.getID())/2-60,agk::GetSpriteY(mainPlayer.getID())+(agk::GetSpriteHeight(mainPlayer.getID())/2)-15);
        agk::SetSpritePosition(mainPlayer.getRightSensor(), agk::GetSpriteX(mainPlayer.getID())+agk::GetSpriteWidth(mainPlayer.getID())/2+60,agk::GetSpriteY(mainPlayer.getID())+(agk::GetSpriteHeight(mainPlayer.getID())/2)-15);

    
    // Holding down drops faster and Doors, S

    if (agk::GetRawKeyState(83)) {
        mainPlayer.movementDashDown();
    }
    
    
   
    // Update Direction
    //mainPlayer.updateDirection();
    
    // Dash Left and Right Q . E
    if (agk::GetRawKeyPressed(81)) {
        mainPlayer.movementDashLeft();
    }
    if (agk::GetRawKeyPressed(69)) {
        mainPlayer.movementDashRight();
    }
       
    if (agk::GetRawKeyPressed(67)) {
        mainPlayer.movementDash();
    }
        
    // Decrease DashTimer_
    mainPlayer.dashTimerDecrease();
    if (mainPlayer.getDashTimer() == 0) {
        mainPlayer.setStateIdle();
        agk::SetSpritePhysicsVelocity(mainPlayer.getID(), 0, agk::GetSpritePhysicsVelocityY(mainPlayer.getID()));
        agk::SetSpritePhysicsImpulse(mainPlayer.getID(), agk::GetSpriteXByOffset(mainPlayer.getID()), agk::GetSpriteYByOffset(mainPlayer.getID()), mainPlayer.getSpeed()*mainPlayer.getDirection(), 0);
    }
    // Changing Weps with 1,2,3,4 etc.
    if (agk::GetRawKeyState(49)) {
        mainPlayer.setWep(1); 
        userInterface.keyPressed(1);
    } else {
        userInterface.keyReleased(1);
    }
    
        // Swap weapon 2
    if (agk::GetRawKeyState(50)) {
        mainPlayer.setWep(2);
        userInterface.keyPressed(2);
    } else {
        userInterface.keyReleased(2);
    }
        // Swap weapon 3
    if (agk::GetRawKeyState(51)) {
        mainPlayer.setWep(3);
        userInterface.keyPressed(3);
    } else {
        userInterface.keyReleased(3);
    }
    if (agk::GetRawKeyState(52)) {
        mainPlayer.setWep(4);
        userInterface.keyPressed(4);
    } else {
        userInterface.keyReleased(4);
    }
    if (agk::GetRawKeyState(53)) {
        mainPlayer.setWep(5);
        userInterface.keyPressed(5);
        int xSize = 600;
        userInterface.displayMessage(1, agk::GetVirtualWidth()/2-xSize/2, 100, xSize, 200, "This is a test!!",40);
    } else {
        userInterface.keyReleased(5);
    }
    
    // Test Attack
    if (agk::GetRawKeyPressed(32)) {
        mainPlayer.attack();
    }
    
    // When time runs out, projectile dissapears
    for (int i = 0; i < mainPlayer.getBullets().size(); i++) {
        (*mainPlayer.getBullets()[i]).decreaseTime();
        (*mainPlayer.getBullets()[i]).updateBullet();
        if ((*mainPlayer.getBullets()[i]).getTime() < 0) {
            mainPlayer.clearBullet(i);
        }
    }
    if (mainPlayer.getBullets().size() > 0) {
//        agk::Print(mainPlayer.getNumBullets());
    }
    
    // Enemy Move
    for (int i = 0; i < levelOne.getEnemies().size(); i++) {
        levelOne.getEnemies()[i]->moveToPlayer(mainPlayer);
    }
    
    mainPlayer.checkEnemyCollisions(levelOne.getEnemies(),userInterface);
    mainPlayer.setRecentlyDamaged(mainPlayer.getRecentlyDamaged()-1);
    
    // Bullet and Enemy Collision
    for (int i = 0; i < levelOne.getEnemies().size(); i++) {
        for (int j = 0; j < mainPlayer.getBullets().size(); j++) {
            // If Melee
            if (mainPlayer.getBullets()[j]->isMelee() && agk::GetSpriteDistance(levelOne.getEnemies()[i]->getID_(), mainPlayer.getBullets()[j]->getID_()) < 3 && levelOne.getEnemies()[i]->getRecentlyDamaged() < 0) {
                levelOne.getEnemies()[i]->isHit(mainPlayer.getBullets()[j],userInterface);
                levelOne.getEnemies()[i]->setRecentlyDamaged(3);
                agk::SetSpritePhysicsImpulse(levelOne.getEnemies()[i]->getID_(), agk::GetSpriteXByOffset(levelOne.getEnemies()[i]->getID_()), agk::GetSpriteYByOffset(levelOne.getEnemies()[i]->getID_()), 1000*mainPlayero->getDirection() , -1000);
            } else if(agk::GetSpriteDistance(levelOne.getEnemies()[i]->getID_(), mainPlayer.getBullets()[j]->getID_()) < 5 && !mainPlayer.getBullets()[j]->isMelee()){
                // How close bullet is to recognize collision
                    levelOne.getEnemies()[i]->isHit(mainPlayer.getBullets()[j],userInterface);
                    agk::SetSpritePosition(mainPlayer.getBullets()[j]->getID_() , 10000, 10000);
                    mainPlayer.clearBullet(j);
            }
        }
        levelOne.getEnemies()[i]->setRecentlyDamaged(levelOne.getEnemies()[i]->getRecentlyDamaged()-1);
        if (levelOne.getEnemies()[i]->getEngaged() == true) {
            levelOne.getEnemies()[i]->updateEnemyHealthBar();
        }
    }
    
    // If enemy is under 1 health delete. Maybe move to when enemy takes dmg to save loopos.
    for (int i = 0; i < levelOne.getEnemies().size(); i++) {
        if (levelOne.getEnemies()[i]->getHealth() <= 0) {
            levelOne.getEnemies()[i]->isDead(mainPlayer,userInterface);
            levelOne.spawnItem(*levelOne.getEnemies()[i]);
            levelOne.deleteEnemy(levelOne.getEnemies()[i]->getID_());
        }
    }
    
    // Update Bullet Position
    for (int i = 0; i < mainPlayer.getBullets().size(); i++) {
         mainPlayer.getBullets()[i]->updateBullet();
    }
//      Update ItemDrop Position
        levelOne.updateItemDrop(mainPlayer);
    
    // Updates health/exp
    mainPlayer.updatePlayer(timeSinceLastFrame,userInterface);

    
    if (mainPlayer.getHealth() <= 0) {
        levelOne.deleteLevel();
        levelOne.loadLevelOne();
        agk::SetSpriteX(mainPlayer.getID(), 0);
        agk::SetSpriteY(mainPlayer.getID(), 0);
        mainPlayer.resetPlayer();
    }
    
    
    // Floating Text Update
    userInterface.fadeTimeUpdate(3);
    
    // Health Update
    mainPlayer.setCurrentHealthRatio(intToString(mainPlayer.getHealth()) + "/" + intToString(mainPlayer.getMaxHealth()));
    agk::SetTextString(1, mainPlayer.getCurrentHealthRatio().c_str());
    
    // Level Update
    mainPlayer.setCharLevelStr("Lv. " + intToString(mainPlayer.getCharLevel()));
    agk::SetTextString(112, mainPlayer.getCharLevelStr().c_str());
    
    
//    // Printing stuff
    if (!levelOne.getEnemies().empty()) {
        agk::Print(levelOne.getEnemies()[0]->getHealth());
    }
    
    agk::Print(mainPlayer.getGold());
    
    // Cheat Codes!
    
    
    // Spawn Slime
    if (agk::GetRawKeyPressed(80)) {
        levelOne.spawnSlime(agk::Random(100,1900), 0);
    }
    // +500 exp
    if (agk::GetRawKeyPressed(79)) {
        mainPlayer.setExperience(99);
//        std::cout << agk::GetSpriteX(mainPlayer.getID()) << " " << agk::GetSpriteY(mainPlayer.getID());
    }
    if (agk::GetRawKeyPressed(82)) {
        levelOne.spawnBat(agk::Random2(-2500,2500), 0);
    }
    if (agk::GetRawKeyPressed(84)) {
        int tempCheck = mainPlayer.checkDoor();
        if (tempCheck != 0) {
            for (int i = 0; i < levelOne.getDoor().size(); i++) {
                if (levelOne.getDoor()[i].getID() == tempCheck) {
                    levelOne.loadLevel(levelOne.getDoor()[i].getNext());
                    agk::SetSpritePosition(mainPlayer.getID(), 0, 0);
                    mainPlayer.movementRight();
                }
            }
        }
    }
    // Pause Game!
    if (agk::GetRawKeyPressed(27)) {
        gameState = 2;
    
    }
        // Camera Stuff
       
//        if (agk::GetSpritePhysicsVelocityX(mainPlayer.getID()) != 0) {
            // Camera Limits
            potato.Camera2DFollow(mainPlayer.getID());
        // If OffMap Reset/Die
        if (agk::GetSpriteY(mainPlayer.getID()) > 3000) {
            agk::SetSpritePosition(mainPlayer.getID(), 0, 0);
            mainPlayer.movementRight();
            mainPlayer.setHealth(mainPlayer.getHealth()/2);
            potato.Camera2DFollow(mainPlayer.getID());
        }
        
	agk::Print( agk::ScreenFPS() );
        
    // Main Menu
    } else if (gameState == 0) {
        if (agk::GetVirtualButtonPressed(1)) {
            gameState = 4;
            levelOne.loadLevel(1);
            agk::DeleteVirtualButton(1);
            agk::DeleteVirtualButton(2);
            agk::DeleteVirtualButton(3);
            userInterface.showUI();
            agk::SetTextVisible(1, 1);
            agk::SetTextVisible(100, 0);
            agk::SetRawMouseVisible(0);
            
        }
    // Pause Menu
    } else if (gameState == 2) {
        if (agk::GetRawKeyPressed(27)) {
            gameState = 1;
        }
        agk::StepPhysics(0);
        
    // Life Overview
    } else if (gameState == 3) {
        
        // Choose class
    } else if (gameState == 4) {
        // For now only one Class
        // 1 is regular , 2 is Mage
        int classChoice = 1;
        
        switch (classChoice) {
            case 1: {
                Player * playerPointer = new Player();
                mainPlayero = playerPointer;
                gameState = 1;
                break;
            }
                
            case 2: {
                Player * magePointer = new Mage();
                mainPlayero = magePointer;
                gameState = 1;
                break;

            }
        }
        // Load Player
        mainPlayer.loadPlayerTest();
        
        // Player Physics
        mainPlayer.loadPlayerPhysics();
        agk::SetSpriteActive(mainPlayer.getID(), 1);
        
        potato.Camera2DInit(1500, 1140, 5, 0);
        potato.Camera2DSet(mainPlayer.getID(), -40000, -40000, 40000, 40000);
        potato.Camera2DFollow(mainPlayer.getID());
        
    } else if (gameState == 5) {
        // Tower Floor Selection Screen
        
    
    }
    
	agk::Sync();
	return 0; // return 1 to close app
}


void app::End (void)
{
    
}









// Notes:
// Change FloatingTexts to HashTable implementation for O(1) delete. ?
// Partition functions into groups
// 



// Art from opengameart


