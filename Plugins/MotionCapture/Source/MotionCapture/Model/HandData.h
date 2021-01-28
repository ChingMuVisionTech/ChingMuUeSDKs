#pragma once
#include "GameFramework/Actor.h"
#include "HandData.generated.h"

USTRUCT(BlueprintType)
struct FFingersName
{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName palm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName thumb1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName thumb2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName thumb3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName index1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName index2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName index3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName middle1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName middle2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName middle3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName ring1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName ring2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName ring3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName pinky1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName pinky2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FName pinky3;
};


USTRUCT(BlueprintType)
struct FWorldSpaceHandState
{
	GENERATED_USTRUCT_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")		
		FTransform palm=FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform thumb1 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform thumb2 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform thumb3 = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform index1 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform index2 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform index3 = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform middle1 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform middle2 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform middle3 = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform ring1 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform ring2 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform ring3 = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform pinky1 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform pinky2 = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FTransform pinky3 = FTransform();
};
