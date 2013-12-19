-- Housing Behavior
function isHouseNearBy()
x = HasHouse()
print( x )
return x
end

function hasNoHouse()
x = HasHouse()

if(x == false) then
print("I got no House, go Buy one")
return 1
else
print("I got House liao")
return 0
end
end

function buyVacantHouse()
cash = GetCashOnHand()

print ("cash:" .. cash)
GetAllHomeIDs()
for i=0, #homes do
print("house: " .. homes[i])
cost = GetCostOfBuildingWithID(homes[i])
print("cost: " .. cost)

if (cash >= cost and cost ~= 0) then
print("Buying House: " .. homes[i])
bought = BuyHouseWithID(homes[i])
if (bought == true) then
GoHome()
print("ET Go home")
break
else
print("Failed to buy. Next!")
end
end
end
end

function gotNoHouseWarning()
x = HasHouse()
if (x == false) then
print ("no house go wander")
Wander()
--StopMoving()
return 1
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
print("Going to Work")
GoToWork()
return 1
end

function hasWorkAlready()
a = HasWork()
if (a == true) then
print("Has Work Already")
return 1
else
print("no Job, Next")
return 0
end
end

function gotNoMoney()
a = GetCashOnHand()
print ("I have cash: " .. a)

if (a <= 1000) then
return 1
else
return 0
end
end

-- check for the need to find work

function gotNoWork()
a = HasWork()
if (a ~= true) then
print("I got no work")
return 1
else
return 0
end
end

function findWork()
print("find Work")

GetBuildingsOfType("amenity")
if (returned_buildings == nil) then
return 0
end
for i=0, #returned_buildings do
print ("finding job in building " .. returned_buildings[i])
gotjob = AssignJobWithID(returned_buildings[i])
if (gotjob == true) then
print ("got a job")
GoToWork()
break
else
print ("problem signing up for job, NEXT")
end
end
-- never return 0 in the middle of the loop as it will end the loop early.
return 0
end

function findLowerJob()
print("find lower level work")
return 0
end

function noJobFound()
print("Cannot find a job! Go Home")
GoHome()
return 1
end

--
function dummy()
return 1
end

function moveIn()
return 0
end

function findShop()
print ("finding shop")
GetBuildingsOfType("commerce")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("go Shopping")
GoToBuildingWithID(loc)
else
print ("cant go shopping")

end
return 0
end

function findSocial()
print ("finding social")
GetBuildingsOfType("social")
loc = math.random(0, #returned_buildings)
dist = IsDestinationInRange(loc)
if (dist == true) then
print ("go Social")
GoToBuildingWithID(loc)
else
print ("cant go social")

end
return 0
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