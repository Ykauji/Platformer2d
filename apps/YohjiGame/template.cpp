// Includes
#include "template.h"
#include "level.h"
#include <cmath>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
// Namespace
using namespace AGK;

app App;


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
    agk::SetPhysicsGravity(0, 1000);
    agk::SetPhysicsWallBottom(0);
    agk::SetViewZoom(.60);
    
//    agk::SetPhysicsDebugOn();
    
    // Load Player
    mainPlayer.loadPlayerTest();
    
    // Player Physics
    mainPlayer.loadPlayerPhysics();
    agk::SetSpriteActive(mainPlayer.getID(), 1);
    
    // Ground Sensor
    
    // When loading images, if performance issues delete after usage?
    
    // Load Music/Sound
    agk::LoadMusicOGG(1,"media/song.ogg"); // Mooncatcher temporary music.
//    agk::PlayMusicOGG(1,1);
    
    // Load Tile
     agk::LoadImage(2, "Tiles/castleHalf.png");
    
    // Load Projectiles
    agk::LoadImage(3, "Player/laserPurpleDot.png");
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
    
    // Load Dragon
    
    
    // Load UI
    userInterface.initUI();
    
    // Load Health Numbers
    std::string currentHealth = intToString(mainPlayer.getHealth()) + "/";
    agk::CreateText(1,  currentHealth.c_str());
    agk::SetTextPosition(1, 945, 966);
    agk::SetTextSize(1, 20);
    agk::SetTextDepth(1, 1);
    agk::FixTextToScreen(1, 1);
    
    // Load Tiles for Test Level
    levelOne.loadLevelOne();
    
    // Cursor invisible
    agk::SetRawMouseVisible(0);
    
    // Highest Image = 27.
}


