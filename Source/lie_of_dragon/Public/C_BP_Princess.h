// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_BP_Princess.generated.h"

class USphereComponent;
class USkeletalMeshComponent;

UCLASS()
class LIE_OF_DRAGON_API AC_BP_Princess : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_BP_Princess();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* PrincessSkeletalMesh;
};
