package.cpath = package.cpath .. ';bin/?.dll;'
package.path = package.path .. ';src/?.lua;src/?/init.lua;'

local lib = require 'modellib'


require 'examples.ui_model'
