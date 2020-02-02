-- AW Core binding layer and util
aw_functions = {}
aw_objects = {}

_require = require;
require = function(libPath)
	local lua = getLua()
	if lua.doFile ~= nil then
		lua.doFile("res/lua/" .. libPath .. ".lua")
	end
end

function getLua()
	if aw_objects["lua"] ~= nil then
		return aw_objects.lua;
	end
end
