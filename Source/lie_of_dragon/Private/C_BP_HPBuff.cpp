// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BP_HPBuff.h"
#include "BP_C_Player.h"
#include "InputCoreTypes.h"
#include "BP_C_Player.h"



// Sets default values
AC_BP_HPBuff::AC_BP_HPBuff()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SphereComp  = CreateDefaultSubobject<USphereComponent>(TEXT("C_SphereCollision"));
	RootComponent = SphereComp;
	SphereComp ->SetSphereRadius(180.f);
	
	SphereComp ->SetGenerateOverlapEvents(true);
	SphereComp ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp ->SetCollisionResponseToAllChannels(ECR_Overlap); // ======= MAYBEE DELETEEE=====
}

// Called when the game starts or when spawned
void AC_BP_HPBuff::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("HP BeginPlay сработал"));
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BP_HPBuff::HPBuffOnOverlapBegin);
	
	UE_LOG(LogTemp, Warning, TEXT("HP Overlap подписан"));

}

// Called every frame
void AC_BP_HPBuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_BP_HPBuff::HPBuffOnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	
	UE_LOG(LogTemp, Warning, TEXT("IM INSIDE HP"));
	ABP_C_Player* Player = Cast<ABP_C_Player>(OtherActor);
	
	if (!IsValid(Player)) return;
	Player->LavaDamage(); 
	UE_LOG(LogTemp, Warning, TEXT("IM sended message to player  HP"));
	Destroy();
	
}
