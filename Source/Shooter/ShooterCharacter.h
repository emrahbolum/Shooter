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

	//INPUT PARAMETER
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate); //Dönüþ Mouse
	void LookUpAtRate(float Rate);	

	void Turn(float Value);
	void LookUp(float Value);


	//SAG MOUSE (ZOOM) TIKLI MI?
	void AimingButtonPressed();
	void AimingButtonReleased();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OutputLog();


private:
	/*Heryerde gözüksün, blueprintten ulaþýlabilsin ama deðiþtirilmesin, 
	kategori kamera olsun, özel (private) tanýmladýðýmýz için bu keywordu
	yazmamýz gerekiyor. yoksa kabul etmezdi*/

	// CAMERA SPRING ARM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//FOLLOW CAMERA

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//Mouse hassaslýðý TEMEL DÖNÜÞ ORANI saniye/derece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/*Aiming sirasinda mouse hassasiyet icin yeni degiskenler*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	/*Mouse hassasiyet degiskenleri*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true", ClampMin="0.0", ClampMax="1.0",UIMin="0.0", UIMax="1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;


	//SOUND CUE		Silah Mermi random cue 10 ses dosyasindan olustu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	//MUZZLE FLASH (Namlu efekti)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	//MONTAGE 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	//IMPACT PARTICLES	//silah mermisinin darbe efekti
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	//BEAM PARTICLES mermiler icin duman izi efektix	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;


	/*ATES ETME MOUSE SOL CLICK*/
	void FireWeapon();

	/*PARCACIK KONUMU AL*/
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);
	
	/*ZOOM YAPMIS MI*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	//Gecerli kamera yakinligi
	float CameraCurrentFOV;

	//Kamera zoom enterpolasyon hizi
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ZoomInterpSpeed;

	//event tick icin kamera enterpolasyonu ve donusoranlari ayarlama 
	void CameraInterpZoom(float DeltaTime);
	void SetLookRates(float DeltaTime);


	



	/* Kamera varsayilan yakinlasma degeri*/
	float CameraDefaultFOV;
	/* Kamera zoomlu yakinlastirma degeri*/
	float CameraZoomedFOV;
public:
	//GET CAMERABOOM
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }		

	//GET FOLLOWCAMERA
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	//herhangi bir deðer deðiþtirmediðimiz için const yaptýk
	//Forceinline dediðimizde sadece >>return CameraBoom<< kýsmýný derlemeye çalýþacak

	FORCEINLINE bool GetAiming() const { return bAiming; }

};
