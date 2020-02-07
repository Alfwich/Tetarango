
exports = {
	module = function(self)
		return aw_objects.logger
	end,

	log = function(msg)
		self:module().log(msg, tostring(aw_cid))
	end
}
