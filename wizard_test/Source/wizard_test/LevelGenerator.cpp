// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "Door.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Error, TEXT("A cube has been spawned"));
	FActorSpawnParameters spawnParams;
	GetWorld()->SpawnActor<ADoor>(FVector(1200, 1500, 400), FRotator(0, 0, 0), spawnParams);
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

