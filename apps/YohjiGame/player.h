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

class Player {
public:
    Player() {health_ = 100; speed_ = 6000; jumpHeight_ = 7000; maxJumps = 2; recentlyJumped = 0; currentWep = 1; direction_ = 1; dashTimer_ = 0; recentlyDamaged_ = 0; maxHealth_ = 100; experience_ = 0; maxExperience_ = 100; charLevel_ = 1; damage_ = 10;}
    
    // Movement
    void movementLeft();
    void movementRight();
    void movementDashLeft();
    void movementDashRight();
    void movementJump();
    void movementDashDown();
    void stopMovement();
    void setDirection(int direction) {direction_ = direction;}
    int getDirection() {return direction_;}
    void recentJumpPlus() {recentlyJumped++;}
    void resetJump() {recentlyJumped = 0;}
    void dashTimerDecrease() {dashTimer_--;}
    bool levelUp(UI &userInterface);
    void updateDirection(float mouseX);
    void updateDirection();
    
    // Setters and Getters. Bad practice, change later.
    void setHealth(int health) {health_ = health;}
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
    int getHealth() {return health_;}
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
    void setRecentlyDamaged(int timer) {recentlyDamaged_ = timer;}
    int getRecentlyDamaged() {return recentlyDamaged_;}
    void clearBullet(int index);
    std::vector<Projectile*> getBullets() {return numBullets_;}
    
    // Health Updates
    void updateHealth(UI &healthBar);
    // Experience Update
    void updateExperience(UI &experienceBar);
    void updateLevelStats();
    
    // Use currentWep to decide which attack is used.
    void attack();
    void mouseAttack();
    
private:
    int maxHealth_;
    int health_;
    int healthBar_;
    float speed_;
    double damage_;
    int iD_;
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
    // Invulnerable frame time
    int recentlyDamaged_;
    // -1 equals left 1 equals right
    int direction_;
    std::vector<Projectile*> numBullets_;
};

#endif
