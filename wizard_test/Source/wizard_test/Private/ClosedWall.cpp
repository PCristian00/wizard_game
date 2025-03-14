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

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/ThirdPerson/Maps/_GENERATED/barto/SMesh_closedWall"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/External_meshes/ClosedWallWithTorches/closedWallWithTorches"));

	Mesh->SetStaticMesh(MeshObj.Object);

	Light_1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_1"));
	Light_2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_2"));
	Light_3 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_3"));
	Light_4 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_4"));

	Light_1->SetupAttachment(RootComponent);
	Light_2->SetupAttachment(RootComponent);
	Light_3->SetupAttachment(RootComponent);
	Light_4->SetupAttachment(RootComponent);

	Light_1->SetRelativeLocation(FVector(-85, -125, 320));
	Light_2->SetRelativeLocation(FVector(-85, 125, 320));
	Light_3->SetRelativeLocation(FVector(85, -125, 320));
	Light_4->SetRelativeLocation(FVector(85, 125, 320));

	Light_1->SetLightColor(FLinearColor(1.0f, 0.53f, 0.012f, true));
	Light_2->SetLightColor(FLinearColor(1.0f, 0.53f, 0.012f, true));
	Light_3->SetLightColor(FLinearColor(1.0f, 0.53f, 0.012f, true));
	Light_4->SetLightColor(FLinearColor(1.0f, 0.53f, 0.012f, true));

	Light_1->SetAttenuationRadius(1400);
	Light_2->SetAttenuationRadius(1400);
	Light_3->SetAttenuationRadius(1400);
	Light_4->SetAttenuationRadius(1400);

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
