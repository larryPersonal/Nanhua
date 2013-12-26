-- Housing Behavior
function isHouseNearBy()
x = HasHouse()
print( x )
return x
end

function hasNoHouse()
x = HasHouse()

if(x == false) then
print("chief got no House, go Buy one")
return 1
else
print("chief got House liao")
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
print("chief Buying House: " .. homes[i])
--break
return 1
else
print("chief Failed to buy. Next!")

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
print("chief Squeezing House: " .. homes[i])
--break
return 1
else
print("chief Failed to squeeze. Next!")
end
end
end
return 0
end

function gotNoHouseWarning()
x = HasHouse()
if (x == false) then
print ("chief no house go wander")
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
print("chief Going to Work")
GoToWork()
return 1
end

function hasWorkAlready()
a = HasWork()
if (a == true) then
print("chief Has Job")
return 1
else
print("chief no Job, Next")
return 0
end
end

function gotNoMoney()
a = GetCashOnHand()
print ("chief have cash: " .. a)

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
print("chief got no work")
return 1
else
return 0
end
end

function findWork()
print("chief find Work")

GetBuildingsOfType("education")

if next(returned_buildings) == nil then
print("chief no education available")
return 1
end

for i=1, #returned_buildings do
print ("chief finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("chief got a job")
GoToWork()
return 0
else
print ("chief problem signing up for job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 1
end

function findLowerJob()
print("find lower level work")
GetAllJobIDs()

if (#job_locations == 0) then
print("chief no low jobs available")
return 1
end

for i=1, #job_locations do
print ("chief finding job again in building " .. job_locations[i])
gotjob = AssignLowerJobWithID(job_locations[i])
if (gotjob == true) then
print ("chief got a Lower job")
GoToWork()
return 0
else
print ("chief problem signing up for Lower job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.

return 1
end

function noJobFound()
print("chief Cannot find a job! Go wander")
Wander()
return 1
end

function noNeedActivity()
print("chief There is nothing I need to do now")
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
print("chief is squeezing in house")
return 1
else
return 0
end
end

function upgradeHouse()
print ("chief upgrading House")

findHouse = IsBetterHouseAvailable()

if (findHouse ~= true) then
return 0
end

cash = GetCashOnHand()
GetAllHomeIDs()
for i=1, #homes do
cost = GetCostOfBuildingWithID(homes[i])

if (cash >= cost and cost ~= 0) then
print ("chief sold House")
SellHouse()
bought = BuyHouseWithID(homes[i])
if (bought == true) then
GoHome()
print("chief Upgraded House: " .. homes[i])
--break
return 1
else
print("chief Failed to Upgrade. Next!")
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
print("chief upgrading Job")

findjob = IsBetterJobAvailable()

if (findjob ~= true) then
print("chief no Upgrade Job Available")
return 0
else
print("chief quit Job")
QuitJob()
end


GetBuildingsOfType("education")

if next(returned_buildings) == nil then
print("chief no education available")
return 0
end

for i=1, #returned_buildings do
print ("chief finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("chief upgraded a job")
GoToWork()
return 1
else
print ("chief problem upgrading job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 0
end

function findShop()
print ("chief finding shop")
GetBuildingsOfType("commerce")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("chief go Shopping")
GoToBuildingWithID(loc)
return 1
else
print ("chief cant go shopping")

end
return 0
end

function findSocial()
print ("chief finding social")
GetBuildingsOfType("social")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("chief go Social")
GoToBuildingWithID(loc)
return 1
else
print ("chief cant go social")

end
return 0
end

function checkInteract()
    print ("chief checking for social interaction")
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
print("chief nothing else to do, go jalan jalan")
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