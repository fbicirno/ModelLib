local modellib = {}

local ffi = require 'ffi'
local lib = ffi.load("modellib")

--自动转换成小驼峰 注册指定模块方法
function modellib.auto_method(module, cdef)
    local func_map = {}
    cdef:gsub('([SG]et[%w_]+)%s-%(', function (name)
        if name:find('By') == nil then 
            local pos = 3 + module.type:len()
            local s = {}
            for i = 1, #name do 
                local c = name:sub(i, i)
                if i < 4 or i > pos then 
                    if c:byte() >= ('A'):byte() and  c:byte() <= ('Z'):byte() then 
                        if i > 1 then 
                            s[#s + 1] = '_'
                        end
                    end 
                    s[#s + 1] = c:lower()
                end
            end 
            func_map[name] = table.concat(s)
        end
    end)
   
    for api, name in pairs(func_map) do 
        local s = name:sub(1,1)
        if s == 's' then 
            module[name] = function (self, param)
                lib[api](self.handle, param)
            end
        elseif s == 'g' then 
            module[name] = function (self)
                return lib[api](self.handle)
            end
        end
    end 
end 

return modellib