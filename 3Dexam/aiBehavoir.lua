function AI_Behavior(speed, detectionRadius)
    -- print("AI is acting with speed: " .. speed .. " and detection radius: " .. detectionRadius)
    
    -- Example logic: Increase speed if detection radius is large
    if detectionRadius > 30 then
        speed = speed + 2
    end
    
    -- print("Updated speed: " .. speed)
    
    -- Return the updated speed
    return speed
end