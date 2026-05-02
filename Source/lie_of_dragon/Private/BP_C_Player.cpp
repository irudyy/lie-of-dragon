// Fill out your copyright notice in the Description page of Project Settings.



/*
// Basic message
UE_LOG(LogTemp, Warning, TEXT("Hello from C++!"));

// Message with variables (using FString::Printf style formatting)
FString PlayerName = "John";
int32 Score = 100;
UE_LOG(LogTemp, Display, TEXT("Player %s has a score of: %d"), *PlayerName, Score);











*/

#include "BP_C_Player.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "GameFramework/GameSession.h"
#include "GeometryCollection/GeometryCollectionDebugDrawActor.h"

// Sets default values
ABP_C_Player::ABP_C_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	PlayerCamera->SetupAttachment(SpringArm);
	
	GetMesh()->SetRelativeRotation(FRotator {0.f, -90.f, 0.f});
	

}

// Called when the game starts or when spawned
void ABP_C_Player::BeginPlay()
{
	Super::BeginPlay();
	
if	(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
{
	if (ULocalPlayer * LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}
	
	
	
	
	
	if (MyWidgetClass) // Проверяем, выбрали ли мы класс в редакторе
	{
		// 1. Получаем Player Controller (Owning Player)
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		// 2. Создаем виджет (аналог ноды Construct)
		MyWidgetInstance = CreateWidget<UUserWidget>(PC, MyWidgetClass);

		// 3. Добавляем на экран (аналог ноды Add to Viewport)
		if (MyWidgetInstance)
		{
			MyWidgetInstance->AddToViewport();
		}
	}
}

// Called every frame
void ABP_C_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickCounter+=1;
	if (TickCounter == 100)
	{
		TickCounter = 0;
		PlayerSecond();
	}
}

// Called to bind functionality to input
void ABP_C_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if 	(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent -> BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerJump);
		EnhancedInputComponent -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerMove);
		//EnhancedInputComponent -> BindAction(DashAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerDash);
		//EnhancedInputComponent -> BindAction(LookAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerLook);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABP_C_Player::PlayerStartDash);
		//EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Canceled, this, &ABP_C_Player::PlayerStopDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ABP_C_Player::PlayerEndDash);
	}
	
}

void ABP_C_Player::PlayerJump()
{
	Jump();
}

void ABP_C_Player::PlayerMove(const FInputActionValue& ActionValue)
{
	FVector2D ActionVector = ActionValue.Get<FVector2D>();

	
	AddMovementInput(GetActorForwardVector(), ActionVector.Y);
	AddMovementInput(GetActorRightVector(), ActionVector.X);
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("MOVING!"));
}


void ABP_C_Player::PlayerStartDash(const FInputActionValue& ActionValue)
{
	
	if (StamminaProcent <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
		
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("SPRINT ON"));
	}
}
void ABP_C_Player::PlayerEndDash(const FInputActionValue& ActionValue)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("SPRINT OFF"));
	}
}




/*void ABP_C_Player::PlayerLook(const FInputActionValue& ActionValue)
{
	FVector2D ActionRotation = ActionValue.Get<FVector2D>();
	AddControllerYawInput(ActionRotation.X);
	AddControllerPitchInput(ActionRotation.Y);
}
*/

void ABP_C_Player::PlayerSecond()
{
	if (StamminaProcent <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	if (GetCharacterMovement() -> MaxWalkSpeed == 1500.0f && StamminaProcent != 0)
	{
		StamminaProcent = StamminaProcent -40;
	}
	if (GEngine)
	{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Second"));
	}
	if (StamminaProcent>= 100)
	{}
	else
	{
		StamminaProcent = StamminaProcent +20;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::SanitizeFloat(StamminaProcent));
}

void ABP_C_Player::C_StartQTERound()
{
	
}

void ABP_C_Player::C_FaliRound()
{
}

void ABP_C_Player::C_SuccessRound()
{
}

void ABP_C_Player::C_EnterBatZone()
{
}

void ABP_C_Player::C_ExitBatZone()
{
}

void ABP_C_Player::C_TakeDamageFromEnemy()
{
}

void ABP_C_Player::C_HPBuff()
{
}

void ABP_C_Player::C_LavaDamage()
{
}

void ABP_C_Player::C_HealthBarSettings()
{
}

void ABP_C_Player::C_UpdPlayerHealth()
{
}

void ABP_C_Player::C_UpdPlayerStammina()
{
}

