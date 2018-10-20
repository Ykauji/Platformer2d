//
//  melee.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/15/18.
//
//

#ifndef melee_h
#define melee_h

#include "player.h"
#include "skills.h"
#include "projectile.h"

class MeleeSkill : public Skills {
public:
    void castSkill(Player * p);
    // spell ID
    int spellID_;
};


#endif /* melee_h */
