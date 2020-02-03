for key, obj in pairs(aw_objects) do
	if obj.setColor ~= nil then
		obj.setColor(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255)
	end
end
