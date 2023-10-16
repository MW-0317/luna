WINDOW_WIDTH = 1000
WINDOW_HEIGHT = 800

clearColor(0.5, 0.5, 0.5, 1.0)

luna.createDebugLines()
cube = luna.object.createCube()
cube:setTexture("resources/textures/ash.png")
luna.addObject(cube)
i = 0
function tickUpdate()
    i = i + 1
    cube:setRotation(0, i / 30, 0)
end