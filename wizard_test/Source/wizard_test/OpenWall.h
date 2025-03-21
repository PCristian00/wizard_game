// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "OpenWall.generated.h"

UCLASS()
class WIZARD_TEST_API AOpenWall : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AOpenWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const int roomsLengthInTiles = 7; // lunghezza (espressa in tiles) del lato delle stanze
	const int floorTilesWidth = 200; // lunghezza delle tiles quadrate componenti il pavimento delle stanze

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
