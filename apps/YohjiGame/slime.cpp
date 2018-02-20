//
//  slime.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#include "slime.h"
#include "agk.h"
#include <cmath>

// Slime Functions

void Slime::moveToPlayer(Player mainPlayer) {
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
    
    
        
    if (std::abs(agk::GetSpriteX(mainPlayer.getID()) - agk::GetSpriteX(getID_())) < 550 && (agk::GetSpritePhysicsVelocityY(getID_()) == 0)){
        agk::SetSpritePhysicsVelocity(getID_(), agk::GetSpritePhysicsVelocityX(getID_()), 0);
        agk::SetSpritePhysicsImpulse(getID_(), agk::GetSpriteXByOffset(getID_()), agk::GetSpriteYByOffset(getID_()), (getSpeed()*agk::Random2(2.5, 4))*getDirection(), agk::Random2(-1200, -4500));
    } else if (agk::GetSpritePhysicsVelocityY(getID_()) == 0){
        agk::SetSpritePhysicsVelocity(getID_(), 0, agk::GetSpritePhysicsVelocityY(getID_()));
        agk::SetSpritePhysicsImpulse(getID_(), agk::GetSpriteXByOffset(getID_()), agk::GetSpriteYByOffset(getID_()), (getSpeed()+agk::Random2(800, 2800))*getDirection(), agk::Random2(-1800, -6000));
    }
}

void Slime::enemySpecificHit() {
     agk::PlaySprite(getID_(),5,0,1,2);
}

void Slime::isDead(Player &mainPlayer,UI &userInterface) {
    // Drops, Experience etc.
    
    mainPlayer.setExperience(mainPlayer.getExperience()+(this->getExperience()));
    const std::string experienceNumber = "+" + intToStringo(getExperience()) + " exp";
    userInterface.createFadingText(experienceNumber, 60, agk::GetSpriteX(mainPlayer.getID()), agk::GetSpriteY(mainPlayer.getID()), 40,2);
    
    // Add death animation here
    
}

void Slime::initHealthBar() {
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
