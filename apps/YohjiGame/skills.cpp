//
//  skills.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 10/10/18.
//
//

#include "skills.h"

std::pair<double,double> Skills::updateCooldown(double frameTime) {
    currentCooldown_ -= frameTime;
    // First = current, Second = percent max.
    std::pair<double,double> cooldowns(currentCooldown_,currentCooldown_/cooldown_);
    return cooldowns;
}
