function AI_Behavior(speed, detectionRadius)
    print("AI is acting with speed: " .. speed .. " and detection radius: " .. detectionRadius)
    
   
    if detectionRadius > 30 then
        speed = 0
    end
    
    print("Updated speed: " .. speed)
    
    -- Return the updated speed
    return speed
end