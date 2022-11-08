// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	//Karakter ge�erli de�il ise
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}

	//Karakter ge�erli ise
	if (ShooterCharacter)
	{
		//HIZI HESAPLA
		//Yanal h�z� hesaplayal�m
		FVector Velocity{ ShooterCharacter->GetVelocity() };
		//H�z�n yukar�-a�a�� (Z) y�n�n� s�f�rlamam�z gerekiyor yoksa yukar�dan a�a�� d��erken
		//ya da z�plarken h�z de�i�ir
		Velocity.Z = 0.f;
		Speed = Velocity.Size();	//Vekt�r� float de�ere d�n��t�rm�� olduk.

		//HAVADA MI?
		//#include "GameFramework/CharacterMovementComponent.h"
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();	//CharacterMovement.h � dahil etmemiz gerekiyor.

		//HAREKET HAL�NDE M�?
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
		{//Vekt�r d�nd�rd��� i�in GetCurrentAcceleration dedikten sonra .Size dedik	 
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
			//D�PNOT: Normalde ivme 0 ken h�z 0 dan farkl� olabilir. Ama burda kastedilen ivme karakterin hareket halinde olup olmad���d�r.	
		}
		//Bu hedeflenen yone karsilik gelen rotatoru veriyor 
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();	//Karakter hangi yone bakiyor?
		//FString RotationMessage = FString::Printf(TEXT("Base Aim Rotation: %f"), AimRotation.Yaw); //Karakterin yanal bakis yonu
		
		//Karakterin hareket sirasindaki yonu (tam sag 90, tam sol -90, ileri0, geri 180) x yonunde 
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());//#include "Kismet/KismetMathLibrary.h"
		//Yukaridaki kod karakterin dunya x konumuna gore yonunu veriyor
		//FString MovementRotationMessage = FString::Printf(TEXT("Movement Rotation: %f"), MovementRotation.Yaw);

		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;	//Bu float dondurdugu icin sadece yaw degerini aldik
		//Yukaridaki kod iki rotator arasindaki farki alir
		
		if (ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
		FString OffsetMessage = FString::Printf(TEXT("MovementOffsetYaw: %f"), MovementOffsetYaw);
		
		//PrintScreen kod tarafinda bu sekilde:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, OffsetMessage);

		}
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	//ShooterCharacter pointer� bo�. ��ini dolduruyoruz.
	//TryGetPawnOwner diyerek kullan�lan pawn� Shoter karaktere d�k�yoruz.
	ShooterCharacter =Cast<AShooterCharacter> (TryGetPawnOwner());

}