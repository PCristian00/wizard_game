// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "ClosedWall.generated.h"

UCLASS()
class WIZARD_TEST_API AClosedWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClosedWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	UPointLightComponent* Light_1;

	UPROPERTY()
	UPointLightComponent* Light_2;

	UPROPERTY()
	UPointLightComponent* Light_3;

	UPROPERTY()
	UPointLightComponent* Light_4;
};
