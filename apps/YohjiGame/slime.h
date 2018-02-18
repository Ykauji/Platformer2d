//
//  slime.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//

#ifndef slime_h
#define slime_h

#include "enemy.h"

class Slime : public Enemy {
public:
    
    void moveToPlayer(Player mainPlayer);
    void isDead(Player &mainPlayer,UI &userInterface);
    void initHealthBar();
    void enemySpecificHit();
    
private:
    
};


#endif /* slime_h */
