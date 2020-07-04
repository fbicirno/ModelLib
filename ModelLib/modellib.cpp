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


HANDLE CreateInterpolator()
{
	INTERPOLATOR* interpolator = new INTERPOLATOR;
	return convert_handle(interpolator);
}


HANDLE CopyInterpolator(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}

	return convert_handle(new INTERPOLATOR(*interpolator);
}

void CloseInterpolator(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return ;
	}
	close_handle(interhandle);
	delete interpolator;
}

void InterClear(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return ;
	}

	interpolator->Clear();
}

int GetInterpolatorSize(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}

	return interpolator->GetSize();
}

bool GetInterpolatorIsStatic(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return interpolator->IsStatic();
}

int GetInterpolatorDataType(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return (int)interpolator->GetType();
}

void SetInterpolatorDataType(HANDLE interhandle, int type)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return ;
	}
	interpolator->SetType((INTERPOLATOR_TYPE)type);
}

int GetInterpolatorType(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return (int)interpolator->GetInterpolatorpolationType();
}

void SetInterpolatorType(HANDLE interhandle, int type)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return;
	}
	interpolator->SetInterpolatorpolationType((INTERPOLATION_TYPE)type);
}

void SetInterpolatorGlobalSequenceId(HANDLE interhandle, int sequence_id)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return;
	}
	interpolator->SetGlobalSequenceId(sequence_id);
}

int GetInterpolatorGlobalSequenceId(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return interpolator->GetGlobalSequenceId();
}

static void BuildVector(INTERPOLATOR* interpolator,std::stringstream& Stream, CONST D3DXVECTOR4& Vector)
{
	switch (interpolator->Type)
	{
		case INTERPOLATOR_TYPE_SCALAR:
		case INTERPOLATOR_TYPE_SCALAR_INT:
		{
			Stream << Vector.x;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR2:
		{
			Stream << "{ " << Vector.x << ", " << Vector.y << " }";
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR3:
		{
			Stream << "{ " << Vector.x << ", " << Vector.y << ", " << Vector.z << " }";
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR4:
		{
			Stream << "{ " << Vector.x << ", " << Vector.y << ", " << Vector.z << ", " << Vector.w << " }";
			break;
		}
	}
}

static bool RetrieveVector(INTERPOLATOR* interpolator, std::stringstream& Stream, D3DXVECTOR4& Vector)
{
	Vector = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	switch (interpolator->Type)
	{
	case INTERPOLATOR_TYPE_SCALAR:
	case INTERPOLATOR_TYPE_SCALAR_INT:
	{
		Stream >> Vector.x;
		break;
	}

	case INTERPOLATOR_TYPE_VECTOR2:
	{
		if (!ExpectChar(Stream, '{')) return FALSE;
		Stream >> Vector.x;
		if (!ExpectChar(Stream, ',')) return FALSE;
		Stream >> Vector.y;
		if (!ExpectChar(Stream, '}')) return FALSE;
		break;
	}

	case INTERPOLATOR_TYPE_VECTOR3:
	{
		if (!ExpectChar(Stream, '{')) return FALSE;
		Stream >> Vector.x;
		if (!ExpectChar(Stream, ',')) return FALSE;
		Stream >> Vector.y;
		if (!ExpectChar(Stream, ',')) return FALSE;
		Stream >> Vector.z;
		if (!ExpectChar(Stream, '}')) return FALSE;
		break;
	}

	case INTERPOLATOR_TYPE_VECTOR4:
	{
		if (!ExpectChar(Stream, '{')) return FALSE;
		Stream >> Vector.x;
		if (!ExpectChar(Stream, ',')) return FALSE;
		Stream >> Vector.y;
		if (!ExpectChar(Stream, ',')) return FALSE;
		Stream >> Vector.z;
		if (!ExpectChar(Stream, ',')) return FALSE;
		Stream >> Vector.z;
		if (!ExpectChar(Stream, '}')) return FALSE;
		break;
	}
	}

	return TRUE;
}

static bool ExpectChar(std::stringstream& Stream, CHAR Char)
{
	CHAR TempChar = ' ';

	Stream >> TempChar;
	if (TempChar != Char)
	{
		Error.SetMessage(std::string("Expected \"") + Char + "\", got \"" + TempChar + "\"!");
		return FALSE;
	}

	return TRUE;
}

const char* GetInterpolatorString(HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return ;
	}
	std::stringstream Stream;
	std::list<INTERPOLATOR_NODE>::iterator i;

	if (interpolator->IsStatic())
	{
		Stream << "0: ";
		BuildVector(interpolator, Stream, interpolator->StaticVector);
		Stream << "\n";

		interpolator->Buffer = Stream.str();
		return interpolator->Buffer.c_str();
	}
	i = interpolator->NodeList.begin();
	while (i != interpolator->NodeList.end())
	{
		Stream << i->Time << ": ";
		BuildVector(interpolator, Stream, i->Vector);
		Stream << "\n";

		switch (interpolator->InterpolationType)
		{
		case INTERPOLATION_TYPE_HERMITE:
		case INTERPOLATION_TYPE_BEZIER:
		{
			Stream << "  InTan: ";
			BuildVector(interpolator, Stream, i->InTan);
			Stream << "\n";

			Stream << "  OutTan: ";
			BuildVector(interpolator, Stream, i->OutTan);
			Stream << "\n";
		}
		}

		i++;
	}
	interpolator->Buffer = Stream.str();
	return interpolator->Buffer.c_str();
}


bool SetInterpolatorString(HANDLE interhandle, const char* str)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	std::string Word;
	std::stringstream Stream;
	INTERPOLATOR_NODE Node;

	Stream.str(str);

	interpolator->NodeList.clear();

	while (!Stream.eof())
	{
		Node = INTERPOLATOR_NODE();

		Node.Time = -1;
		Stream >> Node.Time;
		if (Node.Time == -1) break;

		if (!ExpectChar(Stream, ':')) return false;
		if (!RetrieveVector(interpolator, Stream, Node.Vector)) return false;

		switch (interpolator->InterpolationType)
		{
			case INTERPOLATION_TYPE_HERMITE:
			case INTERPOLATION_TYPE_BEZIER:
			{
				Word = "";
				Stream >> Word;
				if (Word != "InTan:")
				{
					Error.SetMessage("Expected \"InTan:\", got \"" + Word + "\"!");
					return false;
				}

				if (!RetrieveVector(interpolator, Stream, Node.InTan)) return false;

				Word = "";
				Stream >> Word;
				if (Word != "OutTan:")
				{
					Error.SetMessage("Expected \"OutTan:\", got \"" + Word + "\"!");
					return false;
				}

				if (!RetrieveVector(interpolator, Stream, Node.OutTan)) return false;
			}
		}
		interpolator->NodeList.push_back(Node);
	}

	interpolator->Static = FALSE;
	return true;
}

bool GetInterpolatorVector2(HANDLE interhandle, SEQUENCE_TIME* time, VECTOR2* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}

	interpolator->GetVector2(time ? *time : SEQUENCE_TIME());
}

