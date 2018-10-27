//
//  Player.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#include "player.h"
#include "agk.h"
#include "fireballskill.h"
#include "melee.h"
#include "tornado.h"
#include "frostbolt.h"
#include <sstream>

void Player::attack() {
    if (skills_.count(currentWep) == 1) {
        if (skills_[currentWep]->canCast()) {
            skills_[currentWep]->castSkill(this);
        }
    }
}
// Mouse update direction
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
// Keyboard update direction
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
        int maxSpeed = 750;
        agk::SetSpriteFlip(iD_, 1, 0);
        // Set State
        if (playerState == Idle) {
            playerState = Walking;
        }
        if (agk::GetSpritePhysicsVelocityX(iD_) > 0) {
            agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_)-200, agk::GetSpritePhysicsVelocityY(iD_));
        } else if (agk::GetSpritePhysicsVelocityX(iD_) > -maxSpeed/4) {
            agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_)-250, agk::GetSpritePhysicsVelocityY(iD_));
        } else if (agk::GetSpritePhysicsVelocityX(iD_) < -maxSpeed) {
            agk::SetSpritePhysicsVelocity(iD_, -maxSpeed, agk::GetSpritePhysicsVelocityY(iD_));
        } else {
            agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_)-40, agk::GetSpritePhysicsVelocityY(iD_));
        }
        
        
//        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
//        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*-1, 0);
    }
}

void Player::movementRight() {
    if (dashTimer_ < 0) {
        agk::SetSpriteFlip(iD_, 0, 0);
        direction_ = 1;
        int maxSpeed = 750;
        // Setting State
        if (playerState == Idle) {
            playerState = Walking;
        }
        if (agk::GetSpritePhysicsVelocityX(iD_) < 100) {
            agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_)+200, agk::GetSpritePhysicsVelocityY(iD_));
        } else if (agk::GetSpritePhysicsVelocityX(iD_) < maxSpeed/4) {
            agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_)+250, agk::GetSpritePhysicsVelocityY(iD_));
        } else if (agk::GetSpritePhysicsVelocityX(iD_) > maxSpeed) {
            agk::SetSpritePhysicsVelocity(iD_, maxSpeed, agk::GetSpritePhysicsVelocityY(iD_));
        } else {
            agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_)+40, agk::GetSpritePhysicsVelocityY(iD_));
        }
//        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
//        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_, 0);
    }
}

void Player::movementDashLeft() {
    if (dashTimer_ < 0) {
        agk::SetSpriteFlip(iD_, 1, 0);
        playerState = Dashing;
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
        playerState = Dashing;
        direction_ = 1;
        currentDash_ = 1;
        dashTimer_ = 20;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*3, 0);
    }
}

void Player::movementDash() {
    if (dashTimer_ < 0) {
        currentDash_ = 1;
        dashTimer_ = 20;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), direction_*speed_*3, 0);
    }
}

void Player::movementJump() {
    playerState = Jumping;
    agk::SetSpritePhysicsVelocity(iD_, agk::GetSpritePhysicsVelocityX(iD_), 0);
    agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), 0, jumpHeight_*-1);
    recentlyJumped++;
}
// If true, then door is hit.
void Player::movementDashDown() {
    agk::SetSpritePhysicsVelocity(this->getID(), agk::GetSpritePhysicsVelocityX(iD_), 0);
    agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), 1, jumpHeight_*2);
}

int Player::checkDoor() {
    return agk::GetSpriteHitGroup(5, agk::GetSpriteX(iD_)+60, agk::GetSpriteY(iD_));
}

