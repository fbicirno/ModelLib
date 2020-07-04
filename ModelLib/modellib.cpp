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


INTERPOLATOR_HANDLE CreateInterpolator()
{
	INTERPOLATOR* interpolator = new INTERPOLATOR;
	return convert_handle(interpolator);
}


INTERPOLATOR_HANDLE CopyInterpolator(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}

	return convert_handle(new INTERPOLATOR(*interpolator));
}

void CloseInterpolator(INTERPOLATOR_HANDLE interhandle, bool del)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	close_handle(interhandle);
	if (interpolator && del)
	{
		delete interpolator;
	}

}

void InterpolatorClear(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return ;
	}

	interpolator->Clear();
}

int GetInterpolatorSize(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}

	return interpolator->GetSize();
}

INTERPOLATOR_NODE* GetInterpolatorNode(INTERPOLATOR_HANDLE interhandle, int index)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	int size = interpolator->GetSize();

	if (index < 0 || index >= size || size == 0)
	{
		return 0;
	}
	auto it = interpolator->NodeList.begin();
	if (index > 0)
	{
		advance(it, index - 1);
	}
	return &(*it);
}
bool GetInterpolatorIsStatic(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return interpolator->IsStatic();
}

int GetInterpolatorDataType(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return (int)interpolator->GetType();
}

void SetInterpolatorDataType(INTERPOLATOR_HANDLE interhandle, int type)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return ;
	}
	interpolator->SetType((INTERPOLATOR_TYPE)type);
}

int GetInterpolatorType(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return (int)interpolator->GetInterpolationType();
}

void SetInterpolatorType(INTERPOLATOR_HANDLE interhandle, int type)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return;
	}
	interpolator->SetInterpolationType((INTERPOLATION_TYPE)type);
}

void SetInterpolatorGlobalSequenceId(INTERPOLATOR_HANDLE interhandle, int sequence_id)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return;
	}
	interpolator->SetGlobalSequenceId(sequence_id);
}

int GetInterpolatorGlobalSequenceId(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return interpolator->GetGlobalSequenceId();
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


const char* GetInterpolatorString(INTERPOLATOR_HANDLE interhandle)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return "";
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


bool SetInterpolatorString(INTERPOLATOR_HANDLE interhandle, const char* str)
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

FLOAT GetInterpolatorStaticScalar(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	return interpolator->GetScalar(time ? *time : SEQUENCE_TIME());
}


bool GetInterpolatorVector2(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time, VECTOR2* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}

	*result = *(VECTOR2*)&interpolator->GetVector2(time ? *time : SEQUENCE_TIME());
	return true;
}

bool GetInterpolatorVector3(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time, VECTOR3* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	*result = *(VECTOR3*)&interpolator->GetVector2(time ? *time : SEQUENCE_TIME());
	return true;
}

bool GetInterpolatorVector4(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time, VECTOR4* result)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}
	*result = *(VECTOR4*)&interpolator->GetVector2(time ? *time : SEQUENCE_TIME());
	return true;
}

bool SetInterpolatorStaticScalar(INTERPOLATOR_HANDLE interhandle, float value, const char* name)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	interpolator->SetStaticScalar(value, name);
	return true;
}
bool SetInterpolatorStaticScalarInt(INTERPOLATOR_HANDLE interhandle, int value, const char* name)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return 0;
	}
	interpolator->SetStaticScalarInt(value, name);
	return true;
}



bool SetInterpolatorStaticVector2(INTERPOLATOR_HANDLE interhandle, VECTOR2* value, const char* name)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}

	interpolator->SetStaticVector2(*(D3DXVECTOR2*)value, name);
	return true;
}
bool SetInterpolatorStaticVector3(INTERPOLATOR_HANDLE interhandle, VECTOR3* value, const char* name)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}

	interpolator->SetStaticVector3(*(D3DXVECTOR3*)value, name);
	return true;
}

bool SetInterpolatorStaticVector4(INTERPOLATOR_HANDLE interhandle, VECTOR4* value, const char* name)
{
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interhandle);
	if (!interpolator)
	{
		return false;
	}

	interpolator->SetStaticVector4(*(D3DXVECTOR4*)value, name);
	return true;
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

