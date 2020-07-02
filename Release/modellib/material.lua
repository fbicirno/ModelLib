local ffi = require 'ffi'

local cdef = [[
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

ffi.cdef(cdef)

local lib = ffi.load("modellib")


local modellib = require 'modellib.modellib'

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


--材质有 多层图层
modellib.contariner(material, 'layer')

modellib.auto_method(material, cdef)


return material