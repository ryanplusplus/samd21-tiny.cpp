local keys = {
  'LicenseEDUMini_DontShowAgainToday',
  'LicenseOpenSDA_DontShowAgainToday'
}

local date = os.date('*t')
local filename = os.getenv('HOME') .. '/.config/SEGGER/SEGGER_REG_HKEY_CURRENT_USER.xml'
local value = ('0x07E%01X%02X%02X'):format(date.year - 2016, date.month, date.day)

local contents do
  local read_file = io.open(filename, 'r')
  contents = io.open(filename):read('a')
  read_file:close()
end

for _, key in ipairs(keys) do
  contents = contents:gsub(key .. '="0x07E....."', key .. '="' .. value .. '"')
end

local write_file = io.open(filename, 'w+')
write_file:write(contents)
write_file:close()
