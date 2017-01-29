#include "CrescendoEditor.h"
#include "CrsEdEngine.h"
#include "Crescendo.h"

UCrsEdEngine::UCrsEdEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogCrsEditor, Warning, TEXT("CrsEdEngine: Engine started successfully"));
}
