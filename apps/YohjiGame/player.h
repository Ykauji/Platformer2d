//
//  Player.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#ifndef player_h
#define player_h

#include "UI.h"
#include "projectile.h"
#include <vector>
#include "skills.h"
#include "enemy.h"

class Player {
public:
    Player() {health_ = 100; speed_ = 9000; jumpHeight_ = 12000; maxJumps = 1; recentlyJumped = 0; currentWep = 1; direction_ = 1; dashTimer_ = 0; recentlyDamaged_ = 0; maxHealth_ = 100; experience_ = 0; maxExperience_ = 100; charLevel_ = 1; damage_ = 10; gold_ = 0; playerState = Idle;}
    ~Player() {

    }
    // Load Players
    void loadPlayer();
    void loadPlayerTest();
    void loadPlayerPhysics();
    void resetPlayer();
    
    // Movement
    void movementLeft();
    void movementRight();
    virtual void movementDashLeft();
    virtual void movementDashRight();
    virtual void movementDash();
    void movementJump();
    void movementDashDown();
    int checkDoor();
    void stopMovement();
    void setDirection(int direction) {direction_ = direction;}
    int getDirection() {return direction_;}
    void recentJumpPlus() {recentlyJumped++;}
    void resetJump();
    void dashTimerDecrease() {dashTimer_--;}
    bool levelUp(UI &userInterface);
    void updateDirection(float mouseX);
    void updateDirection();
    bool touchingLeftWall();
    bool touchingRightWall();
    
    // Setters and Getters. Bad practice? change later.
    void setHealth(int health) {health_ = health;}
    void setGold(int gold) {gold_ = gold;}
    void setSpeed(float speed) {speed_ = speed;}
    void setID(int iD) {iD_ = iD;}
    void setJump(int jump) {jumpHeight_ = jump;}
    void setWep(int wep) {currentWep = wep;}
    void setExperience(int experience) {experience_ = experience;}
    void setHealthBar(int healthBar) {healthBar_ = healthBar;}
    void setCharLevel(int charLevel) {charLevel_ = charLevel;}
    void setMaxExperience(int maxExperience) {maxExperience_ = maxExperience;}
    void setMaxHealth(int maxHealth) {maxHealth_ = maxHealth;}
    void setCharLevelStr(std::string charLevelStr) {charLevelStr_ = charLevelStr;}
    void setCurrentHealthRatio(std::string currentHealthRatio) {currentHealthRatio_ = currentHealthRatio;}
    
    // States
    void setStateIdle() {playerState = Idle;}
    void setStateHurt() {playerState = Hurt;}
    void setStateWallSlide() {playerState = WallSliding;}
    void setStateMeleeAttack() {playerState = MeleeAttacking;}
    void setStateRangedAttack() {playerState = Attacking2;}
    
    int getGroundSensor() {return groundSensorID_;}
    int getLeftSensor() {return wallSensorIDLeft_;}
    int getRightSensor() {return wallSensorIDRight_;}
    int getHealth() {return health_;}
    int getGold() {return gold_;}
    int getMaxHealth() {return maxHealth_;}
    int getHealthBar() {return healthBar_;}
    float getSpeed() {return speed_;}
    int getID() {return iD_;}
    int getJump() {return jumpHeight_;}
    int getRecentlyJumped() {return recentlyJumped;}
    int getMaxJumps() {return maxJumps;}
    unsigned long getNumBullets() {return numBullets_.size();}
    int getDashTimer() {return dashTimer_;}
    int getCurrentWep() {return currentWep;}
    int getExperience() {return experience_;}
    int getCharLevel() {return charLevel_;}
    std::string getCharLevelStr() {return charLevelStr_;}
    std::string getCurrentHealthRatio() {return currentHealthRatio_;}
    int getMaxExperience() {return maxExperience_;}
    void setRecentlyDamaged(int timer) {
        recentlyDamaged_ = timer;
        if (recentlyDamaged_ == 0 && playerState == Hurt) {
            playerState = Idle;
        }
    }
    int getRecentlyDamaged() {return recentlyDamaged_;}
    void clearBullet(int index);
    std::vector<Projectile*> getBullets() {return numBullets_;}
    void pushBullet(Projectile * p) {numBullets_.push_back(p); }
    
    // Updates
    void updatePlayer(double frameTime,UI & userInterface);
    void checkEnemyCollisions(std::vector<Enemy*> & enemies, UI & userInterface);
    
    // Health Updates
    void updateHealth(UI &healthBar);
    // Experience Update
    void updateExperience(UI &experienceBar);
    void updateLevelStats();
    
    // Use currentWep to decide which attack is used.
    void attack();
    void mouseAttack();
    
    // Add skills!
    void changeSkill(int skillBox, Skills * skill);
    
    // Animations
    void playAnimations();
    
protected:
    int maxHealth_;
    int health_;
    int healthBar_;
    float speed_;
    double damage_;
    int iD_;
    int gold_;
    int groundSensorID_;
    int wallSensorIDLeft_;
    int wallSensorIDRight_;
    int iDBox_;
    int jumpHeight_;
    int recentlyJumped;
    int experience_;
    int maxExperience_;
    int maxJumps;
    int currentWep;
    int currentDash_;
    int dashTimer_;
    int charLevel_;
    std::string charLevelStr_;
    std::string currentHealthRatio_;
    
    enum State {
        Idle,
        Walking,
        Dashing,
        MeleeAttacking,
        Attacking2,
        Jumping,
        Hurt,
        DoubleJump,
        WallSliding,
        JumpAttack,
        Aquire,
    };
    
    State playerState;
    
    // Invulnerable frame time
    int recentlyDamaged_;
    // -1 equals left 1 equals right
    int direction_;
    // Projectiles
    std::vector<Projectile*> numBullets_;
    // Skills -> this holds functions called on usage of skill from the designated box.
    std::map<int,Skills*> skills_;

};

#endif
