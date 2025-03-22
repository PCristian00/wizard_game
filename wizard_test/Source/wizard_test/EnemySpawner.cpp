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

	// Assegnazione degli assets dei nemici melee alle rispettive classi C++
	static ConstructorHelpers::FClassFinder<ACharacter> iceSpiderAsset(TEXT("/Game/AI/enemy_melee/enemy_Spider/BP_enemy_spider_ice.BP_enemy_spider_ice_C"));
	if (iceSpiderAsset.Class != nullptr) {
		iceSpider = iceSpiderAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Ice_spider was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> fireSpiderAsset(TEXT("/Game/AI/enemy_melee/enemy_Spider/BP_C_enemy_spider_fire.BP_C_enemy_spider_fire_C"));
	if (fireSpiderAsset.Class != nullptr) {
		fireSpider = fireSpiderAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Fire_spider was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> lightSpiderAsset(TEXT("/Game/AI/enemy_melee/enemy_Spider/BP_C_enemy_spider_light.BP_C_enemy_spider_light_C"));
	if (lightSpiderAsset.Class != nullptr) {
		lightSpider = lightSpiderAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Light_spider was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> iceSnakeAsset(TEXT("/Game/AI/enemy_melee/enemy_Snake/BP_enemy_snake_ice.BP_enemy_snake_ice_C"));
	if (iceSnakeAsset.Class != nullptr) {
		iceSnake = iceSnakeAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Ice_snake was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> lightSnakeAsset(TEXT("/Game/AI/enemy_melee/enemy_Snake/BP_C_enemy_snake_light.BP_C_enemy_snake_light_C"));
	if (lightSnakeAsset.Class != nullptr) {
		lightSnake = lightSnakeAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Light_snake was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> boarAsset(TEXT("/Game/AI/enemy_melee/enemy_Boar/BP_enemy_boar.BP_enemy_boar_C"));
	if (boarAsset.Class != nullptr) {
		boar = boarAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Boar was not found"));
	}

	// Assegnazione degli assets dei nemici ranged alle rispettive classi C++
	static ConstructorHelpers::FClassFinder<ACharacter> iceBatAsset(TEXT("/Game/AI/enemy_fly/enemy_Bat/BP_enemy_bat_ice.BP_enemy_bat_ice_C"));
	if (iceBatAsset.Class != nullptr) {
		iceBat = iceBatAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Ice_bat was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> fireBatAsset(TEXT("/Game/AI/enemy_fly/enemy_Bat/BP_C_enemy_bat_fire.BP_C_enemy_bat_fire_C"));
	if (fireBatAsset.Class != nullptr) {
		fireBat = fireBatAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Fire_bat was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> lightBatAsset(TEXT("/Game/AI/enemy_fly/enemy_Bat/BP_C_enemy_bat_light.BP_C_enemy_bat_light_C"));
	if (lightBatAsset.Class != nullptr) {
		lightBat = lightBatAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Light_bat was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> iceWaspAsset(TEXT("/Game/AI/enemy_fly/enemy_Wasp/BP_enemy_wasp_ice.BP_enemy_wasp_ice_C"));
	if (iceWaspAsset.Class != nullptr) {
		iceWasp = iceWaspAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Ice_wasp was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> fireWaspAsset(TEXT("/Game/AI/enemy_fly/enemy_Wasp/BP_C_enemy_wasp_fire.BP_C_enemy_wasp_fire_C"));
	if (fireWaspAsset.Class != nullptr) {
		fireWasp = fireWaspAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Fire_wasp was not found"));
	}

	static ConstructorHelpers::FClassFinder<ACharacter> lightWaspAsset(TEXT("/Game/AI/enemy_fly/enemy_Wasp/BP_C_enemy_wasp_light.BP_C_enemy_wasp_light_C"));
	if (lightWaspAsset.Class != nullptr) {
		lightWasp = lightWaspAsset.Class;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Light_wasp was not found"));
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

