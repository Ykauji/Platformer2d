//
//  level.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#ifndef level_h
#define level_h
#include <vector>
#include "enemy.h"
#include "door.h"
#include "ItemDrop.h"
#include <list>


class Level {
public:
    Level() {}
    ~Level();
    void loadLevel(int levelNumber);
    void loadLevelTmx(std::string fileName,std::string tileSetName);
    void loadStartScreen();
    void deleteLevel();
    void loadLevelOne();
    void loadLevelTwo();
    void loadCustomLevel(int level,std::string tmxFile,std::string tileSet);
    void spawnSlime(int x, int y);
    void spawnBat(int x, int y);
    void spawnKingSlime(int x, int y);
    void spawnKingSlime(int x, int y, int maxHealth);
    void spawnTrainingDummy(int x, int y);
    void loadTile(int x, int y);
    void loadTile(int x, int y, int image);
    void loadTile(int x, int y, std::string image,int imageIndex);
    void loadTile(int x, int y, std::string image);
    void loadTile(int x, int y, int image,int physicsOn,int depth);
    void loadLadderTile(int x, int y, int image);
    void loadLadder(int x, int y, int yLength);
    void loadDoor(int x, int y,int nextLevel);
    void deleteEnemy(int iD);
    void deleteItem(int iD);
    std::list<ItemDrop>::iterator deleteItemFromList(int iD);
    std::vector<Enemy*> getEnemies() {return enemies;}
    std::vector<int> getLadders() {return ladders;}
    std::vector<Door> getDoor() {return door_;}
    std::list<ItemDrop> getItems() {return itemDrops_;}
    
    int getLevel() {return level_;}
    
    void spawnItem(Enemy enemy);
    void updateItemDrop(Player &mainPlayer);
    
    
    void setSpawn(int x,int y) {spawnLocation.first = x;spawnLocation.second = y;}
    void generateRandomLevel();
    // Loads platform with 1y
    void loadPlatform(int xTiles, int xPos, int yPos);
    void loadPlatform(int xTiles, int yTiles, int xPos, int Ypos);
    void loadPlatform(int xTiles, int yTiles, int xPos, int yPos,int PhysicsOn,int depth);
    void updateDamageText(float xSin);

private:
    std::vector<int> ladders;
    std::vector<int> castleBlock;
    std::vector<Enemy*> enemies;
    std::vector<Door> door_;
    std::list<ItemDrop> itemDrops_;
    
    int backDrop;
    int nextLevelPortal;
    std::pair<int,int> spawnLocation;
    int background_;
    int level_;
    int bottomDepth_;
    int xRes = 1920;
    int yRes = 1080;
};






















#endif /* level_h */
