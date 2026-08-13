// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayUtils.h"

void UGameplayUtils::PrintDebug(const float TimeToDisplay,const FColor TextColor,const FString& PrintText, const UObject* ThisObject)
{
	if (!ThisObject) return;
	if (PrintText.IsEmpty()) return;
	
	const FString& ObjectName = GetNameSafe(ThisObject);
	const FString& DebugFormat = FString::Printf(TEXT("Object: %s, %s"), *ObjectName, *PrintText);
	
	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, DebugFormat);
}