void CloseModel(HANDLE handle,bool del)
{
	MODEL* model = (MODEL*)convert_object(handle);
	close_handle(handle);
	if (model && del)
	{

		delete model;
	}
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

HANDLE CopyTexture(HANDLE texhandle)
{
	MODEL_TEXTURE* texture = (MODEL_TEXTURE*)convert_object(texhandle);
	if (!texture)
	{
		return 0;
	}

	return convert_handle(new MODEL_TEXTURE(*texture));
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




HANDLE CreateLayer()
{
	MODEL_MATERIAL_LAYER* layer = new MODEL_MATERIAL_LAYER;

	return convert_handle(layer);
}

HANDLE CopyLayer(HANDLE layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
		return 0;

	MODEL_MATERIAL_LAYER* copy = new MODEL_MATERIAL_LAYER(*layer);

	return convert_handle(layer);
}

void CloseLayer(HANDLE layerhandle, bool del)
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


INTERPOLATOR_HANDLE GetLayerAnimatedTextureId(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &layer->Data().AnimatedTextureId;

	return convert_handle(interpolator);
}

void SetLayerAnimatedTextureId(HANDLE  layerhandle, INTERPOLATOR_HANDLE interpolatorhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interpolatorhandle);
	if (!interpolator)
	{
		return;
	}
	layer->Data().AnimatedTextureId = *interpolator;
}


INTERPOLATOR_HANDLE GetLayerAlpha(HANDLE  layerhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &layer->Data().Alpha;

	return convert_handle(interpolator);
}

void SetLayerAlpha(HANDLE layerhandle, INTERPOLATOR_HANDLE interpolatorhandle)
{
	MODEL_MATERIAL_LAYER* layer = (MODEL_MATERIAL_LAYER*)convert_object(layerhandle);

	if (!layer)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interpolatorhandle);
	if (!interpolator)
	{
		return;
	}
	layer->Data().Alpha = *interpolator;
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


INTERPOLATOR_HANDLE GetCameraSourceTranslation(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &camera->Data().SourceTranslation;

	return convert_handle(interpolator);
}

void SetCameraSourceTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE interpolatorhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interpolatorhandle);
	if (!interpolator)
	{
		return;
	}
	camera->Data().SourceTranslation = *interpolator;
}


INTERPOLATOR_HANDLE GetCameraTargetTranslation(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &camera->Data().TargetTranslation;

	return convert_handle(interpolator);
}

void SetCameraTargetTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE interpolatorhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interpolatorhandle);
	if (!interpolator)
	{
		return;
	}
	camera->Data().TargetTranslation = *interpolator;
}

INTERPOLATOR_HANDLE GetCameraRotation(HANDLE  camhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &camera->Data().Rotation;

	return convert_handle(interpolator);
}

void SetCameraTargetRotation(HANDLE camhandle, INTERPOLATOR_HANDLE interpolatorhandle)
{
	MODEL_CAMERA* camera = (MODEL_CAMERA*)convert_object(camhandle);

	if (!camera)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interpolatorhandle);
	if (!interpolator)
	{
		return;
	}
	camera->Data().Rotation = *interpolator;
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


HANDLE CreateBone()
{
	MODEL_BONE* bone = new MODEL_BONE;

	return convert_handle(bone);
}

HANDLE CopyBone(HANDLE bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
		return 0;

	MODEL_BONE* copy = new MODEL_BONE(*bone);

	return convert_handle(bone);
}

void CloseBone(HANDLE bonehandle, bool del)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	close_handle(bonehandle);
	if (bone && del)
	{
		delete bone;
	}
}

HANDLE GetBoneByModel(HANDLE modelhandle, int index)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	auto& container = model->Data().BoneContainer;
	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetModelBoneSize(HANDLE modelhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	return model->Data().BoneContainer.GetSize();
}

bool AddModelBone(HANDLE modelhandle, HANDLE bonehandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}

	return model->AddBone(bone);
}

