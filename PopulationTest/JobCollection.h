//
//  JobCollection.h
//  PopulationTest
//
//  Created by Serious Games on 6/5/13.
//
//

#ifndef __PopulationTest__JobCollection__
#define __PopulationTest__JobCollection__

#include <iostream>
#include <json/json.h>
#include "Job.h"


/*the role of this class is to be a general collection of jobs. Job objects will be duplicated and assigned to the buildings as they are placed on the map.*/
class JobCollection
{
    void createJobWithJson(Json::Value json);
public:
    JobCollection();
    ~JobCollection();
    
    CCArray* allJobsInGame;
    
    Job* getJobWithName(std::string name);
    
    
};
#endif /* defined(__PopulationTest__JobCollection__) */
