// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	bAiming(false),
	CameraDefaultFOV(0.f),
	CameraZoomedFOV(35.f),
	ZoomInterpSpeed(30.f),
	CameraCurrentFOV(0.f),
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingLookUpRate(20.f),
	AimingTurnRate(20.f),
	MouseHipTurnRate(1.f),
	MouseHipLookUpRate(1.f),
	MouseAimingTurnRate(0.2f),
	MouseAimingLookUpRate(0.2f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* CAMERA BOOM ADDING AND SETTINGS	*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);  //"GameFramework/SpringArmComponent.h"
	CameraBoom->TargetArmLength = 180.f;	//Yay kolu uzunlu�u (karakterin arkas�nda mesafe )
	CameraBoom->bUsePawnControlRotation = true;	//Controller ne zaman d�nerse CameraBoom da ona g�re d�ns�n
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);	//Cameran�n 50 birim yukarida ve 70 sagda olmasini sagliyoruz

	/* FOLLOW CAMERA ADDING AND SETTINGS	*/

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);	//Kamera, cameraboomun �n�nde varsay�lan sokete entegre ediliyor 
	FollowCamera->bUsePawnControlRotation = false; //Kamera yayl� kola g�re d�nmesin (Kamera sadece camerabooma g�re hareket etsin.
	//CTRL + SHIFT + SPACE 

	/*CONTROLLER DONERKEN MESH DONMESIN SADECE KAMERA DONSUN*/
	bUseControllerRotationPitch = false;	//Mesh i�in kontrollerin dikey (evet hay�r) d�n���n� �nemseme
	bUseControllerRotationYaw = true;	//Mesh i�in kontrollerin yatay(d�ner koltuk) d�n���n� onemseme	++artik onemseS
	bUseControllerRotationRoll = false;	//Mesh i�in kontrollerin yatay (yana d����) d�n���n� �nemseme

	/*CHARACTER MOVEMENT SETTINGS*/
	GetCharacterMovement()->bOrientRotationToMovement = false; //Karakter girdi y�n�ne g�re hareket edecek. Hangi y�ne bas�l�rsa o tarafa do�ru d�nm�� olacak. ++artik etmeyecek
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);	//Yanal (yaw= d�ner koltuk) d�nme h�z�
	GetCharacterMovement()->JumpZVelocity = 600.f; //Karakter z�plama h�z�
	GetCharacterMovement()->AirControl = 0.2f;	//Karakter havadayken %20 kontrol edilebilir.

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OutputLog();

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;	//Kameranin varsayilan view ini getir ve defaultfov a ekle
		CameraCurrentFOV = CameraDefaultFOV;
		CameraCurrentFOV = CameraDefaultFOV;
	}
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value!=0.f) //Controller ge�erli ise (posses ise) ve tu�a bas�l� ise
	{
		//�nce y�n�m�z� belirliyoruz
		const FRotator Rotation{ GetControlRotation() };	//Mevcut y�n�m�z
		const FRotator YawRotation{ 0.0f,Rotation.Yaw,0.0f };	// Sadece yanal d�n��leri almak istiyoruz

		//A�a��daki i�lem yaw rotation a bakarak x ekseni do�rultusunda bir vekt�r olu�turuyor
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		//�imdi hareketi veriyoruz
		AddMovementInput(Direction, Value);


	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f) //Controller ge�erli ise (posses ise) ve tu�a bas�l� ise
	{
		//�nce y�n�m�z� belirliyoruz
		const FRotator Rotation{ GetControlRotation() };	//Mevcut y�n�m�z
		const FRotator YawRotation{ 0.0f,Rotation.Yaw,0.0f };	// Sadece yanal d�n��leri almak istiyoruz

		//A�a��daki i�lem yaw rotation a bakarak y ekseni do�rultusunda bir vekt�r olu�turuyor
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		//�imdi hareketi veriyoruz
		AddMovementInput(Direction, Value);


	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	// derece / saniye * saniye / frame * rate  saniyeler g�t�r�r sonu� :  Oran* derece / frame
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value)
{
	float TurnScaleFactor{};
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value)
{
	float LookUpScaleFactor{};
	if (bAiming)
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AShooterCharacter::AimingButtonPressed()
{
	bAiming = true;
}

void AShooterCharacter::AimingButtonReleased()
{
	bAiming = false; 
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraInterpZoom(DeltaTime);
	SetLookRates(DeltaTime);
	//Crosshair hizini hesapliyoruz
	CalculateCrosshairSpread(DeltaTime);
}
// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);	//Bu makro playerinputcomponent ge�erli de�ilse a�a�� kodlar� �a��rmaz
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);	
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AShooterCharacter::FireWeapon);
	PlayerInputComponent->BindAction("AimingButton", EInputEvent::IE_Pressed, this, &AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", EInputEvent::IE_Released, this, &AShooterCharacter::AimingButtonReleased);
}

