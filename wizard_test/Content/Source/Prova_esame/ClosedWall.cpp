// Fill out your copyright notice in the Description page of Project Settings.


#include "ClosedWall.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AClosedWall::AClosedWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/ThirdPerson/Maps/_GENERATED/barto/SMesh_closedWall"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void AClosedWall::BeginPlay()
{
	Super::BeginPlay();
	
	// Per impedire che due o più muri vengano spawnati nello stesso punto da RoomGenerators diversi...
	TArray<AActor*> closedWalls;
	FVector otherWallPos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AClosedWall::StaticClass(), closedWalls);

	for (AActor* closedWall : closedWalls) {
		otherWallPos = closedWall->GetActorLocation();

		if (closedWall != this) {
			if (FVector::DistXY(otherWallPos, GetActorLocation()) < 5) {
				Destroy();
			}
		}
	}
}

// Called every frame
void AClosedWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

