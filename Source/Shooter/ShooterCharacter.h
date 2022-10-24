// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OutputLog();

private:
	/*Heryerde g�z�ks�n, blueprintten ula��labilsin ama de�i�tirilmesin, 
	kategori kamera olsun, �zel (private) tan�mlad���m�z i�in bu keywordu
	yazmam�z gerekiyor. yoksa kabul etmezdi*/

	// CAMERA SPRING ARM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//FOLLOW CAMERA

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	//GET CAMERABOOM
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }		

	//GET FOLLOWCAMERA
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	//herhangi bir de�er de�i�tirmedi�imiz i�in const yapt�k
	//Forceinline dedi�imizde sadece >>return CameraBoom<< k�sm�n� derlemeye �al��acak


};
