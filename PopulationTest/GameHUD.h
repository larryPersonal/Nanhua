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
#include "GameManager.h"
#include "BuildScroll.h"

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

enum LabelStatus
{
    Default = 0, InMoneyLabel, InFoodLabel, InPopulationLabel = 3
};

class GameHUD : public CCLayer
{
    static GameHUD* SP;

    int mGameDay;
    int mGameWeek;
    int mGameMonth;
    int mGameYear;
    
    int mGameReputation;
    int mGameReputationMax;
    
    int mGameCurrentCitizenPopulation;
    int mGameCurrentPopulationRoom;
    
    float mAverageHappiness;
    
    int mGameCurrentFood;
    int mGameCurrentStorage;
    
    int mGameMoney;
    
    typedef enum
    {
        Normal = 0,
        Build = 1,
        Deconstruct = 2,
        BuildPathPreview = 3,
        UnBuildPath = 4,
        BuildPathLine = 5
    }tapMode;
    
    tapMode currTapMode;
    tapMode mGameTapMode;
    
    CCMenu* pMenu;
    
    float cumulatedTime;
    
public:
    LabelStatus labelStatus;
    bool isToggle;
    CCPoint newPos;
    float targetOpacity;
    float fadeSpeed;
    bool label_fade_in;
    bool label_fade_out;
    
    float leftPos;
    float maxPos;
    
    bool scroll_in;
    bool scroll_out;
    
    bool fade_in;
    bool fade_out;
    
    bool scrolled_in;
    
    bool setTutorial;
    
    BuildScroll* buildScroll;
    
    bool getMoney;
    int money;
    int reputation;
    int reputationMax;
    int growthPopulation;
    
    bool isThisTapCounted;
    
    float average_happiness;
    CCLabelTTF* average_happiness_label;
    
    bool is_token_drop_cooldown;
    float token_drop_cooldown_time;
    
    // tap mode label;
    CCLabelTTF* tapModeLabel;
    
    // stats menu group
    CCSprite* statsMenu;
    CCSprite* moneyIcon;
    CCSprite* foodIcon;
    CCSprite* populationIcon;
    CCLabelTTF* moneyLabel;
    CCLabelTTF* foodLabel;
    CCLabelTTF* populationLabel;
    CCLabelTTF* achivementsLabel;
    
    // Time label
    Date* date;
    CCLabelTTF* timeLabel;
    CCLabelTTF* timeLabel_1;
    CCLabelTTF* timeLabel_2;
    
    // time menu group
    CCSprite* timeMenu;
    CCSprite* timeBackground;
    CCSprite* firstWeekLabel;
    CCSprite* secondWeekLabel;
    CCSprite* thirdWeekLabel;
    CCSprite* lastWeekLabel;
    
    // objective menu group
    CCSprite* objectiveMenu;
    CCSprite* objectiveButton;
    
    CCLabelTTF* objectiveTitle;
    CCArray* objectiveStrs;
    
    // build menu group
    CCSprite* buildButton;
    
    // system menu group
    CCSprite* systemButton;
    
    bool pause;
    CCSprite* pauseButton;
    CCSprite* resumeButton;
    
    int originalHappiness;
    bool stickHappiness;
    CCSprite* stickHappinessButton;
    CCSprite* resumeHappinessButton;
    
    bool startWar;
    CCSprite* peaceButton;
    CCSprite* warButton;
    
    CCSprite* infoBackground;
    
    CCSprite* mask;
    
    // soldier helper
    CCLabelTTF* soldierName;
    CCLabelTTF* enermyName;
    CCLabelTTF* stopActionLabel;
    
public:
    
    GameHUD();
    ~GameHUD();
    static GameHUD* create();
    static GameHUD* getThis();
    
    CCLabelTTF* miscLabel;
    
    bool init();
    void createInitialGUI();
    void onOrientationChanged();
    void update(float deltaTime);
    
    int getTapMode();
    void setTapMode(int mode);
    
    void closeAllMenuAndResetTapMode();
    
    void buyBuilding(int cost);
    
    void onSpriteAddedToMap(GameSprite* sprite);
    void onSpriteRemovedFromMap(GameSprite* sprite);
    
    void updatePopTotalLabel();
    void updateAvgHapLabel();
    void updateMoneyLabel();
    void updateBuildCostLabel(int cost, int dist=-1);
    
    void showHint(std::string hintText);
    
    Date* getDate();
    
    // jerry added
    void createStatsMenu();
    void createTimeMenu();
    void createObjectiveMenu();
    void createBuildMenu();
    void createSystemMenu();
    
    void createSoldierHelper();
    void updateSoldierHelper(float dt);
    
    void clickObjectiveButton();
    void clickBuildButton();
    void clickSystemButton();
    
    void clickMoneyLabel();
    void clickFoodLabel();
    void clickPopulationLabel();
    
    void pauseGame();
    void stickGameHappiness();
    void banditsAttack();
    
    void addReputation(int);
    void addPopulation();
    
    void setAllStats();
    
    void timeGroupReposition();
    void scrollIn(float);
    void scrollOut(float);
    void scheduleScrollIn();
    void scheduleScrollOut();
    
    void fadeIn(float);
    void fadeOut(float);
    
    void UpdateBuildButton();
    void labelBackgroundFade(float);
};

#endif
