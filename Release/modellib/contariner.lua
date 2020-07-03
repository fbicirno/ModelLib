local ffi = require 'ffi'

local cdef = [[

    //贴图容器
    int GetModelTextureSize(HANDLE handle);
    bool AddModelTexture(HANDLE modelhandle, HANDLE texhandle);
    bool RemoveModelTexture(HANDLE modelhandle, HANDLE  texhandle);

    //材质容器
    int GetModelMaterialSize(HANDLE modelhandle);
    bool AddModelMaterial(HANDLE modelhandle, HANDLE mathandle);
    bool RemoveModelMaterial(HANDLE modelhandle, HANDLE  mathandle);


    //材质图层容器
	int GetMaterialLayerSize(HANDLE mathandle);
	bool AddMaterialLayer(HANDLE mathandle, HANDLE layerhandle);
	bool RemoveMaterialLayer(HANDLE mathandle, HANDLE  layerhandle);


    //镜头容器
    HANDLE GetCameraByModel(HANDLE modelhandle, int index);
	int GetModelCameraSize(HANDLE modelhandle);
	bool AddModelCamera(HANDLE modelhandle, HANDLE camhandle);
	bool RemoveModelCamera(HANDLE modelhandle, HANDLE  camhandle);
    
]]

ffi.cdef(cdef)

local lib = ffi.load("modellib")

local modellib = require 'modellib.modellib'


--给模块注册容器 
--module: object  model|texture|materiable 这些模块 
--child_name: string 子模块名  
--[[
    --例如
    modellib.contariner(modellib.model, 'texture') 
    会生成一套  容器模板
        model:get_texture_by_model(index):texture
        model:get_texture_size():int
        model:add_texture(texture):bool
        model:remove_texture(texture):bool
        model:clear_texture():bool
        model:each_texture() --iterator

]]

function modellib.contariner(module, child_name)
    local module_name = module.type

    local module_name2 = module_name:sub(1, 1):upper() .. module_name:sub(2, module_name:len()) --首字母转大写

    local child_name2 = child_name:sub(1, 1):upper() .. child_name:sub(2, child_name:len()) --首字母转大写
    
    local name = module_name2 .. child_name2

    local get_child_by_module = 'get_' .. child_name .. '_by_' .. module_name
    local get_child_size = 'get_' .. child_name .. '_size'

    --查询容器尺寸
    module[get_child_size] = function (self)
        return lib['Get'.. name .. 'Size'](self.handle)
    end 

    --获取容器指定索引元素
    module[get_child_by_module] = function (self, index)
        local object = modellib[child_name].open(self, index)
        self:contariner_add(object, index)
        return object
    end 

    local i = 0
    --为容器添加新元素
    module['add_' .. child_name] = function (self, object)
        if object.owner then 
            print('对象已经有所属模型 需要new 或者 copy一个新的对象')
            return
        end 
        i = i + 1
        object.owner = self

        local bool = lib['Add'.. name](self.handle, object.handle)

        if bool then 
            local index = self[get_child_size](self) - 1
            self:contariner_add(object, index)
        end 
        return bool
    end 

    --为容器删除指定元素
    module['remove_' .. child_name] = function (self, object)
        if object.owner ~= self then 
            return 
        end 
        local bool = lib['Remove'.. name](self.handle, object.handle)
        self:contariner_remove(object)
    end


    --容器迭代器
    module['each_' .. child_name] = function (self)
        local size = self[get_child_size](self) 

        local result = {}
        for i = 0, size - 1 do 
            table.insert(result,  self[get_child_by_module](self, i))
        end 

        local n = 0
    	return function (t, v)
    		n = n + 1
    		return t[n]
    	end, result
    end 

    --清空容器
    module['clear_' .. child_name] = function (self)
        for object in self['each_' .. child_name](self) do 
            self['remove_' .. child_name](self, object)
        end 
    end


    
    function module:contariner_add(object)
        if object == nil or object.type == nil then 
            return 
        end 
    
        local contariner = rawget(self,object.type .. '_contariner')
        if contariner == nil then 
            contariner = {}
            self[object.type .. '_contariner'] = contariner
        end 
    
        contariner[object] = true
    end
    
    function module:contariner_remove(object)
        if object == nil or object.type == nil then 
            return 
        end 
    
        local contariner = rawget(self,object.type .. '_contariner')
        if contariner == nil then 
            return 
        end 
        contariner[object] = nil
    end

    local close = module.close
    function module:close()
        close(self)

        for key, value in pairs(self) do 
            if key:find('_contariner') and type(value) == 'table' then 
                for object in pairs(value) do 
                    object:close()
                end 
            end
            self[key] = nil
        end 
    end 
end 

