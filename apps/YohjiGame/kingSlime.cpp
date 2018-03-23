//
//  kingSlime.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 3/3/18.
//
//

#include "kingSlime.h"
#include "agk.h"
#include <cmath>




void KingSlime::moveToPlayer(Player mainPlayer) {
    // Change Direction when land
    if (agk::GetSpritePhysicsVelocityY(getID_()) == 0) {
        if (agk::GetSpriteX(mainPlayer.getID()) < agk::GetSpriteX(getID_())) {
            agk::SetSpriteFlip(getID_(), 0, 0);
            setDirection(-1);
        } else if (agk::GetSpriteX(mainPlayer.getID()) > agk::GetSpriteX(getID_())){
            setDirection(1);
            agk::SetSpriteFlip(getID_(), 1, 0);
        }
    }
    // Decides how slime jumps
    
    // When each kingslime lands, shoot projectiles
    
    if (std::abs(agk::GetSpriteX(mainPlayer.getID()) - agk::GetSpriteX(getID_())) < 550 && (agk::GetSpritePhysicsVelocityY(getID_()) == 0)){
        agk::SetSpritePhysicsVelocity(getID_(), agk::GetSpritePhysicsVelocityX(getID_()), 0);
        agk::SetSpritePhysicsImpulse(getID_(), agk::GetSpriteXByOffset(getID_()), agk::GetSpriteYByOffset(getID_()), (getSpeed()*agk::Random2(2.5, 4))*getDirection(), agk::Random2(-200200, -350500));
    } else if (agk::GetSpritePhysicsVelocityY(getID_()) == 0){
        agk::SetSpritePhysicsVelocity(getID_(), 0, agk::GetSpritePhysicsVelocityY(getID_()));
        agk::SetSpritePhysicsImpulse(getID_(), agk::GetSpriteXByOffset(getID_()), agk::GetSpriteYByOffset(getID_()), (getSpeed()+agk::Random2(800, 2800))*getDirection(), agk::Random2(-300000, -350000));
    }
}
// Recursion-like function?
void KingSlime::enemySpecificHit() {
    agk::PlaySprite(getID_(),5,0,1,2);
    
}

void KingSlime::kingSlimeHit() {
    if (health_ < 0 && maxHealth_ < 100) {
        // Divide into two regular slimes
        if (mainLevel_) {
            mainLevel_->spawnSlime(agk::GetSpriteX(getID_())+50, agk::GetSpriteY(getID_()));
            mainLevel_->spawnSlime(agk::GetSpriteX(getID_())-50, agk::GetSpriteY(getID_()));
        }
    } else if (health_ < 100) {
        mainLevel_->spawnKingSlime(agk::GetSpriteX(getID_())+500, agk::GetSpriteY(getID_()),maxHealth_/2);
        mainLevel_->spawnKingSlime(agk::GetSpriteX(getID_())-500, agk::GetSpriteY(getID_()),maxHealth_/2);
        // Divide into smaller KingSlime?
    }

}

void KingSlime::isDead(Player &mainPlayer,UI &userInterface) {
    // Drops, Experience etc.
    Enemy::isDead(mainPlayer, userInterface);
//    mainPlayer.setExperience(mainPlayer.getExperience()+(this->getExperience()));
//    const std::string experienceNumber = "+" + intToStringo(getExperience()) + " exp";
//    userInterface.createFadingText(experienceNumber, 60, agk::GetSpriteX(mainPlayer.getID()), agk::GetSpriteY(mainPlayer.getID()), 40,2);
    // Death Stuff
    if (maxHealth_ < 126) {
        // Divide into two regular slimes
        if (mainLevel_) {
            mainLevel_->spawnSlime(agk::GetSpriteX(getID_())+50, agk::GetSpriteY(getID_()));
            mainLevel_->spawnSlime(agk::GetSpriteX(getID_())-50, agk::GetSpriteY(getID_()));
        }
    } else {
        mainLevel_->spawnKingSlime(agk::GetSpriteX(getID_())+300, agk::GetSpriteY(getID_()),maxHealth_/2);
        mainLevel_->spawnKingSlime(agk::GetSpriteX(getID_())-300, agk::GetSpriteY(getID_()),maxHealth_/2);
        // Divide into smaller KingSlime?
    }
    
    // Add death animation here
    
}
// Boss Health Bar?
void KingSlime::initHealthBar() {
    healthBar_.healthBarBox_ = agk::CreateSprite(38);
    healthBar_.greenHealthBar_ = agk::CreateSprite(37);
    healthBar_.x1_ = -35;
    healthBar_.y1_ = -50;
    healthBar_.x2_ = -30.5;
    healthBar_.y2_ = -44;
    
    //    agk::SetSpriteVisible(healthBar_.greenHealthBar_, 0);
    
    agk::SetSpriteSize(healthBar_.healthBarBox_,163,23);
    agk::SetSpriteSize(healthBar_.greenHealthBar_,152,12.5);
    
    agk::SetSpritePosition(healthBar_.healthBarBox_, agk::GetSpriteX(SpriteID_)+healthBar_.x1_, agk::GetSpriteY(SpriteID_)+healthBar_.y1_);
    agk::SetSpritePosition(healthBar_.greenHealthBar_, agk::GetSpriteX(SpriteID_)+healthBar_.x2_, agk::GetSpriteY(SpriteID_)+healthBar_.y2_);
    
}
