//
//  JobCollection.cpp
//  PopulationTest
//
//  Created by Serious Games on 6/5/13.
//
//

#include "JobCollection.h"

#include "JobsInGame.h"

JobCollection::JobCollection()
{
    CCLog ("jobs initialized");
    allJobsInGame = CCArray::create();
    allJobsInGame->retain();
    /*populate this*/
    
    Json::Reader reader;
    Json::Value root;
    
    
    bool parsingSuccessful = reader.parse( AllJobs, root );
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse jobs\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    std::string encoding = root.get("encoding", "UTF-8" ).asString();
    CCLog("%s", encoding.c_str());
    for (int i = 0; i<root["items"].size(); i++) {
         createJobWithJson(root["items"].operator[](i));
        
    }
    
    CCLog("%d", allJobsInGame->count());
}

void JobCollection::createJobWithJson(Json::Value json)
{
    Job* j = Job::create();
    
    
    j->jobName = json["job_name"].asString();
    j->job_currworkval = 0;
    j->job_workvalreq = json["job_workvalreq"].asInt();
    j->job_expmod = json["job_expgain"].asInt();
    j->job_hapmod = json["job_mod_hap"].asInt();
    j->job_loymod = json["job_mod_loy"].asInt();
    j->job_isTaken = false;
   
    j->job_targetSpriteType = json["job_spriteclass"].asString();
    allJobsInGame->addObject(j);
}

JobCollection::~JobCollection()
{
    /*
    for (int i = 0; i < allJobsInGame->count(); ++i)
       delete allJobsInGame->objectAtIndex(i);
    */
    allJobsInGame->removeAllObjects();
    allJobsInGame->release();
}

Job* JobCollection::getJobWithName(std::string name)
{
    //copy construct a job here.
    if (name.length() == 0) return NULL;
    
    CCLog("Getting job with name %s", name.c_str());
    if (allJobsInGame->count() > 0)
        for (int i = 0; i < allJobsInGame->count(); ++i)
        {
          if (name.compare(
                        ((Job*)allJobsInGame->objectAtIndex(i))->jobName) == 0)
          {
              Job* targetJob = (Job*)allJobsInGame->objectAtIndex(i);
              Job* anotherJob = (Job*)targetJob ->copy();
              
              
            return anotherJob;// (Job*)allJobsInGame->objectAtIndex(i)->copy();
         }
        }
    
    return NULL;
}