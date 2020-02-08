
return {
	bindGameObjectMethods = function(self, obj) 
		local shouldRegisterHostCallableEnterFrame = obj.onEnterFrame ~= nil
		if shouldRegisterHostCallableEnterFrame then
			obj.AW_enterFrame = function(frameTime) obj:onEnterFrame(frameTime) end
		end

		local shouldRegisterHostCallableOnKey = obj.onKey ~= nil
		if shouldRegisterHostCallableOnKey then
			obj.AW_onKey = function(key, pressed) 
				obj.key_states[tonumber(key)] = pressed == "1"
				obj:onKey(tonumber(key), pressed == "1") 
			end
		end
	end
}