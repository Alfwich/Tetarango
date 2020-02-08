
return {
	module = function(self)
		return aw_objects.screen
	end,

	width = function(self)
		return tonumber(self:module().screenWidth())
	end,

	height = function(self)
		return tonumber(self:module().screenHeight())
	end
}