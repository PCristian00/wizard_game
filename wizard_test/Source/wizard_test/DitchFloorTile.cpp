// Fill out your copyright notice in the Description page of Project Settings.


#include "DitchFloorTile.h"

// Sets default values
ADitchFloorTile::ADitchFloorTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/3D_Models/DitchFloorTile/Meshes/SMesh_ditchFloorTile"));

	Mesh->SetStaticMesh(MeshObj.Object);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(GetActorLocation() + FVector(0, 0, 100));
	Trigger->SetRelativeScale3D(FVector(3.1, 3.1, 1));
}

// Called when the game starts or when spawned
void ADitchFloorTile::BeginPlay()
{
	Super::BeginPlay();

	//Serve per far sì che la navmesh non venga posizionata sulle tiles con effetto
	// (i nemici non devono camminare su tali tiles)
	Mesh->SetCanEverAffectNavigation(false);

}

// Called every frame
void ADitchFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

