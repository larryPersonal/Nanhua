//
//  refugeeCFG.h
//  PopulationTest
//
//  Created by Serious Games on 15/4/13.
//
//

#ifndef PopulationTest_refugeeCFG_h
#define PopulationTest_refugeeCFG_h

std::string refugeeDefaults =
"{\"default_movement_range\":\"20\","
"\"default_movement_speed\":\"50\","  // basic default value is 50 for mayan game
"\"default_animate_speed\":\"50\","   // basic default value is 50 for mayan game
"\"default_hungry_limit\":\"100\","
"\"default_food_carriage_limit\":\"0\","
"\"default_energy_limit\":\"100\","
"\"default_work_unit_per_day\":\"0\","
"\"default_starting_endurance\":\"100\","
"\"default_attacking_cooldown_time\":\"3.2\","
"\"default_attack_power_min\":\"0\","
"\"default_attack_power_max\":\"0\""
"}";

std::string refugeeConfig =
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
"\"file_name\":\"refugeebehavior.lua\","
"\"func_name\":\"goWander\""
"}]"
"}]"
"}";

#endif
