#include <iostream>
#include <map>
#include <string>
#include <model.h>

#include <string>
#include <direct.h> //_mkdir函数的头文件
#include <io.h>     //_access函数的头文件

#include "modellib.h"

std::map<HANDLE, void*> handle_map;

HANDLE convert_handle(void* object)
{
	handle_map[(HANDLE)object] = object;
	return (HANDLE)object;
}

void* convert_object(HANDLE handle)
{
	auto it = handle_map.find(handle);
	if (it == handle_map.end())
		return nullptr;

	return it->second;
}

void close_handle(HANDLE handle)
{
	handle_map.erase(handle);
}



bool InitMpqResource()
{
	if (!Register.FindWarcraftDirectory())
		return FALSE;
	Graphics.Setup();

	bool Result = FALSE;
	std::string path;
	std::string ErrorMessage;
	std::string WarcraftDirectory;

	WarcraftDirectory = Register.GetWarcraftDirectory();

	while (TRUE)
	{
		path = WarcraftDirectory + "\\" + PATH_MPQ_WAR3;
		if (!MpqWar3.Open(path)) break;

		path = WarcraftDirectory + "\\" + PATH_MPQ_WAR3X;
		if (!MpqWar3x.Open(path)) break;

		path = WarcraftDirectory + "\\" + PATH_MPQ_WAR3X_LOCAL;
		if (!MpqWar3xLocal.Open(path)) break;

		path = WarcraftDirectory + "\\" + PATH_MPQ_WAR3_PATCH;
		if (!MpqWar3Patch.Open(path)) break;

		Result = TRUE;
		break;
	}

	if (!Result)
	{
		ErrorMessage = "Unable to open \"" + path + "\"!\n\n";
		ErrorMessage += "Make sure that Warcraft 3 is installed and that the registry key\n";
		ErrorMessage += "\"HKEY_CURRENT_USER\\Blizzard Entertainment\\Warcraft III\\InstallPath\" exists!";
		Error.SetMessage(ErrorMessage);
		return FALSE;
	}

	ResourceLoader.RegisterAllLoaders();
	return true;
}

bool LoadAllReplaceableTextures()
{
	return TextureManager.LoadAllReplaceableTextures();
}

HANDLE CreateModel()
{
	MODEL* model = new MODEL;
	return convert_handle(model);
}

HANDLE OpenModel(const char* path)
{
	BUFFER Buffer;

	if (!FileLoader.Load(path, Buffer))
		return nullptr;
	
	MODEL* model = new MODEL;
	if (!ResourceLoader.LoadModel(*model, path, Buffer))
	{
		delete model;
		return nullptr;
	}
	
	return convert_handle(model);
}

bool SaveModel(HANDLE handle, const char* path)
{
	MODEL* model = (MODEL*)convert_object(handle);
	if (!model) 
	{
		return false;
	}
		

	BUFFER Buffer;

	if (!ResourceLoader.SaveModel(*model, path, Buffer))
	{
		return false;
	}

	if (!FileLoader.SaveToFile(path, Buffer))
	{
		return false;
	}
	return true;
}

void CloseModel(HANDLE handle)
{
	MODEL* model = (MODEL*)convert_object(handle);
	if (!model)
	{
		return;
	}

	close_handle(handle);

	delete model;
}

HANDLE CopyModel(HANDLE handle)
{
	MODEL* model = (MODEL*)convert_object(handle);
	if (!model)
	{
		return 0;
	}

	MODEL* copy = new MODEL(*model);
	return convert_handle(copy);
}


HANDLE CreateTexture()
{
	MODEL_TEXTURE* texture = new MODEL_TEXTURE();
	return convert_handle(texture);
}

HANDLE GetTextureByModel(HANDLE handle, int index)
{
	MODEL* model = (MODEL*)convert_object(handle);
	if (!model)
	{
		return 0;
	}
	auto& container = model->Data().TextureContainer;
	if (index >= 0 && index < container.GetSize())
	{
		auto texture = container[index];

		return convert_handle(texture);
	}

	return 0;
}

