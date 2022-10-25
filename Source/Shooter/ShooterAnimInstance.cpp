// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	//ShooterCharacter pointer� bo�. ��ini dolduruyoruz.
	//TryGetPawnOwner diyerek kullan�lan pawn� Shoter karaktere d�k�yoruz.
	ShooterCharacter =Cast<AShooterCharacter> (TryGetPawnOwner());

}