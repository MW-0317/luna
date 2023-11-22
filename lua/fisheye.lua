luna.setCameraPosition(7, 0, 0)
luna.setCameraLookAt(0, 0, 0)
luna.setCameraFOV(10)
cube = luna.object.createSphere()
cube:setCubeMapTexture("resources/textures/test.jpg")
luna.clearColor(0.75, 0.75, 0.75, 1.0)
--luna.createDebugLines()
luna.addObject(cube)
i = 0
function update(deltatime)
    luna.setCameraPosition(7 * math.cos(i), 0, 7 * math.sin(i))
    luna.setCameraLookAt(0, 0, 0)
    i = i + (3.14 / 180) * (18/2.5)
end