bool GetInterpolatorVector3(HANDLE interhandle, SEQUENCE_TIME* time, VECTOR3* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	interpolator->GetVector3(time ? *time : SEQUENCE_TIME());
}

bool SetInterpolatorVector4(HANDLE interhandle, SEQUENCE_TIME* time, VECTOR4* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	interpolator->GetVector4(time ? *time : SEQUENCE_TIME());
}

bool SetInterpolatorVector2(HANDLE interhandle, SEQUENCE_TIME* time, VECTOR2* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}

	interpolator->GetVector2(time ? *time : SEQUENCE_TIME());
}

bool SetInterpolatorVector3(HANDLE interhandle, SEQUENCE_TIME* time, VECTOR3* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	interpolator->GetVector3(time ? *time : SEQUENCE_TIME());
}

bool SetInterpolatorVector4(HANDLE interhandle, SEQUENCE_TIME* time, VECTOR4* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	interpolator->GetVector4(time ? *time : SEQUENCE_TIME());
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





HANDLE CreateCamera()
{
	MODEL_CAMERA* camera = new MODEL_CAMERA;

	return convert_handle(camera);
}

HANDLE CopyCamera(HANDLE camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
		return 0;

	MODEL_CAMERA* copy = new MODEL_CAMERA(*camera);

	return convert_handle(camera);
}

void CloseCamera(HANDLE camhandle, bool del)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	close_handle(camhandle);
	if (camera && del)
	{
		delete camera;
	}
}

HANDLE GetCameraByModel(HANDLE modelhandle, int index)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	auto& container = model->Data().CameraContainer;
	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetModelCameraSize(HANDLE modelhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	return model->Data().CameraContainer.GetSize();
}

bool AddModelCamera(HANDLE modelhandle, HANDLE camhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}

	return model->AddCamera(camera);
}

