// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BP_DragonEnemy.h"
#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"


// Sets default values
AC_BP_DragonEnemy::AC_BP_DragonEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator{0.f, 0.f, 0.f});

	DragonStateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

// Called when the game starts or when spawned
void AC_BP_DragonEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_BP_DragonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_BP_DragonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AC_BP_DragonEnemy::DragonMove(const FInputActionValue& ActionValue)
{
	FVector2D ActionVector = ActionValue.Get<FVector2D>();

	
	AddMovementInput(GetActorForwardVector(), ActionVector.Y);
	AddMovementInput(GetActorRightVector(), ActionVector.X);
}

