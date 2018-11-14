//
//  particleSystem.hpp
//  agk_interpreter
//
//  Created by Yohji Kusakabe on 11/11/18.
//

#ifndef particleSystem_h
#define particleSystem_h

#include <stdio.h>
    // Create different particle system types? i.e emission, timed, etc. use this as base class.
class ParticleSystem {
  public:
    // Should I call init in constructor?
    ParticleSystem() {timer_ = -100; isMoving_ = false; xMovingOffset_ = 0; yMovingOffset_ = 0; stopEmittingTimer_ = 0;}
    void deleteParticleSystem();
    // Always call this first!
    void initParticleSystem(float x, float y);
    int getParticleImage() {return particleImage_;}
    int getParticleID() {return particleID_;}
    void setParticleImage(int iD);
    void setPositionParticleSystem(int x, int y);
    void setParticleEmissionZone(int x1, int y1,int x2,int y2);
    void setParticleDirection(float x,float y);
    //
    void setTimer(float timer) {timer_ = timer;}
    void setStopEmittingTimer(float timer) {stopEmittingTimer_ = timer;}
    void setParticleLife(float time1);
    void setParticleAngle(float angle);
    void setParticleFrequency(float particlePerSec);
    void setVelocityRange(float v1,float v2);
    void addColorFrames(float timeChange, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
    void addPhysicsForce(float startTime, float endTime, float xForce, float yForce);
    void setParticleSize(float size);
    
    void setFollowSprite(int spriteID,float xOffset, float yOffset);
    
    // Can change particle properties on runtime. Also counts down timer.
    bool update(float timeSinceLastFrame);
  private:
    int particleImage_;
    int particleID_;
    float timer_;
    float stopEmittingTimer_;
    bool isMoving_;
    int spriteIDToFollow_;
    int xMovingOffset_;
    int yMovingOffset_;
    bool isInit_ = false;
    
};

#endif /* particleSystem_hpp */
