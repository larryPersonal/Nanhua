//
//  InfoBar.h
//  PopulationTest
//
//  Created by Ong Heng Le on 15/4/13.
//
//

#ifndef PopulationTest_InfoBar_h
#define PopulationTest_InfoBar_h

#include "cocos2d.h"
using namespace cocos2d;

#include <map.h>
#include <string.h>
using namespace std;

/* SUMMARY_
 *
 * Main purpose of this InfoBar is to show an anchored information bar 
 * with repeating POT background texture (both x/y-axis compatible).
 * Adding items to this InfoBar will keep them anchored relatively,
 * centered on the y-axis.
 *
 * Add your CCNode (or derivations of CCNode) using addItem(), with a <string> key.
 * Use the key to identify the specific node in this InfoBar.
 * All nodes are kept in a <key, value> dictionary.
 *
 *
 *
 * Construction_
 *
 * Create an InfoBar object, and set its [left, top, right, bottom] variables,
 * as anchored offsets (similar to CSS attributes).
 * Then call createInfoBar() with the size and background texture.
 * Add this InfoBar to the parent CCLayer.
 *
 *
 *
 * Adding_Nodes_
 *
 * It is recommended to keep a pointer to the CCNode before addItem(),
 * so that it is easier to locate and modify the CCNode, 
 * rather than using getItem(key), which returns <CCNode*>.
 *
 * addItem() increments the z-order of newer added nodes,
 * so they are drawn above older added nodes.
 *
 * Use anchorItem(key) to anchor the added node,
 * with offsetX and whether it is to be anchored on the right side.
 *
 *
 *
 * Modifying_Nodes_
 *
 * If you have kept a pointer to the added node, modify it as usual.
 * You may want to call repositionItem(key) after, if the content size has changed.
 *
 */
enum InfoBarAnchor { ANCHOR_LEFT = 0, ANCHOR_CENTER, ANCHOR_RIGHT };

class InfoBar:public cocos2d::CCLayer
{
    
private:
    
    // Stored for own use
    CCSize initialSize;
    CCSize spriteSize;
    
    // InfoBar background
    CCSprite* background;
    CCSize backgroundTexSize;
    
    // Struct to hold InfoBar properties for each node
    struct InfoBarItem
    {
        CCNode* item = NULL;
        float offsetX = 0.0f;
        InfoBarAnchor anchor = ANCHOR_LEFT;
        
        InfoBarItem() {}
        InfoBarItem(CCNode* item)
        {
            this->item = item;
        }
    };
    
    // Dictionary of items in this info bar
    map<string, InfoBarItem> itemDictionary;
    
public:
    
    static InfoBar* create();
    
    // Constructor / Destructor
    InfoBar();
    ~InfoBar();
    
    // InfoBar functions
    void createInfoBar(float width, float height, const char* bgFileName);
    void reposition();
    void onOrientationChanged();
    
    // InfoBar attributes
    CCPoint position;
    CCSize size;
    float left, top, right, bottom; // CSS-like anchors, set to negative for no anchor
    
    // Item functions
    void addItem(string key, CCNode* item);
    void removeItem(string key);
    void anchorItem(string key, float offsetX, InfoBarAnchor anchor=ANCHOR_LEFT);
    void repositionItem(string key);
    CCNode* getItem(string key);
    
    void removeAllItems();
    void repositionAllItems();
};

#endif
