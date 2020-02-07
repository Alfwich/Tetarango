
exports = {
	module = function(self)
		return aw_objects.font
	end,

	loadFont = function(self, path, key)
		self:module().loadFont(path, key)
	end,
	
	createFont = function(self, key, fontSize)
		self:module().createFont(key, fontSize)
	end
}
