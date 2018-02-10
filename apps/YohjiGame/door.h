//
//  door.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/10/18.
//
//

#ifndef door_h
#define door_h

class Door {
  public:
    void setiD(int iD) {iD_ = iD;}
    void setNext(int nextLevel) {nextLevel_ = nextLevel;}
    int getID() {return iD_;}
    int getNext() {return nextLevel_;}
    void setXY(int x, int y) {x_ = x; y_ = y;}
    
    
  private:
    int iD_;
    int nextLevel_;
    int x_;
    int y_;
};

#endif /* door_h */
