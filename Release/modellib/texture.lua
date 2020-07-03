local ffi = require 'ffi'

local cdef = [[

    bool LoadAllReplaceableTextures();

	HANDLE CreateTexture();
    HANDLE GetTextureByModel(HANDLE modelhandle, int index);

    void CloseTexture(HANDLE texhandle, bool del);
    const char* GetTexturePath(HANDLE texhandle);
    void SetTexturePath(HANDLE texhandle, const char* path);

    bool GetTextureWrapWidth(HANDLE texhandle);
    void SetTextureWrapWidth(HANDLE texhandle, bool flag);
    bool GetTextureWrapHeight(HANDLE texhandle);
    void SetTextureWrapHeight(HANDLE texhandle, bool flag);
    int GetTextureReplaceableId(HANDLE texhandle);
    void SetTextureReplaceableId(HANDLE texhandle, int id);
    int GetTextureWidth(HANDLE texhandle);
	int GetTextureHeight(HANDLE texhandle);
]]

ffi.cdef(cdef)


local lib = ffi.load("modellib")

lib.LoadAllReplaceableTextures()


local modellib = require 'modellib.modellib'


local texture = {}

setmetatable(texture, texture)

texture.type = 'texture'

texture.all_texture_map = {}

function texture.new()
    local handle = lib.CreateTexture()

    local object = setmetatable({handle = handle}, texture)

    texture.all_texture_map[handle] = object
    return object
end 

function texture.open(model, index)
    local handle = lib.GetTextureByModel(model.handle, index)
    if handle == nil then 
        return 
    end 

    local object = texture.all_texture_map[tonumber(handle)]
    if object then 
        return object
    end 

    object = setmetatable({handle = handle, owner = model}, texture)

    texture.all_texture_map[tonumber(handle)] = object
    return object
end 

function texture:copy()
    local object = texture.new()
    object.path = self.path 
    object.wrap_width = self.wrap_width
    object.wrap_height = self.wrap_height 
    object.replaceable_id = self.replaceable_id
    return object
end 

function texture:close()
    if self.handle == nil then 
        return
    end 

    --关闭贴图时 没有所属模型的贴图要被销毁
    lib.CloseTexture(self.handle, self.owner == nil)

    texture.all_texture_map[tonumber(self.handle)] = nil 

    self.handle = nil
end 


modellib.auto_method(texture, cdef)

return texture