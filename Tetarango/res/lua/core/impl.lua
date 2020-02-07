local resource_loaded_for_impl = {}
local impls = {}
local next_impl_key = nil

exports = {
	set = function(binding_id, impl_key)
		if aw_objects[binding_id] == nil then
			aw_objects[binding_id] = {}
		end

		if impls[impl_key] ~= nil then
			local obj = aw_objects[binding_id]
			for key, fn in pairs(impls[impl_key]) do
				obj[key] = fn
			end

			if obj.onLoadResources ~= nil and resource_loaded_for_impl[impl_key] == false then
				obj:onLoadResources()
				resource_loaded_for_impl[impl_key] = true
			end

			if obj.onInit ~= nil then
				obj:onInit()
			end
		else
			log("Failed to set obj implemetation for binding_id=" .. binding_id .. ", for impl_key=" .. impl_key)
		end
	end,

	setNextImplKey = function (key)
		next_impl_key = key
	end,

	define = function(impl_table)
		if next_impl_key ~= nil then
			impl_table.impl_key = next_impl_key
			impls[next_impl_key] = impl_table
			resource_loaded_for_impl[next_impl_key] = false
			next_impl_key = nil
		else
			log("Failed to define object implemetation")
		end
	end,

	loadResources = function()
		for key, obj in pairs(impls) do
			local impl_key = obj.impl_key
			if obj.onLoadResources ~= nil and resource_loaded_for_impl[impl_key] == false then
				obj:onLoadResources()
				resource_loaded_for_impl[impl_key] = true
			end
		end
	end
}