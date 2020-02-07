local impl = require("core/impl")
local sound = require("core/sound")
local texture = require("core/texture")
local font = require("core/font")
local screen = require("core/screen")
local logger = require("core/logger")
local event = require("core/event")

local moveI = 100
local moveDistance = 1000

impl:define({
	onLoadResources = function (self)
		font:loadFont("Roboto-ThinItalic.ttf", "test-font")
		font:createFont("test-font", 99)
		texture:loadTexture("prop/environment/moon.png", "my-moon")
		sound:loadSound("game-over.wav", "test-snd")
		sound:loadSound("tetarango.wav", "test-snd2")
		local sws = tostring(screen:width()) 
		local shs = tostring(screen:height()) 
		logger:log("Screen width=" .. sws .. ", height="..shs)
		event:setTimeout(function() sound:playSound("test-snd2") end, 4000)
	end,

	onInit = function (self)
		self.mX = moveI
		self.mY = moveI
		self.x = moveI
		self.y = moveI
		moveI = moveI + 10
		self.xO = 0
		self.yO = 0
		self.p = math.random() * math.pi * 2
		self:setTexture("my-moon")
		self:setPosition(self.x + self.xO, self.y + self.yO)
		self:setColor(math.random(0, 255), math.random(0, 255), math.random(0, 255))
		event:setTimeout(function() self:setColor(0, 0, 0) end, 5000)
	end,

	onEnterFrame = function (self, frameTime) 
		if math.random(0, 10000) == 50 then
			sound:playSound("test-snd")
		end

		if self.colorTimeoutId == nil then
			self.colorTimeoutId = event:setTimeout(function() 
				self:setColor(math.random(0, 255), math.random(0, 255), math.random(0, 255)) 
				self.colorTimeoutId = nil
			end, math.random(1000, 3000))
		end

		self.p = self.p + (frameTime / 1000.0) / 10.0
		self.xO = math.cos(self.p * math.pi * 2) * self.mX
		self.yO = math.sin(self.p * math.pi * 2) * self.mY
		self:setPosition(self.x + self.xO, self.y + self.yO)
	end
})