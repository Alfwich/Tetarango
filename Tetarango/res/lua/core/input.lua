
return {
	module = function(self)
		return aw_objects.input
	end,

	registerKey = function(self, keys, obj)
		if type(keys) == "table" then
			self:module().registerKey(obj.id, table.unpack(keys))
		else
			self:module().registerKey(obj.id, keys)
		end
	end
}