local resource_loaded_for_impl = {}
local impls = {}
local next_impl_key = nil

exports = {
	set = function(bindingId, implKey)
		if aw_objects[bindingId] == nil then
			aw_objects[bindingId] = {}
		end

		if impls[implKey] ~= nil then
			local obj = aw_objects[bindingId]
			for key, fn in pairs(impls[implKey]) do
				obj[key] = fn
			end

			if obj.onLoadResources ~= nil and resource_loaded_for_impl[implKey] == false then
				obj:onLoadResources()
				resource_loaded_for_impl[implKey] = true
			end

			if obj.onInit ~= nil then
				obj:onInit()
			end
		else
			log("Failed to set obj implemetation for bindingId=" .. bindingId .. ", for implKey=" .. implKey)
		end
	end,

	setNextImplKey = function (key)
		next_impl_key = key
	end,

	define = function(implTable)
		if next_impl_key ~= nil then
			implTable.impl_key = next_impl_key
			impls[next_impl_key] = implTable
			resource_loaded_for_impl[next_impl_key] = false
			next_impl_key = nil
		else
			log("Failed to define object implemetation")
		end
	end
}