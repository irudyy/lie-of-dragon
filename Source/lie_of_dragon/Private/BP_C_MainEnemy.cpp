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
	
	

	WidgetHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("C_WidgetHealthBar"));
	WidgetHealthBar->SetupAttachment(RootComponent);
	WidgetHealthBar->SetWidgetSpace(EWidgetSpace::World);
	
	USceneComponent* WidgetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetRoot"));
	WidgetRoot->SetupAttachment(RootComponent);
	WidgetRoot->SetAbsolute(false, true, false); // наследует позицию, НО НЕ вращение

	WidgetHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealthBar"));
	WidgetHealthBar->SetupAttachment(WidgetRoot);
	WidgetHealthBar->SetWidgetSpace(EWidgetSpace::World);
	
}



// Called when the game starts or when spawned
void ABP_C_MainEnemy::BeginPlay()
{
	Super::BeginPlay();
    
	C_CurrentHealth = C_MaxHealth;

	// Ищем StateTreeComponent который создан в Blueprint
	StateTree = FindComponentByClass<UStateTreeComponent>();
    
	if (IsValid(StateTree))
	{
		StateTree->StartLogic();
		UE_LOG(LogTemp, Warning, TEXT("StateTree найден и запущен"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StateTree НЕ найден!"));
	}
	
	
	
}

	


// Called every frame
void ABP_C_MainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*// --- Обновление здоровья в виджете ---
	if (WidgetHealthBar)
	{
		UUserWidget* Widget = WidgetHealthBar->GetUserWidgetObject();
		if (Widget)
		{
			// Cast to WB_EnemyHealthBar и установка процента
			// Предполагается что у виджета есть функция SetPercent или ProgressBar с именем "HealtBar"
			UProgressBar* HealthBarWidget = Cast<UProgressBar>(
				Widget->GetWidgetFromName(TEXT("HealtBar"))
			);

			if (HealthBarWidget && C_MaxHealth > 0.f)
			{
				float Percent = C_CurrentHealth / C_MaxHealth;
				HealthBarWidget->SetPercent(Percent);
			}
		}
	}

	// --- Поворот виджета к камере игрока ---
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && WidgetHealthBar)
	{
		APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
		if (CameraManager)
		{
			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector ActorLocation = GetActorLocation();

			// Find Look at Rotation
			FRotator LookAtRot = FRotationMatrix::MakeFromX(
				CameraLocation - ActorLocation
			).Rotator();

			// Get Rotation X Vector + обратно в ротацию
			FVector RotXVector = WidgetHealthBar->GetComponentRotation().Vector();

			// Set World Rotation виджета
			WidgetHealthBar->SetWorldRotation(LookAtRot);
		}
	}*/
	
	// Cast To WB_EnemyHealthBar → SetPercent
	UUserWidget* Widget = WidgetHealthBar->GetUserWidgetObject();
	if (IsValid(Widget) && C_MaxHealth > 0.f)
	{
		UProgressBar* Bar = Cast<UProgressBar>(
			Widget->GetWidgetFromName(TEXT("HealtBar"))
		);
		if (Bar)
			Bar->SetPercent(C_CurrentHealth / C_MaxHealth);
	}

	// Set World Rotation X=0, Y=0, Z=180
	//WidgetHealthBar->SetWorldRotation(FRotator(0.f, 180.f, 0.f));

	// Поворот к камере в World Space
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC) || !IsValid(PC->PlayerCameraManager)) return;

	FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
	FVector WidgetLoc = WidgetHealthBar->GetComponentLocation();

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(WidgetLoc, CamLoc);

	// Коррекция — виджет в World Space смотрит по оси X
	// поэтому добавляем 180 к Yaw чтобы лицевая сторона смотрела на камеру
	LookAt.Yaw = 180.f;

	WidgetHealthBar->SetWorldRotation(LookAt);
	UE_LOG(LogTemp, Warning, TEXT("Rotator IS OOOOOOOOOOOOOOOOOOOOOOOOOnnnn"));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::SanitizeFloat(LookAt.Yaw));
	
	if (!IsValid(WidgetHealthBar)) return;

	
	
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

