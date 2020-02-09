local impl = require("core/impl")
local sound = require("core/sound")
local texture = require("core/texture")
local font = require("core/font")
local screen = require("core/screen")
local logger = require("core/logger")
local event = require("core/event")
local input = require("core/input")
local keys = require("core/keymap")
local event = require("core/event")

local moveI = 100
local moveDistance = 1000

impl:define({
	onLoadResources = function (self)
		texture:loadTexture("tile/grass.png", "my-g2")
	end,

	onInit = function (self)
		self.r = math.random(0, 255)
		self.g = math.random(0, 255)
		self.b = math.random(0, 255)
		self.x = math.random(-1000, 1000)
		self.y = math.random(-1000, 1000)
		self.hasMadeObj = false

		self.moveSpeed = 800 + math.random() * 50;

		for i = 1,10 do
			self:addScreenPoint(math.random(-50, 50), math.random(-50, 50))
		end

		self:updateColor()
		self:setPosition(self.x, self.y)
		input:registerKey({ keys.W, keys.A, keys.S, keys.D, keys.Q }, self)
		event:registerOnEnterFrame(self)

		self:setTexture("my-g2")
	end,

	updateColor = function(self)
		self.r = self.r + 1
		if self.r >= 255 then
			self.r = 0
			self.g = self.g + 1
			if self.g >= 255 then
				self.g = 0
				self.b = self.b + 1
				if self.b >= 255 then
					self.b = 0
				end
			end
		end

		self:setColor(self.r, self.g, self.b)
	end,

	onEnterFrame = function (self, frameTime) 
		self:updateColor()

		local deltaTime = frameTime / 1000
		local dX = (self.key_states[keys.A] and -(deltaTime * self.moveSpeed)) or (self.key_states[keys.D] and (deltaTime * self.moveSpeed)) or 0
		local dY = (self.key_states[keys.W] and -(deltaTime * self.moveSpeed)) or (self.key_states[keys.S] and (deltaTime * self.moveSpeed)) or 0

		if math.abs(dX) + math.abs(dY) > 0 then
			self:movePosition(dX, dY)
		end
	end,

	onKey = function(self, key, pressed)
		if key == keys.Q and not pressed and not self.hasMadeObj then
			local obj = self:clone()
			self.hasMadeObj = true
		end
	end
})

