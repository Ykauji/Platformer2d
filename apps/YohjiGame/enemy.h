//
//  enemy.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#ifndef enemy_h
#define enemy_h

//#include "player.h"
#include <iostream>
#include <vector>
#include "player.h"
#include "projectile.h"
#include "UI.h"
//#include "level.h"


class Player;
class Projectile;
class UI;

class enemyHealthBar {
  public:
    int healthBarBox_;
    int greenHealthBar_;
    int x1_;
    int y1_;
    int x2_;
    int y2_;
};

class Enemy {
public:
    Enemy(int spriteID,int MaxHealth,int damage,int speed,int experience) {SpriteID_ = spriteID; health_ = MaxHealth; damage_ = damage; experience_ = experience; recentlyDamaged_ = 0; maxHealth_ = MaxHealth; isEngage_ = 0;}
    Enemy();
    ~Enemy() {}
    
    void setID_(int iD) {SpriteID_ = iD;}
    int getID_() {return SpriteID_;}
    void setHealth(int health) {health_ = health;}
    int getHealth() {return health_;}
    void setDamage(int damage) {damage_ = damage;}
    int getDamage() {return damage_;}
    void setSpeed(int speed) {speed_ = speed;}
    int getSpeed() {return speed_;}
    int getDirection() {return direction_;}
    void setDirection(int direction) {direction_ = direction;}
    void setExperience(int experience) {experience_ = experience;}
    int getExperience() {return experience_;}
    void setRecentlyDamaged(int time) {recentlyDamaged_ = time;}
    int getRecentlyDamaged() {return recentlyDamaged_;}
    virtual void moveToPlayer(Player mainPlayer);
    void canMoveToPlayer(Player mainPlayer);
    int checkCollision(Player mainPlayer);
    void setMaxHealth(int maxHealth) {maxHealth_ = maxHealth;}
    int getMaxHealth() {return maxHealth_;}
    void setEngaged(bool engaged) {isEngage_ = engaged;}
    bool getEngaged() {return isEngage_;}
    
    std::vector<int> getDamageText() {return damageText;}
    
    void deleteEnemy();
    virtual void attack() {}
    virtual void isHit(Projectile bullet, UI &userInterface);
    virtual void isDead(Player &mainPlayer, UI &userInterface);
    std::string intToStringo(int value);
    virtual void enemySpecificHit();
    
    // HealthBar Stuff
    // X and Y = fix to center healthbars
    void initHealthBar(int x1, int y1,int x2,int y2);
    virtual void initHealthBar();
    void updateEnemyHealthBar();
    void showHealthBar();
    void deleteSprites();
    void testUpdateHealth();
    
protected:
    bool isEngage_;
    int maxHealth_;
    int health_;
    int damage_;
    int speed_;
    int experience_;
    int drop_;
    int SpriteID_;
    int direction_;
    std::vector<int> damageText;
    int recentlyDamaged_;
    enemyHealthBar healthBar_;
};
#endif /* enemy_h */
