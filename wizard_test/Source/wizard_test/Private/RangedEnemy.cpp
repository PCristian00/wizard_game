// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"
#include "RoomGenerator.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARangedEnemy::ARangedEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/ThirdPerson/Maps/_GENERATED/barto/SMesh_rangedEnemy"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();

}


void ARangedEnemy::Destroyed() {
	UE_LOG(LogTemp, Error, TEXT("A ranged enemy was destroyed"));

	// Trova l'attore di tipo "ARoomGenerator" più vicino e ne chiama la funzione ""
	double currentDistance, shortestDistance;
	int indexOfClosestRoomGen = 0;
	FVector roomGeneneratorPos, enemyPos;
	TArray<AActor*> roomGenerators;

	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ARoomGenerator::StaticClass(), roomGenerators);

	roomGeneneratorPos = roomGenerators[0]->GetActorLocation();
	enemyPos = GetActorLocation();
	shortestDistance = FVector::DistXY(roomGeneneratorPos, enemyPos);
	indexOfClosestRoomGen = 0;

	for (int i = 1; i < roomGenerators.Num(); i++) {
		roomGeneneratorPos = roomGenerators[i]->GetActorLocation();
		currentDistance = FVector::DistXY(roomGeneneratorPos, enemyPos);

		if (currentDistance < shortestDistance) {
			shortestDistance = currentDistance;
			indexOfClosestRoomGen = i;
		}
	}

	ARoomGenerator* nearestRoomGenerator = Cast<ARoomGenerator>(roomGenerators[indexOfClosestRoomGen]);
	nearestRoomGenerator->UpdateRoomUponEnemyDeath();
}


// Called every frame
void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

