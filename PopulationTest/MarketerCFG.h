//
//  MarketerCFG.h
//  PopulationTest
//
//  Created by admin on 4/9/14.
//
//

#ifndef PopulationTest_MarketerCFG_h
#define PopulationTest_MarketerCFG_h

std::string marketerDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"0\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"100\","
"\"default_starting_endurance\":\"100\","
"\"default_attacking_cooldown_time\":\"3.2\","
"\"default_attack_power_min\":\"0\","
"\"default_attack_power_max\":\"0\""
"}";

std::string marketerConfig =
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
"\"file_name\":\"marketerbehavior.lua\","
"\"func_name\":\"goWander\""
"}]"
"}]"
"}";

#endif
