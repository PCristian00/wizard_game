// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWall.h"

// Sets default values
AOpenWall::AOpenWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOpenWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOpenWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

