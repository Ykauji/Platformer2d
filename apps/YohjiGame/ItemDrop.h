//
//  ItemDrop.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 3/20/18.
//
//

#ifndef ItemDrop_h
#define ItemDrop_h

#include "player.h"

class ItemDrop {
    
  public:
    ItemDrop(int spriteID,int gold,int health);
    
    void moveToPlayer(Player &player);
    int getID_() {return spriteID_;}
    int getGold_() {return gold_;}
    void setIsMoving(bool value) {isMoving_ = value;}
    const int distanceFromPlayer(Player player);
    
  private:
    int spriteID_;
    int gold_;
    int upgradeID_;
    int health_;
    int justSpawned_;
    bool isMoving_;
    int test;
};

#endif /* ItemDrop_h */
