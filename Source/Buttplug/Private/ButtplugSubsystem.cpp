#include "ButtplugSubsystem.h"

#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "IWebSocketsManager.h"
#include "Serialization/JsonSerializer.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"



#include "Buttplug.h"

void UButtplugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UButtplugSubsystem::ConnectToServer()
{
	const FString ServerURL = TEXT("ws://127.0.0.1:12345/buttplug"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws");              // The WebServer protocol you want to use.

	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	Socket->OnConnected().AddLambda([=]() -> void 
		{
		UE_LOG(LogTemp, Warning, TEXT("Successfully connected to server"));
		Connected();
		}
	);

	Socket->OnConnectionError().AddLambda([](const FString& Error) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Connection Error: %s"), *Error);
		});

	Socket->OnMessage().AddLambda([](const FString& Message) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Msg from Server: %s"), *Message)
		});

	Socket->Connect();
}

void UButtplugSubsystem::Connected()
{
	Socket->Send("[\n  {\n    \"RequestServerInfo\": {\n      \"Id\": 1,\n      \"ClientName\": \"Unreal Buttplug\",\n      \"MessageVersion\": 3\n    }\n  }\n]");	
}

void UButtplugSubsystem::StartScan()
{
	Socket->Send("[\n  {\n    \"StartScanning\": {\n      \"Id\": 3\n    }\n  }\n]");
}

void UButtplugSubsystem::StopScan()
{
	Socket->Send("[\n  {\n    \"StopScanning\": {\n      \"Id\": 4\n    }\n  }\n]");
}

void UButtplugSubsystem::RequestDeviceList()
{
	Socket->Send("[\n  {\n    \"RequestDeviceList\": {\n      \"Id\": 2\n    }\n  }\n]");
}

void UButtplugSubsystem::StopAllDevices()
{
	Socket->Send("[\n  {\n    \"StopAllDevices\": {\n      \"Id\": 1\n    }\n  }\n]");
}

void UButtplugSubsystem::ButtplugGoBrr()
{
	Socket->Send("[{ \n\t\"ScalarCmd\": {\n\t\t\"Id\": 1,\n\t\t\"DeviceIndex\": 0,\n\t\t\"Scalars\":\n\t\t[ \n\t\t\t{\n\t\t\t\t\"Index\": 0,\n\t\t\t\t\"ActuatorType\": \"Vibrate\",\n\t\t\t\t\"Scalar\": 0.1\n\t\t\t}\n\t\t]\n\t}\n}]");
}

void UButtplugSubsystem::Disconnect()
{
	Socket->Close();
}


struct FDungle : public FJsonSerializable
{

public:
	int Index;
	FString ActuatorType;
	double Scalar;

	// FJsonSerializable
	BEGIN_JSON_SERIALIZER
	JSON_SERIALIZE("Index", Index);
	JSON_SERIALIZE("ActuatorType", ActuatorType);
	JSON_SERIALIZE("Scalar", Scalar);
	END_JSON_SERIALIZER
};


class FButtplugScalarMsg : public FJsonSerializable
{

public:
	int        Id;
	int        DeviceIndex;
	TArray<FDungle> Scalars;
		
	// FJsonSerializable
	BEGIN_JSON_SERIALIZER
	JSON_SERIALIZE("Id", Id);
	JSON_SERIALIZE("DeviceIndex", DeviceIndex);
	JSON_SERIALIZE_ARRAY_SERIALIZABLE("Scalars", Scalars, FDungle);
	END_JSON_SERIALIZER
};


void UButtplugSubsystem::GoButtplugGo(int Index, FString ActuatorType, double Scalar, int Id, int DeviceIndex)
{
	FButtplugScalarMsg Joe;
	FDungle Bob;
	Bob.Index = Index;
	Bob.ActuatorType = ActuatorType;
	Bob.Scalar = Scalar;
	Joe.Id = Id;
	Joe.DeviceIndex = DeviceIndex;
	Joe.Scalars.Emplace(Bob);


	FString pebis = "[{\n\t\"ScalarCmd\": ";
	pebis.Append(Joe.ToJson());
	pebis.Append("}]");
	UE_LOG(LogTemp, Warning, TEXT("json time:%s"), *pebis);

	Socket->Send(pebis);
};


