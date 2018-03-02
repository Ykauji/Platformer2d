//
//  Player.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#include "player.h"
#include "agk.h"
#include "fireball.h"
#include <sstream>


void Player::attack() {
    // How many bullets can exist at once
    if (currentWep == 1 && numBullets_.size() < 10) {
        Projectile * beam = new Projectile();
        (*beam).setID(agk::CreateSprite(3));
        // How long the bullet exists for
        (*beam).setTime(120);
        
        agk::SetSpritePhysicsOn((*beam).getID_());
        agk::SetSpriteShape((*beam).getID_(),3);
        if (direction_ == -1) {
            agk::SetSpritePosition((*beam).getID_(),agk::GetSpriteX(iD_)+20*direction_, agk::GetSpriteY(iD_));
            agk::SetSpritePhysicsImpulse((*beam).getID_(), agk::GetSpriteX(iD_)+direction_*50, agk::GetSpriteY(iD_)+50, 100*direction_, 50);
        } else {
            agk::SetSpritePosition((*beam).getID_(),agk::GetSpriteX(iD_)+70*direction_, agk::GetSpriteY(iD_));
            agk::SetSpritePhysicsImpulse((*beam).getID_(), agk::GetSpriteX(iD_)+direction_*70, agk::GetSpriteY(iD_)+50, 100*direction_, 50);
        }
        
        agk::SetSpritePhysicsIsBullet((*beam).getID_(), 1);
        agk::SetSpritePhysicsRestitution((*beam).getID_(), .5);
        agk::SetSpriteSize(beam->getID_(),125);
        numBullets_.push_back(beam);
    }
    if (currentWep == 2 && numBullets_.size() < 10) {
        Projectile * beam = new Fireball();
        
        beam->setID(agk::CreateSprite(7));
        beam->setTime(100);
        beam->setDamage(25);
        beam->setDirection(direction_);
        beam->setSpeed(14);
        // How long the bullet exists for
        agk::SetSpriteSize((*beam).getID_(), 100,-1);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 7);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 8);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 9);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 10);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 11);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 12);
        agk::PlaySprite((*beam).getID_());
        agk::SetSpriteActive(beam->getID_(), 1);
        
        if (direction_ == 1) {
            agk::SetSpriteFlip((*beam).getID_(), 1, 0);
            agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+80*direction_), agk::GetSpriteY(iD_)+35);
        }
        if (direction_ == -1) {
            agk::SetSpriteFlip((*beam).getID_(), 0, 0);
            agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+40*direction_), agk::GetSpriteY(iD_)+35);
        }
        
        
        
        numBullets_.push_back(beam);
    }
    
    if (currentWep == 3) {
        Projectile *beam = new Projectile(agk::CreateSprite(30),10,10+(damage_*.1));
        agk::SetSpriteSize((*beam).getID_(), 100,-1);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 30);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 31);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 32);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 33);
        agk::AddSpriteAnimationFrame((*beam).getID_(), 34);
        
        if (direction_ == 1) {
            agk::SetSpriteFlip((*beam).getID_(), 0, 1);
            agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+150*direction_), agk::GetSpriteY(iD_));
        }
        if (direction_ == -1) {
            agk::SetSpriteFlip((*beam).getID_(), 1, 1);
            agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(iD_)+80*direction_), agk::GetSpriteY(iD_));
        }
        if (!(agk::GetSpriteCurrentFrame(iD_) > 9 && agk::GetSpriteCurrentFrame(iD_) < 14)) {
            agk::PlaySprite(iD_,20,1,10,13);
        }
        agk::PlaySprite(iD_,10,0,10,13);
        agk::PlaySprite((*beam).getID_(),40,0);
        beam->setMelee(true);
        numBullets_.push_back(beam);
    }
}

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
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*-3, 0);
    }
}

void Player::movementDashRight() {
    if (dashTimer_ < 0) {
        agk::SetSpriteFlip(iD_, 0, 0);
        direction_ = 1;
        currentDash_ = 1;
        dashTimer_ = 20;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*3, 0);
    }
}
void Player::movementJump() {
//    agk::PlaySprite(iD_,60,0,12,12);
    agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_), 0);
    agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), 0, jumpHeight_*-1);
    recentlyJumped++;
}
// If true, then door is hit.
void Player::movementDashDown() {
    agk::SetSpritePhysicsVelocity(this->getID(), agk::GetSpritePhysicsVelocityX(iD_), 0);
    agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), 1, jumpHeight_);
}

int Player::checkDoor() {
    return agk::GetSpriteHitGroup(5, agk::GetSpriteX(iD_)+60, agk::GetSpriteY(iD_));
}

