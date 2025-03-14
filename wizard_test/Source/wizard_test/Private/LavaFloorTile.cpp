// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaFloorTile.h"

// Sets default values
ALavaFloorTile::ALavaFloorTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/3D_Models/LavaFloorTile/SMesh_lavaFloorTile"));

	Mesh->SetStaticMesh(MeshObj.Object);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(GetActorLocation() + FVector(0, 0, 100));
	Trigger->SetRelativeScale3D(FVector(3.1, 3.1, 1));

}

// Called when the game starts or when spawned
void ALavaFloorTile::BeginPlay()
{
	Super::BeginPlay();

	//Serve per far sì che la navmesh non venga posizionata sulle tiles con effetto
	// (i nemici non devono camminare su tali tiles)
	Mesh->SetCanEverAffectNavigation(false);

	//Collega la funzione delegato al trigger
	OnActorBeginOverlap.AddDynamic(this, &ALavaFloorTile::OnTriggerOverlap);
}

// Called every frame
void ALavaFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

