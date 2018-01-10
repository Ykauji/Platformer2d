//
//  bat.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#include "bat.h"
#include "agk.h"

// Bat Functions

Bat::~Bat() {
    
}

void Bat::moveToPlayer(Player mainPlayer) {
    // Get Direction and Stuff
    if (agk::GetSpriteX(mainPlayer.getID()) - agk::GetSpriteX(getID_()) < 50) {
        setDirection(-1);
        agk::SetSpriteFlip(getID_(), 1, 0);
    } else if (agk::GetSpriteX(mainPlayer.getID()) - agk::GetSpriteX(getID_()) > 50) {
        setDirection(1);
        agk::SetSpriteFlip(getID_(), 0, 0);
    }
    
    if (agk::GetSpriteY(mainPlayer.getID()) - agk::GetSpriteY(getID_()) < 50) {
        yDirection = -1;
    } else if (agk::GetSpriteY(mainPlayer.getID()) - agk::GetSpriteY(getID_()) > 50) {
        yDirection = 1;
    }
    // Is bat in angry mode?
    if (angryMode == 0) {
        if (agk::GetSpriteDistance(getID_(), mainPlayer.getID()) > 150) {
            if (!agk::GetSpritePlaying(getID_())) {
                agk::PlaySprite(getID_(),10,1,1,5);
            }
            agk::SetSpritePosition(getID_(), (agk::GetSpriteX(getID_())+4*getDirection()), (agk::GetSpriteY(getID_())+1*yDirection));
        } else if (agk::GetSpriteDistance(getID_(), mainPlayer.getID()) < 150 && agk::GetSpriteDistance(getID_(), mainPlayer.getID()) > 130) {
            agk::StopSprite(getID_());
            agk::SetSpritePosition(getID_(), (agk::GetSpriteX(getID_())+5*getDirection()), (agk::GetSpriteY(getID_())+2*yDirection));
        } else {
            if (!agk::GetSpritePlaying(getID_())) {
                agk::PlaySprite(getID_(),10,1,5,10);
            }
            agk::SetSpritePosition(getID_(), (agk::GetSpriteX(getID_())+5*getDirection()), (agk::GetSpriteY(getID_())+2*yDirection));
        }
    }
    
    if (angryMode == 1) {
        if (!agk::GetSpritePlaying(getID_())) {
            agk::PlaySprite(getID_(), 10, 1, 13, 17);
        }
        agk::SetSpritePosition(getID_(), (agk::GetSpriteX(getID_())+8*getDirection()), (agk::GetSpriteY(getID_())+3*yDirection));
        
    }
    
    
    
}

void Bat::isHit(Projectile bullet,UI &userInterface) {
    setHealth(getHealth()-bullet.getDamage());
    agk::PlaySprite(getID_(),10,0,18,19);
    if (getHealth() < enrageHealth_) {
        angryMode = 1;
    }
}

void Bat::isDead(Player &mainPlayer) {
    mainPlayer.setExperience(mainPlayer.getExperience()+getExperience());
}
