local modellib = {}

local ffi = require 'ffi'
local lib = ffi.load("modellib")
--基本的数据
ffi.cdef [[
    typedef void* HANDLE;
    typedef void* INTERPOLATOR_HANDLE;

    struct VECTOR2 {
		float x, y;
    };
    
    struct VECTOR3 {
		float x, y, z;
    };

    struct VECTOR4 {
		float x, y, z, w;
    };
    
    struct EXTENT
    {
    	float radius;
    	VECTOR3 min;
    	VECTOR3 max;
    };

    struct SEQUENCE_TIME
    {
    	int time;
    	int interval_start;
    	int interval_end;
    };

    
    struct INTERPOLATOR_NODE
    {
        int time;
        VECTOR4 vector;
        VECTOR4 in_tan;
        VECTOR4 out_tan;
    }
]]

--lua参数转换为 c 参数
modellib.object2c = {
    ['string'] = function (value)
        return tostring(value)
    end,

    ['VECTOR2*'] = function (v)
        if v == nil then 
            v = ffi.NULL
        elseif type(v) == 'table' then 
            local vec2 = ffi.new('struct VECTOR2')
            vec2.x = v.x or v[1] or 0
            vec2.y = v.y or v[2] or 0
            return vec2
        end
        return v
    end,


    ['VECTOR3*'] = function (v)
        if v == nil then 
            v = ffi.NULL
        elseif type(v) == 'table' then 
            local vec3 = ffi.new('struct VECTOR3')
            vec3.x = v.x or v[1] or 0
            vec3.y = v.y or v[2] or 0
            vec3.z = v.z or v[3] or 0
            return vec3
        end
        return v
    end,

    ['VECTOR4*'] = function (v)
        if v == nil then 
            v = ffi.NULL
        elseif type(v) == 'table' then 
            local vec4 = ffi.new('struct VECTOR4')
            vec4.x = v.x or v[1] or 0
            vec4.y = v.y or v[2] or 0
            vec4.z = v.z or v[3] or 0
            vec4.w = v.w or v[4] or 0
            return vec4
        end
        return v
    end,

    ['EXTENT*'] = function (v)
        if v == nil then 
            v = ffi.NULL
        else
            local extent = v 
            if type(v) == 'table' then 
                extent = ffi.new('struct EXTENT')
            end
            extent.min = modellib.object2c['VECTOR3*'](v.min or {0, 0, 0})
            extent.max = modellib.object2c['VECTOR3*'](v.max or {0, 0, 0})
            return extent
        end
        return v
    end,


    ['INTERPOLATOR_NODE*'] = function (v)
        local extent = v 
        if type(v) == 'table' then 
            extent = ffi.new('struct INTERPOLATOR_NODE')
        end
        extent.vector = modellib.object2c['VECTOR4*'](v.vector or {})
        extent.in_tan = modellib.object2c['VECTOR4*'](v.in_tan or {})
        extent.out_tan = modellib.object2c['VECTOR4*'](v.out_tan or {})
        return v
    end,
    

    ['SEQUENCE_TIME*'] = function (v)
        if v == nil then 
            return ffi.NULL 
        elseif type(v) == 'table' then 
            local time = ffi.new('struct SEQUENCE_TIME')
            time.time = v.time or v[1] or 0
            time.interval_start = v.interval_start or v[2] or 0
            time.interval_end = v.interval_end or v[3] or 0
            return v
        end

        return v
    end,


    ['INTERPOLATOR_HANDLE'] = function (v)
        if type(v) == 'table' then 
            return v.handle or 0 
        end 
        return v
    end,
}
--c参数 转换为lua参数
modellib.c2object = {
    ['string'] = function (value)
        return ffi.string(value)
    end,

    ['VECTOR2*'] = function (v)
        if v == ffi.NULL then 
            return nil 
        end
        return v
    end,

    ['VECTOR3*'] = function (v)
        if v == ffi.NULL then 
            return nil 
        end
        return v
    end,

    ['EXTENT*'] = function (v)
        if v == ffi.NULL then 
            return nil 
        end
        return v
    end,

    ['SEQUENCE_TIME*'] = function (v)
        if v == ffi.NULL then 
            return nil 
        end
        return v
    end,

    ['INTERPOLATOR_NODE*'] = function (v)
        if v == ffi.NULL then 
            return nil 
        end
        return v
    end,

}


