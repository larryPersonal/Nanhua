//
//  TutorialHandler.h
//  PopulationTest
//
//  Created by Serious Games on 4/6/13.
//
//

#ifndef __PopulationTest__TutorialHandler__
#define __PopulationTest__TutorialHandler__

#include <iostream>
#include <vector>
#include <json/json.h> //for the area unlock section

struct TutorialPiece
{
    TutorialPiece();
    
    bool isComplete;
    
    void PerformAction(std::string& action, std::string& targetType, std::string& targetName); //for prepare and complete
    void ReportAction(std::string& action, std::string& targetType, std::string& targetName); //for update.
    
    
    /*PREPARE performs a standard action. Performs the action only once.*/
    std::string prepareAction;
    std::string prepareTargetType; //building or sprite, OR messagebox or hint.
    std::string prepareTargetContent; //building or sprite TYPE, or message content.
    void prepare();
    
    /*UPDATE waits for an action to be performed, rather than performs the action.*/
    std::string updateWaitAction;
    std::string updateTargetType; //building or sprite, OR messagebox or hint.
    std::string updateTargetContent; //building or sprite TYPE. //ignored for message boxes.
    bool correctActionReported;
    void update();

    std::string completeAction;
    std::string completeType; //building or sprite, OR messagebox or hint.
    std::string completeContent; //building or sprite TYPE, or message content
    void complete();
    

};


class TutorialHandler
{
    bool isActive;
    
    std::vector<TutorialPiece> tutorialSequence;
    int currSequenceIndex;
    
    Json::Value root;
    Json::Reader reader;
    
    static TutorialHandler* SP;
public:
    
    
    TutorialHandler();
    ~TutorialHandler();
    
    static TutorialHandler* getThis();
    
    void BeginTutorial();
    
    
    void SetActive(bool active);
    bool IsActive();
    
    
    void ReportAction(std::string action, std::string type, std::string name);
    void ShowMessage(std::string& message);
    
    
    void update();
    
    int getSequenceIndex();
    void setSequenceIndex(int idx);
    
};


#endif /* defined(__PopulationTest__TutorialHandler__) */
