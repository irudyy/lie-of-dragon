// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_C_MainEnemy.h"

#include "C_BP_EnemyWidget.h"
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
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator{0.f, 0.f, 0.f});

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
/*
	WidgetHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	WidgetHealthBar->SetupAttachment(RootComponent);
	WidgetHealthBar->SetWidgetSpace(EWidgetSpace::World);
	WidgetHealthBar->SetDrawSize(FVector2D(300.0f, 40.0f));
	WidgetHealthBar->SetPivot(FVector2D(0.5f, 0.5f));
	WidgetHealthBar->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
	WidgetHealthBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
}



// Called when the game starts or when spawned
void ABP_C_MainEnemy::BeginPlay()
{
	Super::BeginPlay();

/*	if (!WidgetHealthBar)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy BeginPlay: WidgetHealthBar is NULL"));
		return;
	}

	if (MyEnemyWidgetClass)
	{
		WidgetHealthBar->SetWidgetClass(MyEnemyWidgetClass);
	}

	WidgetHealthBar->InitWidget();

	MyEnemyWidgetInstance = WidgetHealthBar->GetUserWidgetObject();
	EnemyHealthBarWidget = Cast<UC_BP_EnemyWidget>(MyEnemyWidgetInstance.Get());

	if (!MyEnemyWidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy BeginPlay: MyEnemyWidgetInstance is NULL. Set MyEnemyWidgetClass to WB_EnemyHealthBar."));
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enemy widget instance is NULL. Set MyEnemyWidgetClass."));
		}

		return;
	}

	if (!EnemyHealthBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy BeginPlay: Cast to UC_BP_EnemyWidget failed. Actual class: %s"),
			*MyEnemyWidgetInstance->GetClass()->GetName());

		if (GEngine)
		{
			const FString Msg = FString::Printf(
				TEXT("Enemy widget cast failed: %s"),
				*MyEnemyWidgetInstance->GetClass()->GetName()
			);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Msg);
		}

		return;
	}

	//UpdateEnemyHealthWidget();*/
}

	


// Called every frame
void ABP_C_MainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateEnemyHealthWidget();

	/*if (WidgetHealthBar)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC && PC->PlayerCameraManager)
		{
			const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				WidgetHealthBar->GetComponentLocation(),
				CameraLocation
			);

			WidgetHealthBar->SetWorldRotation(LookAtRotation);
		}
	}*/
}

/*void ABP_C_MainEnemy::UpdateEnemyHealthWidget()
{
	if (!WidgetHealthBar)
	{
		return;
	}

	if (!EnemyHealthBarWidget)
	{
		UUserWidget* UserWidget = WidgetHealthBar->GetUserWidgetObject();

		if (!UserWidget)
		{
			if (MyEnemyWidgetClass)
			{
				WidgetHealthBar->SetWidgetClass(MyEnemyWidgetClass);
			}

			WidgetHealthBar->InitWidget();
			UserWidget = WidgetHealthBar->GetUserWidgetObject();
		}

		MyEnemyWidgetInstance = UserWidget;
		EnemyHealthBarWidget = Cast<UC_BP_EnemyWidget>(UserWidget);
	}

	if (!EnemyHealthBarWidget)
	{
		return;
	}

	const float HealthPercent = C_MaxHealth > 0.0
		? FMath::Clamp(static_cast<float>(C_CurrentHealth / C_MaxHealth), 0.0f, 1.0f)
		: 0.0f;

	EnemyHealthBarWidget->UpdateHealth(HealthPercent);
}
*/
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
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		// numAnim начинается с 1, поэтому -1 для индекса массива
		int32 Index = numAnim - 1;

		if (!enemyAnimations.IsValidIndex(Index) || !enemyAnimations[Index]) return;
    
		// если эта анимация уже играет — не перезапускаем
		if (enemyCurrentAnimIndex == Index) return;

		enemyCurrentAnimIndex = Index;
		GetMesh()->PlayAnimation(enemyAnimations[Index], true); // true = зацикливать
	}
}

