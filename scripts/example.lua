-- Initialization, if any

-- Angle in degrees by which the objects will be rotated every tick
local deg = 42
rad = deg * (math.pi / 180)

-- Auxilary functions
function rotate_xz(vec, rad)
    local cos_a = math.cos(rad)
    local sin_a = math.sin(rad)

    local x = vec.x * cos_a - vec.z * sin_a
    local z = vec.x * sin_a + vec.z * cos_a

    vec.x = x
    vec.z = z

    return vec
end

-- System (or several)
function sys_spin()
    print("Hello from Lua!")
    -- Process each entity with a callback
    for_each_entity(world, function(entity)
        local t = get_transform(world, entity)
        rotate_xz(t.position, rad)
        print("Rotated entity " .. entity .. ": "
            .. tostring(t.position.x) .. " "
            .. tostring(t.position.z))
    end)
end
