map = {}
map.playerhitboxes = {}
map.terrain = {}
map.terrain.fixtures = {}

currentFixtures = 0

function initmap()
	love.physics.setMeter(1)
	map.world = love.physics.newWorld()
	
	map.terrain.body = love.physics.newBody(map.world, 0, 0, "static")
end

function createRect(_x, _y, _w, _h)
	local shape = love.physics.newRectangleShape(_x+_w/2, _y+_h/2, _w, _h, 0)
	
	local fixture = love.physics.newFixture(map.terrain.body, shape)
	
	currentFixtures = currentFixtures + 1
	
	map.terrain.fixtures[currentFixtures] = fixture
	
	table.insert(map.playerhitboxes, {x=_x,y=_y,w=_w,h=_h})
end

function loadFromFile(str)
	
end

function loadFromString(str)
	
end