bool RemoveModelBone(HANDLE modelhandle, HANDLE  bonehandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);
	if (!bone)
	{
		return false;
	}

	close_handle(bonehandle);
	model->RemoveBone(bone);
	return true;

}


const char* GetBoneName(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().Name.c_str();
}

void SetBoneName(HANDLE  bonehandle, const char* name)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return ;
	}
	bone->Data().Name = name;
}

int GetBoneObjectId(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().ObjectId;
}

void SetBoneObjectId(HANDLE  bonehandle, int value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().ObjectId = value;
}

int GetBoneParentId(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().ParentId;
}

void SetBoneParentId(HANDLE  bonehandle, int value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().ParentId = value;
}


INTERPOLATOR_HANDLE GetBoneTranslation(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &bone->Data().Translation;

	return convert_handle(interpolator);
}

void SetBoneTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(bonehandle);
	if (!interpolator)
	{
		return;
	}
	bone->Data().Translation = *interpolator;
}


INTERPOLATOR_HANDLE GetBoneRotation(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &bone->Data().Rotation;

	return convert_handle(interpolator);
}

void SetBoneRotation(HANDLE camhandle, INTERPOLATOR_HANDLE bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(bonehandle);
	if (!interpolator)
	{
		return;
	}
	bone->Data().Rotation = *interpolator;
}


INTERPOLATOR_HANDLE GetBoneScaling(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &bone->Data().Scaling;

	return convert_handle(interpolator);
}

void SetBoneScaling(HANDLE camhandle, INTERPOLATOR_HANDLE bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(bonehandle);
	if (!interpolator)
	{
		return;
	}
	bone->Data().Scaling = *interpolator;
}


bool GetBoneDontInheritTranslation(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().DontInheritTranslation;
}

void SetBoneDontInheritTranslation(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().DontInheritTranslation = value;
}


bool GetBoneDontInheritRotation(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().DontInheritRotation;
}

void SetBoneDontInheritRotation(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().DontInheritRotation = value;
}

bool GetBoneDontInheritScaling(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().DontInheritScaling;
}

void SetBoneDontInheritScaling(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().DontInheritScaling = value;
}

bool GetBoneBillboarded(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().Billboarded;
}

void SetBoneBillboarded(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().Billboarded = value;
}

bool GetBoneBillboardedLockX(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().BillboardedLockX;
}

void SetBoneBillboardedLockX(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().BillboardedLockX = value;
}

bool GetBoneBillboardedLockY(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().BillboardedLockY;
}

void SetBoneBillboardedLockY(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().BillboardedLockY = value;
}

bool GetBoneBillboardedLockZ(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().BillboardedLockZ;
}

void SetBoneBillboardedLockZ(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().BillboardedLockZ = value;
}

bool GetBoneCameraAnchored(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return bone->Data().CameraAnchored;
}

void SetBoneCameraAnchored(HANDLE  bonehandle, bool value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().CameraAnchored = value;
}


VECTOR3* GetBonePivotPoint(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return (VECTOR3*)&bone->Data().PivotPoint;
}

void SetBonePivotPoint(HANDLE  bonehandle, VECTOR3* value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().PivotPoint = *(D3DXVECTOR3*)value;
}

int GetBoneType(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return (int)bone->Data().Type;
}

void SetBoneType(HANDLE  bonehandle, int value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().Type = (NODE_TYPE)value;
}

int GetBoneGeosetId(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return (int)bone->Data().GeosetId;
}

void SetBoneGeosetId(HANDLE  bonehandle, int value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().GeosetId = value;
}


int GetBoneGeosetAnimationId(HANDLE  bonehandle)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return 0;
	}
	return (int)bone->Data().GeosetId;
}

void SetBoneGeosetAnimationId(HANDLE  bonehandle, int value)
{
	MODEL_BONE* bone = (MODEL_BONE*)convert_object(bonehandle);

	if (!bone)
	{
		return;
	}
	bone->Data().GeosetAnimationId = value;
}




HANDLE CreateAttachment()
{
	MODEL_ATTACHMENT* attachment = new MODEL_ATTACHMENT;

	return convert_handle(attachment);
}