local function capi_parser(class_name, cdef)
    local func_map = {}

    cdef:gsub('(%w[%w_ ]+[%w%*]) ([CSG][%w_]+)%s-(%b())', function (ret, api, param)

        local s = {}
        local list = {}
        --解析函数名 去掉模块名 并转小写加下划线
        for i = 1, #api do 
            local c = api:sub(i, i)
            if c:byte() >= ('A'):byte() and  c:byte() <= ('Z'):byte() and i > 1 then 
                list[#list + 1] = table.concat(s)
                s = {}
            end 
            s[#s + 1] = c:lower()
        end 
        list[#list + 1] = table.concat(s)
        for i = #list, 1, -1 do 
            local str = list[i]
            if str == class_name or str:len() == 0 then 
                table.remove(list, i)
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
        local name = table.concat(list, '_')
        local type = 'none'
        local c = api:sub(1, 1)
        if api:find('By') == nil then 
            if c == 'S' then 
                type = 'set'
            elseif c == 'G' then 
                type = 'get'
            end
        else 
            name = 'open'
        end
        if type == 'none' then 
            if name == 'create' then 
                name = 'new'
            end
        end
        --记录函数信息
        func_map[api] = {
            name = name,
            type = type,
            ret = ret:find('char*') and 'string' or ret,
            args = args,
        }
    end)

    return func_map
end

function modellib.register_class(class_name, cdef)
    local class = {type = class_name}
    setmetatable(class, class)

    local map_name = 'all_' .. class_name .. '_map'

    local func_map = capi_parser(class_name, cdef)

    class[map_name] = {}

    ffi.cdef(cdef)

    for api, info in pairs(func_map) do 
        local name = info.name
        local ret = info.ret 
        local args = info.args
        
        local function convert_param(param, count)
            for i = 1, count do 
                local v = param[i]
                local func = modellib.object2c[args[i]]
                if func then 
                    param[i] = func(v)
                end 
            end 
            return param
        end 

        if rawget(class, name) == nil then

            if info.type == 'set' then 
                --注册 set属性方法 参数进行转换
                class[name] = function (self, ...)
                    local param = {...}
                    local count = select('#', ...)
                    local p = table.unpack(convert_param(param, count))
                    lib[api](self.handle, p)
                end
            elseif info.type == 'get' then
                --注册所有get属性的方法
                local pos = ret:find('_HANDLE') --如果返回值是 handle 则进行对象转换
                if pos then 
                    local ret_class_name = ret:sub(1, pos - 1):lower()
                    class[name] = function (self)
                        local handle = lib[api](self.handle)
                        if handle == 0 then 
                            return 
                        end 
                        return modellib[ret_class_name].open(self, handle)
                    end
                else 
                    --否则从根据相应规则进行转换
                    class[name] = function (self)
                        local value = lib[api](self.handle)
                        local func = modellib.c2object[ret]
                        if func then 
                            value = func(value)
                        end
                        return value
                    end
                end
            else
                --构造函数
                if name == 'new' then 
                    class[name] = function ()
                        local handle = lib[api]()
                        local object = setmetatable({handle = handle}, class)
                        class[map_name][tonumber(handle)] = object
                        return object
                    end 
                
                --复制构造
                elseif name == 'copy' then 
                    class[name] = function (self)
                        if self.handle == nil then 
                            return 
                        end 
                        local handle = lib[api](self.handle)
                        local object = setmetatable({handle = handle}, class)
                        class[map_name][tonumber(handle)] = object
                        return object
                    end
                elseif name == 'open' then 
                    --从指定对象 上 读取
                    class[name] = function (owner, index)
                        local handle = lib[api](owner.handle, index)
                        if handle == nil then 
                            return 
                        end 
                        local object =  class[map_name][tonumber(handle)]
                        if object then 
                            return object 
                        end 
                        object = setmetatable({handle = handle, owner = owner}, class)
                        class[map_name][tonumber(handle)] = object
                        return object
                    end 
                
                elseif name == 'close' then 
                    --析构方法
                    class[name] = function (self)
                        if self.handle == nil then 
                            return 
                        end 
                        lib[api](self.handle, self.owner == nil)
                        class[map_name][tonumber(self.handle)] = nil
                        self.handle = nil
                    end 
                end 
            end
        end
    end 

    function class:__index(name)
        local func = rawget(class, name)
        if func then return func end 
        
        if rawget(self, 'handle') then 
            local func = rawget(class, 'get_' .. name)
            if func then return func(self) end 
        end 
    
        return rawget(self, name)
    end 
    
    function class:__newindex(name, value)
        if rawget(self, 'handle') then
            local func = rawget(class, 'set_' .. name)
            if func then return func(self, value) end 
    
            local func = rawget(class, 'get_' .. name)
            if func then 
                print('nothing writer', class.type, name, debug.traceback())
                return 
            end 
        end
        rawset(self, name, value)
    end 
    
    
    function class:__tostring()
        if self.handle then
            return self.type .. "<" .. ("%x"):format(tonumber(self.handle)) .. '>'
        end 
        return self.type .. "<null>"
    end 


    
    local function pairs2(self, key)
       return function (self, key)
            return next(self, key)
        end, self, nil
    end

    function class:__pairs()
        local list = {}
        local func = function (tbl, key)
            local nk, nv = next(self, key)
            if nk then 
                nv = tbl[nk]
                table.insert(list, {nk, nv})
                func(self, nk)
            end
        end
        for k, v in pairs2(modellib[self.type]) do 
            if type(v) == 'function' and k:len() > 5 and k:sub(1, 4) == 'get_' and k:find('_[Bb]y') == nil then 
                table.insert(list, {k:sub(5, k:len()), v(self)})
            end 
        end 
        local n = 1
        return function()
                local v = list[n]
                if not v then
                    return
                end
                n = n + 1
                return v[1], v[2]
        end, self
    end

    return class
end 

return modellib