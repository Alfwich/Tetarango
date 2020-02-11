
return {
	module = function(self)
		return aw_objects.lua
	end,

	doString = function(self, code)
		return self:module().doString(code)
	end,

	doFile = function(self, file, cached)
		return self:module().doFile(file, cached)
	end,

    registerImpl = function(self, file, key)
		return self:module().registerImpl(file, key)
	end
}