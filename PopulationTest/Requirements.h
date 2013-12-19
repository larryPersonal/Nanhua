//
//  Requirements.h
//  PopulationTest
//
//  Created by Serious Games on 17/5/13.
//
//

#ifndef PopulationTest_Requirements_h
#define PopulationTest_Requirements_h

struct Requirements: public CCObject
{
    std::string className;
    int required_loyalty;
    int required_social;
    int required_intelligence;
    int required_education_level;
    
    Requirements()
    {
        className = "";
        required_loyalty = 0;
        required_social = 0;
        required_intelligence = 0;
        required_education_level = 0;
    }
    
    
    bool hasMetRequirements(int loy, int soc, int intel, int edlvl, const char* prevClass)
    {
        
        return (required_education_level <= edlvl) &&
            (required_social <= soc) &&
            (required_intelligence <= intel) &&
        (required_loyalty <= loy) &&
         strcmp(className.c_str(), prevClass) != 0;
    
    
    }
    
    
};

#endif
