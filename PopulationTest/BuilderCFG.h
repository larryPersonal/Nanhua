//
//  builderCFG.h
//  PopulationTest
//
//  Created by Serious Games on 15/4/13.
//
//

#ifndef PopulationTest_builderCFG_h
#define PopulationTest_builderCFG_h

std::string builderDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"0\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"100\""
"}";

std::string builderRequirements =
"{\"name\":\"builder\","
"\"required_loyalty\":\"50\","
"\"required_education_level\":\"1\","
"\"required_intelligence\":\"50\","
"\"required_social\":\"50\""
"}";



std::string builderConfig =
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
"\"file_name\":\"builderbehavior.lua\","
"\"func_name\":\"goWander\""
"}]"
"}]"
"}";

#endif
