
local lib = require 'modellib'

--打开模型 --支持mdx mdl 路径
local model = lib.model.open("units\\other\\DranaiAkama\\DranaiAkama.mdx")



for helper in model:each_helper() do 
    print(helper, helper.name)
    --for key, value in pairs(helper) do 
    --    print(key, value)
    --end
end 

model:save("xxx.mdx")

model:close()