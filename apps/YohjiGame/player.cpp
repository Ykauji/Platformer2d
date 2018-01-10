//
//  Player.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#include "player.h"
#include "agk.h"
#include <sstream>


void Player::attack() {
    // How many bullets can exist at once
    if (currentWep == 1 && numBullets_.size() < 10) {
        Projectile * beam = new Projectile();
        (*beam).setID(agk::CreateSprite(3));
        // How long the bullet exists for
        (*beam).setTime(120);
        agk::SetSpritePosition((*beam).getID_(),agk::GetSpriteX(iD_)+42*direction_, agk::GetSpriteY(iD_));
        agk::SetSpritePhysicsOn((*beam).getID_());
        agk::SetSpriteShape((*beam).getID_(),3);
        agk::SetSpritePhysicsImpulse((*beam).getID_(), agk::GetSpriteX(iD_)+direction_+50, agk::GetSpriteY(iD_)+50, 300*direction_, 0);
        agk::SetSpritePhysicsIsBullet((*beam).getID_(), 1);
        agk::SetSpritePhysicsRestitution((*beam).getID_(), .5);
        numBullets_.push_back(beam);
    }
    if (currentWep == 2 && numBullets_.size() < 10) {
        Projectile * beam = new Projectile(agk::CreateSprite(6),120,20+(damage_*.5));
        // How long the bullet exists for
        agk::SetSpriteSize((*beam).getID_(), 100,-1);
        if (direction_ == 1) {
            agk::SetSpriteFlip((*beam).getID_(), 1, 0);
        }
        if (direction_ == -1) {
            agk::SetSpriteFlip((*beam).getID_(), 0, 0);
        }
        agk::AddSpriteAnimationFrame((*beam).getID_(), 7);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 8);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 9);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 10);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 11);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 12);
        agk::PlaySprite((*beam).getID_());
        
        agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+60*direction_), agk::GetSpriteY(iD_));
        agk::SetSpritePhysicsOn((*beam).getID_(),3);
        agk::SetSpriteShape(beam->getID_(),3);
        agk::SetSpritePhysicsVelocity((*beam).getID_(), 750*direction_, 0);
        agk::SetSpritePhysicsCanRotate((*beam).getID_(), 0);
        agk::SetSpritePhysicsIsBullet((*beam).getID_(), 1);
        
        numBullets_.push_back(beam);
    }
    
    if (currentWep == 3) {
        Projectile *beam = new Projectile(agk::CreateSprite(30),10,5+(damage_*.1));
        agk::SetSpriteSize((*beam).getID_(), 100,-1);
        if (direction_ == 1) {
            agk::SetSpriteFlip((*beam).getID_(), 0, 1);
        }
        if (direction_ == -1) {
            agk::SetSpriteFlip((*beam).getID_(), 1, 1);
        }
        agk::AddSpriteAnimationFrame((*beam).getID_(), 30);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 31);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 32);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 33);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 34);
        
        agk::PlaySprite((*beam).getID_(),40,0);
        beam->setMelee(true);
        agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+120*direction_), agk::GetSpriteY(iD_));
        numBullets_.push_back(beam);
    }
}

//void Player::mouseAttack() {
//    if (currentWep == 1 && numBullets_.size() < 10) {
//        int wepSpeed = 100;
//        Projectile * beam = new Projectile();
//        (*beam).setID(agk::CreateSprite(3));
//        // How long the bullet exists for
//        (*beam).setTime(120);
//        agk::SetSpritePosition((*beam).getID_(),agk::GetSpriteX(iD_)+42*direction_, agk::GetSpriteY(iD_));
//        std::pair<float,float> trajectory(agk::GetRawMouseX() ,agk::GetRawMouseY());
//        beam->setTrajectory(trajectory);
//        beam->setSpeed(wepSpeed);
//        agk::SetSpritePhysicsOn((*beam).getID_());
//        agk::SetSpriteShape((*beam).getID_(),3);
////        agk::SetSpritePhysicsImpulse((*beam).getID_(), agk::GetSpriteX(iD_)+direction_+50, agk::GetSpriteY(iD_)+50, speed_*direction_, 0);
//
//        agk::SetSpritePhysicsIsBullet((*beam).getID_(), 1);
//        agk::SetSpritePhysicsRestitution((*beam).getID_(), .5);
//        numBullets_.push_back(beam);
//    }
//    if (currentWep == 2 && numBullets_.size() < 3) {
//        Projectile * beam = new Projectile(agk::CreateSprite(6),120,20+(damage_*.5));
//        // How long the bullet exists for
//        beam->setTime(60);
//        agk::SetSpriteSize((*beam).getID_(), 100,-1);
//        if (direction_ == 1) {
//            agk::SetSpriteFlip((*beam).getID_(), 1, 0);
//        }
//        if (direction_ == -1) {
//            agk::SetSpriteFlip((*beam).getID_(), 0, 0);
//        }
//        agk::AddSpriteAnimationFrame((*beam).getID_(), 7);
//        agk::AddSpriteAnimationFrame((*beam).getID_(), 8);
//        agk::AddSpriteAnimationFrame((*beam).getID_(), 9);
//        agk::AddSpriteAnimationFrame((*beam).getID_(), 10);
//        agk::AddSpriteAnimationFrame((*beam).getID_(), 11);
//        agk::AddSpriteAnimationFrame((*beam).getID_(), 12);
//        agk::PlaySprite((*beam).getID_());
//        
//        agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+60*direction_), agk::GetSpriteY(iD_));
//        std::pair<float,float> trajectory(agk::GetRawMouseX() ,agk::GetRawMouseY());
//        beam->setTrajectory(trajectory);
//        agk::SetSpritePhysicsOn((*beam).getID_(),3);
//        agk::SetSpritePhysicsIsSensor(beam->getID_(), 1);
//        agk::SetSpriteShape((*beam).getID_(),3);
//        agk::SetSpritePhysicsIsBullet((*beam).getID_(), 1);
//        agk::SetSpritePhysicsRestitution((*beam).getID_(), .5);
//        
//        numBullets_.push_back(beam);
//    }
//}

