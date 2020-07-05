
local lib = require 'modellib'

--将模型转换为ui模型的参数

--打开模型 --支持mdx mdl 路径
local model = lib.model.open("units\\other\\DranaiAkama\\DranaiAkama.mdx")


model:save_ui_model("ui.mdx") --内存另存为mdx文件

model:close() --关闭模型 回收内存