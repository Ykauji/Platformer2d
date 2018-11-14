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
#include "kingSlime.h"
#include "bat.h"
#include "TmxConverter.h"
#include <algorithm>

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
//            loadCustomLevel(3, "media/Levels/Boss_Level/Test.tmx", "media/Levels/TileSets/newTileSet.tsx");
            loadCustomLevel(3, "media/Levels/TileSets/test_map2.tmx", "media/Levels/TileSets/castleTileset70x70.tsx");
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
        enemies[i]->deleteSprites();
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
    agk::SetSpriteCategoryBits(castleBlock.back(), 0x2);
}
void Level::loadTile(int x, int y, int image) {
    castleBlock.push_back(agk::CreateSprite(image));
    agk::SetSpritePosition(castleBlock.back(), x, y);
    agk::SetSpritePhysicsOn(castleBlock.back(),1);
    agk::SetSpriteShape(castleBlock.back(),3);
    agk::AddSpriteShapeBox(castleBlock.back(), -35, -30, 35, -38,0);
    agk::SetSpriteGroup(castleBlock.back(), 2);
    agk::SetSpriteCategoryBits(castleBlock.back(), 0x2);
}
void Level::loadTile(int x, int y, std::string imageStr) {
    castleBlock.push_back(agk::CreateSprite(agk::LoadImage(imageStr.c_str())));
    agk::SetSpritePosition(castleBlock.back(), x, y);
    agk::SetSpritePhysicsOn(castleBlock.back(),1);
    agk::SetSpriteShape(castleBlock.back(),3);
    agk::AddSpriteShapeBox(castleBlock.back(), -35, -30, 35, -38,0);
    agk::SetSpriteGroup(castleBlock.back(), 2);
    agk::SetSpriteCategoryBits(castleBlock.back(), 0x2);
}

