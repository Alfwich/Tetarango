local aw_resource_loaded_for_impl = {}
local aw_impls = {}
local aw_impl_key = nil

exports = {
	set = function(bindingId, implKey)
		if aw_objects[bindingId] == nil then
			aw_objects[bindingId] = {}
		end

		if aw_impls[implKey] ~= nil then
			local obj = aw_objects[bindingId]
			for key, fn in pairs(aw_impls[implKey]) do
				obj[key] = fn
			end

			if obj.onLoadResources ~= nil and aw_resource_loaded_for_impl[implKey] == false then
				obj:onLoadResources()
				aw_resource_loaded_for_impl[implKey] = true
			end

			if obj.onInit ~= nil then
				obj:onInit()
			end
		else
			log("Failed to set obj implemetation for bindingId=" .. bindingId .. ", for implKey=" .. implKey)
		end
	end,

	setNextImplKey = function (key)
		aw_impl_key = key
	end,

	define = function(implTable)
		if aw_impl_key ~= nil then
			implTable.impl_key = aw_impl_key
			aw_impls[aw_impl_key] = implTable
			aw_resource_loaded_for_impl[aw_impl_key] = false
			aw_impl_key = nil
		else
			log("Failed to define object implemetation")
		end
	end
}