int GetModelTextureSize(HANDLE handle)
{
	MODEL* model = (MODEL*)convert_object(handle);
	if (!model)
	{
		return 0;
	}
	return model->Data().TextureContainer.GetSize();
}

bool AddModelTexture(HANDLE modelhandle, HANDLE texhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return false;
	}

	model->AddTexture(texture);
	return true;
}

bool RemoveModelTexture(HANDLE modelhandle, HANDLE  texhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return false;
	}
	close_handle(texhandle);
	model->RemoveTexture(texture);
	return true;

}

void CloseTexture(HANDLE texhandle, bool del)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);

	close_handle(texhandle);
	if (texture && del)
	{
		delete texture;
	}
}

const char* GetTexturePath(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return "";
	}

	return texture->Data().FileName.c_str();
}

void SetTexturePath(HANDLE texhandle, const char* path)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return;
	}

	texture->Data().FileName = path;
}

bool GetTextureWrapWidth(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}

	return texture->Data().WrapWidth;
}

void SetTextureWrapWidth(HANDLE texhandle, bool flag)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return ;
	}

	texture->Data().WrapWidth = flag;
}

bool GetTextureWrapHeight(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}

	return texture->Data().WrapHeight;
}

void SetTextureWrapHeight(HANDLE texhandle, bool flag)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return;
	}

	texture->Data().WrapHeight = flag;
}


int GetTextureWidth(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}
	int width = 0;

	TextureManager.Load(texture->Data().FileName);
	TEXTURE* tex = TextureManager.GetTexture(texture->Data().FileName);
	if (tex)
	{
		width = tex->GetWidth();
		TextureManager.Unload(texture->Data().FileName);
	}
	else
	{
		tex = TextureManager.GetReplaceableTexture(texture->Data().ReplaceableId);
		if (tex) {
			width = tex->GetWidth();
		}
	}

	return width;
}

int GetTextureHeight(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}
	int height = 0;

	TextureManager.Load(texture->Data().FileName);
	TEXTURE* tex = TextureManager.GetTexture(texture->Data().FileName);
	if (tex)
	{
		height = tex->GetHeight();
		TextureManager.Unload(texture->Data().FileName);
	}
	else
	{
		tex = TextureManager.GetReplaceableTexture(texture->Data().ReplaceableId);
		if (tex) {
			height = tex->GetHeight();
		}
	}

	return height;
}

bool GetTexturHeight(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}

	return texture->Data().WrapHeight;
}





int GetTextureReplaceableId(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}

	return texture->Data().ReplaceableId;
}

void SetTextureReplaceableId(HANDLE texhandle, int id)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return;
	}

	texture->Data().ReplaceableId = id;

	
}


HANDLE CreateMaterial()
{
	MODEL_MATERIAL* material = new MODEL_MATERIAL;

	return convert_handle(material);
}

HANDLE CopyMaterial(HANDLE mathandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);

	if (!material)
		return 0;

	MODEL_MATERIAL* copy = new MODEL_MATERIAL(*material);

	return convert_handle(material);
}

void CloseMaterial(HANDLE mathandle, bool del)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	
	close_handle(mathandle);
	if (material && del)
	{
		delete material;
	}
}

HANDLE GetMaterialByModel(HANDLE modelhandle,int index)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	auto& container = model->Data().MaterialContainer;
	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetModelMaterialSize(HANDLE modelhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	return model->Data().MaterialContainer.GetSize();
}

bool AddModelMaterial(HANDLE modelhandle, HANDLE mathandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);

	if (!material)
	{
		return 0;
	}
		
	return model->AddMaterial(material);
}

bool RemoveModelMaterial(HANDLE modelhandle, HANDLE  mathandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return false;
	}

	close_handle(mathandle);
	model->RemoveMaterial(material);
	return true;

}

int GetMaterialPriorityPlane(HANDLE mathandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}
	return material->Data().PriorityPlane;
}