void AShooterCharacter::OutputLog()
{
	//UE_LOG FORMAT STRING 
	// 
	//Normal text
	UE_LOG(LogTemp, Warning, TEXT("Normal text"))

		int MyInt{ 32 }; //s�sl� parantezde yapmam�z�n sebebi hatay� �nlemektir. �rne�in int tan�mlay�p int MyInt {33.4} yaparsak hata verecektir.
	UE_LOG(LogTemp, Warning, TEXT("MyInt value: %d"), MyInt)

		float MyFloat{ 3.14159f }; //f demezsek double alg�lar ve bellekte fazla yer kaplar
	UE_LOG(LogTemp, Warning, TEXT("MyFloat value: %f"), MyFloat)

		double MyDouble{ 0.0000043 };
	UE_LOG(LogTemp, Warning, TEXT("MyDouble value: %lf"), MyDouble) // %lf double i�in kullan�l�yor

		char MyChar{ 'a' };
	UE_LOG(LogTemp, Warning, TEXT("MyChar value: %c"), MyChar)	// %c char i�in kullan�l�yor

		wchar_t wide_ch { L'a' };
	UE_LOG(LogTemp, Warning, TEXT("MyWideChar value: %lc"), wide_ch) // %lc widechar (wchar) i�in kullan�l�yor.

		bool MyBool{ true };
	UE_LOG(LogTemp, Warning, TEXT("MyBool value: %d"), MyBool)

		UE_LOG(LogTemp, Warning, TEXT("%d %f %d"), MyInt, MyFloat, MyBool)

		FString MyString {
		TEXT("This is FString baby!..")
	};

	UE_LOG(LogTemp, Warning, TEXT("FString value: %s"), *MyString); // %s ile g�sterilir. FStringte a��r� y�kleme oldu�u i�in ba�a * i�areti koyuyoruz.
	UE_LOG(LogTemp, Warning, TEXT("GetName Islemi: %s"), *GetName());
}

void AShooterCharacter::FireWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"))

	if (FireSound)
	{
		//Gameplay statics i�in: #include "Kismet/GameplayStatics.h"
		UGameplayStatics::PlaySound2D(this,FireSound);
		//FireSound i�in de include etmemiz gerekiyor #include "Sound/SoundCue.h"
	}
	if (MuzzleFlash)
	{
		const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh()); //#include "Engine/SkeletalMeshSocket.h" ekle
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);		//Par�ac�k sistemi yumurtla

			FVector BeamEnd;
			bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);
			if (bBeamEnd)
			{
				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						ImpactParticles,
						BeamEnd
					);
				}
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					BeamParticles,
					SocketTransform
				);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}

			//UGameplayStatics::GetPlayerController(this, 0)	bu zaten playercontroller donduruyor 0. oyuncu ve bu dunya dedik
			

			/*LINE TRACE BY SINGLE CHANNEL*/
			/*
			FHitResult FireHit;
			const FVector Start{ SocketTransform.GetLocation() };	//Socketin konumunu al�yoruz
			const FQuat Rotation{ SocketTransform.GetRotation() };	//Normalde Rotation olarak tan�mlamam�z� beklesek de socketlere �zg� bir �ekilde FQuat tan�mlan�yor
			// FQuat Quaternion denen matematiksel bir kavramd�r ve rotasyondan haric bilgiler de tutar. Bir vektorun ayrica dogrultu ve donus bilgileri >>AYRICA<< bakilacak.
			const FVector RotationAxis{ Rotation.GetAxisX() };		//Rotation'�n vekt�rlere b�ler ve x,y ve z dogrultularini cikarir.
			//yukaridaki islemler aslinda soketin x dogrultusunu ogrenmek icin
			const FVector End{ Start + RotationAxis * 50'000.f };	//Bu baslangic noktasindan rotationaxis*50k dogrultusunda bir vector cizecek
			//Matematiksel olarak Vector A + Vector B bileske vektoru cikariyor. (Ucuca ekleme yontemi. Bakiniz: Fizik Vektorler... 
			//Onun icin RotationAxis*50k ve Start vectorunu topladik
			//yukaridaki kesme isareti yok sayilir sadece daha okunabilir sayilar icin kullanilmaktadir.

			FVector BeamEndPoint{ End };	//Hicbir seye carpmazsa varsayilan olarak bitis noktasinda olacak duman efekti


			//HitResult const olmad��� i�in de�i�tirilebilir
			GetWorld()->LineTraceSingleByChannel(FireHit,Start,End,ECollisionChannel::ECC_Visibility);


			if (FireHit.bBlockingHit)	//bir seye �aprti mi?
			{
				//#include "DrawDebugHelpers.h"
				//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);						//Debug Line cizimi
				//DrawDebugPoint(GetWorld(), FireHit.Location, 5.0f, FColor::Green, false, 3.0f);			//Debug kuresi cizimi

				BeamEndPoint = FireHit.Location;	//Carparsa bitis noktasi carpisma noktasi olacak.

				//Impact Particle MERMI TEMAS EFEKTI
				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.Location, FRotator(0.f), true);
				}
			}

			if (BeamParticles)
			{
				//Gecici bir efekt pointer olusturduk ve bunu spawn ettigimiz particle a iliskilendirmis olduk.
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);

				if (Beam)	//parcacik gecerli ise
				{		//#include "Particles/ParticleSystemComponent.h"
					Beam->SetVectorParameter(FName("Target"), BeamEndPoint);	//Bu parcacik sistemi parametre aliyor. Target parametresi bir vector deger aldigi icin vektor noktasi verdik.
				}
			}
			*/
		}
	}

	//Montaj oynatmak i�in AnimIntance e ihtiyac�m�z var. Bunu meshten �ekebiliriz.

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);	//Burada montaj� �a��r�yoruz.
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

}

