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
    float default_hapiness;
    float default_hapiness_limit;
    
    float default_money;
    
    float default_loyalty;
    float default_loyalty_limit;
    
    float default_current_hungry;
    float default_hungry_limit;
    
    float default_current_energy;
    float default_energy_limit;
    
    float default_work_rate;
    
    
    ConfigSettings()
    {
        default_hapiness = 50;
        default_hapiness_limit = 100;
        default_money = 200;
        default_loyalty = 50;
        default_loyalty_limit = 100;
        default_current_hungry = 100;
        default_current_hungry = 100;
        default_current_energy = 100;
        default_energy_limit = 100;
        default_work_rate = 100;
    }
    
    ~ConfigSettings()
    {
    }
    
};

#endif
