local modellib = {}

local ffi = require 'ffi'
local lib = ffi.load("modellib")
--基本的数据
ffi.cdef [[
    typedef void* HANDLE;

    struct VECTOR3 {
		float x;
		float y;
		float z;
	};
]]

--lua参数转换为 c 参数
local param_type_convert_map = {
    ['string'] = function (value)
        return tostring(value)
    end,

    ['VECTOR3*'] = function (v)
        if type(v) == 'table' then 
            local vector3 = ffi.new('struct VECTOR3')
            vector3.x = v.x or v[1] or 0
            vector3.y = v.y or v[2] or 0
            vector3.z = v.z or v[3] or 0
            return vector3
        end
        return v
    end,
}
--c参数 转换为lua参数
local retn_type_convert_map = {
    ['string'] = function (value)
        return ffi.string(value)
    end,

    ['VECTOR3*'] = function (v)
        return v
    end,
}

--自动转换成下划线 注册指定模块方法
--@module 模块类
--@cdef  c接口的 定义
--[=[
    例如 
    local cdef = [[
        const char* GetTexturePath(HANDLE texhandle);
        void SetTexturePath(HANDLE texhandle, const char* path);
    ]]
    modellib.auto_method(modellib.texture, cdef)
   
    --会自动给 modellib.texture 实例对象 注册  以下 属性接口  
        texture:set_path(string)
        texture:get_path():string
        texture.path = string 
        print(texture.path)
]=]
function modellib.auto_method(module, cdef)
    local func_map = {}

    cdef:gsub('(%w[%w_ ]+[%w%*]) ([SG]et[%w_]+)%s-(%b())', function (ret, api, param)
        if api:find('By') then 
            return
        end
        
        local pos = 3 + module.type:len()
        local s = {}
        --解析函数名 去掉模块名 并转小写加下划线
        for i = 1, #api do 
            local c = api:sub(i, i)
            if i < 4 or i > pos then 
                if c:byte() >= ('A'):byte() and  c:byte() <= ('Z'):byte() and i > 1 then 
                    s[#s + 1] = '_'
                end 
                s[#s + 1] = c:lower()
            end
        end 

        --解析参数类型
        local args = {}
        param:gsub('(%w[%w_ ]+[%w%*]) [%w_]+[,%)]', function (ptype)
            if ptype:len() > 2 then 
                if ptype:find('char*') then 
                    ptype = 'string'
                end 
                table.insert(args, ptype)
            end
        end)

        --记录函数信息
        func_map[api] = {
            name = table.concat(s),
            is_set = api:sub(1,1) == 'S',
            ret = ret,
            args = args,
        }
    end)
   
    for api, info in pairs(func_map) do 
        local name = info.name
        local ret = info.ret 
        local args = info.args
        local is_set = info.is_set

        local function convert_param(param, count)
            for i = 1, count do 
                local v = param[i]
                local func = param_type_convert_map[args[i]]
                if func then 
                    param[i] = func(v)
                end 
            end 
            return param
        end 
        if info.is_set then 
            --注册 set属性方法 参数进行转换
            module[name] = function (self, ...)
                local param = {...}
                local count = select('#', ...)
                local p = table.unpack(convert_param(param, count))
                lib[api](self.handle, p)
            end
        else
            module[name] = function (self)
                local value = lib[api](self.handle)
                local func = retn_type_convert_map[ret]
                if func then 
                    value = func(value)
                end
                return value
            end
        end
    end 
end 

return modellib