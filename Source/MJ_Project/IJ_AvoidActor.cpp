// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_AvoidActor.h"

// Sets default values
AIJ_AvoidActor::AIJ_AvoidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIJ_AvoidActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIJ_AvoidActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;
	
	if (currentTime >= destoryTime)
	{
		Destroy();
	}
}

