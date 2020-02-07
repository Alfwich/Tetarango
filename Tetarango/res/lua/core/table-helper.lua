
exports = {
	copy = function (t)
	  local u = {}
	  for k, v in pairs(t) do u[k] = v end
	  return setmetatable(u, getmetatable(t))
	end,

	-- https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
	dump = function (o)
	   if type(o) == 'table' then
		  local s = '{ '
		  for k,v in pairs(o) do
			 if type(k) ~= 'number' then k = '"'..k..'"' end
			 s = s .. '['..k..'] = ' .. table_helper.dump(v) .. ','
		  end
		  return s .. '} '
	   else
		  return tostring(o)
	   end
	end
}
