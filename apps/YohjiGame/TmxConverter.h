//
//  TmxConverter.hpp
//  agkinterpreter
//
//  Created by Yohji Kusakabe on 2/27/18.
//
//

// Only works with one layer, can change using depth later if necessary. Tile Layer. ANY OTHER INPUT WILL FAIL.
// Change data structure to hash table later for O(1) search.


#ifndef TmxConverter_h
#define TmxConverter_h
#include <iostream>
#include <vector>
#include <map>

class tileSet {
public:
    
    friend bool operator== ( const tileSet &n1, const int &n2)
    {
        return n1.iD_ == n2;
    }
    friend bool operator== ( const int &n1, const tileSet &n2)
    {
        return n1 == n2.iD_;
    }
    
    
    int tileWidth_;
    int tileHeight_;
    int iD_;
    std::string fileName_;
};


class TmxConverter {
public:
    void parseTmx(std::string fileName, int line);
    void parseTileSetTmx(std::string fileName);
    // Call after parseTmx.
    void printTmxFile();
    void findParametersFromArray();
    void printTileSet();
    void parseTmx2dArray(std::string fileName);
    void printTilePlacements();
    
    int getWidth() {return width;}
    int getHeight() {return height;}
    int getTileWidth() {return tileWidth;}
    int getTileHeight() {return tileHeight;}
    std::vector< std::vector<int> > getTilePlacements() {return tilePlacements_;}
    std::map<int,tileSet> getTileSet() {return tmxTileSet;}
    
    int binarySearch(std::vector<tileSet> arr, int l, int r, int x)
    {
        if (r >= l)
        {
            int mid = l + (r - l)/2;
            
            // If the element is present at the middle
            // itself
            if (arr[mid].iD_ == x)
                return mid;
            
            // If element is smaller than mid, then
            // it can only be present in left subarray
            if (arr[mid].iD_ > x)
                return binarySearch(arr, l, mid-1, x);
            
            // Else the element can only be present
            // in right subarray
            return binarySearch(arr, mid+1, r, x);
        }
        
        // We reach here when element is not
        // present in array
        return -1;
    }

    
private:
    // When I made this, I was very tired pls no judge later fix with getline( , '"')
    std::string lineOneArr_[10];
    std::string lineTwoArr_[10];
    std::string lineThreeArr_[10];
    std::string lineFourArr_[10];
    std::string lineFiveArr_[10];
    std::string returnArr_[10];
    
    // Change this to map!!!! O(1) search vs O(N)
//    std::vector<tileSet> tmxTileSet;
    std::map<int,tileSet> tmxTileSet;
    
    std::vector< std::vector<int> > tilePlacements_;
    
    int stringToInt(std::string str);
    
    
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    
    std::string tileSetName;
    
    // Placeholder values, can for loop width and height to not waste resource. Maybe change to dynamic array.
    // int map[5000][5000];
    
    
};


#endif /* TmxConverter_h */
