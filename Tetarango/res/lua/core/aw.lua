-- AW Core binding layer and util
-- Globals `aw_objects`, `aw_functions`, `aw_cid` are defined by the host before any scripts are executed

-- Custom `require` system - module files are expected to set the `exports` field on exit to define their return value
aw_modules = {}
exports = {}
require = function(libPath)
	if aw_modules[libPath] == nil then
		aw_modules[libPath] = {}
		aw_objects.lua:doFile("res/lua/" .. libPath .. ".lua")
		for key, value in pairs(exports) do
			aw_modules[libPath][key] = value 
			exports[key] = nil
		end
	end

	return aw_modules[libPath]
end

-- AW global functions which are callable from the host
local event = require("core/event")
aw_functions.AW_enterFrame = function(frameTime)
	event:enterFrames(frameTime)
end

aw_functions.AW_exe_timeout = function(id)
	event:executeTimeout(id)
end

local impl = require("core/impl")
aw_functions.AW_loadImplResources = function()
	impl:loadResources()
end

aw_functions.AW_registerObjectImpl = function(implFile, implKey)
	impl:setNextImplKey(implKey)
	aw_objects.lua:doFile(implFile, false)
end

aw_functions.AW_setObjectImpl = function(bindingId, implKey)
	impl:set(bindingId, implKey)
end

