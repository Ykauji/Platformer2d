//
//  UI.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 8/25/17.
//
//

#ifndef UI_h
#define UI_h
#include <vector>


class UI {
public:
    void setHealthBarEmpty(int healthBarEmpty) {healthBarEmpty_ = healthBarEmpty;}
    int getHealthBarEmpty() {return healthBarEmpty_;}
    void setHealthBarGreen(int healthBarGreen) {healthBarGreen_ = healthBarGreen;}
    int getHealthBarGreen() {return healthBarGreen_;}
    void setexperienceBar(int experienceBar) {experienceBar_ = experienceBar;}
    int getexperienceBar() {return experienceBar_;}
    int getCursor() {return cursor_;}
    void setCursor(int cursor) {cursor_ = cursor;}
    void createFadingText(std::string text,int timer,int x, int y,int size);
    void createFadingText(std::string text,int timer,int x, int y,int size, int color);
    void fadeTimeUpdate(int decreaseValue);
    
    void hideUI();
    void showUI();
    
    // UI Init
    void initUI();
    
    // Load TextBox
    void loadTextBox(int xPos, int yPos, int xSize, int ySize, std::string text);

    
private:
    int healthBarEmpty_;
    int healthBarGreen_;
    int experienceBar_;
    int experienceBarBar_;
    int cursor_;
    struct fadingTextStruct {
        int timer_;
        int text_;
    };
    std::vector<fadingTextStruct> texts_;
    int textBoxTest_;
    
    
};


#endif /* UI_hpp */
