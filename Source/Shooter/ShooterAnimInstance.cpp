// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	//Karakter geçerli deðil ise
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}

	//Karakter geçerli ise
	if (ShooterCharacter)
	{
		//HIZI HESAPLA
		//Yanal hýzý hesaplayalým
		FVector Velocity{ ShooterCharacter->GetVelocity() };
		//Hýzýn yukarý-aþaðý (Z) yönünü sýfýrlamamýz gerekiyor yoksa yukarýdan aþaðý düþerken
		//ya da zýplarken hýz deðiþir
		Velocity.Z = 0.f;
		Speed = Velocity.Size();	//Vektörü float deðere dönüþtürmüþ olduk.

		//HAVADA MI?
		//#include "GameFramework/CharacterMovementComponent.h"
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();	//CharacterMovement.h ý dahil etmemiz gerekiyor.

		//HAREKET HALÝNDE MÝ?
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
		{//Vektör döndürdüðü için GetCurrentAcceleration dedikten sonra .Size dedik	 
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
			//DÝPNOT: Normalde ivme 0 ken hýz 0 dan farklý olabilir. Ama burda kastedilen ivme karakterin hareket halinde olup olmadýðýdýr.	
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
	//ShooterCharacter pointerý boþ. Ýçini dolduruyoruz.
	//TryGetPawnOwner diyerek kullanýlan pawný Shoter karaktere döküyoruz.
	ShooterCharacter =Cast<AShooterCharacter> (TryGetPawnOwner());

}