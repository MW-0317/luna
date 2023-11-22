luna.setCameraPosition(7.3589, 4.9583, 6.9258)
luna.setCameraLookAt(0, -0.03, 0)
luna.setCameraFOV(8)
cube = luna.object.createCube()
cube:setTexture("resources/textures/test.jpg")
luna.clearColor(0.75, 0.75, 0.75, 1.0)
--luna.createDebugLines()
luna.addObject(cube)
i = 0
function update(deltatime)
    cube:setRotation(0, i, 0)
    i = i + (3.14 / 180) * (18/2.5)
end