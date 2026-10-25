local ConfigService = require("api/ConfigService")

local Device = ConfigService:new({
	-- delete = false,          -- Disable deletion of UCI sections
	-- create = false,          -- Disable creation of UCI sections
	-- general_section = "main",-- General UCI section name
	-- anonymous = true,        -- Create UCI anonymous sections
	-- increment_name = true,   -- Create UCI sections with numeric incremental names
})

local DeviceConfig = Device:section(
	"tuya-reporterd", -- UCI config name
	"tuya-reporterd"  -- UCI section type
)

DeviceConfig:make_primary()
DeviceConfig.default_options.id.maxlength = 16

function DeviceConfig:create_defaults(sid)
	-- Default values to be added with every creation
	return {
		device_id = "",
		product_id = "",
		secret = ""
	}
end

	local opt_device_id = DeviceConfig:option("device_id")
		opt_device_id.cfg_require = true
		opt_device_id.maxlength = 100
		opt_device_id.minlength = 5

	local opt_product_id = DeviceConfig:option("product_id")
		opt_product_id.cfg_require = true
		opt_product_id.maxlength = 100
		opt_product_id.minlength = 5

	local opt_product_id = DeviceConfig:option("secret")
		opt_product_id.cfg_require = true
		opt_product_id.maxlength = 100
		opt_product_id.minlength = 5

	local opt_enable = DeviceConfig:option("enable")
		function opt_enable:validate(value)
			return self.dt:is_bool(value)
		end

		function opt_enable:set(value)
			self:table_set(self.config, self.sid, "enable", value)
		end

		function opt_enable:get()
			return self:table_get(self.config, self.sid, "enable")
		end

return Device
