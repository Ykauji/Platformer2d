//
//  enemy.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#include "enemy.h"
#include "agk.h"
#include <cmath>
#include <string>
#include <sstream>

std::string Enemy::intToStringo(int value) {
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();
    return str;
}


// Enemy Functions
void Enemy::moveToPlayer(Player mainPlayer) {
    if (speed_ == 0) {
        return;
    }
    if (agk::GetSpriteX(mainPlayer.getID()) < agk::GetSpriteX(SpriteID_)) {
        agk::SetSpriteFlip(SpriteID_, 0, 0);
        direction_ = -1;
        agk::SetSpritePhysicsVelocity(SpriteID_, 0, agk::GetSpritePhysicsVelocityY(SpriteID_));
        agk::SetSpritePhysicsImpulse(SpriteID_, agk::GetSpriteXByOffset(SpriteID_), agk::GetSpriteYByOffset(SpriteID_), speed_*direction_, 0);
    } else if (agk::GetSpriteX(mainPlayer.getID()) > agk::GetSpriteX(SpriteID_)){
        direction_ = 1;
        agk::SetSpriteFlip(SpriteID_, 1, 0);
        agk::SetSpritePhysicsVelocity(SpriteID_, 0, agk::GetSpritePhysicsVelocityY(SpriteID_));
        agk::SetSpritePhysicsImpulse(SpriteID_, agk::GetSpriteXByOffset(SpriteID_), agk::GetSpriteYByOffset(SpriteID_), speed_*direction_, 0);
    }
    if (std::abs(agk::GetSpriteX(mainPlayer.getID()) - agk::GetSpriteX(SpriteID_)) < 100 && (agk::GetSpritePhysicsVelocityY(SpriteID_) == 0)){
        agk::SetSpritePhysicsVelocity(SpriteID_, agk::GetSpritePhysicsVelocityX(SpriteID_), 0);
        agk::SetSpritePhysicsImpulse(SpriteID_, agk::GetSpriteXByOffset(SpriteID_), agk::GetSpriteYByOffset(SpriteID_), 400*direction_,-1500);
    }
}
int Enemy::checkCollision(Player mainPlayer) {
    if ((agk::GetSpriteX(mainPlayer.getID()) < agk::GetSpriteX(SpriteID_)) && mainPlayer.getDirection() == direction_) {
        return -1;
    } else if ((agk::GetSpriteX(mainPlayer.getID()) < agk::GetSpriteX(SpriteID_)) && mainPlayer.getDirection() != direction_) {
        return direction_;
    } else if ((agk::GetSpriteX(mainPlayer.getID()) > agk::GetSpriteX(SpriteID_)) && mainPlayer.getDirection() == direction_) {
        return 1;
    } else if ((agk::GetSpriteX(mainPlayer.getID()) < agk::GetSpriteX(SpriteID_)) && mainPlayer.getDirection() != direction_) {
        return direction_;
    } else {
        return direction_;
    }
    return 0;
}

void Enemy::isHit(Projectile bullet,UI &userInterface) {
    setHealth(getHealth()-bullet.getDamage());
    const std::string bulletNumbero = intToStringo(bullet.getDamage());
    userInterface.createFadingText(bulletNumbero, 60, agk::GetSpriteX(this->getID_())+ agk::Random2(-10,30), agk::GetSpriteY(this->getID_()), 40);
}

void Enemy::deleteEnemy() {
    agk::DeleteSprite(getID_());
}
