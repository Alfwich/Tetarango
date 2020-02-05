-- AW Core binding layer and util

_require = require;
require = function(libPath)
	local lua = getLua()
	if lua.doFile ~= nil then
		lua.doFile("res/lua/" .. libPath .. ".lua")
	end
end

function getLua()
	if aw_objects.lua ~= nil then
		return aw_objects.lua;
	end
end

function getLogger()
	if aw_objects.logger ~= nil then
		return aw_objects.logger;
	end
end

function log(msg)
	local logger = getLogger()
	if logger.log ~= nil then
		logger.log(msg, tostring(aw_cid))
	end
end

function hasObjectImpl(implKey)
	return aw_impls[implKey] ~= nil
end

function defObjectImpl(implTable)
	if aw_impl_key ~= nil then
		aw_impls[aw_impl_key] = implTable
		aw_impl_key = nil
	else
		log("Failed to define object implemetation")
	end
end

function setObjectImpl(bindingId, implKey)
	if aw_objects[bindingId] == nil then
		aw_objects[bindingId] = {}
	end

	if aw_impls[implKey] ~= nil then
		local obj = aw_objects[bindingId]
		for key, fn in pairs(aw_impls[implKey]) do
			obj[key] = fn
		end
	else
		log("Failed to set obj implemetation for bindingId=" .. bindingId .. ", for implKey=" .. implKey)
	end
end

local function AW_setObjectImpl(bindingId, implKey)
	setObjectImpl(bindingId, implKey)
end

local function AW_registerObjectImpl(implFile, implKey)
	aw_impl_key = implKey
	getLua().doFile(implFile)
end

local function AW_enterFrame(frameTime)
	frameTime = tonumber(frameTime)
	for key, obj in pairs(aw_objects) do
		if obj.onEnterFrame ~= nil then
			obj.onEnterFrame(frameTime)
		end
	end
end

aw_cid = nil
aw_impl_key = nil
aw_impls = {}
aw_functions = {
	AW_enterFrame=AW_enterFrame,
	AW_registerObjectImpl=AW_registerObjectImpl,
	AW_setObjectImpl=AW_setObjectImpl
}
aw_objects = {}
