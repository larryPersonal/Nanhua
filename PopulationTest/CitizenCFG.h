//
//  CitizenCFG.h
//  PopulationTest
//
//  Created by Serious Games on 11/4/13.
//
//

#ifndef PopulationTest_CitizenCFG_h
#define PopulationTest_CitizenCFG_h

std::string citizenDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"0\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"0\","
"\"default_starting_endurance\":\"100\""
"}";

std::string citizenConfig =
"{\"name\":\"root\","
    "\"type\":\"sequence\","
    "\"items\":"
    "[{"
            "\"name\":\"Template\","
            "\"type\":\"selector\","
            "\"items\":"
            "[{"
                "\"name\":\"Walk Around\","
                "\"type\":\"behaviour\","
                "\"file_name\":\"citizenbehavior.lua\","
                "\"func_name\":\"goWander\""
            "}]"
    "}]"
"}";

#endif
