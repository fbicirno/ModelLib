local modellib = require 'modellib.modellib'
require 'modellib.contariner'

local components = {
    'interpolator','model', 'texture', 'material', 'layer', 'camera', 'sequence',
    'attachment', 'bone', 'collisionshape','eventobject', 'helper', 'light', 
    'particle', 'particle2'
}

--加载指定模块 并注册元方法
for index, component in ipairs(components) do 
    modellib[component] = require ('modellib.' .. component)
end 



return modellib