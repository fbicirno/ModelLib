local ffi = require 'ffi'

ffi.cdef[[
	typedef void* HANDLE;


	bool InitMpqResource();
	HANDLE CreateModel();
    HANDLE OpenModel(const char* FileName);
    bool SaveModel(HANDLE handle, const char* path);
	void CloseModel(HANDLE handle);
    HANDLE CopyModel(HANDLE handle);



]]

local lib = ffi.load("modellib")

lib.InitMpqResource()

local modellib = require 'modellib.modellib'

local register_contariner = require 'modellib.contariner'

local model = {}

setmetatable(model, model)

model.__index = model

model.type = 'model'

function model.new()
	local handle = lib.CreateModel()

	return setmetatable({handle = handle}, model)
end


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


function model:close()
	if self.handle == nil then
		return
    end

	lib.CloseModel(self.handle)
    self.handle = nil

end

function model:copy()
    local handle = lib.CopyModel(self.handle)
    if handle == nil then 
        return 
    end 

    return setmetatable({handle = handle}, model)
end

function model:save(path)
    lib.SaveModel(self.handle, path)
end 



register_contariner(model, 'texture') --注册贴图容器
register_contariner(model, 'material') --注册材质容器


return model