HANDLE CopyAttachment(HANDLE attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
		return 0;

	MODEL_ATTACHMENT* copy = new MODEL_ATTACHMENT(*attachment);

	return convert_handle(attachment);
}

void CloseAttachment(HANDLE attachmenthandle, bool del)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	close_handle(attachmenthandle);
	if (attachment && del)
	{
		delete attachment;
	}
}

HANDLE GetAttachmentByModel(HANDLE modelhandle, int index)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	auto& container = model->Data().AttachmentContainer;
	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetModelAttachmentSize(HANDLE modelhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	return model->Data().AttachmentContainer.GetSize();
}

bool AddModelAttachment(HANDLE modelhandle, HANDLE attachmenthandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}

	return model->AddAttachment(attachment);
}

bool RemoveModelAttachment(HANDLE modelhandle, HANDLE  attachmenthandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);
	if (!attachment)
	{
		return false;
	}

	close_handle(attachmenthandle);
	model->RemoveAttachment(attachment);
	return true;

}


const char* GetAttachmentName(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().Name.c_str();
}

void SetAttachmentName(HANDLE  attachmenthandle, const char* name)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().Name = name;
}

int GetAttachmentObjectId(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().ObjectId;
}

void SetAttachmentObjectId(HANDLE  attachmenthandle, int value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().ObjectId = value;
}

int GetAttachmentParentId(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().ParentId;
}

void SetAttachmentParentId(HANDLE  attachmenthandle, int value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().ParentId = value;
}


INTERPOLATOR_HANDLE GetAttachmentTranslation(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &attachment->Data().Translation;

	return convert_handle(interpolator);
}

void SetAttachmentTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(attachmenthandle);
	if (!interpolator)
	{
		return;
	}
	attachment->Data().Translation = *interpolator;
}


INTERPOLATOR_HANDLE GetAttachmentRotation(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &attachment->Data().Rotation;

	return convert_handle(interpolator);
}

void SetAttachmentRotation(HANDLE camhandle, INTERPOLATOR_HANDLE attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(attachmenthandle);
	if (!interpolator)
	{
		return;
	}
	attachment->Data().Rotation = *interpolator;
}


INTERPOLATOR_HANDLE GetAttachmentScaling(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &attachment->Data().Scaling;

	return convert_handle(interpolator);
}

void SetAttachmentScaling(HANDLE camhandle, INTERPOLATOR_HANDLE attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(attachmenthandle);
	if (!interpolator)
	{
		return;
	}
	attachment->Data().Scaling = *interpolator;
}


bool GetAttachmentDontInheritTranslation(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().DontInheritTranslation;
}

void SetAttachmentDontInheritTranslation(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().DontInheritTranslation = value;
}


bool GetAttachmentDontInheritRotation(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().DontInheritRotation;
}

void SetAttachmentDontInheritRotation(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().DontInheritRotation = value;
}

bool GetAttachmentDontInheritScaling(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().DontInheritScaling;
}

void SetAttachmentDontInheritScaling(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().DontInheritScaling = value;
}

bool GetAttachmentBillboarded(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().Billboarded;
}

void SetAttachmentBillboarded(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().Billboarded = value;
}

bool GetAttachmentBillboardedLockX(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().BillboardedLockX;
}

void SetAttachmentBillboardedLockX(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().BillboardedLockX = value;
}

bool GetAttachmentBillboardedLockY(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().BillboardedLockY;
}

void SetAttachmentBillboardedLockY(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().BillboardedLockY = value;
}

bool GetAttachmentBillboardedLockZ(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().BillboardedLockZ;
}

void SetAttachmentBillboardedLockZ(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().BillboardedLockZ = value;
}

bool GetAttachmentCameraAnchored(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().CameraAnchored;
}

void SetAttachmentCameraAnchored(HANDLE  attachmenthandle, bool value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().CameraAnchored = value;
}


VECTOR3* GetAttachmentPivotPoint(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return (VECTOR3*)&attachment->Data().PivotPoint;
}

void SetAttachmentPivotPoint(HANDLE  attachmenthandle, VECTOR3* value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().PivotPoint = *(D3DXVECTOR3*)value;
}

