

local lib = require 'modellib'



local model = lib.model.open("units\\other\\DranaiAkama\\DranaiAkama.mdl")

local model2 = model:copy()


--镜头
--for i = 1, 10 do 
--    local camera = lib.camera.new()
--    camera.source = {7, 8, 9}
--    camera.target = camera.source
--
--    print(camera.source, camera.source.x, camera.source.y, camera.source.z)
--    print(camera.target, camera.target.x, camera.target.y, camera.target.z)
--
--    model2:add_camera(camera)
--end 

for sequence in model2:each_sequence() do 
    print(sequence)
    for k, v in pairs(sequence) do 
        if type(v) == 'number' then 
            sequence[k] = 999
        end 
    end 
    sequence.name = sequence.name .. '_test'
    sequence.interval = {555, 666}
    sequence.extent = {
        radius = 888,
        min = {2, 2, 2},
        max = {3, 3, 3},
    }
end 

--贴图
--for texture in model:each_texture() do 
--    print(texture)
--end 


--for material in model2:each_material() do 
--    print(material)
--    for layer in material:each_layer() do 
--        for k, v in pairs(layer) do 
--            print(layer, k, v)
--            if type(v) == 'boolean' then 
--                layer[k] = true
--            end
--        end 
--    end 
--    --material:close()
--end 
--

--for camera in model2:each_camera() do 
--    print(camera)
--end
--for k, v in pairs(model2) do 
--    print(k, v)
--end 
--for material in model2:each_material() do 
--    material.priority_plance = 99
--    material.sort_Z = true 
--    material.full_resolution = true 
--    material.constant_color = true
--    print(material, '11111111111')
--    for k, v in pairs(material) do 
--        print(material, k, v)
--    end 
--end
--

--local texture = lib.texture.new()
--
--texture.path = "xxx.blp"
--
--model2:add_texture(texture)
--
--

--model2:clear_material()
--
--for i = 1, 10 do 
--    local material = lib.material.new()
--    model2:add_material(material)
--end 


model2:save("xxx2.mdx")
model2:close()

--model:save("xxx.mdx")
model:close()