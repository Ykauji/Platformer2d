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
