
exports = {
	nextCallbackId = 0,
	callbackFns = {},
	module = function(self)
		return aw_objects.event
	end,

	setTimeout = function(self, fn, delayMs)
		local id = self.nextCallbackId
		self.callbackFns[id] = fn
		self:module().setTimeout(id, delayMs)
		self.nextCallbackId = self.nextCallbackId + 1
		return id
	end,

	clearTimeout = function(self, id)
		if self.callbackFns[id] ~= nil then
			self.callbackFns[id] = nil
			self:module().clearTimeout(id)
		end
	end,

	executeTimeout = function(self, sId)
		local id = tonumber(sId)
		if self.callbackFns[id] ~= nil then
			self.callbackFns[id]()
			self.callbackFns[id] = nil
		end
	end,

	enterFrames = function(self, frameTime)
		frameTime = tonumber(frameTime)
		for key, obj in pairs(aw_objects) do
			if obj.onEnterFrame ~= nil then
				obj:onEnterFrame(frameTime)
			end
		end
	end
}