//
//  bat.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/26/17.
//
//potato

#ifndef bat_h
#define bat_h

#include "enemy.h"

class Bat : public Enemy {
public:
    
//    Bat() : Enemy() {
//    angryMode = 0; enrageHealth_ = 50;
//    }
    
    ~Bat();
    void attack() {}
    void isHit(Projectile bullet, UI &userInterface);
    void moveToPlayer(Player mainPlayer);
    void isDead(Player &mainPlayer);
    void setEnrage(int enrageHealth) {enrageHealth_ = enrageHealth;}
    int getEnrage() {return enrageHealth_;}
    
private:
    int yDirection;
    int angryMode;
    int enrageHealth_;
    int isAttacking_;
    
    
    
};

















#endif /* bat_h */
