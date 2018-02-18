//
//  camera.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/16/18.
//
//

#include "camera.h"
#include "agk.h"


void Camera2D::Camera2DInit(float offsetX,float offsetY,float damping,int limitSprite) {
    Camera1.offset.x = offsetX;
    Camera1.offset.y = offsetY;
    Camera1.damping = damping;
    Camera1.limitSprite = limitSprite;
    
    Camera1.trapSize.x = 100; // Width of the camera trap
    Camera1.trapSize.y = 200;	// Hight of the Camera trap
    Camera1.lookAhead.x = 0.0;	// lookahead x ,y allow addition space in the moving direction, //normally used on small mobile screens
    Camera1.lookAhead.y = 0.0;
}


void Camera2D::Camera2DSet(int objectId,int bottomX,int bottomY,int topX,int topY) {
    Camera1.position.x = agk::GetSpriteX(objectId) - Camera1.offset.x;
    Camera1.position.y = agk::GetSpriteY(objectId) - Camera1.offset.y;
    Camera1.trapPosition.x = agk::GetSpriteX(objectId) - Camera1.trapSize.x / 2;
    Camera1.trapPosition.y = agk::GetSpriteY(objectId) - Camera1.trapSize.y;
    
    Camera1.limitMin.x = bottomX;
    Camera1.limitMin.y = bottomY;
    Camera1.limitMax.x = topX - agk::GetVirtualWidth();
    Camera1.limitMax.y = topY - agk::GetVirtualHeight();
}


void Camera2D::Camera2DFollow(int objectId) {
    
    Camera1.position.x = Camera2DMove(0,0,objectId,agk::GetSpriteX(objectId),agk::GetSpriteWidth(objectId));
    Camera1.position.y = Camera2DMove(1,1,objectId,agk::GetSpriteY(objectId),agk::GetSpriteHeight(objectId));

    agk::SetViewOffset( Camera1.position.x, Camera1.position.y);
    
//    If Camera1.debug = 1
//
//    Camera2DDebug()
    
}



float Camera2D::Camera2DMove(int X, int Y,int objectId,float position,float size) {
    
    float returnValue;
    float lookAhead;
    lookAhead = 0.0;
    
    if (X == 0 | Y == 0) {
        if (Camera1.limitSprite == 1) {
            if (position < Camera1.limitMin.x) {
                position = Camera1.limitMin.x;
                agk::SetSpriteX(objectId,position);
            } else if (position > Camera1.limitMax.x - size) {
                position = Camera1.limitMax.x - size;
                agk::SetSpriteX(objectId,position);
            }
        }
        if (position < Camera1.trapPosition.x) {
            Camera1.trapPosition.x = position;
            Camera1.currentLookAhead.x = Camera1.lookAhead.x;
        } else if ((position + size) > ( Camera1.trapPosition.x + Camera1.trapSize.x )) {
            Camera1.trapPosition.x =  (position + size)  - Camera1.trapSize.x;
            Camera1.currentLookAhead.x = -(Camera1.lookAhead.x);
        }
        returnValue =(Camera1.position.x - (Camera1.position.x - Camera1.trapPosition.x + Camera1.offset.x + Camera1.currentLookAhead.x) * agk::GetFrameTime() * Camera1.damping);
        returnValue = Camera2DLimit(returnValue,Camera1.limitMin.x,Camera1.limitMax.x);

    } else {
        if (Camera1.limitSprite == 1) {
            if (position < Camera1.limitMin.y) {
                position = Camera1.limitMin.y;
                agk::SetSpriteY(objectId,position);
            } else if (position > Camera1.limitMax.y - size) {
                position = Camera1.limitMax.y - size;
                agk::SetSpriteY(objectId,position);
            }
        }
        if (position < Camera1.trapPosition.y) {
            Camera1.trapPosition.y = position;
            Camera1.currentLookAhead.y = Camera1.lookAhead.y;
        } else if ((position + size) > ( Camera1.trapPosition.y + Camera1.trapSize.y)) {
            Camera1.trapPosition.y = ( position + size ) - Camera1.trapSize.y;
            Camera1.currentLookAhead.y = -(Camera1.lookAhead.y);
        }
        returnValue = (Camera1.position.y - ( Camera1.position.y - Camera1.trapPosition.y + Camera1.offset.y + Camera1.currentLookAhead.y) * agk::GetFrameTime() * Camera1.damping);
        returnValue = Camera2DLimit(returnValue,Camera1.limitMin.y,Camera1.limitMax.y);
    }
    return returnValue;
}

float Camera2D::Camera2DLimit(float position,float min, float max) {
    float rtnValue;
    if (position < min) {
        rtnValue = min;
    } else if (position > max) {
        rtnValue = max;
    } else {
        rtnValue = position;
    }
    return rtnValue;
}

