local moveDistance = 1000

aw_impl({
	onInit = function (self)
		self.mX = math.random(100, 1000)
		self.mY = math.random(100, 1000)
		self.x = math.random(-200, 200)
		self.y = math.random(-200, 200)
		self.xO = 0
		self.yO = 0
		self.p = math.random() * math.pi * 2
		self:setTexture("noise-solid-512")
		self:setPosition(self.x + self.xO, self.y + self.yO)
		--self:setColor(math.random(0, 255), 0, 0)
		self:setColor(math.random(0, 255), math.random(0, 255), math.random(0, 255))
	end,

	onEnterFrame = function (self, frameTime) 
		self.p = self.p + (frameTime / 1000.0) / 10.0
		self.xO = math.cos(self.p * math.pi * 2) * self.mX
		self.yO = math.sin(self.p * math.pi * 2) * self.mY
		self:setPosition(self.x + self.xO, self.y + self.yO)
	end
})