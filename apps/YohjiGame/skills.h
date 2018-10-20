//
//  skills.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/10/18.
//
//

#ifndef skills_h
#define skills_h

#include <iostream>

class Player;

class Skills {
  public:
    Skills() {cooldown_ = .25; currentCooldown_ = 0;}
    Skills(double cooldown, double damage, double scaling) {cooldown_ = cooldown; damage_ = damage; damageScaling_ = scaling;}
    virtual void castSkill(Player * p) {}
    
    bool canCast() {
        if (currentCooldown_ < 0) {
            return true;
        }
        return false;
    }
    std::pair<double,double> updateCooldown(double frameTime);
    
  protected:
    double cooldown_;
    double currentCooldown_;
    double damage_;
    double damageScaling_;
    int skillIcon_;
};





#endif /* skills_h */
