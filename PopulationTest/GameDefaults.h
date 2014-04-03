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

#endif
