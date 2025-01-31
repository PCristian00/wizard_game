// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall_with_door.h"

// Sets default values
AWall_with_door::AWall_with_door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/ThirdPerson/Maps/_GENERATED/barto/SMesh_wallWithDoor"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void AWall_with_door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall_with_door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

