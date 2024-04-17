#include "AIBaseController.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAIBaseController::BeginPlay()
{
	Super::BeginPlay();
	ABaseCharacter* AICharacter = Cast<ABaseCharacter>(GetPawn());
	if(AICharacter)
	{
		Agent = AICharacter;
		TeamId = FGenericTeamId(Agent->ID);
	}
}

ETeamAttitude::Type AAIBaseController::GetTeamAttitudeTowards(const AActor& Other) const
{
	//check if pawn
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if(OtherPawn==nullptr)//아니면 중립으로 반환
		return ETeamAttitude::Neutral;

	//check if actor implements GenericTeamAgentInterface // 인터페이스가 없으면 중립으로 반환
	const auto PlayerTI = Cast<IGenericTeamAgentInterface>(&Other);
	const IGenericTeamAgentInterface* BotTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if(PlayerTI==nullptr&&BotTI==nullptr)
		return ETeamAttitude::Neutral;

	//actor 아이디 확인
	FGenericTeamId OtherActorTeamID = NULL;
	if(BotTI!=nullptr)
		OtherActorTeamID = BotTI->GetGenericTeamId();
	if(PlayerTI!=nullptr)
		OtherActorTeamID = PlayerTI->GetGenericTeamId();

	//check if hostile
	const FGenericTeamId ThisId = GetGenericTeamId();
	if(OtherActorTeamID== 8)
		return ETeamAttitude::Neutral;
	else if(OtherActorTeamID == ThisId)
		return ETeamAttitude::Friendly;
	else
	{
		return ETeamAttitude::Hostile;
	}
}

void AAIBaseController::OnPerception(AActor* actor, FAIStimulus Stimulus)
{
	ABaseCharacter* Unknown = Cast<ABaseCharacter>(actor);
	if(Unknown==nullptr)
		return;

	TArray<AActor*> Threats;
	PerceptionComponent->GetHostileActors(Threats);//배열에 인식된 적들을 넣기
	if(Threats.Num()<=0)//반환 위협 없는 경우
		return;
	const int32 i = Threats.Find(actor);
	if(i<0)//일치하는 위협이 아닐 경우
		return;

	UE_LOG(LogTemp,Log,TEXT("Find Enemy"));
	GetBlackboardComponent()->SetValueAsBool(FName("IsStop"),true);
	SetFocus(Stimulus.WasSuccessfullySensed() ? Unknown:nullptr);
}

void AAIBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
