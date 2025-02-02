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

