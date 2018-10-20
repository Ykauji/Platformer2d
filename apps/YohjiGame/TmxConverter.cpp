//
//  TmxConverter.cpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/27/18.
//
//

#include "TmxConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>


int TmxConverter::stringToInt(std::string str) {
    std::stringstream ss(str);
    int intHolder;
    ss >> intHolder;
    return intHolder;
}

void TmxConverter::parseTileSetTmx(std::string fileName) {
    std::ifstream text;
    const char * c = fileName.c_str();
    text.open(c);
    if (!text.is_open()) {
        std::cout << "failed";
    }
    std::string tempStr;
    for (int i = 0; i < 2; i++) {
        std::getline(text,tempStr);
    }
    while (std::getline(text,tempStr)) {
        std::getline(text,tempStr,'"'); // Gets up to the first quotation not including it.
        tileSet newTile;
        // <tile id="172">
        std::getline(text,tempStr,'"');
        int tempInt = stringToInt(tempStr);
        newTile.iD_ = tempInt;
        // Remove beginning of next line
        std::getline(text,tempStr,'"');
        // Finds Image Width
        std::getline(text,tempStr,'"');
        newTile.tileWidth_ = stringToInt(tempStr);
        
        std::getline(text,tempStr,'"');
        std::getline(text,tempStr,'"');
        newTile.tileHeight_ = stringToInt(tempStr);
        
        std::getline(text,tempStr,'"');
        std::getline(text,tempStr,'"');
        
        newTile.fileName_ = "media/" + tempStr;
        
        tmxTileSet[tempInt] = newTile;
    }
//    tmxTileSet.pop_back();
    
}

void TmxConverter::parseTmx(std::string fileName, int line) {
    if (line == 6) {
        parseTmx2dArray(fileName);
        return;
    }
    std::ifstream text;
    const char * c = fileName.c_str();
    text.open(c);
    if (!text.is_open()) {
        std::cout << "failed";
    }
    std::string tempStr;
    std::string tempStr2;
    // Parse first line
    for (int i = 0; i < line; i++) {
        std::getline(text,tempStr);
    }
    //Tile Width and Tile Height
    std::string width;
    std::string height;
    std::string potato;
    // <map version="1.0" tiledversion="1.0.2" orientation="orthogonal" renderorder="right-down" width="100" height="20" tilewidth="32" tileheight="32" nextobjectid="1">
    int counter = 0;
    int size = 0;
    std::stack<char> quoteCount;
    std::string returnArr_[10];
    while (tempStr[counter] != '>') {
        if (tempStr[counter] == '"' && quoteCount.empty()) {
            size++;
            quoteCount.push('"');
            counter++;
            while (tempStr[counter] != '"') {
                tempStr2 = tempStr2 + tempStr[counter];
                counter++;
            }
            tempStr2 = tempStr2 + " ";
            quoteCount.pop();
        } else {
            // Do nothing
        }
        counter++;
    }
    
    std::stringstream tempStr3(tempStr2);
    for (int i = 0; i < size; i++) {
        tempStr3 >> returnArr_[i];
    }
    switch (line) {
        case 1:
            for (int i = 0; i < size; i++) {
                lineOneArr_[i] = returnArr_[i];
            }
            break;
        case 2:
            for (int i = 0; i < size; i++) {
                lineTwoArr_[i] = returnArr_[i];
            }
            break;
        case 3:
            for (int i = 0; i < size; i++) {
                lineThreeArr_[i] = returnArr_[i];
            }
            break;
        case 4:
            for (int i = 0; i < size; i++) {
                lineFourArr_[i] = returnArr_[i];
            }
            break;
        case 5:
            for (int i = 0; i < size; i++) {
                lineFiveArr_[i] = returnArr_[i];
            }
            break;
    }
    
}

void TmxConverter::printTmxFile() {
    for (int i = 0; i < 2; i++) {
        std::cout << lineOneArr_[i] << std::endl;
    }
    for (int i = 0; i < 8; i++) {
        std::cout << lineTwoArr_[i] << std::endl;
    }
    for (int i = 0; i < 2; i++) {
        std::cout << lineThreeArr_[i] << std::endl;
    }
    for (int i = 0; i < 2; i++) {
        std::cout << lineFourArr_[i] << std::endl;
    }
    std::cout << lineFiveArr_[0] << std::endl;
}

void TmxConverter::printTilePlacements() {
    for (int i = 0; i < tilePlacements_.size(); i++) {
        for (int j = 0; j < tilePlacements_[i].size(); j++) {
            std::cout << tilePlacements_[i][j];
        }
    }
}

void TmxConverter::parseTmx2dArray(std::string fileName) {
    
    std::ifstream text;
    const char * c = fileName.c_str();
    text.open(c);
    std::string tempStr;
    for (int i = 0; i < 5; i++) {
        std::getline(text,tempStr);
    }
    
    for (int y = 0; y < height; y++) {
        std::vector<int> x2dArray;
        for (int x = 0; x < width; x++) {
            std::getline(text,tempStr,',');
            int intTemp = stringToInt(tempStr);
            x2dArray.push_back(intTemp);
        }
        tilePlacements_.push_back(x2dArray);
    }
}


void TmxConverter::findParametersFromArray() {
    width = stringToInt(lineTwoArr_[4]);
    height = stringToInt(lineTwoArr_[5]);
    tileWidth = stringToInt(lineTwoArr_[6]);
    tileHeight = stringToInt(lineTwoArr_[7]);
}

void TmxConverter::printTileSet() {
    std::cout << tmxTileSet[0].tileWidth_ << " " << tmxTileSet[0].tileHeight_ << " " << std::endl;
    std::cout << tmxTileSet[0].fileName_ << " ";
}


//int main(int argc, char **argv) {
//    TmxConverter Test;
//    for (int i = 1; i < 5; i++) {
//        Test.parseTmx(argv[1],i);
//    }
//    Test.findParametersFromArray();
//    Test.parseTileSetTmx(argv[1]);
//    // Test.printTileSet();
//    // Test.printTmxFile();
//    
//}