bool AShooterCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	//Ekranin ortasindan linetrace olusturma
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)	//GEngine ve GameViewport gecerli ise
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);	//Ekran boyutunu ViewportSize vektorune atiyoruz.
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);		//Ekran boyutunun ortasini aliyoruz
	CrosshairLocation.Y -= 50.f;			//Burada ekran ortasinin 50 birim ustunu hedef almistik


	//CROSSHAIRIN DUNYA LOKASYONUNU ALIYORUZ
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f };

		//Duman efekti son noktasi
		
		OutBeamLocation = End;

		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		if (ScreenTraceHit.bBlockingHit)
		{
			OutBeamLocation = ScreenTraceHit.Location;		//Eger carparsa duman bitis noktasi carpisma noktasi olarak ayarlansin


		}
		//Burada ekran ortasindan bir trace cizdigimiz icin silah namlusu ve crosshair arasinda bir nesne varsa o nesneye carpsa bile duman efekti devam ediyor
		//bunu bir trace daha cizerek halledebiliriz.
		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart{ MuzzleSocketLocation };
		const FVector WeaponTraceEnd{ OutBeamLocation };
		GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);
		if (WeaponTraceHit.bBlockingHit)	//Eger silah namlusu ve crosshair arasinda bir carpisma varsa
		{
			OutBeamLocation = WeaponTraceHit.Location;
		}

		return true;
	}

	return false;
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{	//Kamera'ya enterpolasyon uyguluyoruz YAKINLASIR
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZoomedFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{	//Kamera'ya enterpolasyon uyguluyoruz UZAKLASIR
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);	//kamera mesafesine uyarliyoruz
}

void AShooterCharacter::SetLookRates(float DeltaTime)
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.f,600.f };	//Karakter hizimizi temsil ediyor
	FVector2D VelocityMultiplierRange{ 0.f,1.f };	//Karakterin hizini ayarlamak istedigimiz deger
	FVector Velocity{ GetVelocity() };	//Karakterin hizi
	Velocity.Z = 0.f;		//Karakterin sadece yanal hizini onemsiyoruz. Z degeri bizim icin onemli degil
	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
	//velocity.size degerini walkspeedrange'e yaklasik olarak gelecek degeri, velocitymultiplierrange degerlerine donusturecek
	//yani 0 ve 600 arasindaki gelen degeri, 0 ve 1 arasina cekecek.
	
	//Karakter havada mi?
	if (GetCharacterMovement()->IsFalling())
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
		//Enterpolasyon uyguladik. CrosshairInAirFactor mevcut, 2.25 hedef deltatime suresi ve 2.25 hizi ile
	}
	else
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime,30.f);
		//yine enterpolasyon uyguladik. crosshairinairfactor mevcut, 0 hedef, deltatime suresi ve 30 birim hizinda
	}
	CrosshairSpreadMultiplier = 0.5 + CrosshairVelocityFactor+CrosshairInAirFactor;
}

float AShooterCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}


