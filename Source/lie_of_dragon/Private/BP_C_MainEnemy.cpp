// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_C_MainEnemy.h"
#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"


// Sets default values
ABP_C_MainEnemy::ABP_C_MainEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator{0.f, 0.f, 0.f});

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

void ABP_C_MainEnemy::C_UpdEnemyAnimation(int numAnim)
{
	{
		USkeletalMeshComponent* MeshComponent = GetMesh();
		if (!MeshComponent) return;

		UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
		if (!AnimInstance) return;

		// numAnim starts from 1, so for massive index -1
		int32 Index = numAnim - 1;

		if (!enemyAnimations.IsValidIndex(Index) || !enemyAnimations[Index]) return;

		//if this animation playing then do not restart this animation
		if (enemyCurrentAnimIndex == Index) return;

		enemyCurrentAnimIndex = Index;
		MeshComponent->PlayAnimation(enemyAnimations[Index], true); // true = loop
	}
}

