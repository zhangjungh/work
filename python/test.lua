

local function gen_table()
	local t = {}
	for i=100, 110 do
		table.insert(t, i)
	end
	print('table in lua:')
	for _, v in ipairs(t) do io.write(v, ' ') end
	print()
	--funclib.func_c(t, 'aaa', 'bbb')
end

print('load lua file')

gen_table()


local function f(i)
	return 'asdf' .. tostring(i)
end

local g='aaa'
x='mmmm'
a = { [f(1)] = g; "x", "y"; x = 1, f(x), [30] = 23; [5]=45, 32, 44}

for key, value in pairs(a) do
	print(key, value)
end