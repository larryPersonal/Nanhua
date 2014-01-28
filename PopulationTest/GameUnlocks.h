//
//  GameUnlocks.h
//  PopulationTest
//
//  Created by Serious Games on 23/5/13.
//
//

#ifndef PopulationTest_GameUnlocks_h
#define PopulationTest_GameUnlocks_h

/*note: GameSprite unlocks are in ChiefCFG, CitizenCFG, FarmerCFG, WarriorCFG, ResearcherCFG*/
/*Building unlocks are defined inside the TMX file*/

std::string Unlocks
= "{\"name\":\"unlock\","
    "\"items\":"
    "[{"
        "\"unlock_name\":\"area_default\","
        "\"unlock_prereq\":\"\","
        "\"unlock_min_x\":5,"
        "\"unlock_max_x\":80,"
        "\"unlock_min_y\":5,"
        "\"unlock_max_y\":50,"
        "\"unlock_bcount\":25,"
        "\"unlock_popn\":25,"
        "\"unlock_cash\":25,"
        "\"research_time\":10,"
        "\"research_cost\":0"
    "},"
     "{"
        "\"unlock_name\":\"area_1\","
        "\"unlock_prereq\":\"\","
        "\"unlock_min_x\":1,"
        "\"unlock_max_x\":24,"
        "\"unlock_min_y\":5,"
        "\"unlock_max_y\":30,"
        "\"unlock_bcount\":50,"
        "\"unlock_popn\":50,"
        "\"unlock_cash\":5000,"
        "\"research_time\":50,"
        "\"research_cost\":5000"

    "},"
    "{"
        "\"unlock_name\":\"area_max\","
        "\"unlock_prereq\":\"\","
        "\"unlock_min_x\":1,"
        "\"unlock_max_x\":39,"
        "\"unlock_min_y\":1,"
        "\"unlock_max_y\":39,"
        "\"unlock_bcount\":75,"
        "\"unlock_popn\":75,"
        "\"unlock_cash\":7500,"
        "\"research_time\":100,"
        "\"research_cost\":7500"
     "},"
    "{"
        "\"unlock_name\":\"menu_research\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":0,"
        "\"unlock_popn\":0,"
        "\"unlock_cash\":0"
    "},"
    "{"
        "\"unlock_name\":\"menu_policy\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":0,"
        "\"unlock_popn\":0,"
        "\"unlock_cash\":0"

    "},"

    "{"
        "\"unlock_name\":\"menu_birthrate\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":20,"
        "\"unlock_popn\":15,"
        "\"unlock_cash\":0"

    "},"

    "{"
        "\"unlock_name\":\"menu_aliencitizen\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":50,"
        "\"unlock_popn\":50,"
        "\"unlock_cash\":0"

    "},"

    "{"
        "\"unlock_name\":\"menu_alienresearcher\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":0,"
        "\"unlock_popn\":0,"
        "\"unlock_cash\":0"

    "},"

    "{"
        "\"unlock_name\":\"menu_culturalex\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":50,"
        "\"unlock_popn\":50,"
        "\"unlock_cash\":0"

    "},"

    "{"
        "\"unlock_name\":\"menu_taxrate\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":10,"
        "\"unlock_popn\":10,"
        "\"unlock_cash\":0"

    "},"
    "{"
        "\"unlock_name\":\"menu_alienpoplimit\","
        "\"unlock_prereq\":\"\","
        "\"unlock_bcount\":20,"
        "\"unlock_popn\":20,"
        "\"unlock_cash\":0"

    "}]"
"}";


#endif