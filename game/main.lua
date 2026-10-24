local touches = {}
local lastButtonPressed = "None"
local playerX, playerY = 160, 120
local axisX, axisY = 0, 0

local walkFrames = {}
local currentFrame = 1
local animTimer = 0
local animSpeed = 0.08
local isMoving = false

function love.load()
    for i = 1, 12 do
        local path = "assets/Sprite/Walk" .. i .. ".png"
        local success, img = pcall(love.graphics.newImage, path)
        if success then
            walkFrames[i] = img
        else
            print("Warning: Could not load " .. path)
        end
    end
end

function love.update(dt)
    local joysticks = love.joystick.getJoysticks()
    if #joysticks > 0 then
        local joystick = joysticks[1]
        
        axisX = joystick:getGamepadAxis("leftx") or 0
        axisY = -(joystick:getGamepadAxis("lefty") or 0)

        if math.abs(axisX) < 0.2 then axisX = 0 end
        if math.abs(axisY) < 0.2 then axisY = 0 end

        playerX = math.max(15, math.min(305, playerX + axisX * 150 * dt))
        playerY = math.max(15, math.min(225, playerY + axisY * 150 * dt))

        isMoving = (axisX ~= 0 or axisY ~= 0)
    end

    if isMoving and #walkFrames > 0 then
        animTimer = animTimer + dt
        if animTimer >= animSpeed then
            animTimer = 0
            currentFrame = (currentFrame % #walkFrames) + 1
        end
    else
        currentFrame = 1
    end
end

function love.draw(screen)
    if screen == "bottom" then
        love.graphics.clear(0.08, 0.08, 0.12)

        love.graphics.setColor(0.3, 0.3, 0.35)
        love.graphics.rectangle("line", 5, 5, 310, 230)

        love.graphics.setColor(1, 1, 1)
        local activeSprite = walkFrames[currentFrame]
        
        if activeSprite then
            love.graphics.draw(activeSprite, playerX, playerY, 0, 1, 1, 15, 15)
        else
            love.graphics.setColor(1, 0.8, 0.2)
            love.graphics.circle("fill", playerX, playerY, 15)
        end

        for id, t in pairs(touches) do
            love.graphics.setColor(0.2, 0.9, 0.4, 0.6)
            love.graphics.circle("fill", t.x, t.y, 15)
        end

    elseif screen == "top" then
        love.graphics.clear(0.1, 0.1, 0.15)

        love.graphics.setColor(0.15, 0.2, 0.3)
        love.graphics.rectangle("fill", 10, 10, 380, 220, 6, 6)

        love.graphics.setColor(1, 1, 1)
        love.graphics.print("LÖVE Potion Test", 25, 20)
        love.graphics.print("FPS: " .. love.timer.getFPS(), 310, 20)

        love.graphics.setColor(0.4, 0.8, 1)
        love.graphics.print("Last Button: " .. tostring(lastButtonPressed), 25, 60)
        love.graphics.print("Stick X: " .. string.format("%.2f", axisX), 25, 85)
        love.graphics.print("Stick Y: " .. string.format("%.2f", axisY), 25, 105)

        local touchCount = 0
        for _ in pairs(touches) do touchCount = touchCount + 1 end
        love.graphics.setColor(0.8, 0.9, 0.4)
        love.graphics.print("Active Touches: " .. touchCount, 25, 140)

        love.graphics.setColor(0.7, 0.7, 0.7)
        love.graphics.print("• Circle Pad moves crewmate on bottom screen", 25, 180)
        love.graphics.print("• Touch bottom screen to place indicators", 25, 195)
    end
end

function love.touchpressed(id, x, y, dx, dy, pressure)
    touches[id] = {x = x, y = y}
end

function love.touchreleased(id, x, y, dx, dy, pressure)
    touches[id] = nil
end

function love.touchmoved(id, x, y, dx, dy, pressure)
    touches[id] = {x = x, y = y}
end

function love.gamepadpressed(joystick, button)
    lastButtonPressed = button

    if button == "dpup" then playerY = playerY - 10 end
    if button == "dpdown" then playerY = playerY + 10 end
    if button == "dpleft" then playerX = playerX - 10 end
    if button == "dpright" then playerX = playerX + 10 end
end

function love.gamepadreleased(joystick, button)
    if lastButtonPressed == button then
        lastButtonPressed = button .. " (released)"
    end
end