local r = 0
local g = 0
local b = 0
for key, obj in pairs(aw_objects) do
	log(obj._type)
	if obj.setColor ~= nil then
		--obj.setColor(math.random(255, 255), math.random(0, 0), math.random(0, 0), 255)
		--obj.setColor(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255)
		r = r + 3 % 255
		g = g + 4 % 255
		b = b + 5 % 255
		obj.setColor(r, g, b)
	end
end
