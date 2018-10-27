//
//  LevelMap.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 5/31/18.
//
//


// Creates MAP of nodes that when clicked on change user to that level. Map selection.

#ifndef LevelMap_h
#define LevelMap_h

#include <vector>
#include "agk.h"

class LevelMapButton {
  public:
    LevelMapButton(int x, int y, int imageID, int levelPointer) {iD_ = agk::CreateSprite(imageID); agk::SetSpritePosition(iD_, x, y); levelPointer_ = levelPointer;}
    
  private:
    int x_;
    int y_;
    int levelPointer_;
    int iD_;
    int imageID_;
};

class LevelMap {
  public:
    void addLevelButton();
    void clearLevelMap();
    
    
  private:
    // Maybe try implementing tree here for fun!
    std::vector<LevelMapButton> buttons_;
    
    

};



#endif /* LevelMap_h */
