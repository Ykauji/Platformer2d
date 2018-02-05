//
//  fireball.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 1/7/18.
//
//

#include "fireball.h"
#include "agk.h"


void Fireball::updateBullet() {
    agk::SetSpritePosition(getID_(), agk::GetSpriteX(getID_())+(getDirection()*getSpeed()), agk::GetSpriteY(getID_()));
}
