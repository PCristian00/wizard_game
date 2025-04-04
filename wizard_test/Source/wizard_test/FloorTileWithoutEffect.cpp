// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTileWithoutEffect.h"

// Sets default values
AFloorTileWithoutEffect::AFloorTileWithoutEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/3D_Models/StandardFloorTile/Meshes/SMesh_standardFloorTile"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void AFloorTileWithoutEffect::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFloorTileWithoutEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

