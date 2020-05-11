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

local s = {'\n'}

for index,info in ipairs(model_list) do 
    local path = info.path
    local model = core.open_model(path)
    local has = false
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
        local file_name = path:gsub('.+\\','')
        local out_screen_path = 'screen\\' .. file_name
        local out_fix_path = 'model\\' .. file_name
        local out_model_path = 'resource\\' .. file_name

        if list then 
            s[#s + 1] = "['" .. info.name .. "']\n"
            s[#s + 1] = 'screen = [[' .. out_screen_path .. ']]\n'
            s[#s + 1] = 'model = [[' .. out_model_path .. ']]\n'
            s[#s + 1] = 'animation = {\n'
            for i,data in ipairs(list) do 
                s[#s + 1] = "'".. data.name:lower() .. "' = {\n"
                s[#s + 1] = ' index = ' .. tostring(i - 1) ..',\n'
                s[#s + 1] = ' start = ' .. data.start ..',\n'
                s[#s + 1] = ' end = ' .. data['end'] ..',\n'
                s[#s + 1] = '},\n'
            end 
            s[#s + 1] = '}\n\n'
        end 
        core.load_mpq_file(path,out_model_path)
        core.save_model(model,out_screen_path, out_fix_path)

        local texture_list = core.load_texture_list(model)
        if texture_list then 
            for index,name in ipairs(texture_list) do 
                if name and name:len() > 0 then 
                    core.load_mpq_file(name,'model\\' .. name)
                end
            end 
        end 


        core.close_model(model)

        has = true 
    end
    
    if has ~= true then 
        print(info.name, path, has)
    end
   

end 

local file = io.open('ModelData.ini','w')
file:write(table.concat(s))
file:close()

print('end')