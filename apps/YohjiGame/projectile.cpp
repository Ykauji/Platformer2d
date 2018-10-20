//
//  Projectile.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//
#include "agk.h"
#include "projectile.h"
#include <cmath>
#include "enemy.h"

using namespace AGK;

void Projectile::deleteProjectile() {
    agk::DeleteSprite(weaponType_);
}
Projectile::Projectile(std::pair<float,float> trajectory) {
    trajectory_ = trajectory;
    // Normalize Vector : Unit Vector = OldVector/Magnitude <- Length of Vector
    int unitVectorMagnitude = sqrt((trajectory_.first * trajectory_.first) + (trajectory_.second * trajectory_.second));
    trajectory_.first = trajectory_.first / unitVectorMagnitude;
    trajectory_.second = trajectory_.second / unitVectorMagnitude;
    agk::SetSpritePhysicsVelocity(getID_(), trajectory_.first * speed_, trajectory_.second * speed_);
}

void Projectile::updateBullet() {
//    agk::SetSpriteAngle(this->getID_(), agk::GetSpriteAngle(getID_())+2*getDirection());
}

void Projectile::specialEffect(Enemy & e) {
    
}
