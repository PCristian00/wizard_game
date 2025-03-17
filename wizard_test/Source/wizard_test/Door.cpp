// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/3D_Models/Door/Meshes/Door"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Per impedire che due o più porte vengano spawnate nello stesso punto da RoomGenerators diversi...
	TArray<AActor*> doors;
	FVector otherDoorPos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), doors);

	for (AActor* door : doors) {
		otherDoorPos = door->GetActorLocation();

		if (door != this) {
			if (FVector::DistXY(otherDoorPos, GetActorLocation()) < 5) {
				Destroy();
			}
		}
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

