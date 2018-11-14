//
//  particleSystem.cpp
//  agk_interpreter
//
//  Created by Yohji Kusakabe on 11/11/18.
//

#include "particleSystem.h"
#include "agk.h"
#include <iostream>

void ParticleSystem::setParticleImage(int iD) {
    particleImage_ = iD;
    if (isInit_ == false) {
        std::cout << "must init particleSystem first!" << std::endl;
        return;
    }
    agk::SetParticlesImage(particleID_, particleImage_);
}
// returns false if needs to be deleted from ParticleManager
bool ParticleSystem::update(float timeSinceLastFrame) {
    timer_ -= timeSinceLastFrame;
    agk::UpdateParticles(particleID_, 1.0f);
    agk::SetParticlesVisible(particleID_, 1 );
    
    // For moving particleSystems
    if (isMoving_) {
        setPositionParticleSystem(agk::GetSpriteX(spriteIDToFollow_)+xMovingOffset_, agk::GetSpriteY(spriteIDToFollow_)+yMovingOffset_);
    }
    
    // Stop emission here!
    
    // Deleting after certain time!
    if (timer_ < stopEmittingTimer_ && timer_ > -100) {
        agk::SetParticlesFrequency(particleID_, 0);
    }
    // only delete after 5 secs so particles dont randomly dissapear.
    if (timer_ < -5 && timer_ > -100) {
         // Delete system
        agk::DeleteParticles(particleID_);
        return false;
    }
    
    return true;
}

void ParticleSystem::setFollowSprite(int spriteID,float xOffset, float yOffset) {
    isMoving_ = true;
    spriteIDToFollow_ = spriteID;
    xMovingOffset_ = xOffset;
    yMovingOffset_ = yOffset;
}

void ParticleSystem::initParticleSystem(float x, float y) {
    isInit_ = true;
    particleID_ = agk::CreateParticles(x, y);
    // Default particle
    particleImage_ = 41;
//    setParticleImage(particleImage_);
    setParticleDirection(0.0f, -1.0f);
    setParticleLife(5.0f);
    setParticleAngle(180.0f);
    setParticleFrequency(10.0f);
    setVelocityRange(1.0f, 3.0f);
//    addColorFrames ( 0.0f, 0, 100, 255, 0 );
//    addColorFrames ( 0.5f, 0, 100, 255, 255 );
//    addColorFrames ( 19.0f, 150, 50, 100, 255 );
//    addColorFrames ( 20.0f, 0, 0, 0, 0 );
    agk::SetParticlesDepth(particleID_, 0);
    agk::SetParticlesSize(particleID_, 5.0f);
    agk::AddParticlesForce(particleID_, 10.0f, 30.0f, 0.0f, 1.0f);
    timer_ = 10.0f;
}

void ParticleSystem::addPhysicsForce(float startTime, float endTime, float xForce, float yForce) {
    agk::AddParticlesForce(particleID_, startTime, endTime, xForce, yForce);
}

void ParticleSystem::setParticleSize(float size) {
    agk::SetParticlesSize(particleID_, size);
}

void ParticleSystem::deleteParticleSystem() {
    agk::DeleteParticles(particleID_);
}

void ParticleSystem::setPositionParticleSystem(int x, int y) {
    agk::SetParticlesPosition(particleID_, x, y);
}
void ParticleSystem::setParticleEmissionZone(int x1, int y1,int x2, int y2) {
    agk::SetParticlesStartZone(particleID_, x1, y1, x2, y2);
}
void ParticleSystem::setParticleDirection(float x,float y) {
    agk::SetParticlesDirection(particleID_, x, y);
}
//
void ParticleSystem::setParticleLife(float time1) {
    agk::SetParticlesLife(particleID_, time1);
}
void ParticleSystem::setParticleAngle(float angle) {
    agk::SetParticlesAngle(particleID_, angle);
}
void ParticleSystem::setParticleFrequency(float particlePerSec) {
    agk::SetParticlesFrequency(particleID_,particlePerSec);
}
void ParticleSystem::setVelocityRange(float v1,float v2) {
    agk::SetParticlesVelocityRange(particleID_,v1,v2);
}
void ParticleSystem::addColorFrames(float timeChange, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) {
    agk::AddParticlesColorKeyFrame(particleID_,timeChange,red,green,blue,alpha);
}
