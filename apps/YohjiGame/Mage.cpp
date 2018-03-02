//
//  Mage.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/14/18.
//
//

#include "Mage.h"
#include "agk.h"

void Mage::movementDashLeft() {
    if (dashTimer_ < 0) {
        agk::SetSpriteFlip(iD_, 1, 0);
        direction_ = -1;
        currentDash_ = 1;
        // Cooldown of Dash
        dashTimer_ = 8;
        agk::SetSpritePhysicsVelocity(iD_, 0, agk::GetSpritePhysicsVelocityY(iD_));
        agk::SetSpritePhysicsImpulse(iD_, agk::GetSpriteXByOffset(iD_), agk::GetSpriteYByOffset(iD_), speed_*-15.0, 0);
    }
}
void Mage::movementDashRight() {
    agk::SetSpritePosition(iD_, agk::GetSpriteX(iD_)+400, agk::GetSpriteY(iD_));
    movementRight();
}
