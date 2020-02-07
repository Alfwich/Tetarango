
local function aw_get_texture()
	if aw_objects.texture ~= nil then
		return aw_objects.texture
	end
end

exports = {
	loadTexture = function(path, key)
		local texture = aw_get_texture()
		if texture ~= nil and texture.loadTexture ~= nil then
			texture.loadTexture(path, key)
		else
			log("Failed to load texture on path=" .. path .. ", for key=" .. key .. ", with texture module nil state=" .. texture == nil)
		end
	end
}
