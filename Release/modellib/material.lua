local ffi = require 'ffi'

ffi.cdef[[
	typedef void* HANDLE;

    HANDLE CreateMaterial();
    HANDLE CopyMaterial(HANDLE mathandle);
    HANDLE GetMaterialByModel(HANDLE modelhandle, int index);
    void CloseMaterial(HANDLE mathandle, bool del);

    int GetMaterialPriorityPlane(HANDLE mathandle);
	void SetMaterialPriorityPlane(HANDLE mathandle, int PriorityPlane);
	bool GetMaterialConstantColor(HANDLE mathandle);
	void SetMaterialConstantColor(HANDLE mathandle, bool ConstantColor);
	bool GetMaterialSortPrimitivesFarZ(HANDLE mathandle);
	void SetMaterialSortPrimitivesFarZ(HANDLE mathandle, bool SortPrimitivesFarZ);
	bool GetMaterialFullResolution(HANDLE mathandle);
    void SetMaterialFullResolution(HANDLE mathandle, bool FullResolution);
    
]]

local lib = ffi.load("modellib")

local register_contariner = require 'modellib.contariner'

local material = {}

setmetatable(material, material)

material.type = 'material'

material.all_material_map = {}

function material.new()
    local handle = lib.CreateMaterial()

    local object = setmetatable({handle = handle}, material)

    material.all_material_map[tonumber(handle)] = object

    return object
end 


function material.open(model, index)
    local handle = lib.GetMaterialByModel(model.handle, index)
    if handle == nil then 
        return 
    end 
    local object = material.all_material_map[tonumber(handle)]
    if object then 
        return object 
    end 

    object = setmetatable({handle = handle, owner = model}, material)

    material.all_material_map[tonumber(handle)] = object

    return object
end 


function material:close()
    if self.handle == nil then 
        return 
    end 

  
    lib.CloseMaterial(self.handle, self.owner == nil)
    
    material.all_material_map[tonumber(self.handle)] = nil
end 


function material:copy()
    if self.handle == nil then 
        return 
    end 

    local handle = lib.CopyMaterial(self.handle)

    local object = setmetatable({handle = handle}, material)

    material.all_material_map[tonumber(handle)] = object

    return object
end 


--平面优先级
function material:set_priority_plance(num)
    lib.SetMaterialPriorityPlane(self.handle, num)
end 

function material:get_priority_plance()
    return lib.GetMaterialPriorityPlane(self.handle)
end 

--固定颜色
function material:set_constant_color(bool)
    lib.SetMaterialConstantColor(self.handle, bool)
end 
function material:get_constant_color()
    return lib.GetMaterialConstantColor(self.handle)
end 

--延z轴排列
function material:set_sort_Z(bool)
    lib.SetMaterialSortPrimitivesFarZ(self.handle, bool)
end 
function material:get_sort_Z()
    return lib.GetMaterialSortPrimitivesFarZ(self.handle)
end 

--最大分辨率
function material:set_full_resolution(bool)
    lib.SetMaterialFullResolution(self.handle, bool)
end 
function material:get_full_resolution()
    return lib.GetMaterialFullResolution(self.handle)
end 






register_contariner(material, 'layer')

return material