luna.setCameraPosition(0, 7, 0)
luna.setCameraLookAt(0, 0, 0)
luna.setCameraFOV(12)
cube = luna.object.createSphere()
cube:setCubeMapTexture("resources/textures/testSquare.png")
luna.clearColor(0.75, 0.75, 0.75, 1.0)
--luna.createDebugLines()
luna.addObject(cube)
i = 0
function update(deltatime)
    luna.setCameraPosition(7 * math.cos(i), 7, 7 * math.sin(i))
    i = i + (3.14 / 180) * (18/2.5)
    luna.setCameraLookAt(0, 0, 0)
end