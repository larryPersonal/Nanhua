-- Housing Behavior
function isHouseNearBy()
x = HasHouse()
print( x )
return x
end

function hasNoHouse()
x = HasHouse()

if(x == false) then
print("soldier got no House, go Buy one")
return 1
else
print("soldier got House liao")
return 0
end
end

function buyVacantHouse()
cash = GetCashOnHand()

print ("cash:" .. cash)
GetAllHomeIDs()
for i=1, #homes do
cost = GetCostOfBuildingWithID(homes[i])

if (cash >= cost and cost ~= 0) then
bought = BuyHouseWithID(homes[i])
if (bought == true) then
GoHome()
print("soldier Buying House: " .. homes[i])
--break
return 1
else
print("soldier Failed to buy. Next!")

end
end
end
return 0
end

function squeezeHouse()
cash = GetCashOnHand()
GetAllHomeIDs()
for i=1, #homes do
cost = GetCostOfBuildingWithID(homes[i])

if (cash >= cost and cost ~= 0) then
bought = BuySqueezeHouseWithID(homes[i])
if (bought == true) then
GoHome()
print("soldier Squeezing House: " .. homes[i])
--break
return 1
else
print("soldier Failed to squeeze. Next!")
end
end
end
return 0
end

function gotNoHouseWarning()
x = HasHouse()
if (x == false) then
print ("soldier no house go wander")
Wander()
--StopMoving()
return 1
else
return 0
end
end

function getMyEnergy()
a = GetEnergy()
if (a <= 15) then
print ("I am low on Energy")
return 1
else
print ("Not hungry yet")
return 0
end
end

function findFood()
cash = GetCashOnHand()
end

-- methods for determining the need to go work

function goWork()
print("soldier Going to Work")
GoToWork()
return 1
end

function hasWorkAlready()
a = HasWork()
if (a == true) then
print("soldier Has Job")
return 1
else
print("soldier no Job, Next")
return 0
end
end

function gotNoMoney()
a = GetCashOnHand()
print ("soldier have cash: " .. a)

if (a <= 50) then
return 1
else
return 0
end
end

-- check for the need to find work

function gotNoWork()
a = HasWork()
if (a ~= true) then
print("soldier got no work")
return 1
else
return 0
end
end

function findWork()
print("soldier find Work")

GetBuildingsOfType("military")

if next(returned_buildings) == nil then
print("soldier no military available")
return 1
end

for i=1, #returned_buildings do
print ("soldier finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("soldier got a job")
GoToWork()
return 0
else
print ("soldier problem signing up for job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 1
end

function findLowerJob()
print("find lower level work")
GetAllJobIDs()

if (#job_locations == 0) then
print("soldier no low jobs available")
return 1
end

for i=1, #job_locations do
print ("soldier finding job again in building " .. job_locations[i])
gotjob = AssignLowerJobWithID(job_locations[i])
if (gotjob == true) then
print ("soldier got a Lower job")
GoToWork()
return 0
else
print ("soldier problem signing up for Lower job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.

return 1
end

function noJobFound()
print("soldier Cannot find a job! Go wander")
Wander()
return 1
end

function noNeedActivity()
print("soldier There is nothing I need to do now")
return 0
end

--
function dummy()
return 1
end

function moveIn()
return 0
end

function isSqueezingHouse()
s = IsHomeSuitable()
if (a~=true) then
print("soldier is squeezing in house")
return 1
else
return 0
end
end

function upgradeHouse()
print ("soldier upgrading House")

findHouse = IsBetterHouseAvailable()

if (findHouse ~= true) then
return 0
end

cash = GetCashOnHand()
GetAllHomeIDs()
for i=1, #homes do
cost = GetCostOfBuildingWithID(homes[i])

if (cash >= cost and cost ~= 0) then
print ("soldier sold House")
SellHouse()
bought = BuyHouseWithID(homes[i])
if (bought == true) then
GoHome()
print("soldier Upgraded House: " .. homes[i])
--break
return 1
else
print("soldier Failed to Upgrade. Next!")
end
end
end
return 0
end

function isMaxClass()
a = IsAtMaxClassLevel()
if (a == true) then
print("farmer is ready to upgrade class")
return 1
else
return 0
end
end

function upgradeClass()
b = HasMetUpgradeCriteria()
if (b == true) then
TriggerUpgrade()
return 1
else
return 0
end
end

function isJobMatching()
a = IsJobSuitable()
if (a~=true) then
return 1
else
return 0
end
end

function upgradeJob()
print("soldier upgrading Job")

findjob = IsBetterJobAvailable()

if (findjob ~= true) then
print("soldier no Upgrade Job Available")
return 0
else
print("soldier quit Job")
QuitJob()
end


GetBuildingsOfType("military")

if next(returned_buildings) == nil then
print("soldier no military available")
return 0
end

for i=1, #returned_buildings do
print ("soldier finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("soldier upgraded a job")
GoToWork()
return 1
else
print ("soldier problem upgrading job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 0
end

function findShop()
print ("soldier finding shop")
GetBuildingsOfType("commerce")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("soldier go Shopping")
GoToBuildingWithID(loc)
return 1
else
print ("soldier cant go shopping")

end
return 0
end

function findSocial()
print ("soldier finding social")
GetBuildingsOfType("social")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("soldier go Social")
GoToBuildingWithID(loc)
return 1
else
print ("soldier cant go social")

end
return 0
end

function findEdu()
print ("soldier finding education")
GetBuildingsOfType("education")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("soldier go education")
GoToBuildingWithID(loc)
return 1
else
print ("soldier cant go education")

end
return 0
end

function checkInteract()
    print ("soldier checking for social interaction")
    interactchance = math.random(0, 100)
    print (interactchance)
    print (GetSocialInteractionChance())
    
    if (interactchance <= GetSocialInteractionChance()) then
        return 1
    else
        return 0
    end

end

function goInteract()
    TriggerSocialInteraction()
end


function jalanJalan()
Wander()
print("soldier nothing else to do, go jalan jalan")
return 1
end

function goToShop()
return 0
end

function buyFood()
return 0
end

function eatFood()
return 0
end

function noFood()
return 0
end

function isHungry()
return 0
end

function randEvent()
a = math.random(0,1)
return a
end