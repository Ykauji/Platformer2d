//
//  tornado.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/16/18.
//
//

#include "tornado.h"
#include "agk.h"

void TornadoSkill::castSkill(Player * p) {
    int potato = agk::CreateSprite(27);
    agk::SetSpritePosition(potato, agk::GetSpriteX(p->getID()), agk::GetSpriteY(p->getID()));
    agk::CreatePhysicsForce(agk::GetSpriteX(p->getID()), agk::GetSpriteY(p->getID()), 100, 100, 500, 0);
}
