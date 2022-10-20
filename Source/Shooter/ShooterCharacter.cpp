// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	OutputLog();
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

}

void AShooterCharacter::OutputLog()
{
	//UE_LOG FORMAT STRING 
	// 
	//Normal text
	UE_LOG(LogTemp, Warning, TEXT("Normal text"))

	int MyInt{ 32 }; //süslü parantezde yapmamýzýn sebebi hatayý önlemektir. Örneðin int tanýmlayýp int MyInt {33.4} yaparsak hata verecektir.
	UE_LOG(LogTemp, Warning, TEXT("MyInt value: %d"), MyInt)



}