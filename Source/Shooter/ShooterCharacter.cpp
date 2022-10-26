// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* CAMERA BOOM ADDING AND SETTINGS	*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);  //"GameFramework/SpringArmComponent.h"
	CameraBoom->TargetArmLength = 300.f;	//Yay kolu uzunluðu (karakterin arkasýnda mesafe )
	CameraBoom->bUsePawnControlRotation = true;	//Controller ne zaman dönerse CameraBoom da ona göre dönsün


	/* FOLLOW CAMERA ADDING AND SETTINGS	*/

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);	//Kamera, cameraboomun önünde varsayýlan sokete entegre ediliyor 
	FollowCamera->bUsePawnControlRotation = false; //Kamera yaylý kola göre dönmesin (Kamera sadece camerabooma göre hareket etsin.
	//CTRL + SHIFT + SPACE 

	/*CONTROLLER DONERKEN MESH DONMESIN SADECE KAMERA DONSUN*/
	bUseControllerRotationPitch = false;	//Mesh için kontrollerin dikey (evet hayýr) dönüþünü önemseme
	bUseControllerRotationYaw = false;	//Mesh için kontrollerin yatay(döner koltuk) dönüþünü önemseme
	bUseControllerRotationRoll = false;	//Mesh için kontrollerin yatay (yana düþüþ) dönüþünü önemseme

	/*CHARACTER MOVEMENT SETTINGS*/
	GetCharacterMovement()->bOrientRotationToMovement = true; //Karakter girdi yönüne göre hareket edecek. Hangi yöne basýlýrsa o tarafa doðru dönmüþ olacak.
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);	//Yanal (yaw= döner koltuk) dönme hýzý
	GetCharacterMovement()->JumpZVelocity = 600.f; //Karakter zýplama hýzý
	GetCharacterMovement()->AirControl = 0.2f;	//Karakter havadayken %20 kontrol edilebilir.

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OutputLog();
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value!=0.f) //Controller geçerli ise (posses ise) ve tuþa basýlý ise
	{
		//Önce yönümüzü belirliyoruz
		const FRotator Rotation{ GetControlRotation() };	//Mevcut yönümüz
		const FRotator YawRotation{ 0.0f,Rotation.Yaw,0.0f };	// Sadece yanal dönüþleri almak istiyoruz

		//Aþaðýdaki iþlem yaw rotation a bakarak x ekseni doðrultusunda bir vektör oluþturuyor
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		//Þimdi hareketi veriyoruz
		AddMovementInput(Direction, Value);


	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f) //Controller geçerli ise (posses ise) ve tuþa basýlý ise
	{
		//Önce yönümüzü belirliyoruz
		const FRotator Rotation{ GetControlRotation() };	//Mevcut yönümüz
		const FRotator YawRotation{ 0.0f,Rotation.Yaw,0.0f };	// Sadece yanal dönüþleri almak istiyoruz

		//Aþaðýdaki iþlem yaw rotation a bakarak y ekseni doðrultusunda bir vektör oluþturuyor
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		//Þimdi hareketi veriyoruz
		AddMovementInput(Direction, Value);


	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	// derece / saniye * saniye / frame * rate  saniyeler götürür sonuç :  Oran* derece / frame
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);	//Bu makro playerinputcomponent geçerli deðilse aþaðý kodlarý çaðýrmaz
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AShooterCharacter::FireWeapon);
}

void AShooterCharacter::OutputLog()
{
	//UE_LOG FORMAT STRING 
	// 
	//Normal text
	UE_LOG(LogTemp, Warning, TEXT("Normal text"))

		int MyInt{ 32 }; //süslü parantezde yapmamýzýn sebebi hatayý önlemektir. Örneðin int tanýmlayýp int MyInt {33.4} yaparsak hata verecektir.
	UE_LOG(LogTemp, Warning, TEXT("MyInt value: %d"), MyInt)

		float MyFloat{ 3.14159f }; //f demezsek double algýlar ve bellekte fazla yer kaplar
	UE_LOG(LogTemp, Warning, TEXT("MyFloat value: %f"), MyFloat)

		double MyDouble{ 0.0000043 };
	UE_LOG(LogTemp, Warning, TEXT("MyDouble value: %lf"), MyDouble) // %lf double için kullanýlýyor

		char MyChar{ 'a' };
	UE_LOG(LogTemp, Warning, TEXT("MyChar value: %c"), MyChar)	// %c char için kullanýlýyor

		wchar_t wide_ch { L'a' };
	UE_LOG(LogTemp, Warning, TEXT("MyWideChar value: %lc"), wide_ch) // %lc widechar (wchar) için kullanýlýyor.

		bool MyBool{ true };
	UE_LOG(LogTemp, Warning, TEXT("MyBool value: %d"), MyBool)

		UE_LOG(LogTemp, Warning, TEXT("%d %f %d"), MyInt, MyFloat, MyBool)

		FString MyString {
		TEXT("This is FString baby!..")
	};

	UE_LOG(LogTemp, Warning, TEXT("FString value: %s"), *MyString); // %s ile gösterilir. FStringte aþýrý yükleme olduðu için baþa * iþareti koyuyoruz.
	UE_LOG(LogTemp, Warning, TEXT("GetName Islemi: %s"), *GetName());
}

void AShooterCharacter::FireWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"))

	if (FireSound)
	{
		//Gameplay statics için: #include "Kismet/GameplayStatics.h"
		UGameplayStatics::PlaySound2D(this,FireSound);
		//FireSound için de include etmemiz gerekiyor #include "Sound/SoundCue.h"
	}
	if (MuzzleFlash)
	{
		const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh()); //#include "Engine/SkeletalMeshSocket.h" ekle
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);		//Parçacýk sistemi yumurtla
		}
	}
}
