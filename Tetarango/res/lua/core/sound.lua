
local function aw_get_sound()
	if aw_objects.sound ~= nil then
	return aw_objects.sound
	end
end

exports = {
	loadSound = function(path, key)
		local sound = aw_get_sound()
		if sound ~= nil and sound.loadSound ~= nil then
			sound.loadSound(path, key)
		end
	end,

	playSound = function(key, vol)
		local sound = aw_get_sound()
		if sound ~= nil and sound.playSound ~= nil then
			sound.playSound(key, vol)
		end
	end,

	loadMusic = function(path, key)
		local sound = aw_get_sound()
		if sound ~= nil and sound.loadMusic ~= nil then
			sound.loadMusic(key, vol)
		end
	end,

	playMusic = function(key, fadeInMs)
		local sound = aw_get_sound()
		if sound ~= nil and sound.playMusic ~= nil then
			sound.playMusic(key, fadeInMs)
		end
	end,

	stopMusic = function(key, fadeOutMs)
		local sound = aw_get_sound()
		if sound ~= nil and sound.stopMusic ~= nil then
			sound.stopMusic(key, fadeOutMs)
		end
	end,

	stopAllSounds = function(fadeOutMs)
		local sound = aw_get_sound()
		if sound ~= nil and sound.stopAllSounds ~= nil then
			sound.stopAllSounds(fadeOutMs)
		end
	end,

	setMasterVolume = function(vol)
		local sound = aw_get_sound()
		if sound ~= nil and sound.setMasterVolume~= nil then
			sound.setMasterVolume(vol)
		end
	end,

	setMusicVolume = function(vol)
		local sound = aw_get_sound()
		if sound ~= nil and sound.setMusicVolume~= nil then
			sound.setMusicVolume(vol)
		end
	end,

	setEffectVolume = function(vol)
		local sound = aw_get_sound()
		if sound ~= nil and sound.setEffectVolume~= nil then
			sound.setEffectVolume(vol)
		end
	end
}