int GetAttachmentType(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return (int)attachment->Data().Type;
}

void SetAttachmentType(HANDLE  attachmenthandle, int value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().Type = (NODE_TYPE)value;
}

const char* GetAttachmentPath(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().Path.c_str();
}

void SetAttachmentPath(HANDLE  attachmenthandle, const char* value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().Path = value;
}

INTERPOLATOR_HANDLE GetAttachmentVisibility(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}

	INTERPOLATOR* interpolator = &attachment->Data().Visibility;

	return convert_handle(interpolator);
}

void SetAttachmentVisibility(HANDLE  attachmenthandle, INTERPOLATOR_HANDLE interpolatorhandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(interpolatorhandle);
	if (!interpolator)
	{
		return;
	}
	attachment->Data().Visibility = *interpolator;
}


int GetAttachmentId(HANDLE  attachmenthandle)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return 0;
	}
	return attachment->Data().AttachmentId;
}

void SetAttachmentId(HANDLE  attachmenthandle, int value)
{
	MODEL_ATTACHMENT* attachment = (MODEL_ATTACHMENT*)convert_object(attachmenthandle);

	if (!attachment)
	{
		return;
	}
	attachment->Data().AttachmentId = value;
}



HANDLE CreateCollisionshape()
{
	MODEL_COLLISION_SHAPE* collisionshape = new MODEL_COLLISION_SHAPE;

	return convert_handle(collisionshape);
}

HANDLE CopyCollisionshape(HANDLE collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
		return 0;

	MODEL_COLLISION_SHAPE* copy = new MODEL_COLLISION_SHAPE(*collisionshape);

	return convert_handle(collisionshape);
}

void CloseCollisionshape(HANDLE collisionshapehandle, bool del)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	close_handle(collisionshapehandle);
	if (collisionshape && del)
	{
		delete collisionshape;
	}
}

HANDLE GetCollisionshapeByModel(HANDLE modelhandle, int index)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	auto& container = model->Data().CollisionShapeContainer;
	if (index >= 0 && index < container.GetSize() && container.GetSize() > 0)
	{
		return convert_handle(container[index]);
	}
	return 0;
}


int GetModelCollisionshapeSize(HANDLE modelhandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}

	return model->Data().CollisionShapeContainer.GetSize();
}

bool AddModelCollisionshape(HANDLE modelhandle, HANDLE collisionshapehandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return 0;
	}
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}

	return model->AddCollisionShape(collisionshape);
}

bool RemoveModelCollisionshape(HANDLE modelhandle, HANDLE  collisionshapehandle)
{
	MODEL* model = (MODEL*)convert_object(modelhandle);
	if (!model)
	{
		return false;
	}
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);
	if (!collisionshape)
	{
		return false;
	}

	close_handle(collisionshapehandle);
	model->RemoveCollisionShape(collisionshape);
	return true;

}


const char* GetCollisionshapeName(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().Name.c_str();
}

void SetCollisionshapeName(HANDLE  collisionshapehandle, const char* name)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().Name = name;
}

int GetCollisionshapeObjectId(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().ObjectId;
}

void SetCollisionshapeObjectId(HANDLE  collisionshapehandle, int value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().ObjectId = value;
}

int GetCollisionshapeParentId(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().ParentId;
}

void SetCollisionshapeParentId(HANDLE  collisionshapehandle, int value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().ParentId = value;
}


INTERPOLATOR_HANDLE GetCollisionshapeTranslation(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &collisionshape->Data().Translation;

	return convert_handle(interpolator);
}

void SetCollisionshapeTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(collisionshapehandle);
	if (!interpolator)
	{
		return;
	}
	collisionshape->Data().Translation = *interpolator;
}


INTERPOLATOR_HANDLE GetCollisionshapeRotation(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &collisionshape->Data().Rotation;

	return convert_handle(interpolator);
}

void SetCollisionshapeRotation(HANDLE camhandle, INTERPOLATOR_HANDLE collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(collisionshapehandle);
	if (!interpolator)
	{
		return;
	}
	collisionshape->Data().Rotation = *interpolator;
}


