// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DitchFloorTile.generated.h"

UCLASS()
class WIZARD_TEST_API ADitchFloorTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADitchFloorTile();

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
	UBoxComponent* Trigger;

};
