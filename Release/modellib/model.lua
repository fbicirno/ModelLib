local ffi = require 'ffi'

local cdef = [[

    bool InitMpqResource();
    bool LoadAllReplaceableTextures();

	HANDLE CreateModel();
    HANDLE OpenModel(const char* FileName);
    bool SaveModel(HANDLE handle, const char* path);
	void CloseModel(HANDLE handle, bool del);
    HANDLE CopyModel(HANDLE handle);



]]

ffi.cdef(cdef)

local lib = ffi.load("modellib")

lib.InitMpqResource()
lib.LoadAllReplaceableTextures()

local modellib = require 'modellib.modellib'

local model = modellib.register_class('model', cdef)

function model.open(path)
	local handle = lib.OpenModel(path)
    if handle == ffi.NULL then
        local file_type = path:sub(path:len() - 3,path:len()):lower()
        if file_type == '.mdl' then
            path = path:sub(1,path:len() - 4) .. '.mdx'
            handle = lib.OpenModel(path)
        elseif file_type == '.mdx' then
            path = path:sub(1,path:len() - 4) .. '.mdl'
            handle = lib.OpenModel(path)
        end
    end

	if handle == ffi.NULL then
		return
	end

	return setmetatable({handle = handle, path = path}, model)
end


function model:save(path)
    lib.SaveModel(self.handle, path)
end 


modellib.contariner(model, 'texture') --注册贴图容器
modellib.contariner(model, 'material') --注册材质容器
modellib.contariner(model, 'camera') --注册镜头容器
modellib.contariner(model, 'sequence') --注册动作容器

modellib.contariner(model, 'attachment') --注册附加点容器
modellib.contariner(model, 'bone') --注册骨骼容器
modellib.contariner(model, 'collisionshape') --注册碰撞容器

return model