void Level::loadTile(int x, int y, std::string imageStr,int imageIndex) {
    castleBlock.push_back(agk::CreateSprite(agk::LoadImage(imageStr.c_str(),imageIndex)));
    agk::SetSpritePosition(castleBlock.back(), x, y);
    agk::SetSpritePhysicsOn(castleBlock.back(),1);
    agk::SetSpriteShape(castleBlock.back(),3);
    agk::AddSpriteShapeBox(castleBlock.back(), -35, -30, 35, -38,0);
    agk::SetSpriteGroup(castleBlock.back(), 2);
    agk::SetSpriteCategoryBits(castleBlock.back(), 0x2);
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
        agk::SetSpriteCategoryBits(castleBlock.back(), 0x2);
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
    bottomDepth_ = 3000;
    spawnLocation_.first = 0;
    spawnLocation.second = 0;
    loadPlatform(100, -500, 500);
    loadPlatform(95,20,-325,500);
    // Castle
//    loadPlatform(20,20,2500,-200,0,100);
//    loadPlatform(18, 20, 2575, -900, 0, 101);
//    loadPlatform(16, 20, 2650, -1600, 0, 102);
//    loadPlatform(14, 20, 2725, -2300, 0, 103);
    
    loadPlatform(20,20,4500,-200,0,100);
    loadPlatform(18, 20, 4575, -900, 0, 101);
    loadPlatform(16, 20, 4650, -1600, 0, 102);
    loadPlatform(14, 20, 4725, -2300, 0, 103);
    
    // Castle Door
    loadDoor(5155, 330,3);
    
    // Small Platforms
    loadPlatform(10,0,150);
    loadPlatform(10,1000,150);
//    loadPlatform(10,4500,150);
//    loadPlatform(10,5500,150);
    
    // Load TrainingDummy
    spawnTrainingDummy(6000, 350);
    
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

void Level::loadCustomLevel(int level,std::string tmxFile,std::string tileSet) {
    deleteLevel();
    level_ = level;
    
    background_ = agk::CreateSprite(17);
    agk::SetSpriteDepth(background_, 10000);
    agk::FixSpriteToScreen(background_, 1);
    agk::SetSpriteSize(background_,xRes,yRes+100);
    
    spawnKingSlime(6500, 3000);
    bottomDepth_ = 6000;
    spawnLocation_.first = 0;
    spawnLocation.second = 0;
    loadLevelTmx(tmxFile, tileSet);
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
    agk::SetSpriteSize(door_.back().getID(),200,200);
    agk::SetSpritePosition(door_.back().getID(), x-50, y-180);
    agk::SetSpriteGroup(door_.back().getID(), 5);
    tempDoor.setiD(agk::CreateSprite(29));
    door_.push_back(tempDoor);
    agk::SetSpriteSize(door_.back().getID(),200,200);
    agk::SetSpritePosition(door_.back().getID(), x-50, y-30);
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

void Level::spawnKingSlime(int x, int y) {
    Enemy * kingSlime = new KingSlime(this);
    kingSlime->setID_(agk::CreateSprite(39));
    agk::AddSpriteAnimationFrame(kingSlime->getID_(), 5);
    agk::AddSpriteAnimationFrame(kingSlime->getID_(), 4);
    agk::AddSpriteAnimationFrame(kingSlime->getID_(), 23);
    agk::SetSpritePosition(kingSlime->getID_(), 200, 200);
    agk::SetSpritePhysicsOn(kingSlime->getID_());
    float sizeTest = 500;
    agk::SetSpriteSize(kingSlime->getID_(),sizeTest);
    agk::SetSpriteShape(kingSlime->getID_(),3);
    agk::SetSpritePhysicsCanRotate(kingSlime->getID_(), 0);
    kingSlime->setSpeed(75000);
    kingSlime->setHealth(sizeTest);
    kingSlime->setMaxHealth(sizeTest);
    kingSlime->setEngaged(false);
    kingSlime->setDamage(20);
    kingSlime->setExperience(5000);
    kingSlime->setGold(1500);
    agk::SetSpritePosition(kingSlime->getID_(), x, y);
    
    enemies.push_back(kingSlime);

}

void Level::spawnKingSlime(int x, int y, int maxHealth) {
    Enemy * kingSlime = new KingSlime(this);
    kingSlime->setID_(agk::CreateSprite(39));
    agk::AddSpriteAnimationFrame(kingSlime->getID_(), 5);
    agk::AddSpriteAnimationFrame(kingSlime->getID_(), 4);
    agk::AddSpriteAnimationFrame(kingSlime->getID_(), 23);
    agk::SetSpritePosition(kingSlime->getID_(), x, y);
    agk::SetSpritePhysicsOn(kingSlime->getID_());
    float sizeTest = maxHealth;
    agk::SetSpriteSize(kingSlime->getID_(),sizeTest);
    agk::SetSpriteShape(kingSlime->getID_(),3);
    agk::SetSpritePhysicsCanRotate(kingSlime->getID_(), 0);
    kingSlime->setSpeed(75000);
    kingSlime->setHealth(sizeTest);
    kingSlime->setMaxHealth(sizeTest);
    kingSlime->setEngaged(false);
    kingSlime->setDamage(20);
    kingSlime->setExperience(5000);
    kingSlime->setGold(500);
    
    
    
    enemies.push_back(kingSlime);
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
    float sizeTest = agk::Random(60,85);
    agk::SetSpriteSize(slime->getID_(),sizeTest);
    agk::SetSpritePhysicsCanRotate(slime->getID_(), 0);
    slime->setSpeed(700);
    sizeTest = sizeTest * 1.5;
    slime->setHealth(sizeTest);
    slime->setMaxHealth(sizeTest);
    slime->setEngaged(false);
    slime->setDamage(10);
    slime->setExperience(50);
    slime->setGold(500);
    enemies.push_back(slime);
}

void Level::loadLevelTmx(std::string fileName,std::string tileSetName) {

    TmxConverter Test;
    for (int i = 1; i < 5; i++) {
        Test.parseTmx(fileName,i);
    }
    Test.findParametersFromArray();
    Test.parseTmx(fileName,6);
    Test.parseTileSetTmx(tileSetName);
    
    for (int y = 0; y < Test.getHeight(); y++) {
        for (int x = 0; x < Test.getWidth(); x++) {
            if (Test.getTilePlacements()[y][x] == 0) {
                // Empty Space
            } else {
                // Finds index of TileSet that holds the correct filename. For some reason i-1? MUST BE SORTED.
//                int index = Test.binarySearch(Test.getTileSet(), 1, (int)Test.getTileSet().size(),Test.getTilePlacements()[y][x]);
                int index = Test.getTilePlacements()[y][x];
                if (index == -1) {
                    std::cout << "error tile not found" << std::endl;
                } else {
                    loadTile(x*Test.getTileWidth(), y*Test.getTileHeight(), Test.getTileSet()[index-1].fileName_);
                }
                
            }
        }
    }
}

void Level::spawnItem(Enemy enemy) {
    // Spawn different number of Coins
    // 100 gold coins.
    for (int i = 0; i < enemy.getGold()/100; i++) {
        ItemDrop item(agk::CreateSprite(40),100,0);
        agk::SetSpriteAnimation(item.getID_(), 32, 32, 8);
        agk::SetSpritePosition(item.getID_(), agk::GetSpriteX(enemy.getID_()), agk::GetSpriteY(enemy.getID_()));
//        agk::SetSpriteSize(item.getID_(),40);
        agk::SetSpritePhysicsOn(item.getID_());
        agk::SetSpriteShape(item.getID_(),1);
        agk::SetSpritePhysicsCanRotate(item.getID_(), 0);
        agk::SetSpritePhysicsImpulse(item.getID_(), agk::GetSpriteXByOffset(enemy.getID_()), agk::GetSpriteYByOffset(enemy.getID_()),agk::Random2(-1300, 1300),-800);
        agk::PlaySprite(item.getID_());
        agk::SetSpriteCollideBits(item.getID_(),0x2);
        itemDrops_.push_front(item);
    }
}

void Level::updateItemDrop(Player &mainPlayer) {
    //      Update ItemDrop Position
    if (!itemDrops_.empty()) {
        // Check items collision, update and delete.
        for (auto it = itemDrops_.begin(); it != itemDrops_.end(); it++) {
            it->moveToPlayer(mainPlayer);
            if (agk::GetSpriteCollision(mainPlayer.getID(), it->getID_())) {
                // Handle sprite collision
                mainPlayer.setGold(mainPlayer.getGold() + it->getGold_());
                agk::DeleteSprite(it->getID_());
                agk::PlaySound(3);
                // delete from list here.
                it = deleteItemFromList(it->getID_());
            }
        }
    }
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
    bat->setMaxHealth(150);
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
    dummy->setMaxHealth(9999);
    dummy->setDamage(5);
    dummy->setExperience(5000);
    
    enemies.push_back(dummy);
}

std::list<ItemDrop>::iterator Level::deleteItemFromList(int iD) {
    auto headPointer = itemDrops_.begin();
    while (headPointer != itemDrops_.end()) {
        if (headPointer->getID_() == iD) {
            return itemDrops_.erase(headPointer);
        } else {
            headPointer++;
        }
    }
} 

void Level::deleteEnemy(int iD) {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i]->getID_() == iD) {
            enemies[i]->deleteSprites();
            enemies.erase(enemies.begin()+i);
        }
    }
}