void Player::stopMovement() {
    if (playerState == State::Walking) {
            playerState = State::Idle;
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
    double experiencePercent = 1850*(experience_/(double)maxExperience_);
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

void Player::updatePlayer(double frameTime, UI & userInterface) {
    updateHealth(userInterface);
    updateExperience(userInterface);
    levelUp(userInterface);
    for (auto & it : skills_) {
        userInterface.updateIcon(it.first,it.second->updateCooldown(frameTime));
    }
    playAnimations();
}

void Player::playAnimations() {
    switch (playerState) {
        case Idle:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 9 && agk::GetSpriteCurrentFrame(getID()) > 4))) {
                agk::PlaySprite(getID(), 7, 1, 5, 8);
            }
            break;
        case Walking:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 5) && agk::GetSpriteCurrentFrame(getID()) > 0)) {
                agk::PlaySprite(getID(), 8, 1, 1, 4);
            }
            break;
        case Jumping:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 20) && agk::GetSpriteCurrentFrame(getID()) > 16)) {
                agk::PlaySprite(getID(), 8, 1, 17, 19);
            }
            break;
        case Hurt:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 24) && agk::GetSpriteCurrentFrame(getID()) > 21)) {
                agk::PlaySprite(getID(), 20, 0, 22, 23);
            }
            break;
        case DoubleJump:
            break;
        case JumpAttack:
            break;
        case Dashing:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 22) && agk::GetSpriteCurrentFrame(getID()) > 19)) {
                agk::PlaySprite(getID(), 5, 0, 20, 21);
            }
            break;
        case WallSliding:
            // If not touching wall, switch states
            if (!touchingLeftWall() && !touchingRightWall() && playerState == WallSliding) {
                playerState = Idle;
            }
            if (!(agk::GetSpriteCurrentFrame(getID()) == 24)) {
                agk::PlaySprite(getID(),5,1,24,24);
            }
            break;
        case MeleeAttacking:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 17) && agk::GetSpriteCurrentFrame(getID()) > 8)) {
                agk::PlaySprite(getID(), 20, 0, 9, 16);
            }
            if (agk::GetSpriteCurrentFrame(getID()) == 16) {
                playerState = Idle;
            }
            break;
        case Attacking2:
            if (!((agk::GetSpriteCurrentFrame(getID()) < 32) && agk::GetSpriteCurrentFrame(getID()) > 24)) {
                agk::PlaySprite(getID(), 20, 0, 25, 31);
            }
            if (agk::GetSpriteCurrentFrame(getID()) == 31) {
                playerState = Idle;
            }

            break;
        case Aquire:
            break;
    }
//    std::cout << "current state =" << playerState << std::endl;
}

void Player::loadPlayerTest() {
    agk::LoadImage(1, "PlayerSprites/Hero/idle/Idle01.png");
    setID(agk::CreateSprite(1));
    agk::SetSpriteSize(getID(),350);
    
    // Run: 1-4
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/run/Run01.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/run/Run02.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/run/Run03.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/run/Run04.png"));
    
    // Idle 5-8
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/idle/Idle01.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/idle/Idle02.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/idle/Idle03.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/idle/Idle04.png"));
    
    // Attack 9-16
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack01.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack02.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack03.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack04.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack05.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack06.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack07.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/attack/Attack08.png"));
    
    // Jumping 17-19
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/jump/Jump01.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/jump/Jump02.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/jump/Jump03.png"));
    
    // Dashing 20-21
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/dash/shadow.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/dash/Dash01.png"));
    
    // Hurt 22-23
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/hurt/Hurt01.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/hurt/Hurt02.png"));
    
    // Wall hold! 24
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/Wall Slide/Wall Slide01.png"));
    
    // Attack#2 25-31
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill101.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill102.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill103.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill104.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill105.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill106.png"));
    agk::AddSpriteAnimationFrame(getID(), agk::LoadImage("PlayerSprites/Hero/skill1/Skill107.png"));

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

    // Add skills
    Skills * fireball = new Fireballskill();
    changeSkill(1, fireball);
    Skills * melee = new MeleeSkill();
    changeSkill(2,melee);
    Skills * tornado = new TornadoSkill();
    changeSkill(3,tornado);
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
    speed_ = 9000;
    jumpHeight_ = 12000;
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
    gold_ = 0;
    playerState = Idle;
    agk::SetSpritePosition(iD_, 0, 0);
    movementRight();
}

void Player::resetJump() {
    recentlyJumped = 0;
    if (playerState == Jumping) {
        playerState = Idle;
    }
}

void Player::changeSkill(int skillBox, Skills * skill) {
    // Clear skill ptr in current box.
    if (skills_.count(skillBox) == 1) {
         delete skills_[skillBox];
    }
    skills_[skillBox] = skill;
}

std::string intToStringooo(int value) {
    char buffer [50];
    int n, a=value;
    n=sprintf (buffer, "%d", a);
    return buffer;
}

void Player::checkEnemyCollisions(std::vector<Enemy*> & enemies, UI & userInterface) {
    if (getRecentlyDamaged() < 0 ) {
        for (int i = 0; i < enemies.size(); i++) {
            if (agk::GetSpriteCollision(getID(), enemies[i]->getID_())) {
                agk::SetSpritePhysicsImpulse(getID(), agk::GetSpriteXByOffset(getID()), agk::GetSpriteYByOffset(getID()),9000*enemies[i]->checkCollision(*this) , -2000);
                setHealth(getHealth()-enemies[i]->getDamage());
                const std::string bulletNumbero = intToStringooo(enemies[i]->getDamage()*-1);
                userInterface.createFadingText(bulletNumbero, 60, agk::GetSpriteX(getID())+ agk::Random2(-20,40), agk::GetSpriteY(getID()), 45,5);
                setStateHurt();
                setRecentlyDamaged(25);
                agk::PlaySound(4);
            }
        }
    }
    setRecentlyDamaged(getRecentlyDamaged()-1);
   
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

