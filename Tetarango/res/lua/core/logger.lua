
return {
	module = function(self)
		return aw_objects.logger
	end,

	log = function(self, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10)
		local msg = ""

		for k, v in pairs({ msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10 }) do
			if msg == "" then
				msg = tostring(v)
			else
				msg = msg .. ", " .. tostring(v)
			end
		end

		self:module().log(msg, tostring(aw_cid))
	end
}
