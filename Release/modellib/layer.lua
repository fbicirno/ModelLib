local ffi = require 'ffi'

local cdef = [[
	typedef void* HANDLE;

    HANDLE CreateMaterialLayer();
	HANDLE CopyMaterialLayer(HANDLE layerhandle);
	void CloseMaterialLayer(HANDLE layerhandle, bool del);
	HANDLE GetLayerByMaterial(HANDLE mathandle, int index);


    int GetLayerFilterMode(HANDLE  layerhandle);
	void SetLayerFilterMode(HANDLE  layerhandle, int filter_mode);
	int GetLayerTextureId(HANDLE  layerhandle);
	void SetLayerTextureId(HANDLE  layerhandle, int textureid);
	int GetLayerTextureAnimationId(HANDLE  layerhandle);
	void SetLayerTextureAnimationId(HANDLE  layerhandle, int animationid);
	bool GetLayerUnshaded(HANDLE layerhandle);
	void SetLayerUnshaded(HANDLE layerhandle, bool flag);
	bool GetLayerUnfogged(HANDLE layerhandle);
	void SetLayerUnfogged(HANDLE layerhandle, bool flag);
	bool GetLayerTwoSided(HANDLE layerhandle);
	void SetLayerTwoSided(HANDLE layerhandle, bool flag);
	bool GetLayerSphereEnvironmentMap(HANDLE layerhandle);
	void SetLayerSphereEnvironmentMap(HANDLE layerhandle, bool flag);
	bool GetLayerNoDepthTest(HANDLE layerhandle);
	void SetLayerNoDepthTest(HANDLE layerhandle, bool flag);
	bool GetLayerNoDepthSet(HANDLE layerhandle);
    void SetLayerNoDepthSet(HANDLE layerhandle, bool flag);
    
]]

ffi.cdef(cdef)

local lib = ffi.load("modellib")

local layer = {}

local modellib = require("modellib.modellib")

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
    local handle = lib.GetLayerByMaterial(material.handle, index)
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


modellib.auto_method(layer, cdef)

return layer