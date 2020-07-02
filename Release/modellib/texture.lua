local ffi = require 'ffi'

ffi.cdef[[
	typedef void* HANDLE;

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

local lib = ffi.load("modellib")

lib.LoadAllReplaceableTextures()

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

function texture:get_path()
    return ffi.string(lib.GetTexturePath(self.handle))
end 

function texture:set_path(path)
    lib.SetTexturePath(self.handle, path)
end 

function texture:set_replaceable_id(id)
    lib.SetTextureReplaceableId(self.handle, id)
end 

function texture:get_replaceable_id()
    return lib.GetTextureReplaceableId(self.handle)
end

function texture:set_wrap_width(bool)
    lib.SetTextureWrapWidth(self.handle, bool)
end

function texture:get_wrap_width()
    return lib.GetTextureWrapWidth(self.handle)
end

function texture:set_wrap_height(bool)
    lib.SetTextureWrapHeight(self.handle, bool)
end

function texture:get_wrap_height()
    return lib.GetTextureWrapHeight(self.handle)
end

function texture:get_width()
    return lib.GetTextureWidth(self.handle)
end

function texture:get_height()
    return lib.GetTextureHeight(self.handle)
end


function texture:__index(name)
    local func = rawget(texture, name)
    if func then return func end 
    
    if rawget(self, 'handle') then 
        local func = rawget(texture, 'get_' .. name)
        if func then return func(self) end 
    end 

    return rawget(self, name)
end 

function texture:__newindex(name, value)
    if rawget(self, 'handle') then
        local func = rawget(texture, 'set_' .. name)
        if func then return func(self, value) end 

        local func = rawget(texture, 'get_' .. name)
        if func then 
            print('nothing writer', name)
            return 
        end 
    end
    rawset(self, name, value)
end 


function texture:__tostring()
    local s = ''
    local path = self.path 
    if path and path:len() > 0 then 
        s = '[' .. path .. ']'
    elseif self.replaceable_id then 
        s = '[' .. self.replaceable_id .. ']'
    end 
    return "texture<" .. ("%x"):format(tonumber(self.handle)) .. '>' .. s
end 


return texture