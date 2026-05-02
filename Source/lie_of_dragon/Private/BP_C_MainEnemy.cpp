// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_C_MainEnemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "GameFramework/GameSession.h"
#include "GeometryCollection/GeometryCollectionDebugDrawActor.h"
#include "StateTree.h"


#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"


// Sets default values
ABP_C_MainEnemy::ABP_C_MainEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeRotation(FRotator {0.f, 0.f, 0.f});
	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}



// Called when the game starts or when spawned
void ABP_C_MainEnemy::BeginPlay()
{
	Super::BeginPlay();
}

	


// Called every frame
void ABP_C_MainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABP_C_MainEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABP_C_MainEnemy::EnemyMove(const FInputActionValue& ActionValue)
{
	FVector2D ActionVector = ActionValue.Get<FVector2D>();

	
	AddMovementInput(GetActorForwardVector(), ActionVector.Y);
	AddMovementInput(GetActorRightVector(), ActionVector.X);
}

