local impl = require("core/impl")
local sound = require("core/sound")
local texture = require("core/texture")
local font = require("core/font")

local moveDistance = 1000

impl.define({
	onLoadResources = function (self)
		font.loadFont("Roboto-ThinItalic.ttf", "test-font")
		font.createFont("test-font", 99)
		texture.loadTexture("prop/environment/moon.png", "my-moon")
		sound.loadSound("game-over.wav", "test-snd")
	end,
	onInit = function (self)
		self.mX = math.random(100, 1000)
		self.mY = math.random(100, 1000)
		self.x = math.random(-200, 200)
		self.y = math.random(-200, 200)
		self.xO = 0
		self.yO = 0
		self.p = math.random() * math.pi * 2
		self:setTexture("my-moon")
		self:setPosition(self.x + self.xO, self.y + self.yO)
		--self:setColor(math.random(0, 255), 0, 0)
		self:setColor(math.random(0, 255), math.random(0, 255), math.random(0, 255))
	end,

	onEnterFrame = function (self, frameTime) 
		if (math.random(0, 10000) == 50) then
			sound.playSound("test-snd")
		end

		self.p = self.p + (frameTime / 1000.0) / 10.0
		self.xO = math.cos(self.p * math.pi * 2) * self.mX
		self.yO = math.sin(self.p * math.pi * 2) * self.mY
		self:setPosition(self.x + self.xO, self.y + self.yO)
	end
})