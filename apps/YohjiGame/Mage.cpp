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
    agk::SetSpritePosition(iD_, agk::GetSpriteX(iD_)-400, agk::GetSpriteY(iD_));
}
void Mage::movementDashRight() {
    agk::SetSpritePosition(iD_, agk::GetSpriteX(iD_)+400, agk::GetSpriteY(iD_));
}
