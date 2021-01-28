#pragma once

#include "GameFramework/Actor.h"
#include "MocapData.generated.h"


USTRUCT(BlueprintType)
struct FHumanBonesName
{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Hips="Hips";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Spine = "Spine";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Spine1 = "Spine1";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Spine2 = "Spine2";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Spine3 = "Spine3";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Neck = "Neck";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName Head = "Head";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftShoulder = "LeftShoulder";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftArm = "LeftArm";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftForeArm = "LeftForeArm";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftHand = "LeftHand";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightShoulder = "RightShoulder";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightArm = "RightArm";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightForeArm = "RightForeArm";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightHand = "RightHand";


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftUpLeg = "LeftUpLeg";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftLeg = "LeftLeg";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftFoot = "LeftFoot";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName LeftToeBase = "LeftToeBase";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightUpLeg = "RightUpLeg";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightLeg = "RightLeg";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightFoot = "RightFoot";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName RightToeBase = "RightToeBase";
};

USTRUCT(BlueprintType)
struct FChingMUSoldier
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FHead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FLeftAnkle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FRightAnkle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FAnotherTracker_01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FAnotherTracker_02;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FAnotherTracker_03;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FAnotherTracker_04;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FAnotherTracker_05;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTransform FAnotherTracker_06;
};



struct FWorldSpaceHumanQuat
{
		FQuat Hips;

		FQuat Spine;

		FQuat Spine1;

		FQuat Spine2;

		FQuat Spine3;

		FQuat Spine4;
		FQuat Spine5;
		FQuat Spine6;
		FQuat Spine7;
		FQuat Spine8;
		FQuat Spine9;

		FQuat Neck;

		FQuat Neck1;
		FQuat Neck2;
		FQuat Neck3;
		FQuat Neck4;
		FQuat Neck5;
		FQuat Neck6;
		FQuat Neck7;
		FQuat Neck8;
		FQuat Neck9;

		FQuat Head;


		FQuat LeftShoulder;
		FQuat LeftShoulderExtra;

		FQuat LeftArm;

		FQuat LeftForeArm;

		FQuat LeftHand;


		FQuat RightShoulder;
		FQuat RightShoulderExtra;

		FQuat RightArm;

		FQuat RightforeArm;

		FQuat RightHand;


		FQuat LeftUpLeg;

		FQuat LeftLeg;

		FQuat LeftFoot;

		FQuat LeftToeBase;



		FQuat RightUpLeg;

		FQuat RightLeg;

		FQuat RightFoot;

		FQuat RightToeBase;

public:
		FORCEINLINE FWorldSpaceHumanQuat() 
		{
			Hips			= FQuat::Identity;
			Spine			= FQuat::Identity;
			Spine1			= FQuat::Identity;
			Spine2			= FQuat::Identity;
			Spine3			= FQuat::Identity;
			Spine4			= FQuat::Identity;
			Spine5			= FQuat::Identity;
			Spine6			= FQuat::Identity;
			Spine7			= FQuat::Identity;
			Spine8			= FQuat::Identity;
			Spine9			= FQuat::Identity;
			Neck			= FQuat::Identity;
			Neck1			= FQuat::Identity;
			Neck2			= FQuat::Identity;
			Neck3			= FQuat::Identity;
			Neck4			= FQuat::Identity;
			Neck5			= FQuat::Identity;
			Neck6			= FQuat::Identity;
			Neck7			= FQuat::Identity;
			Neck8			= FQuat::Identity;
			Neck9			= FQuat::Identity;
			Head			= FQuat::Identity;
			LeftShoulder	= FQuat::Identity;
			LeftShoulderExtra = FQuat::Identity;
			LeftArm			= FQuat::Identity;
			LeftForeArm		= FQuat::Identity;
			LeftHand		= FQuat::Identity;
			RightShoulder	= FQuat::Identity;
			RightShoulderExtra = FQuat::Identity;
			RightArm		= FQuat::Identity;
			RightforeArm	= FQuat::Identity;
			RightHand		= FQuat::Identity;
			LeftUpLeg		= FQuat::Identity;
			LeftLeg			= FQuat::Identity;
			LeftFoot		= FQuat::Identity;
			LeftToeBase		= FQuat::Identity;
			RightUpLeg		= FQuat::Identity;
			RightLeg		= FQuat::Identity;
			RightFoot		= FQuat::Identity;
			RightToeBase	= FQuat::Identity;
		}
};

USTRUCT(BlueprintType)
struct FWorldSpaceHumanRotator
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector HipsPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Hips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Spine9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Neck9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftShoulderExtra;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftForeArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightShoulderExtra;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightforeArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftUpLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator LeftToeBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightUpLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FRotator RightToeBase;

