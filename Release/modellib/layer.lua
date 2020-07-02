local ffi = require 'ffi'

ffi.cdef[[
	typedef void* HANDLE;

    HANDLE CreateMaterialLayer();
	HANDLE CopyMaterialLayer(HANDLE layerhandle);
	void CloseMaterialLayer(HANDLE layerhandle, bool del);
	HANDLE GetMaterialLayerByMaterial(HANDLE mathandle, int index);


]]

local lib = ffi.load("modellib")

local layer = {}

setmetatable(layer, layer)

layer.type = 'layer'

layer.all_layer_map = {}

function layer.new()
    local handle = lib.CreateMaterialLayer()

    local object = setmetatable({handle = handle}, layer)

    layer.all_layer_map[tonumber(handle)] = object

    return object
end 


function layer.open(material, index)
    local handle = lib.GetMaterialLayerByMaterial(material.handle, index)
    if handle == nil then 
        return 
    end 
    local object = layer.all_layer_map[tonumber(handle)]
    if object then 
        return object 
    end 

    object = setmetatable({handle = handle, owner = material}, layer)

    layer.all_layer_map[tonumber(handle)] = object

    return object
end 


function layer:close()
    if self.handle == nil then 
        return 
    end 

  
    lib.CloseMaterialLayer(self.handle, self.owner == nil)
    
    layer.all_layer_map[tonumber(self.handle)] = nil
end 


function layer:copy()
    if self.handle == nil then 
        return 
    end 

    local handle = lib.CopyMaterialLayer(self.handle)

    local object = setmetatable({handle = handle}, layer)

    layer.all_layer_map[tonumber(handle)] = object

    return object
end 


return layer