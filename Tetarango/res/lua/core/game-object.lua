
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

		obj.create = function(self, awType, luaImplKey)
			local objId = self:AW_createGameObject(awType, luaImplKey)
			local obj = aw_objects[objId]

			return obj
		end

		obj.clone = function(self)
			local newMe = self:create(self.aw_type, self.impl.name)
			newMe:moveUpParent()
			return newMe
		end

		obj.moveUpParent = function(self)
			self:AW_moveToParentGameObject()
		end

		obj.setParent = function(self, obj)
			self:AW_setParentGameObject(obj.id)
		end
	end
}