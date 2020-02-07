
local function aw_get_font()
	if aw_objects.font ~= nil then
		return aw_objects.font
	end
end

exports = {
	loadFont = function(path, key)
		local font = aw_get_font()
		if font ~= nil and font.loadFont ~= nil then
			font.loadFont(path, key)
		end
	end,
	
	createFont = function(key, fontSize)
		local font = aw_get_font()
		if font ~= nil and font.createFont ~= nil then
			font.createFont(key, fontSize)
		end
	end
}
