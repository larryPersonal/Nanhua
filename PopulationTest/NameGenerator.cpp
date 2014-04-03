//
//  NameGenerator.cpp
//  PopulationTest
//
//  Created by Serious Games on 31/7/13.
//
//

#include "NameGenerator.h"


std::vector< std::string > NameGenerator::mayanPrefix;
std::vector< std::string > NameGenerator::mayanStem;
std::vector< std::string > NameGenerator::mayanSuffix;
std::vector< std::string > NameGenerator::alienPrefix;
std::vector< std::string > NameGenerator::alienStem;
std::vector< std::string > NameGenerator::alienSuffix;

void NameGenerator::init()
{
    mayanPrefix.push_back("");
    mayanPrefix.push_back("co");
    mayanPrefix.push_back("ez");
    mayanPrefix.push_back("i");
    mayanPrefix.push_back("iz");
    mayanPrefix.push_back("mil");
    mayanPrefix.push_back("ne");
    mayanPrefix.push_back("te");
    mayanPrefix.push_back("t");
    mayanPrefix.push_back("xo");
    mayanPrefix.push_back("yo");
    mayanPrefix.push_back("zy");
    
    mayanStem.push_back("a");
    mayanStem.push_back("axo");
    mayanStem.push_back("hui");
    mayanStem.push_back("i");
    mayanStem.push_back("in");
    mayanStem.push_back("cal");
    mayanStem.push_back("pat");
    mayanStem.push_back("no");
    mayanStem.push_back("lal");
    mayanStem.push_back("tu");
    mayanStem.push_back("chi");
    mayanStem.push_back("lot");
    mayanStem.push_back("an");
    
    mayanSuffix.push_back("tl");
    mayanSuffix.push_back("ch");
    mayanSuffix.push_back("li");
    mayanSuffix.push_back("atl");
    mayanSuffix.push_back("zel");
    mayanSuffix.push_back("tica");
    mayanSuffix.push_back("pac");
    mayanSuffix.push_back("ya");
 
 
    alienPrefix.push_back("b'");
    alienPrefix.push_back("c'");
    alienPrefix.push_back("d'");
    alienPrefix.push_back("f'");
    alienPrefix.push_back("g'");
    alienPrefix.push_back("h'");
    alienPrefix.push_back("j'");
    alienPrefix.push_back("k'");
    alienPrefix.push_back("l'");
    alienPrefix.push_back("m'");
    alienPrefix.push_back("n'");
    alienPrefix.push_back("p'");
    alienPrefix.push_back("q'");
    alienPrefix.push_back("r'");
    alienPrefix.push_back("s'");
    alienPrefix.push_back("t'");
    alienPrefix.push_back("v'");
    alienPrefix.push_back("w'");
    alienPrefix.push_back("x'");
    alienPrefix.push_back("y'");
    alienPrefix.push_back("z'");

    alienStem.push_back("a");
    alienStem.push_back("e");
    alienStem.push_back("i");
    alienStem.push_back("o");
    alienStem.push_back("u");
    
    alienSuffix.push_back("b");
    alienSuffix.push_back("c");
    alienSuffix.push_back("d");
    alienSuffix.push_back("f");
    alienSuffix.push_back("g");
    alienSuffix.push_back("h");
    alienSuffix.push_back("j");
    alienSuffix.push_back("k");
    alienSuffix.push_back("l");
    alienSuffix.push_back("m");
    alienSuffix.push_back("n");
    alienSuffix.push_back("p");
    alienSuffix.push_back("q");
    alienSuffix.push_back("r");
    alienSuffix.push_back("s");
    alienSuffix.push_back("t");
    alienSuffix.push_back("v");
    alienSuffix.push_back("w");
    alienSuffix.push_back("x");
    alienSuffix.push_back("y");
    alienSuffix.push_back("z");
    alienSuffix.push_back("sh");
    alienSuffix.push_back("ch");
    alienSuffix.push_back("gh");
    alienSuffix.push_back("zh");
    alienSuffix.push_back("ix");
    alienSuffix.push_back("iy");
    alienSuffix.push_back("oj");
    alienSuffix.push_back("ex");
}

std::string NameGenerator::GenerateName(char race,
                                        char gender)
{
    std::string generatedName = "";
    if (race == 'a')
    {
     
        int randPrefix = rand()%alienPrefix.size();
        int randStem = rand()%alienStem.size();
        int randSuffix = rand()%alienSuffix.size();
        generatedName = alienPrefix[randPrefix] + alienStem[randStem] + alienSuffix[randSuffix];
        
    }
    else
    {
        int randPrefix = rand()%mayanPrefix.size();
        int randStem = rand()%mayanStem.size();
        int randSuffix = rand()%mayanSuffix.size();
        
        
        generatedName = mayanPrefix[randPrefix] + mayanStem[randStem] + mayanSuffix[randSuffix];
    }

    if (generatedName.size() > 0) generatedName[0] -=32;
    
    return generatedName;
}