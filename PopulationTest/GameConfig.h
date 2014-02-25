//
//  GameConfig.h
//  PopulationTest
//
//  Created by admin on 17/2/14.
//
//

#ifndef PopulationTest_GameConfig_h
#define PopulationTest_GameConfig_h

#include <iostream>

/* contains the config settings for the whole game */
struct ConfigSettings
{
    // related to sprite
    float default_hapiness;
    float default_hapiness_limit;
    float default_loyalty;
    float default_loyalty_limit;
    float default_current_hungry;
    float default_current_energy;
    float default_work_rate;
    
    // related to town hall
    int default_population_cap_level1;
    int default_population_cap_level2;
    int default_population_cap_level3;
    
    // related to reputation
    int default_ini_reputation;
    
    // related to the time
    int secondToDayRatio;
    
    ConfigSettings()
    {
        // related to sprite
        default_hapiness = 50;
        default_hapiness_limit = 100;
        default_loyalty = 50;
        default_loyalty_limit = 100;
        default_current_hungry = 100;
        default_current_energy = 100;
        default_work_rate = 100;
        
        // related to town hall
        default_population_cap_level1 =10;
        default_population_cap_level2 = 15;
        default_population_cap_level3 = 20;
        
        // related to reputation
        default_ini_reputation = 0;
        
        // related to the time
        secondToDayRatio = 3; // x means x seconds for a day in the game, 7x seconds for a week and 28x seconds for a month;
    }
    
    ~ConfigSettings()
    {
    }
    
};

struct SettingsLevel0
{
    int default_start_money;
    int default_max_reputation;
    int total_population_growth;
    
    // hungry decay:
    float resting_hungry_decay;
    float idle_hungry_decay;
    float refugee_hungry_decay;
    float worker_hungry_decay;
    
    // happiness decay:
    float hungry_happiness_happy_decay;
    float hungry_happiness_normal_decay;
    float hungry_happiness_unhappy_decay;
    float hungry_happiness_angry_decay;
    float quit_happiness;
    
    SettingsLevel0()
    {
        default_start_money = 200;
        default_max_reputation = 400;
        total_population_growth = 50;
        
        resting_hungry_decay = 0.1;
        idle_hungry_decay = 0.3;
        refugee_hungry_decay = 0.5;
        worker_hungry_decay = 1;
        
        hungry_happiness_happy_decay = 5;
        hungry_happiness_normal_decay = 2;
        hungry_happiness_unhappy_decay = 1;
        hungry_happiness_angry_decay = 0.1;
        quit_happiness = 0.01;
    }
};

#endif
