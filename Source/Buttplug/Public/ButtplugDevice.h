// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ButtplugDevice.generated.h"

/**
 * 
 */
//UCLASS(BlueprintType, Blueprintable)
class UButtplugDevice
{
	//GENERATED_BODY()
public:
	int DeviceIndex;
	FString DeviceName;
	TArray<FString> Actuators;
private:

};
