

local lib = require 'modellib'



local model = lib.model.open("units\\other\\DranaiAkama\\DranaiAkama.mdl")



for texture in model:each_texture() do 
    print(texture)
end 

local model2 = model:copy()

for material in model2:each_material() do 
    print(material)
    for layer in material:each_layer() do 
        for k, v in pairs(layer) do 
            print(layer, k, v)
            if type(v) == 'boolean' then 
                layer[k] = true
            end
        end 
    end 
    --material:close()
end 

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