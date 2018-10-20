//
//  Projectile.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#ifndef projectile_h
#define projectile_h
#include <iostream>

class Enemy;


class Projectile {
public:
    Projectile() {time_ = 0; weaponDamage_ = 20;}
    Projectile(int iD, int time, int weaponDamage) {weaponType_ = iD; time_ = time; weaponDamage_ = weaponDamage;}
    Projectile(int iD, int time, int weaponDamage,int playerID) {weaponType_ = iD; time_ = time; weaponDamage_ = weaponDamage; userID_ = playerID;}
    Projectile(int iD, int time, int weaponDamage,int direction,int speed) {weaponType_ = iD; time_ = time; weaponDamage_ = weaponDamage; direction_ = direction; speed_ = speed;}
    Projectile(std::pair<float,float> trajectory);
    void setID(int iD_) {weaponType_ = iD_;}
    int getID_() {return weaponType_;}
    int getDirection() {return direction_;}
    void setSpeed(int speed) {speed_ = speed;}
    void setDirection(int direction) {direction_ = direction;}
    void setTrajectory(std::pair<float,float> trajectory) {trajectory_ = trajectory;}
    std::pair<float,float> getTrajectory() {return trajectory_;}
    int getSpeed() {return speed_;}
    int getDamage() {return weaponDamage_;}
    void setDamage(int damage) {weaponDamage_ = damage;}
    void setTime(int time) {time_ = time;}
    int getTime() {return time_;}
    void decreaseTime() {time_--;}
    void deleteProjectile();
    virtual void updateBullet();
    virtual void specialEffect(Enemy & e);
    bool isMelee() {return isMelee_;}
    void setMelee(bool value) {isMelee_ = value;}
    
protected:
    int weaponType_;
    int speed_;
    int weaponDamage_;
    int time_;
    int direction_;
    int knockback_;
    int userID_;
    bool isMelee_;
    // Float of mouse pointer
    std::pair<float,float> trajectory_;
};



































#endif /* Projectile_h */
