
exports = {
	module = function(self)
		return aw_objects.logger
	end,

	log = function(self, msg)
		self:module().log(msg, tostring(aw_cid))
	end
}
