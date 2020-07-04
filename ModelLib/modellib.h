
#include "stdafx.h"

extern "C" {
	typedef void* HANDLE;

	struct VECTOR2 {
		float x;
		float y;
	};

	struct VECTOR3 {
		float x;
		float y;
		float z;
	};

	struct VECTOR4 {
		float x;
		float y;
		float z;
		float w;
	};


	HANDLE CreateInterpolator();


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


	//²ÄÖÊÍ¼²ãÈÝÆ÷
	int GetMaterialLayerSize(HANDLE mathandle);
	bool AddMaterialLayer(HANDLE mathandle, HANDLE layerhandle);
	bool RemoveMaterialLayer(HANDLE mathandle, HANDLE  layerhandle);


	HANDLE CreateMaterialLayer();
	HANDLE CopyMaterialLayer(HANDLE layerhandle);
	void CloseMaterialLayer(HANDLE layerhandle, bool del);
	HANDLE GetLayerByMaterial(HANDLE mathandle, int index);


	int GetLayerFilterMode(HANDLE  layerhandle);
	void SetLayerFilterMode(HANDLE  layerhandle, int filter_mode);
	int GetLayerTextureId(HANDLE  layerhandle);
	void SetLayerTextureId(HANDLE  layerhandle, int textureid);
	int GetLayerTextureAnimationId(HANDLE  layerhandle);
	void SetLayerTextureAnimationId(HANDLE  layerhandle, int animationid);
	bool GetLayerUnshaded(HANDLE  layerhandle);
	void SetLayerUnshaded(HANDLE  layerhandle, bool flag);
	bool GetLayerUnfogged(HANDLE  layerhandle);
	void SetLayerUnfogged(HANDLE  layerhandle, bool flag);
	bool GetLayerTwoSided(HANDLE  layerhandle);
	void SetLayerTwoSided(HANDLE  layerhandle, bool flag);
	bool GetLayerSphereEnvironmentMap(HANDLE  layerhandle);
	void SetLayerSphereEnvironmentMap(HANDLE  layerhandle, bool flag);
	bool GetLayerNoDepthTest(HANDLE  layerhandle);
	void SetLayerNoDepthTest(HANDLE  layerhandle, bool flag);
	bool GetLayerNoDepthSet(HANDLE  layerhandle);
	void SetLayerNoDepthSet(HANDLE  layerhandle, bool flag);





	HANDLE CreateCamera();
	HANDLE CopyCamera(HANDLE camhandle);
	void CloseCamera(HANDLE camhandle, bool del);
	HANDLE GetCameraByModel(HANDLE modelhandle, int index);

	int GetModelCameraSize(HANDLE modelhandle);
	bool AddModelCamera(HANDLE modelhandle, HANDLE camhandle);
	bool RemoveModelCamera(HANDLE modelhandle, HANDLE  camhandle);



	const char* GetCameraName(HANDLE  camhandle);
	void SetCameraName(HANDLE  camhandle, const char* name);
	VECTOR3* GetCameraSource(HANDLE  camhandle);
	void SetCameraSource(HANDLE  camhandle, VECTOR3* value);

	VECTOR3* GetCameraTarget(HANDLE  camhandle);
	void SetCameraTarget(HANDLE  camhandle, VECTOR3* value);

	float GetCameraFieldOfView(HANDLE  camhandle);
	void SetCameraFieldOfView(HANDLE  camhandle, float value);
	float GetCameraNearDistance(HANDLE  camhandle);
	void SetCameraNearDistance(HANDLE  camhandle, float value);
	float GetCameraFarDistance(HANDLE  camhandle);
	void SetCameraFarDistance(HANDLE  camhandle, float value);



	HANDLE CreateSequence();
	HANDLE CopySequence(HANDLE seqhandle);
	void CloseSequence(HANDLE seqhandle, bool del);
	HANDLE GetSequenceByModel(HANDLE modelhandle, int index);
	int GetModelSequenceSize(HANDLE modelhandle);
	bool AddModelSequence(HANDLE modelhandle, HANDLE seqhandle);
	bool RemoveModelSequence(HANDLE modelhandle, HANDLE  seqhandle);



	const char* GetSequenceName(HANDLE  seqhandle);
	void SetSequenceName(HANDLE  seqhandle, const char* name);
	float GetSequenceRarity(HANDLE  seqhandle);
	void SetSequenceRarity(HANDLE  seqhandle, float value);
	float GetSequenceMoveSpeed(HANDLE  seqhandle);
	void SetSequenceMoveSpeed(HANDLE  seqhandle, float value);
	bool GetSequenceNonLooping(HANDLE  seqhandle);
	void SetSequenceNonLooping(HANDLE  seqhandle, bool value);
	VECTOR2* GetSequenceInterval(HANDLE  seqhandle);
	void SetSequenceInterval(HANDLE  seqhandle, VECTOR2* value);
	EXTENT* GetSequenceExtent(HANDLE  seqhandle);
	void SetSequenceExtent(HANDLE  seqhandle, EXTENT* value);


}