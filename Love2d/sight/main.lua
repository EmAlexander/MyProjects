require("tools")
require("map")
require("options")

player = {}

selectedPoint = {}

grapel = {}

coutching = false

function worldRayCastCallback(fixture, x, y, xn, yn, fraction)
    local hit = {}
    hit.fixture = fixture
    hit.x, hit.y = x, y
    hit.xn, hit.yn = xn, yn
    hit.fraction = fraction

    table.insert(ray.hitList, hit)

    return 1 -- Continues with ray cast through all shapes.
end

function love.load()
	player.x = 0
	player.y = 0
	player.w = 50
	player.h = 100
	player.yVel = 0
	player.g = false
	
	initmap()
	
	createRect(0, 300, 400, 20)
	createRect(400, 200, 300, 100)
	createRect(700, 300, 400, 20)
	
	ray = {
        x1 = 0,
        y1 = 0,
        x2 = 0,
        y2 = 0,
        hitList = {}
    }
end

function love.update(dt)
	if grapel.g then
		local xval = 800 * math.cos(grapel.angle) * dt
		local yval = 800 * math.sin(grapel.angle) * dt
		
		player.x = player.x + xval
		player.y = player.y + yval
		player.yVel = 0
		
		for i,v in ipairs(map.playerhitboxes) do
			if checkc({x=player.x, y=player.y, w=player.w, h=player.h}, v) then
				grapel.g = false
				player.x = player.x - xval
				player.y = player.y - yval
			end
		end
	else
		if love.keyboard.isDown(options.moveRightKey) then
			player.x = player.x + 450 * dt
			
			for i,v in ipairs(map.playerhitboxes) do
				if checkc({x=player.x, y=player.y, w=player.w, h=player.h}, v) then
					player.x = player.x - 450 * dt
				end
			end
		end
		
		if love.keyboard.isDown(options.moveLeftKey) then
			player.x = player.x - 450 * dt
			
			for i,v in ipairs(map.playerhitboxes) do
				if checkc({x=player.x, y=player.y, w=player.w, h=player.h}, v) then
					player.x = player.x + 450 * dt
				end
			end
		end
		
		player.yVel = player.yVel + 20 * dt
		
		player.y = player.y + player.yVel
		
		for i,v in ipairs(map.playerhitboxes) do
			if checkc({x=player.x, y=player.y, w=player.w, h=player.h}, v) then
				player.y = player.y - player.yVel
				player.yVel = 0
				player.g = true
			end
		end
	end
	
	map.world:update(dt)
	ray.hitList = {}
	
	angleBMAndPly = math.atan2((love.mouse.getY() - ( player.y + 50/2)), (love.mouse.getX() - (player.x + 50/2)))
	
	local dx = (player.x + 50/2) + 500 * math.cos(angleBMAndPly)
	local dy = (player.y + 50/2) + 500 * math.sin(angleBMAndPly)
	
	ray.x1 = player.x + 50/2
	ray.y1 = player.y + 50/2
	ray.x2 = dx
	ray.y2 = dy
	
	map.world:rayCast(ray.x1, ray.y1, ray.x2, ray.y2, worldRayCastCallback)
end

function love.draw()
	local cnt = 0

	love.graphics.rectangle('fill', player.x, player.y, player.w, player.h)
	
	for i,v in ipairs(map.playerhitboxes) do
		love.graphics.rectangle('fill', v.x, v.y, v.w, v.h)
	end
	
	--love.graphics.rectangle('fill', 0, 300, 400, 20)
	
	for i = 1, #ray.hitList do
        local hit = ray.hitList[i]
        love.graphics.setColor(255, 0, 0)
        love.graphics.print(i, hit.x, hit.y) -- Prints the hit order besides the point.
        love.graphics.circle("line", hit.x, hit.y, 3)
        love.graphics.setColor(0, 255, 0)
        love.graphics.line(hit.x, hit.y, hit.x + hit.xn * 25, hit.y + hit.yn * 25)
		cnt = cnt + 1
    end
	
	love.graphics.setColor(150, 0, 0)
	if cnt > 0 then
		idx, pt = findClosestPoint({x=player.x + 50/2, y=player.y + 50/2}, ray.hitList)
		love.graphics.line(ray.x1, ray.y1, ray.hitList[idx].x, ray.hitList[idx].y)
		selectedpoint = {hit=ray.hitList[idx], selected=true}
	else
		love.graphics.line(ray.x1, ray.y1, ray.x2, ray.y2)
		selectedpoint = -1
		selectedpoint = {selected=false}
	end
	love.graphics.setColor(0, 255, 0)
	
	love.graphics.translate(player.x + 50/2, player.y + 50/2)
	love.graphics.rotate(angleBMAndPly)
	love.graphics.rectangle("fill", 0, 0, 60, 20)
	love.graphics.rotate(-angleBMAndPly)
	love.graphics.translate(-(player.x + 50/2), -(player.y + 50/2))
	
	love.graphics.setColor(255, 255, 255)
end

function love.keypressed(key)
	if key == options.jumpKey and player.g then
		player.yVel = -10
		player.g = false
	end
	
	if grapel.g and (key == options.jumpKey or key == 's' or key == options.moveLeftKey or key == options.moveRightKey) then
		grapel.g = false
		if key == options.jumpKey then
			player.yVel = -10
			player.g = false
		else
			player.g = true
		end
	end
	
	if key == options.crouthKey then
		if options.toggleCroutch then
			coutching = not coutching
		else
			coutching = true
		end
		
		if coutching then
			player.y = player.y + 50
			player.h = 50
		else
			player.y = player.y - 50
			player.h = 100
		end
	end
end

function love.keyreleased(key)
	if key == options.crouthKey then
		if not options.toggleCroutch then
			coutching = false
			player.y = player.y - 50
			player.h = 100
		end
	end
end

function love.mousepressed(x, y, button)
	
end

function love.mousereleased(x, y, button)
	if button == 'r' and selectedpoint.selected then		
		grapel.g = true
		grapel.angle = angleBMAndPly
	end
end