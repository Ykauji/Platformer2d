//
//  meleeProjectile.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/17/18.
//
//

#ifndef meleeProjectile_h
#define meleeProjectile_h

#include <stdio.h>
#include "projectile.h"

class MeleeProjectile : public Projectile {
  public:
    MeleeProjectile(int iD, int time, int weaponDamage,int playerID) {weaponType_ = iD; time_ = time; weaponDamage_ = weaponDamage; userID_ = playerID;}
    void updateBullet();
  private:
};

#endif /* meleeProjectile_hpp */
