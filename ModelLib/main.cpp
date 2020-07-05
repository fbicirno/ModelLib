#include <iostream>
//#include <lua.hpp>
#include <map>
#include <string>
#include <model.h>

#include <string>
#include <direct.h> //_mkdir函数的头文件
#include <io.h>     //_access函数的头文件

using namespace std;

//void CreateDir(string str1)
//{
//	int m = 0, n;
//	string str2;
//
//	INT i = static_cast<INT>(str1.rfind("\\"));
//	if (i != std::string::npos)
//	{
//		str1 = str1.substr(0, i);
//	}
//
//
//	str2 = str1.substr(0, 2);
//	str1 = str1.substr(3, str1.size());
//
//	while (m >= 0)
//	{
//		m = str1.find('\\');
//
//		str2 += '\\' + str1.substr(0, m);
//		n = _access(str2.c_str(), 0); //判断该目录是否存在
//		if (n == -1)
//		{
//			_mkdir(str2.c_str());     //创建目录
//		}
//
//		str1 = str1.substr(m + 1, str1.size());
//	}
//}
//
//
//std::map<int, MODEL*> model_addr_map;
//
//int open_model(lua_State* L)
//{
//	if (!lua_isstring(L, 1))
//	{
//		lua_pushnil(L);
//		return 1;
//	}
//		
//	std::string FileName = lua_tostring(L, 1);
//	BUFFER Buffer;
//
//	if (!FileLoader.Load(FileName, Buffer))
//	{
//		lua_pushnil(L);
//		return 1;
//	}
//	MODEL* model = new MODEL;
//	if (!ResourceLoader.LoadModel(*model, FileName, Buffer))
//	{
//		delete model;
//		lua_pushnil(L);
//		return 1;
//	}
//	//model->CalculateBoundsRadius();
//	int id = (int)model;
//	model_addr_map[id] = model;
//	
//	lua_pushinteger(L, id);
//	return 1;
//}
//
//int close_model(lua_State* L)
//{
//	if (!lua_isinteger(L, 1))
//		return 0;
//	int id = lua_tointeger(L, 1);
//	auto it = model_addr_map.find(id);
//	if (it == model_addr_map.end())
//		return 0;
//	delete(it->second);
//	model_addr_map.erase(it);
//
//	return 0;
//}
//
//int save_model(lua_State* L)
//{
//	if (!lua_isinteger(L, 1))
//		return 0;
//	if (!lua_isstring(L, 2))
//		return 0;
//
//	int id = lua_tointeger(L, 1);
//	const char* path = lua_tostring(L, 2);
//
//
//	auto it = model_addr_map.find(id);
//	if (it == model_addr_map.end())
//		return 0;
//	string out_path = Common.GetProgramDirectory() + "\\" + path;
//	CreateDir(out_path);
//
//	MODEL* model = it->second;
//
//	BUFFER Buffer;
//
//	CreateDir(Common.GetProgramDirectory() + "\\" + path);
//
//	if (ResourceLoader.SaveModel(*model, path, Buffer))
//	{
//		if (!FileLoader.SaveToFile(path, Buffer))
//		{
//			printf("save error %s\n", path);
//		}
//	}
//	else
//	{
//		printf("save error %s\n", path);
//	}
//
//	return 0;
//}
//
//int save_screen_model(lua_State* L)
//{
//	if (!lua_isinteger(L, 1))
//		return 0;
//	if (!lua_isstring(L, 2))
//		return 0;
//
//	int id = lua_tointeger(L, 1);
//	const char* path = lua_tostring(L, 2);
//
//
//	auto it = model_addr_map.find(id);
//	if (it == model_addr_map.end())
//		return 0;
//	string out_path = Common.GetProgramDirectory() + "\\" + path;
//
//
//	CreateDir(out_path);
//
//	MODEL* model = new MODEL(*it->second);
//
//	BUFFER Buffer;
//
//
//	auto& material_list = model->Data().MaterialContainer;
//
//	//遍历所有材质图层 将图层改为 无阴影
//	for (int i = 0; i < material_list.GetTotalSize(); i++)
//	{
//		auto& material = material_list[i]->Data();
//
//		auto& layer_list = material.LayerContainer;
//		
//		for (int i = 0; i < layer_list.GetTotalSize(); i++)
//		{
//			auto& layer = layer_list[i]->Data();
//			layer.Unshaded = true;
//		}
//	}
//
//
//	
//	auto& particle_list = model->Data().ParticleEmitter2Container;
//
//	//遍历所有节点 将粒子发射器 缩小2000倍
//	for (int i = 0; i < particle_list.GetTotalSize(); i++)
//	{
//		auto& particle = particle_list[i];
//
//		auto& scale = particle->Data().ParticleScaling;
//		scale.x /= 2000;
//		scale.y /= 2000;
//		scale.z /= 2000;
//
//	}
//
//	auto& camera_list = model->Data().CameraContainer;
//
//	//删除所有镜头
//	for (int i = 0; i < camera_list.GetTotalSize(); i++)
//	{
//		//auto& camera = camera_list[i];
//		camera_list.Remove(i);
//	}
//
//
//	auto& sequences = model->Data().SequenceContainer;
//	for (int i = 0; i < sequences.GetTotalSize(); i++)
//	{
//		auto& data = sequences[i]->Data();
//
//		data.Extent.Min = {0,0,0};
//		data.Extent.Max = { 0, 0, 0 };
//	}
//	//模型重算点范围
//	model->CalculateBoundsRadius();
//
//
//	if (!ResourceLoader.SaveModel(*model, out_path, Buffer))
//	{
//		delete model;
//		printf("save error %s\n", path);
//		return 0;
//	}
//
//	delete model;
//
//	if (!FileLoader.SaveToFile(out_path, Buffer))
//	{
//		printf("save write error %s\n", path);
//		return 0;
//	}
//	
//	return 0;
//}
//
//int load_model_animate(lua_State* L)
//{
//	if (!lua_isinteger(L, 1))
//	{
//		lua_pushnil(L);
//		return 1;
//	}
//		
//	int id = lua_tointeger(L, 1);
//	auto it = model_addr_map.find(id);
//	if (it == model_addr_map.end())
//	{
//		lua_pushnil(L);
//		return 1;
//	}
//	MODEL* model = it->second;
//	auto& sequences = model->Data().SequenceContainer;
//	char buffer[0x100];
//	lua_newtable(L);
//	for (int i = 0; i < sequences.GetTotalSize(); i++)
//	{
//		auto& data = sequences[i]->Data();
//		lua_newtable(L);
//		
//		lua_pushstring(L, data.Name.c_str());
//		lua_setfield(L, -2, "name");
//		
//		lua_pushinteger(L, data.Interval.x);
//		lua_setfield(L, -2, "start");
//		
//		lua_pushinteger(L, data.Interval.y);
//		lua_setfield(L, -2, "end");
//		
//		lua_rawseti(L, -2, i + 1);
//	}
//	return 1;
//}
//
//int load_model_texture_list(lua_State* L)
//{
//	if (!lua_isinteger(L, 1))
//	{
//		lua_pushnil(L);
//		return 1;
//	}
//
//	int id = lua_tointeger(L, 1);
//	auto it = model_addr_map.find(id);
//	if (it == model_addr_map.end())
//	{
//		lua_pushnil(L);
//		return 1;
//	}
//
//	MODEL* model = it->second;;
//	auto& container = model->Data().TextureContainer;
//	char buffer[0x100];
//	lua_newtable(L);
//	for (int i = 0; i < container.GetTotalSize(); i++)
//	{
//		auto& data = container[i]->Data();
//		lua_pushstring(L, data.FileName.c_str());
//		lua_rawseti(L, -2, i +1);
//	}
//	return 1;
//}
//
//int save_mpq_file(lua_State* L)
//{
//	if (!lua_isstring(L, 1))
//		return 0;
//	if (!lua_isstring(L, 2))
//		return 0;
//	string path = lua_tostring(L, 1);
//	string path2 = lua_tostring(L, 2);
//	string out_path = Common.GetProgramDirectory() + "\\" + path2;
//	CreateDir(out_path);
//
//	BUFFER Buffer;
//
//	if (!FileLoader.Load(path, Buffer))
//	{
//		printf("save load write error %s\n", path);
//		return 0;
//	}
//	if (!FileLoader.SaveToFile(out_path, Buffer))
//	{
//		printf("save write error %s\n", path);
//		return 0;
//	}
//
//
//	return 0;
//}
//
//int merge_animate(lua_State* L)
//{
//	if (!lua_isinteger(L, 1))
//		return 0;
//	if (!lua_isinteger(L, 2))
//		return 0;
//
//	int target_id = lua_tointeger(L, 1);
//	int source_id = lua_tointeger(L, 2);
//
//	auto target_it = model_addr_map.find(target_id);
//	if (target_it == model_addr_map.end())
//	{
//		return 0;
//	}
//
//	auto source_it = model_addr_map.find(source_id);
//	if (source_it == model_addr_map.end())
//	{
//		return 0;
//	}
//	
//	MODEL* source = source_it->second;
//	MODEL* target = new MODEL(*target_it->second);
//	
//	std::map<std::string, MODEL_BONE_DATA*> bone_map;
//
//	for (int i = 0; i < source->Data().BoneContainer.GetTotalSize(); i++)
//	{
//		auto& data = source->Data().BoneContainer[i]->Data();
//		auto name = data.Name;
//		if (name.find("Bone_") != std::string::npos) {
//			name = name.substr(5, name.length());
//		}
//		std::transform(name.begin(), name.end(), name.begin(), std::tolower);
//
//		bone_map[name] = &data;
//	}
//
//	std::map<std::string, MODEL_SEQUENCE_DATA*> sequence_map;
//
//	for (int i = 0; i < target->Data().SequenceContainer.GetTotalSize(); i++)
//	{
//		auto& data = target->Data().SequenceContainer[i]->Data();
//		sequence_map[data.Name] = &data;
//	}
//
//
//	for (int i = 0; i < target->Data().BoneContainer.GetTotalSize(); i++)
//	{
//		auto& target_data = target->Data().BoneContainer[i]->Data();
//		auto name = target_data.Name;
//		if (name.find("Bone_") != std::string::npos) {
//			name = name.substr(5, name.length());
//		}
//
//		std::transform(name.begin(), name.end(), name.begin(), std::tolower);
//
//		if (bone_map.find(name) == bone_map.end())
//		{
//			//printf("target %s nothing\n", name.c_str());
//			continue;
//		}
//		printf("%s has\n", name.c_str());
//
//		auto& source_data = *bone_map[name];
//		
//		
//		auto& target_rotation = target_data.Rotation;
//
//		auto& source_rotation = source_data.Rotation;
//
//		if (source_rotation.NodeList.size() > 0) {
//			auto it = source_rotation.NodeList.begin();
//			while (it != source_rotation.NodeList.end())
//			{
//				auto node = *it;
//				node.Time = it->Time + 1000000;
//				target_rotation.AddNode(node);
//				it++;
//			}
//		}
//	}
//
//
//	for (int i = 0; i < source->Data().SequenceContainer.GetTotalSize(); i++)
//	{
//
//		auto& data = source->Data().SequenceContainer[i]->Data();
//		std::string name = data.Name;
//		//if (sequence_map.find(name) != sequence_map.end()) {
//			name = "copy " + name;
//		//}
//		auto seq = new MODEL_SEQUENCE(*target->Data().SequenceContainer[i]);
//		seq->Data().Interval.x += 1000000;
//		seq->Data().Interval.y += 1000000;
//		seq->Data().Name = name;
//		target->Data().SequenceContainer.Add(seq);
//	}
//
//
//
//	lua_pushboolean(L, true);
//	return 1;
//}
//
//int open(lua_State* L)
//{
//	lua_newtable(L);
//	{
//		lua_newtable(L);
//		{
//
//		}
//		lua_setmetatable(L, -2);
//
//		{
//			luaL_Reg func[] = {
//				{ "open_model", open_model },
//				{ "close_model", close_model },
//				{ "load_animate", load_model_animate },
//				{ "load_texture_list", load_model_texture_list },
//
//				{ "save_model", save_model },
//				{ "save_screen_model", save_screen_model },
//				{ "save_mpq_file", save_mpq_file },
//				{ "merge_animate", merge_animate },
//				
//				{ NULL, NULL },
//			};
//			luaL_setfuncs(L, func, 0);
//		}
//
//
//	}
//	return 1;
//}

//int main()
//{
//	
//	if (!InitEditor())
//	{
//		printf("编辑器初始化失败\n");
//		system("pause");
//		return 0;
//	}
//	
//
//	lua_State* L = luaL_newstate();
//	luaL_openlibs(L);
//
//	lua_getfield(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
//	lua_pushcclosure(L, open, 0);
//	lua_setfield(L, -2, "model.core");
//	lua_pop(L, 1);
//
//	lua_getglobal(L, "require");
//	lua_pushstring(L, "script.main");
//	lua_pcall(L, 1, 0,0);
//
//	system("pause");
//	return 0;
//}

#include "modellib.h"


BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	return TRUE;
}


