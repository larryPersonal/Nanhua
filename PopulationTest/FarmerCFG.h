//
//  FarmerCFG.h
//  PopulationTest
//
//  Created by Serious Games on 15/4/13.
//
//

#ifndef PopulationTest_FarmerCFG_h
#define PopulationTest_FarmerCFG_h

std::string farmerDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"1000\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"100\""
"}";

std::string farmerRequirements =
"{\"name\":\"farmer\","
"\"required_loyalty\":\"50\","
"\"required_education_level\":\"1\","
"\"required_intelligence\":\"50\","
"\"required_social\":\"50\""
"}";



std::string farmerConfig =
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
"\"file_name\":\"farmerbehavior.lua\","
"\"func_name\":\"goWander\""
"}]"
"}]"
"}";

#endif
