package.path = package.path .. ';'
.. '?\\init.lua;'
.. 'script\\?.lua;'
.. 'script\\?\\init.lua;'
package.cpath = package.cpath .. ';'
.. 'bin\\?.dll;'
local core = require 'model.core'


local file = io.open('ModelList.ini','r')
if not file then 
    return 
end 

local str = file:read('*all')
file:close()



local model_list = {}

str:gsub('(.-)=(.-)\n',function (name,path)
    table.insert(model_list,{name = name,path = path})
end)

for index,info in ipairs(model_list) do 
    local path = info.path
    local model = core.open_model(path)
    if model == nil then 
        local file_type = path:sub(path:len() - 3,path:len()):lower()
        if file_type == '.mdl' then 
            path = path:sub(1,path:len() - 4) .. '.mdx'
            model = core.open_model(path)
        elseif file_type == '.mdx' then 
            path = path:sub(1,path:len() - 4) .. '.mdl'
            model = core.open_model(path)
        end 
    end 
    if model then 
        local list = core.load_animate(model)
        if list then 
            for i,data in ipairs(list) do 
                for name,value in pairs(data) do 
                    print(info.name,'index',i,name,value)
                end  
            end 
        end 
        core.save_model(model,path)
        core.close_model(model)
    end
end 

print('end')