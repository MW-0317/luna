cube = 0
cube = luna.object.createCube()
cube:setTexture("resources/textures/ash.png")
luna.clearColor(0.5, 0.5, 0.5, 1.0)
--luna.createDebugLines()
luna.addObject(cube)
i = 0
function tickUpdate()
    cube:setRotation(0, i * (3.14 / 180) * (3/2), 0)
    i = i + 1
end