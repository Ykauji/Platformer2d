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

void Fireball::specialEffect(Enemy & e) {
    // Knockback enemy!
    int enemyDirection = direction_;
        agk::SetSpritePhysicsImpulse(e.getID_(), agk::GetSpriteXByOffset(e.getID_()), agk::GetSpriteYByOffset(e.getID_()), 1000*enemyDirection, -500);
}
