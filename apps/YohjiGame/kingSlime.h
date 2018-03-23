//
//  kingSlime.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 3/3/18.
//
//

#ifndef kingSlime_h
#define kingSlime_h

#include "enemy.h"
#include "level.h"

class KingSlime : public Enemy {
  public:
    
    KingSlime(Level * mainLevel)
    : Enemy() {
        mainLevel_ = mainLevel;
    }
    
    void moveToPlayer(Player mainPlayer);
    void isDead(Player &mainPlayer,UI &userInterface);
    void initHealthBar();
    void enemySpecificHit();
    void kingSlimeHit();
    void setMainLevel(Level * mainLevel) {mainLevel_ = mainLevel;}
    
  private:
    Level * mainLevel_;

};

#endif /* kingSlime_hpp */
