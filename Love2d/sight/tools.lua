function checkc(a, b)
	--Calculate the sides of rect A
    leftA = a.x
    rightA = a.x + a.w
    topA = a.y
    bottomA = a.y + a.h

    --Calculate the sides of rect B
    leftB = b.x
    rightB = b.x + b.w
    topB = b.y
    bottomB = b.y + b.h
	
    if bottomA <= topB then
        return false
    end

    if topA >= bottomB then
        return false
    end

    if rightA <= leftB then
        return false
    end

    if leftA >= rightB then
        return false
    end

    --If none of the sides from A are outside B
    return true
end

function distance ( x1, y1, x2, y2 )
  local dx = x1 - x2
  local dy = y1 - y2
  return math.sqrt ( dx * dx + dy * dy )
end

function max(t, fn)
    if #t == 0 then return nil, nil end
    local key, value = 1, t[1]
    for i = 2, #t do
        if fn(value, t[i]) then
            key, value = i, t[i]
        end
    end
    return key, value
end

function findClosestPoint(from, tos)
	distances = {}

	for i,v in ipairs(tos) do
		_d = distance(from.x,from.y,v.x,v.y)
		distances[i] = _d
	end

	k, v = max(distances, function(a,b) return a > b end)

	return k, tos[k]
end

function setIdentity(to)
	love.filesystem.setIdentity('tmp')
	local new = ""
	local path = love.filesystem.getSaveDirectory()
	path = path:match("/(.+)") or path:match("%s:/(.+)")
	for s in path:gmatch("/") do new = new .. "../" end
	love.filesystem.setIdentity(new .. (to and to:gsub('^/', '') or ""))
end
