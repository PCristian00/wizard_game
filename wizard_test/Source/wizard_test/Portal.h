// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class WIZARD_TEST_API APortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Trigger;


	UFUNCTION()
	void OnTriggerOverlap(class AActor* OverlappedActor, class AActor* OtherActor) {
		FString nameTarget = "BP_MainCharacter_C_0";
		FString actorName = OtherActor->GetDebugName(OtherActor);

		if (actorName == nameTarget) {
			FString currentLevel = GetWorld()->GetMapName();

			if (currentLevel == "UEDPIE_0_Level_1") {
				UE_LOG(LogTemp, Error, TEXT("You just entered level 2"));
				UGameplayStatics::OpenLevel(GetWorld(), "Level_2");
			}
			else if (currentLevel == "UEDPIE_0_Level_2") {
				UE_LOG(LogTemp, Error, TEXT("You just entered level 3"));
				UGameplayStatics::OpenLevel(GetWorld(), "Level_3");
			}
			else {
				UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
			}

			/* Usa questi if statements quando apri il gioco in modalità standalone (i nomi dei livelli cambiano)

			FString currentLevel = GetWorld()->GetMapName();

			if (currentLevel == "ThirdPersonMap") {
				UE_LOG(LogTemp, Error, TEXT("You just entered level 2"));
				UGameplayStatics::OpenLevel(GetWorld(), "Level_2");
			}
			else if (currentLevel == "Level_2") {
				UE_LOG(LogTemp, Error, TEXT("You just entered level 3"));
				UGameplayStatics::OpenLevel(GetWorld(), "Level_3");
			}
			else {
				UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
			}
			*/

		}

	}
};
