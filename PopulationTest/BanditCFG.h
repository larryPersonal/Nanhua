//
//  bandit.h
//  PopulationTest
//
//  Created by Serious Games on 17/6/13.
//
//

#ifndef PopulationTest_bandit_h
#define PopulationTest_bandit_h


std::string banditDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"30\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","   // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"200\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"0\","
"\"default_starting_endurance\":\"1000\","
"\"default_attacking_cooldown_time\":\"2.4\","
"\"default_attack_power_min\":\"48\","
"\"default_attack_power_max\":\"72\""
"}";

std::string banditConfig =
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
"\"file_name\":\"banditbehavior.lua\","
"\"func_name\":\"goWander\""
"}]"
"}]"
"}";

#endif