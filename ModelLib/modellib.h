
#include "stdafx.h"

extern "C" {
	typedef void* HANDLE;
	typedef void* INTERPOLATOR_HANDLE;

	struct VECTOR2 {
		float x, y;
	};

	struct VECTOR3 {
		float x, y, z;
	};

	struct VECTOR4 {
		float x, y, z, w;
	};


	//config
	bool InitMpqResource();
	bool LoadAllReplaceableTextures();



	INTERPOLATOR_HANDLE CreateInterpolator();
	INTERPOLATOR_HANDLE CopyInterpolator(INTERPOLATOR_HANDLE interhandle);
	void CloseInterpolator(INTERPOLATOR_HANDLE interhandle, bool del);
	void InterpolatorClear(INTERPOLATOR_HANDLE interhandle);
	int GetInterpolatorSize(INTERPOLATOR_HANDLE interhandle);
	INTERPOLATOR_NODE* GetInterpolatorNode(INTERPOLATOR_HANDLE interhandle, int index);
	bool GetInterpolatorIsStatic(INTERPOLATOR_HANDLE interhandle);
	int GetInterpolatorDataType(INTERPOLATOR_HANDLE interhandle);
	void SetInterpolatorDataType(INTERPOLATOR_HANDLE interhandle, int type);
	int GetInterpolatorType(INTERPOLATOR_HANDLE interhandle);
	void SetInterpolatorType(INTERPOLATOR_HANDLE interhandle, int type);
	void SetInterpolatorGlobalSequenceId(INTERPOLATOR_HANDLE interhandle, int sequence_id);
	int GetInterpolatorGlobalSequenceId(INTERPOLATOR_HANDLE interhandle);
	const char* GetInterpolatorString(INTERPOLATOR_HANDLE interhandle);
	bool SetInterpolatorString(INTERPOLATOR_HANDLE interhandle, const char* str);

	float GetInterpolatorStaticScalar(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time);
	bool GetInterpolatorVector2(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time, VECTOR2* result);
	bool GetInterpolatorVector3(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time, VECTOR3* result);
	bool GetInterpolatorVector4(INTERPOLATOR_HANDLE interhandle, SEQUENCE_TIME* time, VECTOR4* result);
	bool SetInterpolatorStaticScalar(INTERPOLATOR_HANDLE interhandle, float value, const char* name);
	bool SetInterpolatorStaticScalarInt(INTERPOLATOR_HANDLE interhandle, int value, const char* name);
	bool SetInterpolatorStaticVector2(INTERPOLATOR_HANDLE interhandle, VECTOR2* value, const char* name);
	bool SetInterpolatorStaticVector3(INTERPOLATOR_HANDLE interhandle, VECTOR3* value, const char* name);
	bool SetInterpolatorStaticVector4(INTERPOLATOR_HANDLE interhandle, VECTOR4* value, const char* name);

	//model
	HANDLE CreateModel();
	HANDLE OpenModel(const char* path);
	bool SaveModel(HANDLE handle, const char* path);
	void CloseModel(HANDLE handle, bool del);
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
	HANDLE CopyTexture(HANDLE texhandle);
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


	HANDLE CreateLayer();
	HANDLE CopyLayer(HANDLE layerhandle);
	void CloseLayer(HANDLE layerhandle, bool del);
	HANDLE GetLayerByMaterial(HANDLE mathandle, int index);


	int GetLayerFilterMode(HANDLE  layerhandle);
	void SetLayerFilterMode(HANDLE  layerhandle, int filter_mode);

	INTERPOLATOR_HANDLE GetLayerAnimatedTextureId(HANDLE  layerhandle);
	void SetLayerAnimatedTextureId(HANDLE  layerhandle, INTERPOLATOR_HANDLE interpolatorhandle);
	INTERPOLATOR_HANDLE GetLayerAlpha(HANDLE  layerhandle);
	void SetLayerAlpha(HANDLE layerhandle, INTERPOLATOR_HANDLE interpolatorhandle);
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

	INTERPOLATOR_HANDLE GetCameraSourceTranslation(HANDLE  camhandle);
	void SetCameraSourceTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE interpolatorhandle);
	INTERPOLATOR_HANDLE GetCameraTargetTranslation(HANDLE  camhandle);
	void SetCameraTargetTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE interpolatorhandle);
	INTERPOLATOR_HANDLE GetCameraRotation(HANDLE  camhandle);
	void SetCameraTargetRotation(HANDLE camhandle, INTERPOLATOR_HANDLE interpolatorhandle);


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





	HANDLE CreateBone();
	HANDLE CopyBone(HANDLE bonehandle);
	void CloseBone(HANDLE bonehandle, bool del);
	HANDLE GetBoneByModel(HANDLE modelhandle, int index);
	int GetModelBoneSize(HANDLE modelhandle);
	bool AddModelBone(HANDLE modelhandle, HANDLE bonehandle);
	bool RemoveModelBone(HANDLE modelhandle, HANDLE  bonehandle);


	const char* GetBoneName(HANDLE  bonehandle);
	void SetBoneName(HANDLE  bonehandle, const char* name);
	int GetBoneObjectId(HANDLE  bonehandle);
	void SetBoneObjectId(HANDLE  bonehandle, int value);
	int GetBoneParentId(HANDLE  bonehandle);
	void SetBoneParentId(HANDLE  bonehandle, int value);
	INTERPOLATOR_HANDLE GetBoneTranslation(HANDLE  bonehandle);
	void SetBoneTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE bonehandle);
	INTERPOLATOR_HANDLE GetBoneRotation(HANDLE  bonehandle);
	void SetBoneRotation(HANDLE camhandle, INTERPOLATOR_HANDLE bonehandle);
	INTERPOLATOR_HANDLE GetBoneScaling(HANDLE  bonehandle);
	void SetBoneScaling(HANDLE camhandle, INTERPOLATOR_HANDLE bonehandle);
	bool GetBoneDontInheritTranslation(HANDLE  bonehandle);
	void SetBoneDontInheritTranslation(HANDLE  bonehandle, bool value);
	bool GetBoneDontInheritRotation(HANDLE  bonehandle);
	void SetBoneDontInheritRotation(HANDLE  bonehandle, bool value);
	bool GetBoneDontInheritScaling(HANDLE  bonehandle);
	void SetBoneDontInheritScaling(HANDLE  bonehandle, bool value);
	bool GetBoneBillboarded(HANDLE  bonehandle);
	void SetBoneBillboarded(HANDLE  bonehandle, bool value);
	bool GetBoneBillboardedLockX(HANDLE  bonehandle);
	void SetBoneBillboardedLockX(HANDLE  bonehandle, bool value);
	bool GetBoneBillboardedLockY(HANDLE  bonehandle);
	void SetBoneBillboardedLockY(HANDLE  bonehandle, bool value);
	bool GetBoneBillboardedLockZ(HANDLE  bonehandle);
	void SetBoneBillboardedLockZ(HANDLE  bonehandle, bool value);
	bool GetBoneCameraAnchored(HANDLE  bonehandle);
	void SetBoneCameraAnchored(HANDLE  bonehandle, bool value);
	VECTOR3* GetBonePivotPoint(HANDLE  bonehandle);
	void SetBonePivotPoint(HANDLE  bonehandle, VECTOR3* value);
	int GetBoneType(HANDLE  bonehandle);
	void SetBoneType(HANDLE  bonehandle, int value);

	int GetBoneGeosetId(HANDLE  bonehandle);
	void SetBoneGeosetId(HANDLE  bonehandle, int value);
	int GetBoneGeosetAnimationId(HANDLE  bonehandle);
	void SetBoneGeosetAnimationId(HANDLE  bonehandle, int value);




	HANDLE CreateAttachment();
	HANDLE CopyAttachment(HANDLE attachmenthandle);
	void CloseAttachment(HANDLE attachmenthandle, bool del);
	HANDLE GetAttachmentByModel(HANDLE modelhandle, int index);
	int GetModelAttachmentSize(HANDLE modelhandle);
	bool AddModelAttachment(HANDLE modelhandle, HANDLE attachmenthandle);
	bool RemoveModelAttachment(HANDLE modelhandle, HANDLE  attachmenthandle);


	const char* GetAttachmentName(HANDLE  attachmenthandle);
	void SetAttachmentName(HANDLE  attachmenthandle, const char* name);
	int GetAttachmentObjectId(HANDLE  attachmenthandle);
	void SetAttachmentObjectId(HANDLE  attachmenthandle, int value);
	int GetAttachmentParentId(HANDLE  attachmenthandle);
	void SetAttachmentParentId(HANDLE  attachmenthandle, int value);
	INTERPOLATOR_HANDLE GetAttachmentTranslation(HANDLE  attachmenthandle);
	void SetAttachmentTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE attachmenthandle);
	INTERPOLATOR_HANDLE GetAttachmentRotation(HANDLE  attachmenthandle);
	void SetAttachmentRotation(HANDLE camhandle, INTERPOLATOR_HANDLE attachmenthandle);
	INTERPOLATOR_HANDLE GetAttachmentScaling(HANDLE  attachmenthandle);
	void SetAttachmentScaling(HANDLE camhandle, INTERPOLATOR_HANDLE attachmenthandle);
	bool GetAttachmentDontInheritTranslation(HANDLE  attachmenthandle);
	void SetAttachmentDontInheritTranslation(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentDontInheritRotation(HANDLE  attachmenthandle);
	void SetAttachmentDontInheritRotation(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentDontInheritScaling(HANDLE  attachmenthandle);
	void SetAttachmentDontInheritScaling(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentBillboarded(HANDLE  attachmenthandle);
	void SetAttachmentBillboarded(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentBillboardedLockX(HANDLE  attachmenthandle);
	void SetAttachmentBillboardedLockX(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentBillboardedLockY(HANDLE  attachmenthandle);
	void SetAttachmentBillboardedLockY(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentBillboardedLockZ(HANDLE  attachmenthandle);
	void SetAttachmentBillboardedLockZ(HANDLE  attachmenthandle, bool value);
	bool GetAttachmentCameraAnchored(HANDLE  attachmenthandle);
	void SetAttachmentCameraAnchored(HANDLE  attachmenthandle, bool value);
	VECTOR3* GetAttachmentPivotPoint(HANDLE  attachmenthandle);
	void SetAttachmentPivotPoint(HANDLE  attachmenthandle, VECTOR3* value);
	int GetAttachmentType(HANDLE  attachmenthandle);
	void SetAttachmentType(HANDLE  attachmenthandle, int value);



	const char* GetAttachmentPath(HANDLE  attachmenthandle);
	void SetAttachmentPath(HANDLE  attachmenthandle, const char* value);
	INTERPOLATOR_HANDLE GetAttachmentVisibility(HANDLE  attachmenthandle);
	void SetAttachmentVisibility(HANDLE  attachmenthandle, INTERPOLATOR_HANDLE interpolatorhandle);
	int GetAttachmentId(HANDLE  attachmenthandle);
	void SetAttachmentId(HANDLE  attachmenthandle, int value);





	HANDLE CreateCollisionshape();
	HANDLE CopyCollisionshape(HANDLE collisionshapehandle);
	void CloseCollisionshape(HANDLE collisionshapehandle, bool del);
	HANDLE GetCollisionshapeByModel(HANDLE modelhandle, int index);
	int GetModelCollisionshapeSize(HANDLE modelhandle);
	bool AddModelCollisionshape(HANDLE modelhandle, HANDLE collisionshapehandle);
	bool RemoveModelCollisionshape(HANDLE modelhandle, HANDLE  collisionshapehandle);


	const char* GetCollisionshapeName(HANDLE  collisionshapehandle);
	void SetCollisionshapeName(HANDLE  collisionshapehandle, const char* name);
	int GetCollisionshapeObjectId(HANDLE  collisionshapehandle);
	void SetCollisionshapeObjectId(HANDLE  collisionshapehandle, int value);
	int GetCollisionshapeParentId(HANDLE  collisionshapehandle);
	void SetCollisionshapeParentId(HANDLE  collisionshapehandle, int value);
	INTERPOLATOR_HANDLE GetCollisionshapeTranslation(HANDLE  collisionshapehandle);
	void SetCollisionshapeTranslation(HANDLE camhandle, INTERPOLATOR_HANDLE collisionshapehandle);
	INTERPOLATOR_HANDLE GetCollisionshapeRotation(HANDLE  collisionshapehandle);
	void SetCollisionshapeRotation(HANDLE camhandle, INTERPOLATOR_HANDLE collisionshapehandle);
	INTERPOLATOR_HANDLE GetCollisionshapeScaling(HANDLE  collisionshapehandle);
	void SetCollisionshapeScaling(HANDLE camhandle, INTERPOLATOR_HANDLE collisionshapehandle);
	bool GetCollisionshapeDontInheritTranslation(HANDLE  collisionshapehandle);
	void SetCollisionshapeDontInheritTranslation(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeDontInheritRotation(HANDLE  collisionshapehandle);
	void SetCollisionshapeDontInheritRotation(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeDontInheritScaling(HANDLE  collisionshapehandle);
	void SetCollisionshapeDontInheritScaling(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeBillboarded(HANDLE  collisionshapehandle);
	void SetCollisionshapeBillboarded(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeBillboardedLockX(HANDLE  collisionshapehandle);
	void SetCollisionshapeBillboardedLockX(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeBillboardedLockY(HANDLE  collisionshapehandle);
	void SetCollisionshapeBillboardedLockY(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeBillboardedLockZ(HANDLE  collisionshapehandle);
	void SetCollisionshapeBillboardedLockZ(HANDLE  collisionshapehandle, bool value);
	bool GetCollisionshapeCameraAnchored(HANDLE  collisionshapehandle);
	void SetCollisionshapeCameraAnchored(HANDLE  collisionshapehandle, bool value);
	VECTOR3* GetCollisionshapePivotPoint(HANDLE  collisionshapehandle);
	void SetCollisionshapePivotPoint(HANDLE  collisionshapehandle, VECTOR3* value);
	int GetCollisionshapeType(HANDLE  collisionshapehandle);
	void SetCollisionshapeType(HANDLE  collisionshapehandle, int value);


	float GetCollisionshapeBoundsRadius(HANDLE  collisionshapehandle);
	void SetCollisionshapeBoundsRadius(HANDLE  collisionshapehandle, float value);
	VECTOR3* GetVertexByCollisionshape(HANDLE collisionshapehandle, int index);
	int GetCollisionshapeVertexSize(HANDLE collisionshapehandle);
	bool AddCollisionshapVertex(HANDLE collisionshapehandle, VECTOR3* vec3);
	bool RemoveCollisionshapVertex(HANDLE collisionshapehandle, int index);

}