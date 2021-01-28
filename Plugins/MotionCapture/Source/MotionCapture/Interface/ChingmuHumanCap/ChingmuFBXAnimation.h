#pragma once
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "../../Model/MocapData.h"
#include "Runtime/Engine/Classes/Animation/SkeletalMeshActor.h"
#include"ChingMUComponent.h"
#include "ChingmuFBXAnimation.generated.h"


UCLASS( Blueprintable)
class MOTIONCAPTURE_API AChingmuFBXAnimation:public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	AChingmuFBXAnimation();
	~AChingmuFBXAnimation();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "false"))
		UChingMUComponent *Chingmu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "false"))
		USkeletalMeshComponent *SketalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "false"))
		UPoseableMeshComponent *HumanPoseMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
		FWorldSpaceHumanState finalAnimationdata;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
		FWorldSpaceHumanState MIXState;



	UFUNCTION(BlueprintPure)
	void GetAnimationData(FWorldSpaceHumanState & Final);

	UFUNCTION(BlueprintPure)
	void GetAnimationDataMiXMocap(FString ServerAddr, int humanID,bool useMocapPos, FWorldSpaceHumanRotator AniDataIn, FWorldSpaceHumanState & MIX);


private:
	FRotator offSetRot;
	FWorldSpaceHumanState orinAnimationData;

	FWorldSpaceHumanState worldSpaceHumanState;	
	FQuat AniWorldSpaceBonesQuat[23];


	void GetOrinAnimationData();
	void OrinAnimationDataModify();	
	//ÓÃÓÚ¶¯»­Êý¾Ý×ª»¯ÊµÊ±È«Éí¶¯²¶Êý¾ÝµÄº¯Êý
	FQuat* AxisConvert(FQuat localQuat);

};

