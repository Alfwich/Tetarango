-- AW Core binding layer and util
-- Globals `aw_objects`, `aw_functions`, `aw_cid`, `aw_objects.lua` are defined by the host before any scripts are executed

-- Custom `require` system - module files are expected to return their public interface table
local aw_modules = {}
require = function(libPath)
	if aw_modules[libPath] == nil then
		aw_modules[libPath] = {}
		local export = aw_objects.lua:doFile("res/lua/" .. libPath .. ".lua") -- Can't use lua module as it cannot be loaded before this
		assert(type(export) == "table", "Expect `require(...)` libPath=" .. libPath .. " to return public interface table, got type=" .. type(export) .. " instead")
		for key, value in pairs(export) do
			aw_modules[libPath][key] = value 
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

local lua = require("core/lua")
aw_functions.AW_registerObjectImpl = function(implFile, implKey)
	impl:setNextImplKey(implKey)
	lua:doFile(implFile, false)
end

aw_functions.AW_setObjectImpl = function(bindingId, implKey)
	impl:set(bindingId, implKey)
end

