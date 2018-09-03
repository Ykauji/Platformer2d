//
//  ItemDrop.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 3/20/18.
//
//

#include "ItemDrop.h"
#include "agk.h"
#include <cmath>


ItemDrop::ItemDrop(int spriteID,int gold,int health) {
    spriteID_ = spriteID;
    gold_ = gold;
    health_ = health;
    isMoving_ = false;
    test = 0;
}

void ItemDrop::moveToPlayer(Player &mainPlayer) {
    
    if (agk::GetSpritePhysicsVelocityY(getID_()) == 0) {
        agk::SetSpritePhysicsOff(getID_());
        // If close by, move to Player.
        if (distanceFromPlayer(mainPlayer) < 400) {
            isMoving_ = true;
        }
    }
    
    if (isMoving_ == true) {
        if (agk::GetSpriteX(spriteID_) < agk::GetSpriteX(mainPlayer.getID())+60) {
            agk::SetSpriteX(spriteID_, agk::GetSpriteX(spriteID_)+15);
        } else if (agk::GetSpriteX(spriteID_) > agk::GetSpriteX(mainPlayer.getID())+120) {
            agk::SetSpriteX(spriteID_, agk::GetSpriteX(spriteID_)-15);
        } else {
            
        }
        if (agk::GetSpriteY(spriteID_) < agk::GetSpriteY(mainPlayer.getID())+28) {
            agk::SetSpriteY(spriteID_, agk::GetSpriteY(spriteID_)+10);
        } else if (agk::GetSpriteY(spriteID_) > agk::GetSpriteY(mainPlayer.getID())+48) {
            agk::SetSpriteY(spriteID_, agk::GetSpriteY(spriteID_)-10);
        } else {
            
        }
    }
}

const int ItemDrop::distanceFromPlayer(Player mainPlayer) {
    return sqrt( ((agk::GetSpriteX(mainPlayer.getID())-agk::GetSpriteX(getID_())) * (agk::GetSpriteX(mainPlayer.getID())-agk::GetSpriteX(getID_()) ) ) + ((agk::GetSpriteY(mainPlayer.getID())-agk::GetSpriteY(getID_())) * ((agk::GetSpriteY(mainPlayer.getID())-agk::GetSpriteY(getID_())))) );
}
