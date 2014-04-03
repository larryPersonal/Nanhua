//
//  NameGenerator.h
//  PopulationTest
//
//  Created by Serious Games on 31/7/13.
//
//

#ifndef __PopulationTest__NameGenerator__
#define __PopulationTest__NameGenerator__

#include <iostream>
#include <vector>

class NameGenerator
{

    static std::vector< std::string > mayanPrefix;
    static std::vector< std::string > mayanStem;
    static std::vector< std::string > mayanSuffix;
    
    static std::vector< std::string > alienPrefix;
    static std::vector< std::string > alienStem;
    static std::vector< std::string > alienSuffix;
    
public:
    static void init();
    
    
    static std::string GenerateName(char race, char gender);
    
    
};

#endif /* defined(__PopulationTest__NameGenerator__) */
