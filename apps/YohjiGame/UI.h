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
#include <map>


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
    void displayMessage(int messageID,int xPos, int yPos, int xSize, int ySize, std::string mesage,float fontSize);
    void deleteMessage(int messageID);
    void hideUI();
    void showUI();
    
    
    // UI Init
    void initUI();
    
    // Load TextBox
    void loadTextBox(int xPos, int yPos, int xSize, int ySize, std::string text);
    
    // Skills
    void createSkillBox(int boxNum, int xPos,int yPos,int size,std::string keySpriteFilePath,std::string keySpriteDownFilePath);
    void changeSkillIcon(int boxNumber, std::string skillIconPath);
    void updateIcon(int boxNum,std::pair<double,double> coolDown);
    void keyPressed(int boxNum);
    void keyReleased(int boxNum);
    
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
    
    struct skillBox {
        int skillID_;
        int skillIcon_;
        int skillBox_;
        int keySprite_;
    };
    // maps skill ID to the correct box.
    std::map<int,skillBox> skillBoxMap;
    
    struct messageBox {
        int spriteID_;
        int backgroundSpriteID_;
        int textID_;
        int iD_;
    };
    std::map<int,messageBox> messages_;
    int textBoxTest_;
    
    
};


#endif /* UI_hpp */
