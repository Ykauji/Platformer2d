//
//  ItemDrop.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 3/20/18.
//
//

#include "ItemDrop.h"
#include "agk.h"


ItemDrop::ItemDrop(int spriteID,int gold,int health) {
    spriteID_ = spriteID;
    gold_ = gold;
    health_ = health;
}


void ItemDrop::moveToPlayer(Player mainPlayer) {
    if (agk::GetSpriteX(spriteID_) < agk::GetSpriteX(mainPlayer.getID())+60) {
        agk::SetSpriteX(spriteID_, agk::GetSpriteX(spriteID_)+10);
    } else {
        agk::SetSpriteX(spriteID_, agk::GetSpriteX(spriteID_)-10);
    }
    if (agk::GetSpriteY(spriteID_) < agk::GetSpriteY(mainPlayer.getID())+28) {
        agk::SetSpriteY(spriteID_, agk::GetSpriteY(spriteID_)+2);
    } else {
        agk::SetSpriteY(spriteID_, agk::GetSpriteY(spriteID_)-2);
    }
}
