//
//  level.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#include "level.h"
#include "agk.h"
#include "enemy.h"
#include "slime.h"
#include "bat.h"

// Taken care of in deleteLevel.
Level::~Level() {
    for (int i = 0; i < enemies.size(); i++) {
        delete enemies[i];
    }
}

void Level::loadLevel(int levelNumber) {
    switch (levelNumber) {
        case 1:
            loadLevelOne();
            break;
        case 2:
            loadLevelTwo();
            break;
        case 3:
        default:
            break;
    }
}

void Level::loadStartScreen() {
    deleteLevel();
    
    // Load Background
    background_ = agk::CreateSprite(17);
    agk::SetSpriteDepth(background_, 10000);
    agk::FixSpriteToScreen(background_, 1);
    agk::SetSpriteSize(background_,xRes,yRes+100);
}

void Level::deleteLevel() {
    for (int i = 0; i < castleBlock.size(); i++) {
        agk::DeleteSprite(castleBlock[i]);
    }
    for (int i = 0; i < ladders.size(); i++) {
        agk::DeleteSprite(ladders[i]);
    }
    for (int i = 0; i < enemies.size(); i++) {
        delete enemies[i];
        agk::DeleteSprite(enemies[i]->getID_());
    }
    for (int i = 0; i < door_.size(); i++) {
        agk::DeleteSprite(door_[i].getID());
    }
    agk::DeleteSprite(background_);
    castleBlock.clear();
    ladders.clear();
    enemies.clear();
}

// Level generation functions / spawn enemy

void Level::loadTile(int x, int y) {
    castleBlock.push_back(agk::CreateSprite(2));
    agk::SetSpritePosition(castleBlock.back(), x, y);
    agk::SetSpritePhysicsOn(castleBlock.back(),1);
    agk::SetSpriteShape(castleBlock.back(),3);
    agk::AddSpriteShapeBox(castleBlock.back(), -35, -30, 35, -38,0);
    agk::SetSpriteGroup(castleBlock.back(), 2);
}
void Level::loadTile(int x, int y, int image) {
    castleBlock.push_back(agk::CreateSprite(image));
    agk::SetSpritePosition(castleBlock.back(), x, y);
    agk::SetSpritePhysicsOn(castleBlock.back(),1);
    agk::SetSpriteShape(castleBlock.back(),3);
    agk::AddSpriteShapeBox(castleBlock.back(), -35, -30, 35, -38,0);
    agk::SetSpriteGroup(castleBlock.back(), 2);
}
void Level::loadTile(int x, int y, int image,int physicsOn,int depth) {
    castleBlock.push_back(agk::CreateSprite(image));
    agk::SetSpritePosition(castleBlock.back(), x, y);
    agk::SetSpriteDepth(castleBlock.back(), depth);
    if (physicsOn == 1) {
        agk::SetSpritePhysicsOn(castleBlock.back(),1);
        agk::SetSpriteShape(castleBlock.back(),3);
        agk::AddSpriteShapeBox(castleBlock.back(), -35, -30, 35, -38,0);
        agk::SetSpriteGroup(castleBlock.back(), 2);
    }
}
void Level::loadLadderTile(int x, int y, int image) {
    ladders.push_back(agk::CreateSprite(image));
    agk::SetSpritePosition(ladders.back(), x, y);
    agk::SetSpriteShape(ladders.back(),3);
}


void Level::loadLadder(int x, int y, int yLength) {
        loadLadderTile(x, y, 25);
    for (int i = 0; i < yLength; i++) {
        loadLadderTile(x, y+60+(i*60), 26);
    }
    
}


void Level::loadLevelOne() {
    deleteLevel();
    level_ = 1;
    
    loadPlatform(100, -500, 500);
    loadPlatform(95,20,-325,500);
    // Castle
    loadPlatform(20,20,2500,-200,0,100);
    loadPlatform(18, 20, 2575, -900, 0, 101);
    loadPlatform(16, 20, 2650, -1600, 0, 102);
    loadPlatform(14, 20, 2725, -2300, 0, 103);
    
    // Castle Door
    loadDoor(3155, 330,2);
    
    // Small Platforms
    loadPlatform(10,0,150);
    loadPlatform(10,1000,150);
    loadPlatform(10,4500,150);
    loadPlatform(10,5500,150);
    
    // Load TrainingDummy
    spawnTrainingDummy(6000, 15);
    
    // Load Background
    
    background_ = agk::CreateSprite(17);
    agk::SetSpriteDepth(background_, 10000);
    agk::FixSpriteToScreen(background_, 1);
    agk::SetSpriteSize(background_,xRes,yRes+100);
}

void Level::loadLevelTwo() {
    deleteLevel();
    level_ = 2;
    
    loadPlatform(20, 20, 0, 500, 1, 100);
    loadPlatform(20, 20, -2000, 500, 1, 100);
    loadPlatform(20, 20, 2000, 500, 1, 100);
    loadPlatform(20, 20, -2500, 200, 1, 100);
    loadPlatform(20, 20, 2500, 200, 1, 100);
    loadPlatform(20, 20, -3000, -100, 1, 100);
    loadPlatform(20, 20, 3000, -100, 1, 100);

    
    
    loadPlatform(10, 350, 100);
    loadPlatform(10, 3250, -500);
    loadPlatform(10, -2750, -500);
    
    //Load Background
    background_ = agk::CreateSprite(36);
    agk::SetSpriteDepth(background_, 10000);
    agk::FixSpriteToScreen(background_, 1);
    agk::SetSpriteSize(background_,xRes,yRes+100);
}

