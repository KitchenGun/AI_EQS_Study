// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIBaseController.generated.h"

/**
 * 
 */
UCLASS()
class AI_EQS_STUDY_API AAIBaseController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override {return TeamId;}
	
public:
	UPROPERTY(BlueprintReadWrite)
	class ABaseCharacter* Agent;
	
	UFUNCTION()
	void OnPerception(AActor* actor, FAIStimulus Stimulus);
	
	virtual void OnPossess(APawn* InPawn) override;
	
	class UAISenseConfig_Sight* Sight;
	
	
protected:
	FGenericTeamId TeamId;
};
