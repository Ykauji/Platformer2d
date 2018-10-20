//
//  fireball.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 1/7/18.
//
//

#ifndef fireball_h
#define fireball_h

#include "projectile.h"
#include "enemy.h"

class Enemy;

class Fireball : public Projectile {
  public:
    Fireball() {knockback = 4000;}
    void updateBullet();
    void specialEffect(Enemy & e);
  private:
    int knockback;
    int burn;
    
};









#endif /* fireball_h */
