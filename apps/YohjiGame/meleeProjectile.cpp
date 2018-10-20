//
//  meleeProjectile.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/17/18.
//
//

#include "meleeProjectile.h"
#include "agk.h"

void MeleeProjectile::updateBullet() {
    if (getDirection() == 1) {
            agk::SetSpritePosition(getID_(),((agk::GetSpriteX(userID_)+agk::GetSpriteWidth(userID_)/2)+50), agk::GetSpriteY(userID_)+agk::GetSpriteHeight(userID_)/3);
    } else if (getDirection() == -1) {
        agk::SetSpritePosition(getID_(),agk::GetSpriteX(userID_)-(agk::GetSpriteWidth(userID_)/2-80), agk::GetSpriteY(userID_)+agk::GetSpriteHeight(userID_)/3);
    } else {
        std::cout << "direction = 0." << std::endl;
    }
}
