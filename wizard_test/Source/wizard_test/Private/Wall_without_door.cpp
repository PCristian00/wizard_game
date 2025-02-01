// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall_without_door.h"

// Sets default values
AWall_without_door::AWall_without_door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/ThirdPerson/Maps/_GENERATED/barto/SMesh_wallWithoutDoor"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void AWall_without_door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall_without_door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

