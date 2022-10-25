// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	//ShooterCharacter pointerý boþ. Ýçini dolduruyoruz.
	//TryGetPawnOwner diyerek kullanýlan pawný Shoter karaktere döküyoruz.
	ShooterCharacter =Cast<AShooterCharacter> (TryGetPawnOwner());

}