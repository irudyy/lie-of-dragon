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
	

	WidgetHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealthBar"));
	WidgetHealthBar->SetupAttachment(RootComponent);
	WidgetHealthBar->SetWidgetSpace(EWidgetSpace::World);
	
}



// Called when the game starts or when spawned
void ABP_C_MainEnemy::BeginPlay()
{
	C_CurrentHealth = C_MaxHealth;
}

	


// Called every frame
void ABP_C_MainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// --- Cast To WB_EnemyHealthBar → SetPercent ---
	UUserWidget* Widget = WidgetHealthBar->GetUserWidgetObject();
	if (IsValid(Widget) && C_MaxHealth > 0.f)
	{
		UProgressBar* Bar = Cast<UProgressBar>(
			Widget->GetWidgetFromName(TEXT("HealtBar"))
		);
		if (Bar)
			Bar->SetPercent(C_CurrentHealth / C_MaxHealth);
	}

	// --- Поворот виджета к камере (точно как в Blueprint) ---
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC) || !IsValid(PC->PlayerCameraManager)) return;

	// Get Camera Location (через GetPlayerPawn → GetCameraLocation)
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

	// Get World Rotation компонента виджета → Get Rotation X Vector
	FRotator WidgetWorldRot = WidgetHealthBar->GetComponentRotation();
	FVector RotXVector = WidgetWorldRot.Vector(); // это и есть GetRotationXVector

	// Find Look at Rotation (Start = позиция виджета как точка, Target = камера)
	FVector WidgetLocation = WidgetHealthBar->GetComponentLocation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
		WidgetLocation,  // Start (откуда смотрим)
		CameraLocation   // Target (куда смотрим)
	);

	// Set World Rotation
	WidgetHealthBar->SetWorldRotation(LookAtRot);
	
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

