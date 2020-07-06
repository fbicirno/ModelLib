
local lib = require 'modellib'

--将模型转换为ui模型的参数

--打开模型 --支持mdx mdl 路径
--local model = lib.model.open("Units\\Human\\Jaina\\Jaina.mdx")

--local ui_model = model:make_ui_model()
--
--ui_model:save("ui.mdx") --内存另存为mdx文件
--
--ui_model:close()
--
--model:close() --关闭模型 回收内存


local save = function (name, str)
    local file = io.open(name, "wb")
    file:write(str)
    file:close()
end 

local model = lib.model.open("units\\other\\DranaiAkama\\DranaiAkama.mdx")
local memory = model:save_to_memory("name.mdx")
print(memory:len())
model:save("old.mdx")
save('new.mdx', memory)