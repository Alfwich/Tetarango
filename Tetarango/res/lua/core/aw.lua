-- AW Core binding layer and util
-- Globals `aw_objects`, `aw_functions`, `aw_cid` are defined before any scripts execute

-- Custom `require` system
aw_require_results = {}
exports = nil
require = function(libPath)
	if aw_require_results[libPath] == nil then
		aw_objects.lua.doFile("res/lua/" .. libPath .. ".lua")
		aw_require_results[libPath] = exports
		exports = nil
	end

	return aw_require_results[libPath]
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
aw_functions.AW_loadImplResources = function()
	impl.loadResources()
end

aw_functions.AW_registerObjectImpl = function(implFile, implKey)
	impl.setNextImplKey(implKey)
	aw_objects.lua.doFile(implFile, false)
end

aw_functions.AW_setObjectImpl = function(bindingId, implKey)
	impl.set(bindingId, implKey)
end

