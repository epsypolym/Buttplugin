// Copyright Epic Games, Inc. All Rights Reserved.


#include "ButtplugBPLibrary.h"


#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "IWebSocketsManager.h"


#include "Buttplug.h"


UButtplugBPLibrary::UButtplugBPLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

float UButtplugBPLibrary::ButtplugSampleFunction(float Param)
{
	//TSharedPtr<IWebSocket> Socket = FWebSocketsModule::Get().CreateWebSocket("ws://localhost:12345/buttplug");

	//Socket->Connect();

	return -1;
}
