// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_WBP_MainUI.generated.h"

/**
 * 
 */
UCLASS()
class LIE_OF_DRAGON_API UC_WBP_MainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Score")
	void UpdateScoreFromCoin(int32 ScoreToAdd);
	virtual void UpdateScoreFromCoin_Implementation(int32 ScoreToAdd);
	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HP")
	void UpdateHPFromPlayer(float HPPercent);
	virtual void UpdateHPFromPlayer_Implementation(float HPPercent);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HP")
	void UpdateStammFromPlayer(float StammPercent);
	virtual void UpdateStammFromPlayer_Implementation(float StammPercent);
	
	
};
