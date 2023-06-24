#include "ButtplugSubsystem.h"

#include "WebSocketsModule.h"
#include "ButtplugDevice.h"
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
	Socket->Send("[\n  {\n    \"RequestServerInfo\": {\n      \"Id\": 1,\n      \"ClientName\": \"Unreal Buttplugin\",\n      \"MessageVersion\": 3\n    }\n  }\n]");	
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
	Socket->Send("[{ \n\t\"ScalarCmd\": {\n\t\t\"Id\": 1,\n\t\t\"DeviceIndex\": 1,\n\t\t\"Scalars\":\n\t\t[ \n\t\t\t{\n\t\t\t\t\"Index\": 0,\n\t\t\t\t\"ActuatorType\": \"Vibrate\",\n\t\t\t\t\"Scalar\": 0.1\n\t\t\t}\n\t\t]\n\t}\n}]");
}

UPROPERTY(BlueprintReadWrite)
	TArray<UButtplugDevice> devices;

void UButtplugSubsystem::AddDevice(int DeviceId, FString ActuatorType)
{
	UButtplugDevice swag;
	swag.DeviceIndex = 0;
	swag.DeviceName = "XBox (XInput) Compatible Gamepad";
	swag.Actuators.Add("Vibrate");
	swag.Actuators.Add("Vibrate");
	devices.Emplace(swag);
	UE_LOG(LogTemp, Warning, TEXT("printing first device array data as follows:"));
	UE_LOG(LogTemp, Warning, TEXT("Device Index:%d"), devices[0].DeviceIndex);
	UE_LOG(LogTemp, Warning, TEXT("Device Name:%s"), *devices[0].DeviceName);
	UE_LOG(LogTemp, Warning, TEXT("Actuator 1:%s"), *devices[0].Actuators[0]);
	UE_LOG(LogTemp, Warning, TEXT("Actuator 2:%s"), *devices[0].Actuators[1]);

}



void UButtplugSubsystem::Disconnect()
{
	Socket->Close();
}


struct FButtplugScalarScalars : public FJsonSerializable
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

//void UButtplugSubsystem::ConstructDeviceEntry(int deviceindex, FButtplugScalar scalars)
//{

//}

//void UButtplugSubsystem::AddDeviceToDeviceList(FButtplugDevice device)
//{

//}

class FButtplugScalarMsg : public FJsonSerializable
{

public:
	int        Id;
	int        DeviceIndex;
	TArray<FButtplugScalarScalars> Scalars;
		
	// FJsonSerializable
	BEGIN_JSON_SERIALIZER
	JSON_SERIALIZE("Id", Id);
	JSON_SERIALIZE("DeviceIndex", DeviceIndex);
	JSON_SERIALIZE_ARRAY_SERIALIZABLE("Scalars", Scalars, FButtplugScalarScalars);
	END_JSON_SERIALIZER
};


void UButtplugSubsystem::SendScalarCommand(int MessageId, int DeviceIndex, int ScalarIndex, FString ActuatorType, double Scalar)
{
	FButtplugScalarMsg Joe;
	FButtplugScalarScalars Bob;
	Bob.Index = ScalarIndex;
	Bob.ActuatorType = ActuatorType;
	Bob.Scalar = Scalar;
	Joe.Id = MessageId;
	Joe.DeviceIndex = DeviceIndex;
	Joe.Scalars.Emplace(Bob);


	FString pebis = "[{\n\t\"ScalarCmd\": ";
	pebis.Append(Joe.ToJson());
	pebis.Append("}]");
	UE_LOG(LogTemp, Warning, TEXT("json time:%s"), *pebis);

	Socket->Send(pebis);
};


