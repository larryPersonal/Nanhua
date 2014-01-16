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
"{\"name\":\"root\","
"\"default_happiness\":\"50\","
"\"default_money\":\"200\","
"\"default_loyalty\":\"50\","
"\"default_education_level\":\"1\","
"\"default_move_range\":\"20\","
"\"default_intelligence\":\"50\","
"\"default_social\":\"50\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_energy\":\"100\","
"\"default_exp_level1\":\"50\","
"\"default_exp_level2\":\"60\","
"\"default_exp_level3\":\"70\","
"\"default_exp_level4\":\"85\","
"\"default_exp_level5\":\"100\","
"\"spawn_cost\":\"500\""
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
