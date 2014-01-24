//
//  GameHUD.h
//  PopulationTest
//
//  Created by Serious Games on 8/3/13.
//
//

#ifndef PopulationTest_GameHUD_h
#define PopulationTest_GameHUD_h
#include "cocos2d.h"
#include "PopupMenu.h"
#include "InfoBar.h"
#include "ProgressBar.h"
#include "GameTimer.h"
#include "GameManager.h"

#include "ResearchIndicator.h"

using namespace cocos2d;

struct Date
{
    int year;
    int month;
    int week;
    int day;
    
    Date()
    {
        year = 0;
        month = 0;
        week = 0;
        day = 0;
    }
    
    void addDay()
    {
        day++;
        if(day >= 7)
        {
            day = 0;
            week++;
        }
        
        if(week >= 4)
        {
            week = 0;
            month++;
        }
        
        if(month >= 12)
        {
            month = 0;
            year++;
        }
    }
};

class GameHUD : public CCLayer
{
    static GameHUD* SP;

    int mGameDay;
    int mGameWeek;
    int mGameMonth;
    int mGameYear;
    
    CCMenuItemImage* menuButton;
    CCMenuItemImage* miscButton;
    
    typedef enum
    {
        Normal = 0,
        Build = 1,
        Deconstruct = 2,
        BuildPathPreview = 3,
        UnBuildPath = 4,
        BuildPathLine = 5
    }tapMode;
    
    typedef enum
    {
        None,
        OpenMenu,
        BuildingsMenu,
        ResearchMenu,
        PopulationMenu,
        PolicyMenu,
        InfoMenu,
        SystemMenu,
        NumberOfMenus
    }MenuMode;
    
    tapMode currTapMode;
    MenuMode currMenuMode;
    
    // Time label
    Date* date;
    CCLabelTTF* timeLabel;
    
    
    //InfoBar and items
    InfoBar* bottomInfoBar;
    InfoBar* hintInfoBar;
    
    CCLabelTTF* moneyLabel;
    CCLabelTTF* popTotalLabel;
    CCLabelTTF* avgHapLabel;
    CCLabelTTF* buildLabel;
    CCLabelTTF* buildCostLabel;
    
    CCLabelTTF* hintLabel;
    
    int hint_show_time;
    int curr_hint_show_time;

    int default_hint_font_size;
    
    CCMenu* pMenu;
    
    float cumulatedTime;
    
 public:
    ResearchIndicator* ri;
    
    bool menuIsOpen;
    GameTimer* gameTimer;
    
    GameHUD();
    ~GameHUD();
    static GameHUD* create();
    static GameHUD* getThis();
    
    CCLabelTTF* miscLabel;
    
    bool init();
    void createInitialGUI();
    void onMenuButtonPressed();
    void onMiscButtonPressed();
    void onOrientationChanged();
    void update(float deltaTime);
    
    int getTapMode();
    void setTapMode(int mode);
    int getMenuMode();
    void setMenuMode(int newMode);
    
    void createInfoBars();
    void closeAllMenuAndResetTapMode();
    void backToInGameMenu();
    
    void showBuildLabel(const char* buildingName);
    void hideBuildLabel();
    
    void buyBuilding(int cost);
    
    void onSpriteAddedToMap(GameSprite* sprite);
    void onSpriteRemovedFromMap(GameSprite* sprite);
    
    void updatePopTotalLabel();
    void updateAvgHapLabel();
    void updateMoneyLabel();
    void updateBuildCostLabel(int cost, int dist=-1);
    
    void showHint(std::string hintText);
    
    CCMenuItemImage* getMenuButton();
    
    Date* getDate();
    
};

#endif