void Player::stopMovement() {
    if (!(agk::GetSpriteCurrentFrame(getID()) < 10 && agk::GetSpriteCurrentFrame(getID()) > 6)) {
        agk::PlaySprite(getID(), 7, 1, 7, 9);
    }
}

bool Player::touchingLeftWall() {
    if ((agk::GetSpriteFirstContact(wallSensorIDLeft_) == 1 && agk::GetSpriteHitGroup(2, agk::GetSpriteX(wallSensorIDLeft_), agk::GetSpriteY(wallSensorIDLeft_))) || (agk::GetSpriteFirstContact(wallSensorIDLeft_) == 1 && agk::GetSpriteHitGroup(2, agk::GetSpriteX(wallSensorIDLeft_), agk::GetSpriteY(wallSensorIDLeft_)+40))) {
        
        return true;
    } else {
        return false;
    }
}
bool Player::touchingRightWall() {
    if ((agk::GetSpriteFirstContact(wallSensorIDRight_) == 1 && agk::GetSpriteHitGroup(2, agk::GetSpriteX(wallSensorIDRight_), agk::GetSpriteY(wallSensorIDRight_))) || (agk::GetSpriteFirstContact(wallSensorIDRight_) == 1 && agk::GetSpriteHitGroup(2, agk::GetSpriteX(wallSensorIDRight_), agk::GetSpriteY(wallSensorIDRight_)+40))) {
        
        return true;
    } else {
        return false;
    }
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

void Player::loadPlayer() {
    agk::LoadImage(1, "Player/p3_walk01.png");
    setID(agk::CreateSprite(1));
    agk::SetSpriteSize(getID(),68);
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk01.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk02.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk03.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk04.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk05.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk06.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk07.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk08.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk09.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk10.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_walk11.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("Player/p3_jump.png"));
}
void Player::loadPlayerTest() {
    agk::LoadImage(1, "mainCharacter/rogue like idle_Animation 1_0.png");
    setID(agk::CreateSprite(1));
    agk::SetSpriteSize(getID(),170);
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like run_Animation 1_0.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like run_Animation 1_1.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like run_Animation 1_2.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like run_Animation 1_3.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like run_Animation 1_4.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like run_Animation 1_5.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like idle_Animation 1_0.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like idle_Animation 1_1.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("mainCharacter/rogue like idle_Animation 1_2.png"));
        
    
    // Ground Sensor
    groundSensorID_ = agk::CreateSprite(0);
    agk::SetSpriteSize(groundSensorID_ ,30,5);
    agk::SetSpriteVisible(groundSensorID_, 0);
    agk::SetSpritePhysicsOn(groundSensorID_,2);
    agk::SetSpriteShape(groundSensorID_,3);
    agk::SetSpritePhysicsIsSensor(groundSensorID_, 1);
    
    // Wall Sensors
    wallSensorIDLeft_ = agk::CreateSprite(0);
    agk::SetSpriteSize(wallSensorIDLeft_ ,5,60);
    agk::SetSpriteVisible(wallSensorIDLeft_, 0);
    agk::SetSpritePhysicsOn(wallSensorIDLeft_,2);
    agk::SetSpritePhysicsIsSensor(wallSensorIDLeft_, 1);
    agk::SetSpriteShape(wallSensorIDLeft_,2);
    
    wallSensorIDRight_ = agk::CreateSprite(0);
    agk::SetSpriteSize(wallSensorIDRight_ ,5,60);
    agk::SetSpriteVisible(wallSensorIDRight_, 0);
    agk::SetSpritePhysicsOn(wallSensorIDRight_,2);
    agk::SetSpritePhysicsIsSensor(wallSensorIDRight_, 1);
    agk::SetSpriteShape(wallSensorIDRight_,2);

    
}

void Player::loadPlayerPhysics() {
    agk::SetSpritePosition(getID(), 200, 200);
    agk::SetSpritePhysicsOn(getID());
    agk::SetSpritePhysicsFriction(getID(), 2);
    agk::SetSpritePhysicsCanRotate(getID(), 0);
    agk::SetSpriteShape(getID(),3);
    agk::SetSpritePhysicsRestitution(getID(), 0);
    agk::SetSpriteDepth(getID(), 1);
}

void Player::updateLevelStats() {
    damage_ = damage_ + 5;
    maxHealth_ = maxHealth_ + 20;
    health_ = health_ + 20;
}

void Player::resetPlayer() {
    health_ = 100;
    speed_ = 6500;
    jumpHeight_ = 7500;
    maxJumps = 1;
    recentlyJumped = 0;
    currentWep = 1;
    direction_ = 1;
    dashTimer_ = 0;
    recentlyDamaged_ = 0;
    maxHealth_ = 100;
    experience_ = 0;
    maxExperience_ = 100;
    charLevel_ = 1;
    damage_ = 10;
    agk::SetSpritePosition(iD_, 0, 0);
    movementRight();
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

