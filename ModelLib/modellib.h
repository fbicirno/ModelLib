
#include "stdafx.h"

extern "C" {
	//config
	bool InitMpqResource();
	bool LoadAllReplaceableTextures();

	//model
	HANDLE CreateModel();
	HANDLE OpenModel(const char* path);
	bool SaveModel(HANDLE handle, const char* path);
	void CloseModel(HANDLE handle);
	HANDLE CopyModel(HANDLE handle);

	//Ä£ÐÍÈÝÆ÷
	int GetModelTextureSize(HANDLE modelhandle);
	bool AddModelTexture(HANDLE modelhandle, HANDLE texhandle);
	bool RemoveModelTexture(HANDLE modelhandle, HANDLE  texhandle);

	int GetModelMaterialSize(HANDLE modelhandle);
	bool AddModelMaterial(HANDLE modelhandle, HANDLE mathandle);
	bool RemoveModelMaterial(HANDLE modelhandle, HANDLE  mathandle);





	//texture
	HANDLE CreateTexture();

	HANDLE GetTextureByModel(HANDLE modelhandle, int index);



	void CloseTexture(HANDLE texhandle, bool del);
	const char* GetTexturePath(HANDLE texhandle);
	void SetTexturePath(HANDLE texhandle, const char* path);
	bool GetTextureWrapWidth(HANDLE texhandle);
	void SetTextureWrapWidth(HANDLE texhandle, bool flag);
	bool GetTextureWrapHeight(HANDLE texhandle);
	void SetTextureWrapHeight(HANDLE texhandle, bool flag);
	int GetTextureReplaceableId(HANDLE texhandle);
	void SetTextureReplaceableId(HANDLE texhandle, int id);
	int GetTextureWidth(HANDLE texhandle);
	int GetTextureHeight(HANDLE texhandle);


	//material
	HANDLE CreateMaterial();
	HANDLE CopyMaterial(HANDLE mathandle);
	void CloseMaterial(HANDLE mathandle, bool del);
	HANDLE GetMaterialByModel(HANDLE modelhandle, int index);

	int GetMaterialPriorityPlane(HANDLE mathandle);
	void SetMaterialPriorityPlane(HANDLE mathandle, int PriorityPlane);
	bool GetMaterialConstantColor(HANDLE mathandle);
	void SetMaterialConstantColor(HANDLE mathandle, bool ConstantColor);
	bool GetMaterialSortPrimitivesFarZ(HANDLE mathandle);
	void SetMaterialSortPrimitivesFarZ(HANDLE mathandle, bool SortPrimitivesFarZ);
	bool GetMaterialFullResolution(HANDLE mathandle);
	void SetMaterialFullResolution(HANDLE mathandle, bool FullResolution);





	HANDLE CreateMaterialLayer();
	HANDLE CopyMaterialLayer(HANDLE layerhandle);
	void CloseMaterialLayer(HANDLE layerhandle, bool del);
	HANDLE GetMaterialLayerByMaterial(HANDLE mathandle, int index);

	//²ÄÖÊÍ¼²ãÈÝÆ÷
	int GetMaterialLayerSize(HANDLE mathandle);
	bool AddMaterialLayer(HANDLE mathandle, HANDLE layerhandle);
	bool RemoveMaterialLayer(HANDLE mathandle, HANDLE  layerhandle);


}