//
//  particleManager.cpp
//  agk_interpreter
//
//  Created by Yohji Kusakabe on 11/13/18.
//

#include "particleManager.h"
#include <iostream>
#include <vector>

void ParticleManager::updateAll(float timeSinceLastFrame) {
    std::vector<int> deleteList;
    for (auto & pairs  : particleManagerMap_) {
        bool p = pairs.second.update(timeSinceLastFrame);
        if (p == false) {
            // Delete from map!
            deleteList.push_back(pairs.first);
        }
    }
    
    // Really delete from Map!
    for (auto it : deleteList) {
        particleManagerMap_.erase(it);
    }
}


void ParticleManager::addNewParticleSystem(ParticleSystem p) {
    if (particleManagerMap_.count(p.getParticleID()) >= 1) {
        std::cout << "particleSystem already exists in Map! Overwriting!!!" << std::endl;
    }
    particleManagerMap_[p.getParticleID()] = p;
}