void SetMaterialPriorityPlane(HANDLE mathandle, int PriorityPlane)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return ;
	}
	material->Data().PriorityPlane = PriorityPlane;
}

bool GetMaterialConstantColor(HANDLE mathandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}
	return material->Data().ConstantColor;
}

void SetMaterialConstantColor(HANDLE mathandle, bool ConstantColor)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return;
	}
	material->Data().ConstantColor = ConstantColor;
}

bool GetMaterialSortPrimitivesFarZ(HANDLE mathandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}
	return material->Data().SortPrimitivesFarZ;
}


void SetMaterialSortPrimitivesFarZ(HANDLE mathandle, bool SortPrimitivesFarZ)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return;
	}
	material->Data().SortPrimitivesFarZ = SortPrimitivesFarZ;
}

bool GetMaterialFullResolution(HANDLE mathandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}
	return material->Data().FullResolution;
}


void SetMaterialFullResolution(HANDLE mathandle, bool FullResolution)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return;
	}
	material->Data().FullResolution = FullResolution;
}




HANDLE CreateMaterialLayer()
{
	MODEL_MATERIAL_LAYER* layer = new MODEL_MATERIAL_LAYER;

	return convert_handle(layer);
}

HANDLE CopyMaterialLayer(HANDLE layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
		return 0;

	MODEL_MATERIAL_LAYER* copy = new MODEL_MATERIAL_LAYER(*layer);

	return convert_handle(layer);
}

void CloseMaterialLayer(HANDLE layerhandle, bool del)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	close_handle(layerhandle);
	if (layer && del)
	{
		delete layer;
	}
}

HANDLE GetLayerByMaterial(HANDLE mathandle, int index)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}

	auto& container = material->Data().LayerContainer;

	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetMaterialLayerSize(HANDLE mathandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}

	auto& container = material->Data().LayerContainer;
	return container.GetSize();
}

bool AddMaterialLayer(HANDLE mathandle, HANDLE layerhandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}

	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}

	return material->AddLayer(layer);
}

bool RemoveMaterialLayer(HANDLE mathandle, HANDLE  layerhandle)
{
	MODEL_MATERIAL* material = (MODEL_MATERIAL*)convert_object(mathandle);
	if (!material)
	{
		return 0;
	}
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	close_handle(mathandle);
	return material->RemoveLayer(layer);

}

int GetLayerFilterMode(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().FilterMode;
}

void SetLayerFilterMode(HANDLE  layerhandle, int filter_mode)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return ;
	}
	layer->Data().FilterMode = (FILTER_MODE)filter_mode;
}


int GetLayerTextureId(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().TextureId;
}

void SetLayerTextureId(HANDLE  layerhandle, int textureid)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().TextureId = textureid;
}


int GetLayerTextureAnimationId(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().TextureAnimationId;
}

void SetLayerTextureAnimationId(HANDLE  layerhandle, int animationid)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().TextureAnimationId = animationid;
}



bool GetLayerUnshaded(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().Unshaded;
}

void SetLayerUnshaded(HANDLE  layerhandle, bool flag)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().Unshaded = flag;
}

bool GetLayerUnfogged(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().Unfogged;
}

void SetLayerUnfogged(HANDLE  layerhandle, bool flag)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().Unfogged = flag;
}

bool GetLayerTwoSided(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().TwoSided;
}

void SetLayerTwoSided(HANDLE  layerhandle, bool flag)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().TwoSided = flag;
}

bool GetLayerSphereEnvironmentMap(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().SphereEnvironmentMap;
}

void SetLayerSphereEnvironmentMap(HANDLE  layerhandle, bool flag)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().SphereEnvironmentMap = flag;
}


bool GetLayerNoDepthTest(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().NoDepthTest;
}

void SetLayerNoDepthTest(HANDLE  layerhandle, bool flag)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().NoDepthTest = flag;
}



bool GetLayerNoDepthSet(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	return (int)layer->Data().NoDepthSet;
}

void SetLayerNoDepthSet(HANDLE  layerhandle, bool flag)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	layer->Data().NoDepthSet = flag;
}
