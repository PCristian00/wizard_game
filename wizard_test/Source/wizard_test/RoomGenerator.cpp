// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGenerator.h"
#include "GameFramework/GameState.h"

// Sets default values
ARoomGenerator::ARoomGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Assegnazione degli assets dei power-ups alle rispettive classi C++
	static ConstructorHelpers::FClassFinder<AActor> DamageUpAsset(TEXT("/Game/PowerUp/BP_MoreDamage_PowerUp.BP_MoreDamage_PowerUp_C"));
	if (DamageUpAsset.Class != nullptr) {
		DamageUp = DamageUpAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Damage_up was not found"));
	}

	static ConstructorHelpers::FClassFinder<AActor> HealthUpAsset(TEXT("/Game/PowerUp/BP_MoreHealth_PowerUp.BP_MoreHealth_PowerUp_C"));
	if (HealthUpAsset.Class != nullptr) {
		HealthUp = HealthUpAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Health_up was not found"));
	}

	static ConstructorHelpers::FClassFinder<AActor> ManaUpAsset(TEXT("/Game/PowerUp/BP_MoreMana_PowerUp.BP_MoreMana_PowerUp_C"));
	if (ManaUpAsset.Class != nullptr) {
		ManaUp = ManaUpAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mana_up was not found"));
	}

	static ConstructorHelpers::FClassFinder<AActor> SpeedUpAsset(TEXT("/Game/PowerUp/BP_SpeedProjectiles_PowerUp.BP_SpeedProjectiles_PowerUp_C"));
	if (SpeedUpAsset.Class != nullptr) {
		SpeedUp = SpeedUpAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Speed_up was not found"));
	}

	static ConstructorHelpers::FClassFinder<AActor> TripleShotAsset(TEXT("/Game/PowerUp/BP_TripleShot_powerUp.BP_TripleShot_PowerUp_C"));
	if (TripleShotAsset.Class != nullptr) {
		TripleShot = TripleShotAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Triple_shot was not found"));
	}

	static ConstructorHelpers::FClassFinder<AActor> QuadrupleShotAsset(TEXT("/Game/PowerUp/BP_CrossShot_PowerUp.BP_CrossShot_PowerUp_C"));
	if (QuadrupleShotAsset.Class != nullptr) {
		QuadrupleShot = QuadrupleShotAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Quadruple_shot was not found"));
	}


	/* l'offset di posizione (posOffset) è in reatà
		*	(roomsLengthInTiles * floorTilesWidth) / 2 - floorTilesWidth / 2;
		*	Ora, definendo
		*	A = roomsLengthInTiles;		B = floorTilesWidth		si ha
		*	AB / 2 - B / 2  =  B / 2 * (A - 1)
	*/
	double posOffset = floorTilesWidth / 2 * (roomsLengthInTiles - 1);
	int directionX, directionY;
	int counter = 1;
	FString triggerName = "Trigger_";

	for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
		directionX = round(cos(angle * PI));
		directionY = -round(sin(angle * PI));
		triggers.Add(CreateDefaultSubobject<UBoxComponent>(FName(triggerName.Append(FString::FromInt(counter)))));
		triggers.Top()->SetupAttachment(RootComponent);
		triggers.Top()->SetRelativeScale3D(FVector(2, 2, 2));
		triggers.Top()->SetRelativeLocation(FVector(directionX, directionY, 0) * posOffset + FVector(0, 0, 200));
		counter++;
		triggerName = "Trigger_";
	}
}

// Called when the game starts or when spawned
void ARoomGenerator::BeginPlay()
{
	Super::BeginPlay();

}


void ARoomGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnActorBeginOverlap.AddDynamic(this, &ARoomGenerator::OnTriggerOverlap);
}


// Called every frame
void ARoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

