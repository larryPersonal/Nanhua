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
"{\"name\":\"root\","
"\"default_happiness\":\"50\","
"\"default_money\":\"200\","
"\"default_loyalty\":\"50\","
"\"default_education_level\":\"1\","
"\"default_move_range\":\"20\","
"\"default_intelligence\":\"50\","
"\"default_social\":\"50\","
"\"default_energy\":\"100\","
"\"default_exp_level1\":\"100\","
"\"default_exp_level2\":\"120\","
"\"default_exp_level3\":\"140\","
"\"default_exp_level4\":\"160\","
"\"default_exp_level5\":\"0\","
"\"spawn_cost\":\"500\""
"}";

std::string citizenRequirements =
"{\"name\":\"citizen\","
"\"required_loyalty\":\"50\","
"\"required_education_level\":\"1\","
"\"required_intelligence\":\"50\","
"\"required_social\":\"50\""
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
                "\"func_name\":\"Wander\""
            "}]"
    "}]"
"}";

#endif
