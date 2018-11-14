//
//  melee.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/15/18.
//
//

#include "melee.h"
#include "meleeProjectile.h"
#include "agk.h"

void MeleeSkill::castSkill(Player * p) {
    Projectile * beam = new MeleeProjectile(agk::CreateSprite(30),10,10+(damage_*.1),p->getID());
    agk::SetSpriteSize((*beam).getID_(), 200,75);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 30);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 31);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 32);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 33);
    agk::AddSpriteAnimationFrame((*beam).getID_(), 34);
    beam->setDirection(p->getDirection());
    
    if (p->getDirection() == 1) {
//        p->movementDashRight();
        p->setRecentlyDamaged(25);
        agk::SetSpriteFlip((*beam).getID_(), 0, 1);
        agk::SetSpritePosition((*beam).getID_(),((agk::GetSpriteX(p->getID())+agk::GetSpriteWidth(p->getID())/2+50)*p->getDirection()), agk::GetSpriteY(p->getID())+agk::GetSpriteHeight(p->getID())/3);
    }
    if (p->getDirection() == -1) {
//        p->movementDashLeft();
        p->setRecentlyDamaged(35);
        agk::SetSpriteFlip((*beam).getID_(), 1, 1);
        agk::SetSpritePosition((*beam).getID_(),(agk::GetSpriteX(p->getID())+(agk::GetSpriteWidth(p->getID())/2-80)*p->getDirection()), agk::GetSpriteY(p->getID())+agk::GetSpriteHeight(p->getID())/3);
    }
    // set here, or dash will override animation!
    p->setStateMeleeAttack();
    
    agk::PlaySprite((*beam).getID_(),30,0);
    beam->setMelee(true);
    p->pushBullet(beam);
    currentCooldown_ = cooldown_;
}
