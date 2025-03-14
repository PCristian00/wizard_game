// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Assegnazione all'attore di una radice e di una static mesh
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;


	static ConstructorHelpers::FClassFinder<ACharacter> AssetFile_1(TEXT("/Game/NavigationSystem/MeleeMannequin.MeleeMannequin_C"));
	if (AssetFile_1.Class != nullptr) {
		meleeEnemy = AssetFile_1.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Melee enemy BP was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> AssetFile_2(TEXT("/Game/NavigationSystem/RangedMannequin.RangedMannequin_C"));
	if (AssetFile_2.Class != nullptr) {
		rangedEnemy = AssetFile_2.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Ranged enemy BP was not found"));
	}


}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

