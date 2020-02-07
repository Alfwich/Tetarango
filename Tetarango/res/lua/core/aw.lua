-- AW Core binding layer and util
-- `aw_objects` and `aw_functions` is defined before any scripts are executed

-- Custom `require` system
aw_require_results = {}
exports = nil
require = function(libPath)
	local lua = aw_objects.lua
	if lua ~= nil and lua.doFile ~= nil then
		if aw_require_results[libPath] == nil then
			lua.doFile("res/lua/" .. libPath .. ".lua")
			aw_require_results[libPath] = exports
			exports = nil
		end

		return aw_require_results[libPath]
	end
end

-- AW global functions
aw_functions.AW_enterFrame = function(frameTime)
	frameTime = tonumber(frameTime)
	for key, obj in pairs(aw_objects) do
		if obj.onEnterFrame ~= nil then
			obj:onEnterFrame(frameTime)
		end
	end
end

impl = require("core/impl")
aw_functions.AW_registerObjectImpl = function(implFile, implKey)
	impl.setNextImplKey(implKey)
	local lua = aw_objects.lua
	if lua ~= nil and lua.doFile ~= nil then
		lua.doFile(implFile, false)
	end
end

aw_functions.AW_setObjectImpl = function(bindingId, implKey)
	impl.set(bindingId, implKey)
end

