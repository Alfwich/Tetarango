local impl = require("core/impl")
local logger = require("core/logger")
local input = require("core/input")
local keys = require("core/keymap")

impl:define({
	onLoadResources = function (self)
		impl:load("game/object/color-changer")
	end,

	onInit = function (self)
		self.player = self:create("Polygon", "game/object/color-changer")
		input:registerKeys({ keys.ESCAPE, keys.K }, self)
	end,

	onEnterFrame = function (self, frameTime) 
	end,

	onKey = function (self, key, pressed)
		if key == keys.ESCAPE and pressed then
			self:fireUpEvent("ChangeScene", "menu-saved-games")
		elseif key == keys.K and pressed then 
			local children = self:children()
			print(#children)
			for k, c in pairs(children) do
				if c.id ~= self.player.id then
					c:destroy()
				end
			end
		end
	end

})
