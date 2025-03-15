// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaFloorTile.h"

// Sets default values
ALavaFloorTile::ALavaFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALavaFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALavaFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

