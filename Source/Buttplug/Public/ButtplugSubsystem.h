#pragma once


#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "ButtplugDevice.h"

#include "IWebSocketsManager.h"
#include "ButtplugSubsystem.generated.h"





UCLASS()
class UButtplugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<IWebSocket> Socket;


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	UFUNCTION(BlueprintCallable)
	void SendScalarCommand(int MessageId, int DeviceIndex, int ScalarIndex, FString ActuatorType, double Scalar);

	//UFUNCTION(BlueprintCallable)
	//void AddDeviceToDeviceList(FButtplugDevice device);
	//void ConstructDeviceEntry(int deviceindex, FButtplugScalar scalars);
	UFUNCTION(BlueprintCallable)
	void ConnectToServer();

	void Connected();

	UFUNCTION(BlueprintCallable)
	void AddDevice(int DeviceId, FString ActuatorType);
	UFUNCTION(BlueprintCallable)
	void Disconnect();
//	UFUNCTION(BlueprintCallable)
//	void AddDevice();
//	UFUNCTION(BlueprintCallable)
//	void RemoveDevice();
	UFUNCTION(BlueprintCallable)
	void StartScan();
	UFUNCTION(BlueprintCallable)
	void StopScan();
//	UFUNCTION(BlueprintCallable)
//	void Scan();
//	UFUNCTION(BlueprintCallable)
//	void Send();
	UFUNCTION(BlueprintCallable)
	void StopAllDevices();

	UFUNCTION(BlueprintCallable)
	void RequestDeviceList();

	UFUNCTION(BlueprintCallable)
	void ButtplugGoBrr();



};
