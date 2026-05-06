// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BP_pickubleitem.h"
#include "InputCoreTypes.h"
#include "BP_C_Player.h"
#include "C_WBP_MainUI.h"




// Sets default values
AC_BP_pickubleitem::AC_BP_pickubleitem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp  = CreateDefaultSubobject<USphereComponent>(TEXT("C_BoxCollision"));
	RootComponent = SphereComp;
	SphereComp ->SetSphereRadius(180.f);
	
	SphereComp ->SetGenerateOverlapEvents(true);
	SphereComp ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp ->SetCollisionResponseToAllChannels(ECR_Overlap); // ======= MAYBEE DELETEEE=====

}

// Called when the game starts or when spawned
void AC_BP_pickubleitem::BeginPlay()
{
	
	
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("coin BeginPlay сработал"));
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BP_pickubleitem::CoinOnOverlapBegin);
	
	UE_LOG(LogTemp, Warning, TEXT("coin Overlap подписан"));
	
}

// Called every frame
void AC_BP_pickubleitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AC_BP_pickubleitem::CoinOnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("IM INSIDE COIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIN"));
	ABP_C_Player* Player = Cast<ABP_C_Player>(OtherActor);
	if (!IsValid(Player)) return;

	UE_LOG(LogTemp, Warning, TEXT("Widget class: %s"), 
		IsValid(Player->MyWidgetInstance) ? *Player->MyWidgetInstance->GetClass()->GetName() : TEXT("nullptr"));

	UC_WBP_MainUI* MainUI = Cast<UC_WBP_MainUI>(Player->MyWidgetInstance);
	
	
	MainUI->UpdateScoreFromCoin(100);
	MainUI->UpdateHPFromPlayer(Player->C_CurrentHealth/Player->C_MaxHealth);
	
	Player->C_CurrentStammina -= 20;
	MainUI->UpdateStammFromPlayer(Player->C_CurrentStammina/Player->C_MaxStammina);
	Destroy();
	
}