public:
	FORCEINLINE FWorldSpaceHumanRotator()
	{
		HipsPosition	= FVector::ZeroVector;
		Hips			= FRotator::ZeroRotator;
		Spine			= FRotator::ZeroRotator;
		Spine1			= FRotator::ZeroRotator;
		Spine2			= FRotator::ZeroRotator;
		Spine3			= FRotator::ZeroRotator;
		Spine4			= FRotator::ZeroRotator;
		Spine5			= FRotator::ZeroRotator;
		Spine6			= FRotator::ZeroRotator;
		Spine7			= FRotator::ZeroRotator;
		Spine8			= FRotator::ZeroRotator;
		Spine9			= FRotator::ZeroRotator;
		Neck			= FRotator::ZeroRotator;
		Neck1			= FRotator::ZeroRotator;
		Neck2			= FRotator::ZeroRotator;
		Neck3			= FRotator::ZeroRotator;
		Neck4			= FRotator::ZeroRotator;
		Neck5			= FRotator::ZeroRotator;
		Neck6			= FRotator::ZeroRotator;
		Neck7			= FRotator::ZeroRotator;
		Neck8			= FRotator::ZeroRotator;
		Neck9			= FRotator::ZeroRotator;
		Head			= FRotator::ZeroRotator;
		LeftShoulder	= FRotator::ZeroRotator;
		LeftShoulderExtra = FRotator::ZeroRotator;
		LeftArm			= FRotator::ZeroRotator;
		LeftForeArm		= FRotator::ZeroRotator;
		LeftHand		= FRotator::ZeroRotator;
		RightShoulder	= FRotator::ZeroRotator;
		RightShoulderExtra = FRotator::ZeroRotator;
		RightArm		= FRotator::ZeroRotator;
		RightforeArm	= FRotator::ZeroRotator;
		RightHand		= FRotator::ZeroRotator;
		LeftUpLeg		= FRotator::ZeroRotator;
		LeftLeg			= FRotator::ZeroRotator;
		LeftFoot		= FRotator::ZeroRotator;
		LeftToeBase		= FRotator::ZeroRotator;
		RightUpLeg		= FRotator::ZeroRotator;
		RightLeg		= FRotator::ZeroRotator;
		RightFoot		= FRotator::ZeroRotator;
		RightToeBase	= FRotator::ZeroRotator;
	}
};

USTRUCT(BlueprintType)
struct FWorldSpaceHumanPosition
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Hips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Spine9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Neck9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftShoulderExtra;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftForeArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightShoulderExtra;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightforeArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftUpLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector LeftToeBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightUpLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FVector RightToeBase;

public:
	FORCEINLINE FWorldSpaceHumanPosition()
	{
		Hips = FVector::ZeroVector;
		Spine = FVector::ZeroVector;
		Spine1 = FVector::ZeroVector;
		Spine2 = FVector::ZeroVector;
		Spine3 = FVector::ZeroVector;
		Spine4 = FVector::ZeroVector;
		Spine5 = FVector::ZeroVector;
		Spine6 = FVector::ZeroVector;
		Spine7 = FVector::ZeroVector;
		Spine8 = FVector::ZeroVector;
		Spine9 = FVector::ZeroVector;
		Neck = FVector::ZeroVector;
		Neck1 = FVector::ZeroVector;
		Neck2 = FVector::ZeroVector;
		Neck3 = FVector::ZeroVector;
		Neck4 = FVector::ZeroVector;
		Neck5 = FVector::ZeroVector;
		Neck6 = FVector::ZeroVector;
		Neck7 = FVector::ZeroVector;
		Neck8 = FVector::ZeroVector;
		Neck9 = FVector::ZeroVector;
		Head = FVector::ZeroVector;
		LeftShoulder = FVector::ZeroVector;
		LeftShoulderExtra = FVector::ZeroVector;
		LeftArm = FVector::ZeroVector;
		LeftForeArm = FVector::ZeroVector;
		LeftHand = FVector::ZeroVector;
		RightShoulder = FVector::ZeroVector;
		RightShoulderExtra = FVector::ZeroVector;
		RightArm = FVector::ZeroVector;
		RightforeArm = FVector::ZeroVector;
		RightHand = FVector::ZeroVector;
		LeftUpLeg = FVector::ZeroVector;
		LeftLeg = FVector::ZeroVector;
		LeftFoot = FVector::ZeroVector;
		LeftToeBase = FVector::ZeroVector;
		RightUpLeg = FVector::ZeroVector;
		RightLeg = FVector::ZeroVector;
		RightFoot = FVector::ZeroVector;
		RightToeBase = FVector::ZeroVector;
	}
};

USTRUCT(BlueprintType)
struct FWorldSpaceHumanState
{
	GENERATED_USTRUCT_BODY()
		FWorldSpaceHumanQuat quats;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChingMU")
		FTimecode timecode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FWorldSpaceHumanRotator Rotators;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FWorldSpaceHumanPosition positions;
};







