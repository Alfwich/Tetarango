local logger = require("core/logger")
local gameObject = require("core/game-object")

return {
	resource_loaded_for_impl = {},
	impls = {},
	next_impl_key = nil,

	set = function(self, binding_id, impl_key)
		if aw_objects[binding_id] == nil then
			aw_objects[binding_id] = {}
		end

		local obj = aw_objects[binding_id]
		if self.impls[impl_key] ~= nil and obj ~= nil then
			obj.id = binding_id
			obj.impl = self.impls[impl_key]
			for key, fn in pairs(self.impls[impl_key]) do
				obj[key] = fn
			end

			local shouldLoadImplResources = obj.onLoadResources ~= nil and self.resource_loaded_for_impl[impl_key] == false
			if shouldLoadImplResources then
				obj:onLoadResources()
				self.resource_loaded_for_impl[impl_key] = true
			end

			gameObject:bindGameObjectMethods(obj)

			if obj.onInit ~= nil then
				obj:onInit()
			end
		else
			logger.log("Failed to set obj implemetation for binding_id=" .. binding_id .. ", for impl_key=" .. impl_key)
		end
	end,

	setNextImplKey = function(self, key)
		self.next_impl_key = key
	end,

	define = function(self, impl_table)
		if self.next_impl_key ~= nil then
			impl_table.name = self.next_impl_key
			self.impls[self.next_impl_key] = impl_table
			self.resource_loaded_for_impl[self.next_impl_key] = false
			self.next_impl_key = nil
		else
			logger.log("Failed to define object implemetation")
		end
	end,

	loadResources = function(self)
		for key, obj in pairs(self.impls) do
			local impl_key = obj.impl_key
			if obj.onLoadResources ~= nil and self.resource_loaded_for_impl[impl_key] == false then
				obj:onLoadResources()
				self.resource_loaded_for_impl[impl_key] = true
			end
		end
	end
}