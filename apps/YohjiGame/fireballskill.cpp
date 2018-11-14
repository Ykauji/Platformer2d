//
//  fireballskill.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/10/18.
//
//

#include "fireballskill.h"
#include "agk.h"
#include "particleManager.h"
#include "particleSystem.h"

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
    // Animation
    p->setStateRangedAttack();
    // Singleton access.
    ParticleManager &particleManager = ParticleManager::getInstance();
    ParticleSystem fireParticles;
    fireParticles.initParticleSystem(agk::GetSpriteX(p->getRightSensor()), agk::GetSpriteY(p->getRightSensor()));
    fireParticles.setParticleAngle(45.0f);
    fireParticles.setParticleFrequency(7.0f);
    fireParticles.setParticleLife(10.0f);
    fireParticles.setStopEmittingTimer(0.05f);
    fireParticles.setTimer(0.15f);
    fireParticles.setParticleSize(3.0f);
    fireParticles.addColorFrames(0.0f, 243, 82, 82, 255);
    fireParticles.addColorFrames(0.1f, 220, 124, 61, 255);
    fireParticles.addColorFrames(0.19f, 247, 209, 102, 255);
    fireParticles.addColorFrames(0.2f, 255, 255, 255, 0);
    
    if (p->getDirection() == 1) {
        agk::SetSpriteFlip((*beam).getID_(), 1, 0);
        agk::SetSpritePosition((*beam).getID_(),agk::GetSpriteX(p->getRightSensor())-40, agk::GetSpriteY(p->getRightSensor())+(agk::GetSpriteHeight(p->getRightSensor())/2)-35);
        fireParticles.setParticleDirection(3.0f,0.5f);
        fireParticles.setPositionParticleSystem(agk::GetSpriteX(p->getRightSensor()), agk::GetSpriteY(p->getRightSensor())+(agk::GetSpriteHeight(p->getRightSensor())/2));
        fireParticles.setFollowSprite(p->getRightSensor(), 0, agk::GetSpriteHeight(p->getRightSensor())/2);
    }
    if (p->getDirection() == -1) {
        agk::SetSpriteFlip((*beam).getID_(), 0, 0);
        agk::SetSpritePosition((*beam).getID_(),agk::GetSpriteX(p->getLeftSensor())+40, agk::GetSpriteY(p->getLeftSensor())+(agk::GetSpriteHeight(p->getLeftSensor())/2)-35);
        fireParticles.setParticleDirection(-3.0f,0.5f);
        fireParticles.setPositionParticleSystem(agk::GetSpriteX(p->getLeftSensor()), agk::GetSpriteY(p->getLeftSensor())+(agk::GetSpriteHeight(p->getLeftSensor())/2));
        fireParticles.setFollowSprite(p->getLeftSensor(), 0, agk::GetSpriteHeight(p->getLeftSensor())/2);
    }
    

    
    agk::PlaySound(2);
    p->pushBullet(beam);
    particleManager.addNewParticleSystem(fireParticles);
    currentCooldown_ = cooldown_;
}