bool RemoveModelCamera(HANDLE modelhandle, HANDLE  camhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);
	if (!camera)
	{
		return false;
	}

	close_handle(camhandle);
	model->RemoveCamera(camera);
	return true;

}

const char* GetCameraName(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	return camera->Data().Name.c_str();
}

void SetCameraName(HANDLE  camhandle, const char* name)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return ;
	}
	camera->Data().Name = name;
}

VECTOR3* GetCameraSource(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	return (VECTOR3*)&camera->Data().Source;
}

void SetCameraSource(HANDLE  camhandle, VECTOR3* value)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	camera->Data().Source = *(D3DXVECTOR3*)value;
}

VECTOR3* GetCameraTarget(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	return (VECTOR3*)&camera->Data().Target;
}

void SetCameraTarget(HANDLE  camhandle, VECTOR3* value)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	camera->Data().Target = *(D3DXVECTOR3*)value;
}


float GetCameraFieldOfView(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	return camera->Data().FieldOfView;
}

void SetCameraFieldOfView(HANDLE  camhandle, float value)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	camera->Data().FieldOfView = value;
}

float GetCameraNearDistance(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	return camera->Data().NearDistance;
}

void SetCameraNearDistance(HANDLE  camhandle, float value)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	camera->Data().NearDistance = value;
}

float GetCameraFarDistance(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	return camera->Data().FarDistance;
}

void SetCameraFarDistance(HANDLE  camhandle, float value)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	camera->Data().FarDistance = value;
}





HANDLE CreateSequence()
{
	MODEL_SEQUENCE* sequence = new MODEL_SEQUENCE;

	return convert_handle(sequence);
}

HANDLE CopySequence(HANDLE seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
		return 0;

	MODEL_SEQUENCE* copy = new MODEL_SEQUENCE(*sequence);

	return convert_handle(sequence);
}

void CloseSequence(HANDLE seqhandle, bool del)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	close_handle(seqhandle);
	if (sequence && del)
	{
		delete sequence;
	}
}

HANDLE GetSequenceByModel(HANDLE modelhandle, int index)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	auto& container = model->Data().SequenceContainer;
	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetModelSequenceSize(HANDLE modelhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	return model->Data().SequenceContainer.GetSize();
}

bool AddModelSequence(HANDLE modelhandle, HANDLE seqhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}

	return model->AddSequence(sequence);
}

bool RemoveModelSequence(HANDLE modelhandle, HANDLE  seqhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);
	if (!sequence)
	{
		return false;
	}

	close_handle(seqhandle);
	model->RemoveSequence(sequence);
	return true;

}



const char* GetSequenceName(HANDLE  seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}
	return sequence->Data().Name.c_str();
}

void SetSequenceName(HANDLE  seqhandle, const char* name)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);


	if (!sequence)
	{
		return;
	}
	sequence->Data().Name = name;
}


float GetSequenceRarity(HANDLE  seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}
	return sequence->Data().Rarity;
}

void SetSequenceRarity(HANDLE  seqhandle, float value)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);


	if (!sequence)
	{
		return;
	}
	sequence->Data().Rarity = value;
}

float GetSequenceMoveSpeed(HANDLE  seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}
	return sequence->Data().MoveSpeed;
}

void SetSequenceMoveSpeed(HANDLE  seqhandle, float value)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);


	if (!sequence)
	{
		return;
	}
	sequence->Data().MoveSpeed = value;
}

bool GetSequenceNonLooping(HANDLE  seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}
	return sequence->Data().NonLooping;
}

void SetSequenceNonLooping(HANDLE  seqhandle, bool value)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);


	if (!sequence)
	{
		return;
	}
	sequence->Data().NonLooping = value;
}



VECTOR2* GetSequenceInterval(HANDLE  seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}
	return (VECTOR2*)&sequence->Data().Interval;
}

void SetSequenceInterval(HANDLE  seqhandle, VECTOR2* value)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);


	if (!sequence)
	{
		return;
	}
	sequence->Data().Interval = *(D3DXVECTOR2*)value;
}


EXTENT* GetSequenceExtent(HANDLE  seqhandle)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);

	if (!sequence)
	{
		return 0;
	}
	return (EXTENT*)&sequence->Data().Extent;
}

void SetSequenceExtent(HANDLE  seqhandle, EXTENT* value)
{
	MODEL_SEQUENCE* sequence = (MODEL_SEQUENCE*)convert_object(seqhandle);


	if (!sequence)
	{
		return;
	}
	sequence->Data().Extent = *(EXTENT*)value;
}

