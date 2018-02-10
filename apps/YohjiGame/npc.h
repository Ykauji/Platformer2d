//
//  npc.h
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/7/18.
//
//

#ifndef npc_h
#define npc_h

#include "UI.h"
#include <iostream>

class NPC {
  public:
    void textMessage();
    void openShop();
    
    
  private:
    int iD_;
    int npcID_;
    std::string text_;
    int shopNumber_;
    
    
};


#endif /* npc_h */
