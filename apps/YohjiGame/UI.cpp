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
    agk::SetSpritePosition(getHealthBarEmpty(), 785 , 965);
    agk::SetSpriteSize(getHealthBarEmpty(),400,20); // Original 400,20
    agk::SetSpriteSize(getHealthBarEmpty(),400,22.5);
    agk::FixSpriteToScreen(getHealthBarEmpty(), 1);
    agk::SetSpriteDepth(getHealthBarEmpty(), 2);
    
    // Green HealthBar
    setHealthBarGreen(agk::CreateSprite(16));
    agk::SetSpritePosition(getHealthBarGreen() , 791 , 969);
    agk::SetSpriteSize(getHealthBarGreen(),350,15); // Original 350,15
    agk::SetSpriteSize(getHealthBarGreen(),190,14); // 395 max
    agk::FixSpriteToScreen(getHealthBarGreen(), 1);
    agk::SetSpriteDepth(getHealthBarGreen(), 2);
    
    // ExperienceBar Empty
    experienceBarBar_ = agk::CreateSprite(15);
    agk::SetSpritePosition(experienceBarBar_, 785, 992);
    agk::SetSpriteSize(experienceBarBar_,400,12);
    agk::FixSpriteToScreen(experienceBarBar_, 1);
    agk::SetSpriteDepth(experienceBarBar_, 2);
    
    // ExperienceBar
    setexperienceBar(agk::CreateSprite(18));
    agk::SetSpritePosition(getexperienceBar(), 790, 995);
    agk::SetSpriteSize(getexperienceBar(),350,5);
    agk::FixSpriteToScreen(getexperienceBar(), 1);
    agk::SetSpriteDepth(getexperienceBar(), 2);
    
    // Level Text add to UI later..
    agk::CreateText(112, "Lv. 1");
    agk::SetTextSize(112, 30);
    agk::SetTextPosition(112, 0, 0);
    agk::FixTextToScreen(112, 1);
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
        default:
            break;
    }
    texts_.push_back(fadingText);
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
