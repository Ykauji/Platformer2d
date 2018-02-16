//
//  camera.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/16/18.
//
//

#ifndef camera_h
#define camera_h

#include <stdio.h>

#endif /* camera_h */

// ------------------------------------
// 2D Camera for AGK T1 1088
// Version 1.0
// Stuart Tresadern
// ------------------------------------


// --------------------------------------------
// Point Type
// --------------------------------------------
class Pointo {
public:
    float x;
    float y;
};



// --------------------------------------------
// Camera 2D Type
// --------------------------------------------
class Camera {
  public:
    
    Pointo trapPosition;
    Pointo trapSize;
    Pointo lookAhead;
    Pointo limitMax;
    Pointo limitMin;
    Pointo currentLookAhead;
    Pointo position;
    Pointo offset;
    float damping;
    int debug;
    int limitSprite;
};

class Camera2D {
    
public:
    void Camera2DInit(float offsetX,float offsetY,float damping,int limitSprite);
    void Camera2DSet(int objectId,int bottomX,int bottomY,int topX,int topY);
    void Camera2DFollow(int objectId);
    float Camera2DMove(int X,int Y,int objectID,float position,float size);

    float Camera2DLimit(float position,float min, float max);
    Camera Camera1;
};
