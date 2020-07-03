local modellib = require 'modellib.modellib'
require 'modellib.contariner'

local components = {
    'model', 'texture', 'material', 'layer', 'camera'
}

--加载指定模块 并注册元方法
for index, component in ipairs(components) do 
    local module = require ('modellib.' .. component)

    function module:__index(name)
        local func = rawget(module, name)
        if func then return func end 
        
        if rawget(self, 'handle') then 
            local func = rawget(module, 'get_' .. name)
            if func then return func(self) end 
        end 
    
        return rawget(self, name)
    end 
    
    function module:__newindex(name, value)
        if rawget(self, 'handle') then
            local func = rawget(module, 'set_' .. name)
            if func then return func(self, value) end 
    
            local func = rawget(module, 'get_' .. name)
            if func then 
                print('nothing writer', name)
                return 
            end 
        end
        rawset(self, name, value)
    end 
    
    
    function module:__tostring()
        if self.handle then
            return self.type .. "<" .. ("%x"):format(tonumber(self.handle)) .. '>'
        end 
        return ""
    end 

    
    local function pairs2(self, key)
       return function (self, key)
            return next(self, key)
        end, self, nil
    end

    function module:__pairs()
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

    modellib[component] = module
end 



return modellib