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
    if (std::abs(agk::GetSpriteX(mainPlayer.getID()) - agk::GetSpriteX(getID_())) < 275 && (agk::GetSpritePhysicsVelocityY(getID_()) == 0)){
        agk::SetSpritePhysicsVelocity(getID_(), agk::GetSpritePhysicsVelocityX(getID_()), 0);
        agk::SetSpritePhysicsImpulse(getID_(), agk::GetSpriteXByOffset(getID_()), agk::GetSpriteYByOffset(getID_()), (getSpeed()*1.5)*getDirection(), agk::Random2(-1200, -4000));
    } else if (agk::GetSpritePhysicsVelocityY(getID_()) == 0){
        agk::SetSpritePhysicsVelocity(getID_(), 0, agk::GetSpritePhysicsVelocityY(getID_()));
        agk::SetSpritePhysicsImpulse(getID_(), agk::GetSpriteXByOffset(getID_()), agk::GetSpriteYByOffset(getID_()), (getSpeed()+agk::Random2(0, 500))*getDirection(), agk::Random2(-1500, -3000));
    }
}

void Slime::isHit(Projectile bullet, UI &userInterface) {
    setHealth(getHealth()-bullet.getDamage());
    const std::string bulletNumbero = intToStringo(bullet.getDamage());
    userInterface.createFadingText(bulletNumbero, 60, agk::GetSpriteX(this->getID_())+ agk::Random2(-20,20), agk::GetSpriteY(this->getID_()), 40);
    agk::PlaySprite(getID_(),5,0,1,2);
}

void Slime::isDead(Player &mainPlayer,UI &userInterface) {
    // Drops, Experience etc.
    
    mainPlayer.setExperience(mainPlayer.getExperience()+(this->getExperience()));
    const std::string experienceNumber = "+" + intToStringo(getExperience()) + " exp";
    userInterface.createFadingText(experienceNumber, 60, agk::GetSpriteX(mainPlayer.getID()), agk::GetSpriteY(mainPlayer.getID()), 40,2);
    
    // Add death animation here
    
}
