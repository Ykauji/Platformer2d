//
//  ParticleManager.hpp
//  agk_interpreter
//
//  Created by Yohji Kusakabe on 11/13/18.
//

#ifndef particleManager_h
#define particleManager_h

#include <stdio.h>
#include <map>
#include "particleSystem.h"

// Simpleton design b/c small game!
class ParticleManager {
public:
    static ParticleManager& getInstance()
    {
        static ParticleManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ParticleManager(ParticleManager const&) = delete;
    void operator=(ParticleManager const&)  = delete;
    
    void addNewParticleSystem(ParticleSystem p);
    void updateAll(float timeSinceLastFrame);
    
private:
    ParticleManager() {}
    std::map<int,ParticleSystem> particleManagerMap_;
};

#endif /* particleManager_h */
