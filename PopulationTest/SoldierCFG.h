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
"{\"name\":\"root\","
"\"default_happiness\":\"50\","
"\"default_money\":\"300\","
"\"default_loyalty\":\"50\","
"\"default_education_level\":\"6\","
"\"default_move_range\":\"20\","
"\"default_intelligence\":\"50\","
"\"default_social\":\"50\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"100\","  // basic default value is 50 for mayan game
"\"default_energy\":\"100\","
"\"default_exp_level1\":\"150\","
"\"default_exp_level2\":\"180\","
"\"default_exp_level3\":\"240\","
"\"default_exp_level4\":\"300\","
"\"default_exp_level5\":\"0\","
"\"spawn_cost\":\"500\""
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
"\"file_name\":\"citizenbehavior.lua\","
"\"func_name\":\"Wander\""
"}]"
"}]"
"}";

#endif
