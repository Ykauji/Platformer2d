//
//  fireballskill.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/10/18.
//
//

#include "fireballskill.h"
#include "agk.h"

void Fireballskill::castSkill(Player * p) {
    Projectile * beam = new Fireball();
    beam->setID(agk::CreateSprite(7));
    beam->setTime(100);
    beam->setDamage(25);
    beam->setDirection(p->getDirection());
    beam->setSpeed(14);
    beam->setMelee(true);
    // How long the bullet exists for
    agk::SetSpriteSize((*beam).getID_(), 100,-1);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 7);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 8);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 9);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 10);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 11);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 12);
    agk::PlaySprite((*beam).getID_());
    agk::SetSpriteActive(beam->getID_(), 1);
    
    p->setStateRangedAttack();
    
    if (p->getDirection() == 1) {
        agk::SetSpriteFlip((*beam).getID_(), 1, 0);
        agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(p->getID())+(agk::GetSpriteWidth(p->getID())/2+50)*p->getDirection()), agk::GetSpriteY(p->getID())+(agk::GetSpriteHeight(p->getID())/2)-15);
    }
    if (p->getDirection() == -1) {
        agk::SetSpriteFlip((*beam).getID_(), 0, 0);
        agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(p->getID())+0*p->getDirection()), agk::GetSpriteY(p->getID())+(agk::GetSpriteHeight(p->getID())/2)-15);
    }
    
    agk::PlaySound(2);
    p->pushBullet(beam);
    currentCooldown_ = cooldown_;
}
