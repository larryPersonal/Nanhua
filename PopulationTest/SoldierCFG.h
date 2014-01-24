//
//  WarriorCFG.h
//  PopulationTest
//
//  Created by Serious Games on 15/4/13.
//
//

#ifndef PopulationTest_SoldierCFG_h
#define PopulationTest_SoldierCFG_h

std::string soldierDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"200\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"0\""
"}";

std::string soldierRequirements =
"{\"name\":\"soldier\","
"\"required_loyalty\":\"80\","
"\"required_education_level\":\"1\","
"\"required_intelligence\":\"25\","
"\"required_social\":\"50\""
"}";



std::string soldierConfig =
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
"\"file_name\":\"soldierbehavior.lua\","
"\"func_name\":\"goWander\""
"}]"
"}]"
"}";

#endif
