
exports = {
	module = function(self)
		return aw_objects.sound
	end,

	loadSound = function(self, path, key)
		self:module().loadSound(path, key)
	end,

	playSound = function(self, key, vol)
		self:module().playSound(key, vol)
	end,

	loadMusic = function(self, path, key)
		self:module().loadMusic(path, key)
	end,

	playMusic = function(self, key, fadeInMs)
		self:module().playMusic(key, fadeInMs)
	end,

	stopMusic = function(self, key, fadeOutMs)
		self:module().stopMusic(key, fadeOutMs)
	end,

	stopAllSounds = function(self, fadeOutMs)
		self:module().stopAllSounds(fadeOutMs)
	end,

	setMasterVolume = function(self, vol)
		self:module().setMasterVolume(vol)
	end,

	setMusicVolume = function(self, vol)
		self:module().setMusicVolume(vol)
	end,

	setEffectVolume = function(self, vol)
		self:module().setEffectVolume(vol)
	end
}
