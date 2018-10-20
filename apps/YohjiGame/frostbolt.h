//
//  frostbolt.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 7/14/18.
//
//

#ifndef frostbolt_h
#define frostbolt_h

class Enemy;

#include <stdio.h>
#include "enemy.h"

class Frostbolt : public Projectile {
public:
    Frostbolt() {slow = 1000;}
    void updateBullet();
    void specialEffect(Enemy & e);
private:
    int slow;
};


#endif /* frostbolt_hpp */
