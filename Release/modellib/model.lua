
local cdef = [[

    bool InitMpqResource();
    bool LoadAllReplaceableTextures();

	HANDLE CreateModel();
    HANDLE OpenModel(const char* FileName);
    bool SaveModel(HANDLE handle, const char* path);
	void CloseModel(HANDLE handle, bool del);
    HANDLE CopyModel(HANDLE handle);

    void ModelCalculateBoundsRadius(HANDLE handle);

]]



local modellib = require 'modellib.modellib'

local module = require("modellib.module")

local model = modellib.register_class('model', cdef)
module.InitMpqResource()

module.LoadAllReplaceableTextures()

function model.open(path)
	local handle = module.OpenModel(path)
    if handle == module.object2c['nil']() then
        local file_type = path:sub(path:len() - 3,path:len()):lower()
        if file_type == '.mdl' then
            path = path:sub(1,path:len() - 4) .. '.mdx'
            handle = module.OpenModel(path)
        elseif file_type == '.mdx' then
            path = path:sub(1,path:len() - 4) .. '.mdl'
            handle = module.OpenModel(path)
        end
    end

	if handle == module.object2c['nil']() then
		return
	end

	return setmetatable({handle = handle, path = path}, model)
end

function model:calculate_bounds_radius()
    module.ModelCalculateBoundsRadius(self.handle)
end 
--保存模型
function model:save(path)
    module.SaveModel(self.handle, path)
end 

--另存为ui模型
function model:save_ui_model(path)
    local object = self:copy() --复制一个模型对象

    --遍历所有材质图层 将图层改为无阴影
    for material in object:each_material() do 
        for layer in material:each_layer() do 
            layer.unshaded = true
        end
    end 

    --粒子发射器缩小1000倍
    for particle2 in object:each_particle2() do 
        local scaling = particle2.particle_scaling
        scaling.x = scaling.x / 1000
        scaling.y = scaling.y / 1000
        scaling.z = scaling.z / 1000
    end

    --所有动画的最小最大范围改为0
    for sequence in object:each_sequence() do 
        sequence.extent = {
            min = {0, 0, 0},
            max = {0, 0, 0}
        }
    end

    --删除所有镜头
    object:clear_camera()

    --重算点范围
    object:calculate_bounds_radius()

    --输出文件
    object:save(path)

    --关闭
    object:close()
end

modellib.contariner(model, 'texture') --注册贴图容器
modellib.contariner(model, 'material') --注册材质容器
modellib.contariner(model, 'camera') --注册镜头容器
modellib.contariner(model, 'sequence') --注册动作容器

modellib.contariner(model, 'attachment') --注册附加点容器
modellib.contariner(model, 'bone') --注册骨骼容器
modellib.contariner(model, 'collisionshape') --注册碰撞容器
modellib.contariner(model, 'eventobject') --注册事件容器
modellib.contariner(model, 'helper') --注册帮助体容器
modellib.contariner(model, 'light') --注册光照容器
modellib.contariner(model, 'particle') --注册粒子容器
modellib.contariner(model, 'particle2') --注册粒子2容器


return model
