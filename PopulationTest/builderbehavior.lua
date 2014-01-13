function goBuild()
    if (HasConstructingBuildings()) then
    
        GoBuild()
    else
        GoWander()
    end
    return 1
end