//
//  frostbolt.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 7/14/18.
//
//

#include "frostbolt.h"
#include "agk.h"

void Frostbolt::updateBullet() {
    agk::SetSpritePosition(getID_(), agk::GetSpriteX(getID_())+(getDirection()*getSpeed()), agk::GetSpriteY(getID_()));
}

void Frostbolt::specialEffect(Enemy & e) {
    // Knockback enemy!
    e.setSpeed(e.getSpeed()-slow);
}
