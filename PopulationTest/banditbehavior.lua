-- Housing Behavior
function isHouseNearBy()
x = HasHouse()
print( x )
return x
end

function hasNoHouse()
x = HasHouse()

if(x == false) then
print("citizen got no House, go Buy one")
return 1
else
print("citizen got House liao")
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
print("alien citizen Buying House: " .. homes[i])
--break
return 1
else
print("alien citizen Failed to buy. Next!")

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
print("alien citizen Squeezing House: " .. homes[i])
--break
return 1
else
print("citizen Failed to squeeze. Next!")
end
end
end
return 0
end

function gotNoHouseWarning()
x = HasHouse()
if (x == false) then
print ("citizen no house go wander")
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
print("citizen Going to Work")
GoToWork()
return 1
end

function hasWorkAlready()
a = HasWork()
if (a == true) then
print("citizen Has Job")
return 1
else
print("citizen no Job, Next")
return 0
end
end

function gotNoMoney()
a = GetCashOnHand()
print ("citizen have cash: " .. a)

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
print("citizen got no work")
return 1
else
return 0
end
end

function findWork()
print("acitizen find Work")

GetBuildingsOfType("amenity")

if next(returned_buildings) == nil then
print("citizen no education available")
return 1
end

for i=1, #returned_buildings do
print ("citizen finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("citizen got a job")
GoToWork()
return 0
else
print ("citizen problem signing up for job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 1
end

function findLowerJob()
print("find lower level work")

GetBuildingsOfType("amenity")

if next(returned_buildings) == nil then
print("citizen no low jobs available")
return 1
end

for i=1, #returned_buildings do
print ("finding job again in building " .. returned_buildings[i])
gotjob = AssignLowerJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("citizen got a Lower job")
GoToWork()
return 0
else
print ("citizen problem signing up for Lower job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.

return 1
end

function noJobFound()
print("citizen Cannot find a job! Go wander")
Wander()
return 1
end

function noNeedActivity()
print("citizen There is nothing I need to do now")
return 0
end

function gotLoyalty()
a = GetLoyalty()
if (a>10) then
print("I'm quite a loyal Xenos")
return 1
else
print("Not so loyal Researcher")
return 0
end
end

function changeRace()
print("Time to change Race to Mayan")
BecomeLocal()
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
print("citizen is squeezing in house")
return 1
else
return 0
end
end

function upgradeHouse()
print ("citizen upgrading House")

findHouse = IsBetterHouseAvailable()

if (findHouse ~= true) then
return 0
end

cash = GetCashOnHand()
GetAllHomeIDs()
for i=1, #homes do
cost = GetCostOfBuildingWithID(homes[i])

if (cash >= cost and cost ~= 0) then
print ("citizen sold House")
SellHouse()
bought = BuyHouseWithID(homes[i])
if (bought == true) then
GoHome()
print("citizen Upgraded House: " .. homes[i])
--break
return 1
else
print("citizen Failed to Upgrade. Next!")
end
end
end
return 0
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
print("citizen upgrading Job")

GetBuildingsOfType("education")

if next(returned_buildings) == nil then
print("citizen no education available")
return 1
end

QuitJob()

for i=1, #returned_buildings do
print ("Researcher finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("Researcher upgraded a job")
GoToWork()
return 1
else
print ("Researcher problem upgrading job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 0
end

function findShop()
print ("Researcher finding shop")
GetBuildingsOfType("commerce")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("Citizen go Shopping")
GoToBuildingWithID(loc)
return 1
else
print ("Citizen cant go shopping")

end
return 0
end

function findSocial()
print ("Citizen finding social")
GetBuildingsOfType("social")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("Citizen go Social")
GoToBuildingWithID(loc)
return 1
else
print ("Citizen cant go social")

end
return 0
end

function jalanJalan()
Wander()
print("Citizen nothing else to do, go jalan jalan")
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