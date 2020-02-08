
return {
	bindGameObjectMethods = function(self, obj) 

		obj.AW_enterFrame = function(frameTime) obj:onEnterFrame(frameTime) end

		obj.key_states = {}
		obj.AW_onKey = function(key, pressed) 
			local keyNum = tonumber(key)
			obj.key_states[keyNum] = pressed == "1"
			if obj.onKey ~= nil then
				obj:onKey(keyNum, pressed == "1") 
			end
		end
	end
}