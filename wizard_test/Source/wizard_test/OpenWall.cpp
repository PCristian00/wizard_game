// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWall.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOpenWall::AOpenWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/3D_Models/OpenWallWithTorches/Meshes/openWallWithTorches"));

	Mesh->SetStaticMesh(MeshObj.Object);

	Light_1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_1"));
	Light_2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_2"));
	Light_3 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_3"));
	Light_4 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light_4"));

	Light_1->SetupAttachment(RootComponent);
	Light_2->SetupAttachment(RootComponent);
	Light_3->SetupAttachment(RootComponent);
	Light_4->SetupAttachment(RootComponent);

	Light_1->SetRelativeLocation(FVector(-100, -180, 300));
	Light_2->SetRelativeLocation(FVector(-100, 180, 300));
	Light_3->SetRelativeLocation(FVector(100, -180, 300));
	Light_4->SetRelativeLocation(FVector(100, 180, 300));

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
void AOpenWall::BeginPlay()
{
	Super::BeginPlay();

	// Per impedire che due o più muri vengano spawnati nello stesso punto da RoomGenerators diversi...
	TArray<AActor*> openWalls;
	FVector otherWallPos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOpenWall::StaticClass(), openWalls);

	for (AActor* openWall : openWalls) {
		otherWallPos = openWall->GetActorLocation();

		if (openWall != this) {
			if (FVector::DistXY(otherWallPos, GetActorLocation()) < 5) {
				Destroy();
			}
		}
	}

	//Evita di generare la navmesh sul muro (i nemici dentro una stanza rimangono al suo interno)
	Mesh->SetCanEverAffectNavigation(false);

}

// Called every frame
void AOpenWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

