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


Enemy::Enemy() {
    maxHealth_ = 100;
    health_ = 100;
    isEngage_ = false;
    recentlyDamaged_ = 0;
    damage_ = 10;
    speed_ = 10;
    experience_ = 10;
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
    if (health_ < maxHealth_ && isEngage_ == 0) {
        isEngage_ = 1;
        initHealthBar();
    }
    enemySpecificHit();
}

void Enemy::enemySpecificHit() {
    
}

void Enemy::deleteEnemy() {
    agk::DeleteSprite(getID_());
    agk::DeleteSprite(healthBar_.healthBarBox_);
    agk::DeleteSprite(healthBar_.greenHealthBar_);
}

void Enemy::initHealthBar() {
    healthBar_.healthBarBox_ = agk::CreateSprite(38);
    healthBar_.greenHealthBar_ = agk::CreateSprite(37);
    
    agk::SetSpriteSize(healthBar_.healthBarBox_,163,23);
    agk::SetSpriteSize(healthBar_.greenHealthBar_,152,12.5);
    
    healthBar_.x1_ = -35;
    healthBar_.y1_ = -50;
    healthBar_.x2_ = -30.5;
    healthBar_.y2_ = -44;
}

void Enemy::initHealthBar(int x1, int y1,int x2,int y2) {
    healthBar_.healthBarBox_ = agk::CreateSprite(38);
    healthBar_.greenHealthBar_ = agk::CreateSprite(37);
    healthBar_.x1_ = x1;
    healthBar_.y1_ = y1;
    healthBar_.x2_ = x2;
    healthBar_.y2_ = y2;
    
    agk::SetSpriteSize(healthBar_.healthBarBox_,163,23);
    agk::SetSpriteSize(healthBar_.greenHealthBar_,152,12.5);

    
    agk::SetSpritePosition(healthBar_.healthBarBox_, agk::GetSpriteX(SpriteID_)+healthBar_.x1_, agk::GetSpriteY(SpriteID_)+healthBar_.y1_);
    agk::SetSpritePosition(healthBar_.greenHealthBar_, agk::GetSpriteX(SpriteID_)+healthBar_.x2_, agk::GetSpriteY(SpriteID_)+healthBar_.y2_);
}
// -18,-50   -10,-46
void Enemy::updateEnemyHealthBar() {
    
    int test = (agk::GetSpriteWidth(SpriteID_)/2) - agk::GetSpriteWidth(healthBar_.healthBarBox_)/2;
    agk::SetSpritePosition(healthBar_.healthBarBox_, agk::GetSpriteX(SpriteID_)+test-4.5, agk::GetSpriteY(SpriteID_)-50);
    agk::SetSpritePosition(healthBar_.greenHealthBar_, agk::GetSpriteX(SpriteID_)+test, agk::GetSpriteY(SpriteID_)-44);
    
    double healthPercent = 152*health_/(double)maxHealth_;
    agk::SetSpriteSize(healthBar_.greenHealthBar_,healthPercent,12.6);
    

}

void Enemy::testUpdateHealth() {
    
    int test = (agk::GetSpriteWidth(SpriteID_)/2) - agk::GetSpriteWidth(healthBar_.healthBarBox_)/2;
    agk::SetSpritePosition(healthBar_.healthBarBox_, agk::GetSpriteX(SpriteID_)+test-4.5, agk::GetSpriteY(SpriteID_)-50);
    agk::SetSpritePosition(healthBar_.greenHealthBar_, agk::GetSpriteX(SpriteID_)+test, agk::GetSpriteY(SpriteID_)-44);
}

void Enemy::deleteSprites() {
    agk::DeleteSprite(SpriteID_);
    agk::DeleteSprite(healthBar_.healthBarBox_);
    agk::DeleteSprite(healthBar_.greenHealthBar_);
}





