local ffi = require 'ffi'

local cdef = [[

	HANDLE CreateCamera();
	HANDLE CopyCamera(HANDLE camhandle);
    void CloseCamera(HANDLE camhandle, bool del);
    

    const char* GetCameraName(HANDLE  camhandle);
	void SetCameraName(HANDLE  camhandle, const char* name);
	VECTOR3* GetCameraSource(HANDLE  camhandle);
	void SetCameraSource(HANDLE  camhandle, VECTOR3* value);
	VECTOR3* GetCameraTarget(HANDLE  camhandle);
	void SetCameraTarget(HANDLE  camhandle, VECTOR3* value);
	float GetCameraFieldOfView(HANDLE  camhandle);
	void SetCameraFieldOfView(HANDLE  camhandle, float value);
	float GetCameraNearDistance(HANDLE  camhandle);
	void SetCameraNearDistance(HANDLE  camhandle, float value);
	float GetCameraFarDistance(HANDLE  camhandle);
	void SetCameraFarDistance(HANDLE  camhandle, float value);
    
]]

ffi.cdef(cdef)

local lib = ffi.load("modellib")


local modellib = require 'modellib.modellib'

local camera = {}

setmetatable(camera, camera)

camera.type = 'camera'

camera.all_camera_map = {}

function camera.new()
    local handle = lib.CreateCamera()

    local object = setmetatable({handle = handle}, camera)

    camera.all_camera_map[tonumber(handle)] = object

    return object
end 


function camera.open(model, index)
    local handle = lib.GetCameraByModel(model.handle, index)
    if handle == nil then 
        return 
    end 
    local object = camera.all_camera_map[tonumber(handle)]
    if object then 
        return object 
    end 

    object = setmetatable({handle = handle, owner = model}, camera)

    camera.all_camera_map[tonumber(handle)] = object

    return object
end 


function camera:close()
    if self.handle == nil then 
        return 
    end 

  
    lib.CloseCamera(self.handle, self.owner == nil)
    
    camera.all_camera_map[tonumber(self.handle)] = nil
end 


function camera:copy()
    if self.handle == nil then 
        return 
    end 

    local handle = lib.CopyCamera(self.handle)

    local object = setmetatable({handle = handle}, camera)

    camera.all_camera_map[tonumber(handle)] = object

    return object
end 




modellib.auto_method(camera, cdef)


return camera