// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetInventoryCountNumber(int InventoryNumber)
{
	FString InventoryNumberString = FString::Printf(TEXT("%d"), InventoryNumber);

	if (InventoryCountNumber)
	{
		InventoryCountNumber->SetText(FText::FromString(InventoryNumberString));
	}
}

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UPlayerHUD::ShowGameOverText(bool bGameEnded)
{
	if (GameOverText)
	{
		if (bGameEnded)
		{
			GameOverText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			GameOverText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

