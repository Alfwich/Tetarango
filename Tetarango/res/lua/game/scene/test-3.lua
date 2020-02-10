local impl = require("core/impl")
local logger = require("core/logger")
local input = require("core/input")
local keys = require("core/keymap")

impl:define({
	onLoadResources = function (self)
	end,

	onInit = function (self)
		self.player = self:create("Polygon", "color-changer")
		input:registerKeys({ keys.ESCAPE }, self)
	end,

	onEnterFrame = function (self, frameTime) 
	end,

	onKey = function (self, key, pressed)
		if key == keys.ESCAPE and pressed then
			self:fireUpEvent("ChangeScene", "menu-saved-games")
		end
	end

})
