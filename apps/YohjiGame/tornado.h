//
//  tornado.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/16/18.
//
//

#ifndef tornado_h
#define tornado_h

#include <stdio.h>
#include "skills.h"
#include "player.h"

class TornadoSkill : public Skills{
    void castSkill(Player * p);
};

#endif /* tornado_hpp */
