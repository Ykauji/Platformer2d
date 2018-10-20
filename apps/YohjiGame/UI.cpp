//
//  UI.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#include "UI.h"
#include "agk.h"
#include <iostream>

void UI::hideUI() {
    agk::SetSpriteVisible(healthBarEmpty_, 0);
    agk::SetSpriteVisible(healthBarGreen_, 0);
    agk::SetSpriteVisible(experienceBar_, 0);
    agk::SetSpriteVisible(experienceBarBar_, 0);
}
void UI::showUI() {
    agk::SetSpriteVisible(healthBarEmpty_, 1);
    agk::SetSpriteVisible(healthBarGreen_, 1);
    agk::SetSpriteVisible(experienceBar_, 1);
    agk::SetSpriteVisible(experienceBarBar_, 1);
}

void UI::initUI() {
    // Empty Bar
    setHealthBarEmpty(agk::CreateSprite(15));
    agk::SetSpritePosition(getHealthBarEmpty(), 785 , 10);
    agk::SetSpriteSize(getHealthBarEmpty(),400,22.5);
    agk::FixSpriteToScreen(getHealthBarEmpty(), 1);
    agk::SetSpriteDepth(getHealthBarEmpty(), 2);
    
    // Green HealthBar
    setHealthBarGreen(agk::CreateSprite(16));
    agk::SetSpritePosition(getHealthBarGreen() , agk::GetSpriteX(healthBarEmpty_)+6 , agk::GetSpriteY(healthBarEmpty_)+4);
    agk::SetSpriteSize(getHealthBarGreen(),395,agk::GetSpriteHeight(healthBarEmpty_-8.5)); // 14 max
    agk::FixSpriteToScreen(getHealthBarGreen(), 1);
    agk::SetSpriteDepth(getHealthBarGreen(), 2);
    
    // ExperienceBar Empty
    experienceBarBar_ = agk::CreateSprite(15);
    agk::SetSpriteSize(experienceBarBar_,1920,agk::GetSpriteHeight(healthBarEmpty_)-9.5);
    agk::SetSpritePosition(experienceBarBar_, 0, 1080-agk::GetSpriteHeight(experienceBarBar_));
    agk::FixSpriteToScreen(experienceBarBar_, 1);
    agk::SetSpriteDepth(experienceBarBar_, 2);
    
    // ExperienceBar
    setexperienceBar(agk::CreateSprite(18));
    agk::SetSpritePosition(getexperienceBar(), 30,agk::GetSpriteY(experienceBarBar_)+3);
    agk::SetSpriteSize(getexperienceBar(),agk::GetSpriteWidth(experienceBarBar_)-10);
    agk::FixSpriteToScreen(getexperienceBar(), 1);
    agk::SetSpriteDepth(getexperienceBar(), 2);
    
    // Set up skill boxes!
    createSkillBox(1,agk::GetSpriteX(healthBarEmpty_)-550, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_1.png","ui/KeyboardSprites/Dark/Keyboard_Black_1.png");
    createSkillBox(2,agk::GetSpriteX(healthBarEmpty_)-400, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_2.png","ui/KeyboardSprites/Dark/Keyboard_Black_2.png");
    createSkillBox(3,agk::GetSpriteX(healthBarEmpty_)-250, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_3.png","ui/KeyboardSprites/Dark/Keyboard_Black_3.png");
    
    createSkillBox(4,agk::GetSpriteX(healthBarEmpty_)-100, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_4.png","ui/KeyboardSprites/Dark/Keyboard_Black_4.png");
    createSkillBox(5,agk::GetSpriteX(healthBarEmpty_)+50, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_5.png","ui/KeyboardSprites/Dark/Keyboard_Black_5.png");
    createSkillBox(6,agk::GetSpriteX(healthBarEmpty_)+200, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_6.png","ui/KeyboardSprites/Dark/Keyboard_Black_6.png");
    createSkillBox(7,agk::GetSpriteX(healthBarEmpty_)+350, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_7.png","ui/KeyboardSprites/Dark/Keyboard_Black_7.png");
    createSkillBox(8,agk::GetSpriteX(healthBarEmpty_)+500, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_8.png","ui/KeyboardSprites/Dark/Keyboard_Black_8.png");
    createSkillBox(9,agk::GetSpriteX(healthBarEmpty_)+650, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_9.png","ui/KeyboardSprites/Dark/Keyboard_Black_9.png");
    createSkillBox(0,agk::GetSpriteX(healthBarEmpty_)+800, 945, 95,"ui/KeyboardSprites/Light/Keyboard_White_0.png","ui/KeyboardSprites/Dark/Keyboard_Black_10.png");
    
    changeSkillIcon(1, "ui/skill_icons/fireball-red-1.png");
    changeSkillIcon(2, "ui/skill_icons/enchant-jade-1.png");
    changeSkillIcon(3, "ui/skill_icons/wind-blue-3.png");
    changeSkillIcon(4, "ui/skill_icons/fire-arrows-3.png");
    changeSkillIcon(5, "ui/skill_icons/runes-blue-3.png");
    changeSkillIcon(6, "ui/skill_icons/runes-orange-3.png");
    changeSkillIcon(7, "ui/skill_icons/explosion-red-3.png");

    // Level Text add to UI later..
    agk::CreateText(112, "Lv. 1");
    agk::SetTextSize(112, 30);
    agk::SetTextPosition(112, 0, 0);
    agk::FixTextToScreen(112, 1);
}
// black_border is 800x800, so scale icons in respect to that.
void UI::createSkillBox(int boxNum, int xPos,int yPos,int size,std::string keySpriteFilePath,std::string keySpriteDownFilePath) {
    skillBox newBox;
    newBox.skillBox_ = agk::CreateSprite(agk::LoadImage("ui/black_border.png"));
    agk::SetSpritePosition(newBox.skillBox_, xPos, yPos);
    agk::SetSpriteSize(newBox.skillBox_, size);
    agk::FixSpriteToScreen(newBox.skillBox_, 1);
    agk::SetSpriteDepth(newBox.skillBox_, 0);
    // Icon!
    int imageIDIcon = agk::LoadImage("ui/skill_icons/fireball-red-1.png");
    newBox.skillIcon_ = agk::CreateSprite(imageIDIcon);
    agk::SetSpritePosition(newBox.skillIcon_, xPos+size/6, yPos+size/6);
    agk::FixSpriteToScreen(newBox.skillIcon_, 1);
    agk::SetSpriteSize(newBox.skillIcon_,agk::GetSpriteWidth(newBox.skillBox_)/1.4,agk::GetSpriteHeight(newBox.skillBox_)/1.4);
    agk::SetSpriteDepth(newBox.skillIcon_, 2);
    agk::AddSpriteAnimationFrame(newBox.skillIcon_, imageIDIcon);
    agk::AddSpriteAnimationFrame(newBox.skillIcon_,agk::LoadImage("ui/skill_icons/black_icon.png"));
    
    // Keys!
    int imageIDKey = agk::LoadImage(keySpriteFilePath.c_str());
    newBox.keySprite_ = agk::CreateSprite(imageIDKey);
    agk::SetSpriteSize(newBox.keySprite_,size/2);
    agk::SetSpritePosition(newBox.keySprite_, agk::GetSpriteWidth(newBox.skillBox_)/2 - agk::GetSpriteWidth(newBox.keySprite_)/2 + xPos,agk::GetSpriteHeight(newBox.skillBox_) - agk::GetSpriteHeight(newBox.keySprite_)/2 + yPos);
    agk::FixSpriteToScreen(newBox.keySprite_, 1);
    agk::SetSpriteDepth(newBox.keySprite_, 0);
    agk::AddSpriteAnimationFrame(newBox.keySprite_, imageIDKey);
    agk::AddSpriteAnimationFrame(newBox.keySprite_, agk::LoadImage(keySpriteDownFilePath.c_str()));
    agk::SetSpriteFrame(newBox.keySprite_, 1);
    
    skillBoxMap[boxNum] = newBox;
    
}

void UI::changeSkillIcon(int boxNumber, std::string skillIconPath) {
    int oldIcon = skillBoxMap[boxNumber].skillIcon_;
    int imageIDIcon = agk::LoadImage(skillIconPath.c_str());
    skillBoxMap[boxNumber].skillIcon_ = agk::CreateSprite(imageIDIcon);
    agk::AddSpriteAnimationFrame(skillBoxMap[boxNumber].skillIcon_,agk::LoadImage("ui/skill_icons/black_icon.png"));
    agk::AddSpriteAnimationFrame(skillBoxMap[boxNumber].skillIcon_, imageIDIcon);
    agk::SetSpritePosition(skillBoxMap[boxNumber].skillIcon_, agk::GetSpriteX(oldIcon), agk::GetSpriteY(oldIcon));
    agk::SetSpriteSize(skillBoxMap[boxNumber].skillIcon_,agk::GetSpriteWidth(oldIcon),agk::GetSpriteHeight(oldIcon));
    agk::FixSpriteToScreen(skillBoxMap[boxNumber].skillIcon_, 1);
    agk::SetSpriteDepth(skillBoxMap[boxNumber].skillIcon_, 2);

    agk::DeleteSprite(oldIcon);
}

void UI::keyPressed(int boxNum) {
    if (agk::GetSpriteCurrentFrame(skillBoxMap[boxNum].keySprite_) == 1) {
        agk::SetSpriteFrame(skillBoxMap[boxNum].keySprite_, 2);
    }
}

void UI::keyReleased(int boxNum) {
    if (agk::GetSpriteCurrentFrame(skillBoxMap[boxNum].keySprite_) == 2) {
        agk::SetSpriteFrame(skillBoxMap[boxNum].keySprite_, 1);
    }
}

void UI::createFadingText(std::string text,int timer,int x, int y, int size) {
    fadingTextStruct fadingText;
    fadingText.timer_ = timer;
    fadingText.text_ = agk::CreateText(text.c_str());
    agk::SetTextPosition(fadingText.text_, x, y);
    agk::SetTextSize(fadingText.text_, size);
    texts_.push_back(fadingText);
}

void UI::createFadingText(std::string text,int timer,int x, int y, int size, int color) {
    fadingTextStruct fadingText;
    fadingText.timer_ = timer;
    fadingText.text_ = agk::CreateText(text.c_str());
    agk::SetTextPosition(fadingText.text_, x, y);
    agk::SetTextSize(fadingText.text_, size);
    switch (color) {
        case 0:
            // Black
            agk::SetTextColor(fadingText.text_, 0, 0, 0);
            break;
        case 1:
            // White
            agk::SetTextColor(fadingText.text_, 255, 255, 255);
            break;
        case 2:
            // Pale Yellow
            agk::SetTextColor(fadingText.text_, 255, 204, 153);
            break;
        case 3:
            // Darkish Blue
            agk::SetTextColor(fadingText.text_, 0, 51, 102);
            break;
        case 4:
            // Light Pink
            agk::SetTextColor(fadingText.text_, 255, 204, 229);
            break;
        case 5:
            // Red
            agk::SetTextColor(fadingText.text_, 204, 0,0);
        
        default:
            break;
    }
    texts_.push_back(fadingText);
}

void UI::displayMessage(int messageID,int xPos,int yPos, int xSize, int ySize, std::string message,float fontSize) {
    // pushes new id into
    if (messages_.count(messageID) == 1) {
        std::cout << "message already exists. returning!" << std::endl;
        return;
    }
    messageBox newMessage;
    newMessage.iD_ = messageID;
    newMessage.backgroundSpriteID_ = agk::CreateSprite(agk::LoadImage("ui/ui_board_big_background.png"));
    newMessage.spriteID_ = agk::CreateSprite(agk::LoadImage("ui/ui_board_big.png"));
    newMessage.textID_ = agk::CreateText(message.c_str());
    
    messages_[messageID] = newMessage;
    
    agk::SetSpritePosition(newMessage.backgroundSpriteID_, xPos-xSize/50, yPos-ySize/50);
    agk::SetSpriteSize(newMessage.backgroundSpriteID_,xSize+xSize/25,ySize+ySize/25);
    agk::FixSpriteToScreen(newMessage.backgroundSpriteID_, 1);

    
    agk::SetSpritePosition(newMessage.spriteID_, xPos, yPos);
    agk::SetSpriteSize(newMessage.spriteID_,xSize,ySize);
    agk::FixSpriteToScreen(newMessage.spriteID_, 1);
    
    agk::SetTextPosition(newMessage.textID_, xPos+50, yPos+20);
    agk::SetTextSize(newMessage.textID_,fontSize);
    agk::SetTextColor(newMessage.textID_, 0, 0, 0);
    agk::FixTextToScreen(newMessage.textID_, 1);
    
}

void UI::deleteMessage(int messageID) {
    messageBox deleteMsg = messages_[messageID];
    agk::DeleteSprite(deleteMsg.backgroundSpriteID_);
    agk::DeleteSprite(deleteMsg.spriteID_);
    agk::DeleteText(deleteMsg.textID_);
}

void UI::updateIcon(int boxNum,std::pair<double,double> cooldowns) {
    if (cooldowns.first <= 0) {
        agk::SetSpriteFrame(skillBoxMap[boxNum].skillIcon_, 2);
    } else {
        agk::SetSpriteFrame(skillBoxMap[boxNum].skillIcon_, 1);
    }
}

void UI::fadeTimeUpdate(int decreaseValue) {
    for (int i = 0; i < texts_.size(); i++) {
        texts_[i].timer_ = texts_[i].timer_ - 1;
        agk::SetTextPosition(texts_[i].text_, agk::GetTextX(texts_[i].text_), agk::GetTextY(texts_[i].text_)-decreaseValue);
        if (texts_[i].timer_ <= 0) {
            agk::DeleteText(texts_[i].text_);
            std::vector<fadingTextStruct>::iterator it = texts_.begin();
            for (int j = 0; j < i; j++) {
                it++;
            }
            texts_.erase(it);
        }
    }
}
