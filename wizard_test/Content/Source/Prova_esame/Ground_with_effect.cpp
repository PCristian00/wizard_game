// Fill out your copyright notice in the Description page of Project Settings.


#include "Ground_with_effect.h"

// Sets default values
AGround_with_effect::AGround_with_effect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Assegnazione all'attore di una radice e di una static mesh
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/ThirdPerson/Maps/_GENERATED/barto/SMesh_groundEffect"));

	Mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void AGround_with_effect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGround_with_effect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

