
exports = {
	module = function(self)
		return aw_objects.texture
	end,

	loadTexture = function(self, path, key)
		self:module().loadTexture(path, key)
	end
}