void Level::loadPlatform(int xTiles, int xPos, int yPos) {
    for (int x = 0; x < xTiles; x++) {
        if (x == 0) {
            loadTile(xPos+(x*70), yPos, 21);
        } else if (x == xTiles-1) {
            loadTile(xPos+(x*70), yPos, 22);
        } else {
            loadTile(xPos+(x*70), yPos, 19);
        }
    }
}

void Level::loadPlatform(int xTiles, int yTiles, int xPos, int yPos) {
    for (int y = 0; y < yTiles; y++) {
        for (int x = 0; x < xTiles; x++) {
            if (y == 0 && x == 0) {
                loadTile(xPos+(x*70), yPos+(y*50), 19);
            } else if (y == 0 && x == xTiles-1) {
                loadTile(xPos+(x*70), yPos+(y*50), 19);
            } else if (y == 0) {
                loadTile(xPos+(x*70), yPos+(y*50), 19);
            } else {
                loadTile(xPos+(x*70), yPos+(y*50), 20);
            }
        }
    }
}
void Level::loadPlatform(int xTiles, int yTiles, int xPos, int yPos,int PhysicsOn,int depth) {
    for (int y = 0; y < yTiles; y++) {
        for (int x = 0; x < xTiles; x++) {
            if (y == 0 && x == 0) {
                loadTile(xPos+(x*70), yPos+(y*50), 19,1,depth);
            } else if (y == 0 && x == xTiles) {
                loadTile(xPos+(x*70), yPos+(y*50), 19,PhysicsOn,depth);
            } else if (y == 0) {
                loadTile(xPos+(x*70), yPos+(y*50), 19,1,depth);
            } else {
                loadTile(xPos+(x*70), yPos+(y*50), 20,PhysicsOn,depth);
            }
        }
    }
}

void Level::loadDoor(int x, int y,int nextLevel) {
    Door tempDoor;
    tempDoor.setiD(agk::CreateSprite(28));
    tempDoor.setNext(nextLevel);
    door_.push_back(tempDoor);
    agk::SetSpriteSize(door_.back().getID(),100);
    agk::SetSpritePosition(door_.back().getID(), x, y);
    agk::SetSpriteGroup(door_.back().getID(), 5);
    tempDoor.setiD(agk::CreateSprite(29));
    door_.push_back(tempDoor);
    agk::SetSpriteSize(door_.back().getID(),100);
    agk::SetSpritePosition(door_.back().getID(), x, y+70);
    agk::SetSpriteGroup(door_.back().getID(), 5);
}


void Level::generateRandomLevel() {
    for (int i = 0; i < agk::Random(30,80); i++) {
        loadTile(i*70, 1000);
    }
    for (int j = 0; j < agk::Random(1,5); j++) {
        for (int i = 0; i < agk::Random(5,30) ; i++) {
            loadTile((i*70)+agk::Random(0,2000), agk::Random(600,900));
        }
    }
}


void Level::spawnSlime(int x, int y) {
    Enemy* slime = new Slime();
    slime->setID_(agk::CreateSprite(4));
    agk::AddSpriteAnimationFrame(slime->getID_(), 5);
    agk::AddSpriteAnimationFrame(slime->getID_(), 4);
    agk::AddSpriteAnimationFrame(slime->getID_(), 23);
    agk::SetSpritePosition(slime->getID_(), x, y);
    agk::SetSpritePhysicsOn(slime->getID_());
    agk::SetSpriteShape(slime->getID_(), 3);
    agk::SetSpriteSize(slime->getID_(),agk::Random(65,75));
    agk::SetSpritePhysicsCanRotate(slime->getID_(), 0);
    slime->setSpeed(700);
    slime->setHealth(100);
    slime->setDamage(10);
    slime->setExperience(50);
    enemies.push_back(slime);
}

void Level::spawnBat(int x, int y) {
    Enemy* bat = new Bat();
    bat->setID_(agk::CreateSprite(24));
    agk::SetSpriteAnimation(bat->getID_(), 121, 89, 19);
    agk::SetSpriteShape(bat->getID_(),3);
    agk::SetSpriteSize(bat->getID_(),200);
    agk::SetSpritePosition(bat->getID_(), x, y);
    bat->setSpeed(5);
    bat->setHealth(150);
    bat->setDamage(5);
    bat->setExperience(1000);
    enemies.push_back(bat);
}

void Level::spawnTrainingDummy(int x,int y) {
    Enemy* dummy = new Enemy();
    dummy->setID_(agk::CreateSprite(35));
    agk::SetSpriteShape(dummy->getID_(),3);
    agk::SetSpriteSize(dummy->getID_(),200);
    agk::SetSpritePosition(dummy->getID_(), x, y);
    agk::SetSpriteFlip(dummy->getID_(), 1, 0);
    dummy->setSpeed(0);
    dummy->setHealth(9999);
    dummy->setDamage(5);
    dummy->setExperience(5000);
    enemies.push_back(dummy);
}

void Level::deleteEnemy(int iD) {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i]->getID_() == iD) {
            agk::DeleteSprite(enemies[i]->getID_());
            enemies.erase(enemies.begin()+i);
        }
    }
}
