#pragma once


#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "IWebSocketsManager.h"
#include "ButtplugSubsystem.generated.h"



USTRUCT()
struct FDingle
{
	GENERATED_BODY()
public:
		UPROPERTY()
		int Index;
		UPROPERTY()
		FString ActuatorType;
		UPROPERTY()
		double Scalar;
	

};


UCLASS()
class UButtplugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<IWebSocket> Socket;


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void GoButtplugGo(int Index, FString ActuatorType, double Scalar, int Id, int DeviceIndex);
	UFUNCTION(BlueprintCallable)
	void ConnectToServer();

	void Connected();

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