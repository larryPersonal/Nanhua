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

struct SystemConfig
{
    bool skipSenario;
    bool skipTutorial;
    
    bool skipSenario_tutorial;
    bool skipTutorial_tutorial;
    
    bool skipSenario_level1;
    bool skipTutorial_level1;
    
    bool skipSenario_level2;
    bool skipTutorial_level2;
    
    bool hideSkipButton;
    bool debugMode;
    
    SystemConfig()
    {
        skipSenario_tutorial = false;
        skipTutorial_tutorial = false;
        
        skipSenario_level1 = true;
        skipTutorial_level1 = true;
        
        skipSenario_level2 = false;
        skipTutorial_level2 = false;
        
        hideSkipButton = true;
        debugMode = false;
    }
};

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
    float default_energy_recovery;
    float default_homeless_happiness_drop_multiplier;
    
    // related to town hall
    int default_population_cap_level1;
    int default_population_cap_level2;
    int default_population_cap_level3;
    
    // related to reputation
    int default_ini_reputation;
    
    // related to the time
    int secondToDayRatio;
    
    // token drop
    int token_drop_treshold_time_veryHappy_min;
    int token_drop_treshold_time_veryHappy_max;
    int token_drop_treshold_time_happy_min;
    int token_drop_treshold_time_happy_max;
    int token_drop_treshold_time_normal_min;
    int token_drop_treshold_time_normal_max;
    int token_drop_treshold_time_unhappy_min;
    int token_drop_treshold_time_unhappy_max;
    int token_drop_treshold_time_angry_min;
    int token_drop_treshold_time_angry_max;
    
    int token_drop_rate_veryHappy;
    int token_drop_rate_happy;
    int token_drop_rate_normal;
    int token_drop_rate_unhappy;
    int token_drop_rate_angry;
    
    float token_disappear_time;
    
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
        default_energy_recovery = 10;   // 10 energy per second
        default_homeless_happiness_drop_multiplier = 2;
        
        // related to town hall
        default_population_cap_level1 =10;
        default_population_cap_level2 = 15;
        default_population_cap_level3 = 20;
        
        // related to reputation
        default_ini_reputation = 0;
        
        // related to the time
        secondToDayRatio = 5; // x means x seconds for a day in the game, 7x seconds for a week and 28x seconds for a month;
        
        // token drop treshold time, x seconds per checking
        token_drop_treshold_time_veryHappy_min = 15;
        token_drop_treshold_time_veryHappy_max = 25;
        token_drop_treshold_time_happy_min = 25;
        token_drop_treshold_time_happy_max = 40;
        token_drop_treshold_time_normal_min = 40;
        token_drop_treshold_time_normal_max = 60;
        token_drop_treshold_time_unhappy_min = 55;
        token_drop_treshold_time_unhappy_max = 80;
        token_drop_treshold_time_angry_min = 100;
        token_drop_treshold_time_angry_max = 200;
        
        token_drop_rate_veryHappy = 50; // 60 by default
        token_drop_rate_happy = 40;     // 45 by default
        token_drop_rate_normal = 30;    // 30 by default
        token_drop_rate_unhappy = 20;   // 15 by default
        token_drop_rate_angry = 10;      // 0  by default
        
        token_disappear_time = 15;
    }
};

struct SettingsLevel
{
    int default_start_money;
    int default_max_reputation;
    int projected_population_growth;
    
    // hungry decay:
    float global_hungry_decay;
    
    // happiness decay:
    float hungry_happiness_veryHappy_decay;
    float hungry_happiness_happy_decay;
    float hungry_happiness_normal_decay;
    float hungry_happiness_unhappy_decay;
    float hungry_happiness_angry_decay;
    float quit_happiness;
    
    // energy decay:
    float energy_decay_rate;
    float energy_decay_coefficient;
    
    // bandit maximum rob amount
    int max_money_rob;
    int max_food_rob;
    
    SettingsLevel()
    {
        default_start_money = 0;
        default_max_reputation = 0;
        projected_population_growth = 0;
        
        global_hungry_decay = 0;
        
        hungry_happiness_veryHappy_decay = 0;
        hungry_happiness_happy_decay = 0;
        hungry_happiness_normal_decay = 0;
        hungry_happiness_unhappy_decay = 0;
        hungry_happiness_angry_decay = 0;
        quit_happiness = 0;
        
        energy_decay_rate = 0;
        energy_decay_coefficient = 0;
        
        max_money_rob = 0;
        max_food_rob = 0;
    }
    
    void setLevel0()
    {
        default_start_money = 1200;
        default_max_reputation = 400;
        projected_population_growth = 20;
        
        global_hungry_decay = 60; // decay 100 hungry in 60 seconds.
        
        hungry_happiness_veryHappy_decay = 1;
        hungry_happiness_happy_decay = 0.75;
        hungry_happiness_normal_decay = 0.5;
        hungry_happiness_unhappy_decay = 0.25;
        hungry_happiness_angry_decay = 0.125;
        quit_happiness = 0.01;
        
        energy_decay_rate = 0.5; // decay 1 of the energy in one real second.
        energy_decay_coefficient = 1.0;
        
        max_money_rob = 200;
        max_food_rob = 200;
    }
};

#endif