void Player::updateDirection(float mouseX) {
    if (dashTimer_ < 0) {
        if (mouseX < agk::GetSpriteX(getID())) {
            agk::SetSpriteFlip(iD_, 1, 0);
            direction_ = -1;
        } else {
            agk::SetSpriteFlip(iD_, 0, 0);
            direction_ = 1;
        }
    }
}
void Player::updateDirection() {
    if (dashTimer_ < 0) {
        if (direction_ == -1) {
            agk::SetSpriteFlip(iD_, 1, 0);
        } else if (direction_ == 1){
            agk::SetSpriteFlip(iD_, 0, 0);
        }
    }
}

void Player::movementLeft() {
    if (dashTimer_ < 0) {
        direction_ = -1;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*-1, 0);
    }
}

void Player::movementRight() {
    if (dashTimer_ < 0) {
        direction_ = 1;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_, 0);
    }
}

void Player::movementDashLeft() {
    if (dashTimer_ < 0) {
        agk::SetSpriteFlip(iD_, 1, 0);
        direction_ = -1;
        currentDash_ = 1;
        // Cooldown of Dash
        dashTimer_ = 20;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*-2.5, 0);
    }
}

void Player::movementDashRight() {
    if (dashTimer_ < 0) {
        agk::SetSpriteFlip(iD_, 0, 0);
        direction_ = 1;
        currentDash_ = 1;
        dashTimer_ = 20;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*2.5, 0);
    }
}
void Player::movementJump() {
    agk::PlaySprite(iD_,60,0,12,12);
    agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_), 0);
    agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), 0, jumpHeight_*-1);
    recentlyJumped++;
}
void Player::movementDashDown() {
    agk::SetSpritePhysicsVelocity(this->getID(), agk::GetSpritePhysicsVelocityX(iD_), 0);
    agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), 0, jumpHeight_);
}

void Player::stopMovement() {
    agk::StopSprite(iD_);
}

void Player::clearBullet(int index) {
    numBullets_[index]->deleteProjectile();
    numBullets_.erase(numBullets_.begin()+index);
}

void Player::updateHealth(UI &healthBar) {
    double healthPercent = 390*(health_/(double)maxHealth_);
    agk::SetSpriteSize(healthBar.getHealthBarGreen(),healthPercent,14); // 395,14 max
}

void Player::updateExperience(UI &experienceBar) {
    double experiencePercent = 390*(experience_/(double)maxExperience_);
    agk::SetSpriteSize(experienceBar.getexperienceBar(),experiencePercent,5);
}

bool Player::levelUp(UI &userInterface) {
    if (experience_ >= maxExperience_) {
        charLevel_++;
        experience_ = experience_ - maxExperience_;
        maxExperience_ = 100 * log(charLevel_+2);
        userInterface.createFadingText("Level Up!", 60, agk::GetSpriteX(getID())-120, agk::GetSpriteY(getID())-100, 60, 4);
        updateLevelStats();
        return true;
    }
    return false;
}

void Player::updateLevelStats() {
    damage_ = damage_ + 5;
    maxHealth_ = maxHealth_ + 20;
    health_ = health_ + 20;
}