// Game Logic
int app::Loop (void)
{
    
    // World Scrolling
    agk::SetViewOffset(agk::GetSpriteX(mainPlayer.getID())-(agk::GetDeviceWidth()+160), agk::GetSpriteY(mainPlayer.getID())-(agk::GetDeviceHeight()+500));
    
    agk::Print(agk::GetSpriteCurrentFrame(mainPlayer.getID()));
    
    //Player Movement
    if (mainPlayer.getRecentlyDamaged() < 0) {
        if ((agk::GetRawKeyState(37) || agk::GetRawKeyState(65)) && agk::GetSpriteFirstContact(mainPlayer.getLeftSensor()) != 1) {
            mainPlayer.movementLeft();
        } else if((agk::GetRawKeyState(39) || agk::GetRawKeyState(68)) && agk::GetSpriteFirstContact(mainPlayer.getRightSensor()) != 1) {
            mainPlayer.movementRight();
        }
    }
    if (agk::GetSpritePhysicsVelocityX(mainPlayer.getID()) == 0){
    mainPlayer.stopMovement();
    }

    // Restrict Jumps to maxJumps
    if (mainPlayer.getRecentlyJumped() < mainPlayer.getMaxJumps()) {
        if (agk::GetRawKeyPressed(38) || agk::GetRawKeyPressed(87)) {
            mainPlayer.movementJump();
        }
    }    
    // When landing, reset RecentlyJumped to 0
    if (agk::GetSpriteFirstContact(mainPlayer.getGroundSensor()) == 1 && agk::GetSpritePhysicsVelocityY(mainPlayer.getID()) <= 0){
        mainPlayer.resetJump();
    }
    
    // Move Character Sensors (For some reason if moved before, GetSpriteFirstContact is changed to 0)
    agk::SetSpritePosition(mainPlayer.getGroundSensor(), agk::GetSpriteX(mainPlayer.getID())+66,agk::GetSpriteY(mainPlayer.getID())+130);
    agk::SetSpritePosition(mainPlayer.getLeftSensor(), agk::GetSpriteX(mainPlayer.getID())+30,agk::GetSpriteY(mainPlayer.getID())+50);
    agk::SetSpritePosition(mainPlayer.getRightSensor(), agk::GetSpriteX(mainPlayer.getID())+130,agk::GetSpriteY(mainPlayer.getID())+50);

    
    // Holding down drops faster
    
    if (agk::GetRawKeyState(83)) {
    mainPlayer.movementDashDown();
    }
    
    
    
    // If moving then play animation
    if (agk::GetSpritePhysicsVelocityY(mainPlayer.getID()) < 0) {
        // When moving play running animations!
    } else {
        if (agk::GetSpritePhysicsVelocityX(mainPlayer.getID()) > 0) {
            if (!(agk::GetSpriteCurrentFrame(mainPlayer.getID()) < 6) && agk::GetSpriteCurrentFrame(mainPlayer.getID()) > 1) {
                agk::PlaySprite(mainPlayer.getID(), 8, 1, 1, 6);
            }
        } else if (agk::GetSpritePhysicsVelocityX(mainPlayer.getID()) < 0) {
            if (!(agk::GetSpriteCurrentFrame(mainPlayer.getID()) < 6) && agk::GetSpriteCurrentFrame(mainPlayer.getID()) > 1) {
                agk::PlaySprite(mainPlayer.getID(), 8, 1, 1, 6);
            }   
        }
    }
    // Update Direction
    mainPlayer.updateDirection();
    
    // Dash Left and Right
    if (agk::GetRawKeyPressed(81)) {
        mainPlayer.movementDashLeft();
    }
    if (agk::GetRawKeyPressed(69)) {
        mainPlayer.movementDashRight();
    }
    // Decrease DashTimer_
    mainPlayer.dashTimerDecrease();
    if (mainPlayer.getDashTimer() == 0) {
        agk::SetSpritePhysicsVelocity(mainPlayer.getID(), 0, agk::GetSpritePhysicsVelocityY(mainPlayer.getID()));
        agk::SetSpritePhysicsImpulse(mainPlayer.getID(), agk::GetSpriteXByOffset(mainPlayer.getID()), agk::GetSpriteYByOffset(mainPlayer.getID()), mainPlayer.getSpeed()*mainPlayer.getDirection(), 0);
    }
    // Changing Weps with 1,2,3,4 etc.
    if (agk::GetRawKeyPressed(49)) {
        mainPlayer.setWep(1);
    }
    if (agk::GetRawKeyPressed(50)) {
        mainPlayer.setWep(2);
    }
    if (agk::GetRawKeyPressed(51)) {
        mainPlayer.setWep(3);
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
    
    // Enemy AI
    for (int i = 0; i < levelOne.getEnemies().size(); i++) {
        levelOne.getEnemies()[i]->moveToPlayer(mainPlayer);
    }
    
    
    // Player and Enemy Collision turn into function
    if (mainPlayer.getRecentlyDamaged() < 0 ) {
        for (int i = 0; i < levelOne.getEnemies().size(); i++) {
            if (agk::GetSpriteCollision(mainPlayer.getID(), levelOne.getEnemies()[i]->getID_())) {
                agk::SetSpritePhysicsVelocity(mainPlayer.getID(), 0, 0);
                agk::SetSpritePhysicsImpulse(mainPlayer.getID(), agk::GetSpriteXByOffset(mainPlayer.getID()), agk::GetSpriteYByOffset(mainPlayer.getID()),4000*levelOne.getEnemies()[i]->checkCollision(mainPlayer) , 0);
                mainPlayer.setHealth(mainPlayer.getHealth()-levelOne.getEnemies()[i]->getDamage());
                mainPlayer.setRecentlyDamaged(20);
            }
        }
    }
    mainPlayer.setRecentlyDamaged(mainPlayer.getRecentlyDamaged()-1);
    
   
    
    // Bullet and Enemy Collision
    for (int i = 0; i < levelOne.getEnemies().size(); i++) {
        for (int j = 0; j < mainPlayer.getBullets().size(); j++) {
            // If Melee
            if (mainPlayer.getBullets()[j]->isMelee() && agk::GetSpriteDistance(levelOne.getEnemies()[i]->getID_(), mainPlayer.getBullets()[j]->getID_()) < 3 && levelOne.getEnemies()[i]->getRecentlyDamaged() < 0) {
                levelOne.getEnemies()[i]->isHit(*mainPlayer.getBullets()[j],userInterface);
                levelOne.getEnemies()[i]->setRecentlyDamaged(3);
            } else if(agk::GetSpriteDistance(levelOne.getEnemies()[i]->getID_(), mainPlayer.getBullets()[j]->getID_()) < 5 && levelOne.getEnemies()[i]->getRecentlyDamaged() < 0){
                // How close bullet is to recognize collision
                    levelOne.getEnemies()[i]->isHit(*mainPlayer.getBullets()[j],userInterface);
                    levelOne.getEnemies()[i]->setRecentlyDamaged(10);
                    agk::SetSpritePosition(mainPlayer.getBullets()[j]->getID_() , 10000, 10000);
                    mainPlayer.clearBullet(j);
            }
        }
        levelOne.getEnemies()[i]->setRecentlyDamaged(levelOne.getEnemies()[i]->getRecentlyDamaged()-1);
    }
    
    
    
    // If enemy is under 1 health delete.
    for (int i = 0; i < levelOne.getEnemies().size(); i++) {
        if (levelOne.getEnemies()[i]->getHealth() <= 0) {
            levelOne.getEnemies()[i]->isDead(mainPlayer,userInterface);
            levelOne.deleteEnemy(levelOne.getEnemies()[i]->getID_());
        }
    }
    
    // Update Bullet Position
    for (int i = 0; i < mainPlayer.getBullets().size(); i++) {
         mainPlayer.getBullets()[i]->updateBullet();
    }
    
    
    
    // Updates Health and Experience
    mainPlayer.updateHealth(userInterface);
    mainPlayer.levelUp(userInterface);
    mainPlayer.updateExperience(userInterface);
    
    // If OffMap Reset/Die
    if (agk::GetSpriteY(mainPlayer.getID()) > 3000) {
        agk::SetSpritePosition(mainPlayer.getID(), 0, 0);
        mainPlayer.setHealth(mainPlayer.getHealth()/2);
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
    
    // Cheat Codes!
    
    
    // Spawn Slime
    if (agk::GetRawKeyPressed(80)) {
        levelOne.spawnSlime(agk::Random(100,1900), 0);
    }
    // +500 exp
    if (agk::GetRawKeyPressed(79)) {
        mainPlayer.setExperience(500);
    }
    if (agk::GetRawKeyPressed(82)) {
        levelOne.spawnBat(agk::Random2(-2500,2500), 0);
    }
    if (agk::GetRawKeyPressed(84)) {
        levelOne.spawnTrainingDummy(6000, 15);
    }
    
//    
	agk::Print( agk::ScreenFPS() );
//    agk::Print( agk::GetSpriteX(mainPlayer.getID()));
//    agk::Print( agk::GetSpriteY(mainPlayer.getID()));
    
    
    
	agk::Sync();
    
	return 0; // return 1 to close app
}


void app::End (void)
{
    
}









// Notes:
// Change FloatingText to HashTable implementation for O(1) delete.
// Partition functions into groups
// 



// Art from opengameart


