//
//  fireballskill.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/10/18.
//
//

#ifndef fireballskill_h
#define fireballskill_h

#include <stdio.h>
#include "skills.h"
#include "fireball.h"

class Fireballskill : public Skills {
public:
    void castSkill(Player * p);
    // spell ID
    int spellID_;
};


#endif /* fireballskill_hpp */
