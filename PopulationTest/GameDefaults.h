//
//  GameDefaults.h\.h
//  PopulationTest
//
//  Created by Serious Games on 10/6/13.
//
//

/*defaults that don't belong in the TMX, inidividual sprites*/
#ifndef PopulationTest_GameDefaults_h__h
#define PopulationTest_GameDefaults_h__h

//xcode bug with tracking static vars declared outside of a class. ALL of these are used once during game initialization.
//warnings suppresssed.

//If testmode is true, all functionality is unlocked from game start if the tutorial isn't active.
//note that buildings will be unlocked to research rather than build. 
static bool testMode = false;
#pragma unused(testMode)


static int starting_capital = 9000;
#pragma unused(starting_capital)

static int path_cost_per_tile = 10;
#pragma unused(path_cost_per_tile)

//happiness mods
static int happiness_mod_unsuitable_job = -5;
#pragma unused(happiness_mod_unsuitable_job)

static int happiness_mod_overcrowded_conditions = -5; //per person
#pragma unused(happiness_mod_overcrowded_conditions)

static int happiness_mod_per_alien_in_same_house = -5; //per alien. Social is supposed to deal with this. Aliens are NOT affected by this statistic.
#pragma unused(happiness_mod_per_alien_in_same_house)

static int happiness_mod_jobless = -5;
#pragma unused(happiness_mod_jobless)

static int happiness_mod_homeless = -5;
#pragma unused(happiness_mod_homeless)

static int happiness_level_before_loyalty_affected = 30;
#pragma unused(happiness_level_before_loyalty_affected)

static int loyalty_mod_if_below_happiness_threshold = -5;
#pragma unused(loyalty_mod_if_below_happiness_threshold)


//Policy stuff
/*import alien costs*/
static int import_alien_citizen_cost = 100;
#pragma unused(import_alien_citizen_cost)
static int import_alien_researcher_cost = 1000;
#pragma unused(import_alien_researcher_cost)

/*alien population limit*/
static int default_alien_population_allowed = 10;
#pragma unused(default_alien_population_allowed)

static int cost_per_1_alien_limit_adj = 500;
#pragma unused(cost_per_1_alien_limit_adj)

/*tax rates*/
static int max_tax_rate_percent = 100;
#pragma unused(max_tax_rate_percent)

static int min_tax_rate_percent = 0;
#pragma unused(min_tax_rate_percent)

static int default_tax_rate_per_building = 30;
#pragma unused(default_tax_rate_per_building)

static int cost_per_tax_rate_percent = 10;
#pragma unused(cost_per_tax_rate_percent)
/*tax happiness mods*/


/*birth rates*/
static int default_birth_rate = 1;
#pragma unused(default_birth_rate)

static int default_birth_rate_days = 28;
#pragma unused(default_birth_rate_days)

//So it's like 1 every 28 days etc.

static int max_birth_rate = 10;
#pragma unused(max_birth_rate)

static int min_birth_rate = 0;
#pragma unused(min_birth_rate)

static int min_birth_policy_duration_months = 1;
#pragma unused(min_birth_policy_duration_months)

static int max_birth_policy_duration_months = 12;
#pragma unused(max_birth_policy_duration_months)

static int cost_per_1_birth_rate_per_month = 200;
#pragma unused(cost_per_1_birth_rate_per_month)


/*cultural exchange rate*/
static int default_cultural_exchange_rate_percent = 30;
#pragma unused(default_cultural_exchange_rate_percent)

static int min_cultural_percent = 0;
#pragma unused(min_cultural_percent)

static int max_cultural_percent = 100;
#pragma unused(max_cultural_percent)

static int min_cultural_policy_duration_months = 1;
#pragma unused(min_cultural_policy_duration_months)
static int max_cultural_policy_duration_months = 12;
#pragma unused(max_cultural_policy_duration_months)
static int cost_per_1_cultural_rate_per_month = 10;
#pragma unused(cost_per_1_cultural_rate_per_month)
//unoccupied penalty
static int maint_per_unoccupied_residence = 100;
#pragma unused(maint_per_unoccupied_residence)

static int maint_per_unoccupied_job = 200;
#pragma unused(maint_per_unoccupied_job)


static int alien_visa_duration_months = 24;
#pragma unused(alien_visa_duration_months)


static int lose_condition_average_happiness = 0;
#pragma unused(lose_condition_average_happiness)

static int lose_condition_money = 0;
#pragma unused(lose_condition_money)

#endif
