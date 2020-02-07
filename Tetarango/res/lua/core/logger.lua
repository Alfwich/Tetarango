
local function aw_get_logger()
	if aw_objects.logger ~= nil then
		return aw_objects.logger
	end
end

exports = {
	log = function(msg)
		local logger = aw_get_logger()
		if logger ~= nil and logger.log ~= nil then
			logger.log(msg, tostring(aw_cid))
		end
	end
}
