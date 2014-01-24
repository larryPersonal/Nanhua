//
//  GlobalSettings.h
//  PopulationTest
//
//  Created by admin on 21/1/14.
//
//

#ifndef PopulationTest_GlobalSettings_h
#define PopulationTest_GlobalSettings_h


class GlobalSettings
{
public:
    static const float hungry_decay = 2.0f; // for each second, the hungry will decay for 0.5
    static const float find_food_hungry_treshold = 20.0f; // 20.0f is the default value;
    
    // time setttings
    static const float time_for_one_day = 3.0f; // which means every 3 sconds will be one day in game.
};

#endif
