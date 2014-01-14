function goBuild()
    if (HasConstructingBuildings()) then
    
        GoBuild()
    else
        GoWander()
    end
    return 1
end

function goWander()
    GoWander()
    return 1
end