INTERPOLATOR_HANDLE GetCollisionshapeScaling(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	INTERPOLATOR* interpolator = &collisionshape->Data().Scaling;

	return convert_handle(interpolator);
}

void SetCollisionshapeScaling(HANDLE camhandle, INTERPOLATOR_HANDLE collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	INTERPOLATOR* interpolator = (INTERPOLATOR*)convert_object(collisionshapehandle);
	if (!interpolator)
	{
		return;
	}
	collisionshape->Data().Scaling = *interpolator;
}


bool GetCollisionshapeDontInheritTranslation(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().DontInheritTranslation;
}

void SetCollisionshapeDontInheritTranslation(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().DontInheritTranslation = value;
}


bool GetCollisionshapeDontInheritRotation(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().DontInheritRotation;
}

void SetCollisionshapeDontInheritRotation(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().DontInheritRotation = value;
}

bool GetCollisionshapeDontInheritScaling(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().DontInheritScaling;
}

void SetCollisionshapeDontInheritScaling(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().DontInheritScaling = value;
}

bool GetCollisionshapeBillboarded(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().Billboarded;
}

void SetCollisionshapeBillboarded(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().Billboarded = value;
}

bool GetCollisionshapeBillboardedLockX(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().BillboardedLockX;
}

void SetCollisionshapeBillboardedLockX(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().BillboardedLockX = value;
}

bool GetCollisionshapeBillboardedLockY(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().BillboardedLockY;
}

void SetCollisionshapeBillboardedLockY(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().BillboardedLockY = value;
}

bool GetCollisionshapeBillboardedLockZ(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().BillboardedLockZ;
}

void SetCollisionshapeBillboardedLockZ(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().BillboardedLockZ = value;
}

bool GetCollisionshapeCameraAnchored(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().CameraAnchored;
}

void SetCollisionshapeCameraAnchored(HANDLE  collisionshapehandle, bool value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().CameraAnchored = value;
}


VECTOR3* GetCollisionshapePivotPoint(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return (VECTOR3*)&collisionshape->Data().PivotPoint;
}

void SetCollisionshapePivotPoint(HANDLE  collisionshapehandle, VECTOR3* value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().PivotPoint = *(D3DXVECTOR3*)value;
}

int GetCollisionshapeType(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return (int)collisionshape->Data().Type;
}

void SetCollisionshapeType(HANDLE  collisionshapehandle, int value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().Type = (COLLISION_SHAPE_TYPE)value;
}


float GetCollisionshapeBoundsRadius(HANDLE  collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	return collisionshape->Data().BoundsRadius;
}

void SetCollisionshapeBoundsRadius(HANDLE  collisionshapehandle, float value)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return;
	}
	collisionshape->Data().BoundsRadius = value;
}



VECTOR3* GetVertexByCollisionshape(HANDLE collisionshapehandle, int index)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}

	auto size = collisionshape->Data().VertexList.size();

	if (index < 0 || index >= size || size == 0)
	{
		return 0;
	}
	auto it = collisionshape->Data().VertexList.begin();
	if (index > 0)
	{
		advance(it, index - 1);
	}
	return (VECTOR3*)&(*it);
}


int GetCollisionshapeVertexSize(HANDLE collisionshapehandle)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}

	return collisionshape->Data().VertexList.size();
}

bool AddCollisionshapVertex(HANDLE collisionshapehandle, VECTOR3* vec3)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}


	collisionshape->Data().VertexList.push_back(*(D3DXVECTOR3*)vec3);
	return true;
}

bool RemoveCollisionshapVertex(HANDLE collisionshapehandle, int index)
{
	MODEL_COLLISION_SHAPE* collisionshape = (MODEL_COLLISION_SHAPE*)convert_object(collisionshapehandle);

	if (!collisionshape)
	{
		return 0;
	}
	auto it = collisionshape->Data().VertexList.begin();
	if (index > 0)
	{
		advance(it, index - 1);
	}

	collisionshape->Data().VertexList.erase(it);
	return true;
}


std::list<D3DXVECTOR3> VertexList;