// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* CAMERA BOOM ADDING AND SETTINGS	*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);  //"GameFramework/SpringArmComponent.h"
	CameraBoom->TargetArmLength = 300.f;	//Yay kolu uzunlu�u (karakterin arkas�nda mesafe )
	CameraBoom->bUsePawnControlRotation = true;	//Controller ne zaman d�nerse CameraBoom da ona g�re d�ns�n


	/* FOLLOW CAMERA ADDING AND SETTINGS	*/

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);	//Kamera, cameraboomun �n�nde varsay�lan sokete entegre ediliyor 
	FollowCamera->bUsePawnControlRotation = false; //Kamera yayl� kola g�re d�nmesin (Kamera sadece camerabooma g�re hareket etsin.
	//CTRL + SHIFT + SPACE 
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OutputLog();
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

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);	//Bu makro playerinputcomponent ge�erli de�ilse a�a�� kodlar� �a��rmaz
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
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