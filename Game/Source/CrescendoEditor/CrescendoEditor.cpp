#pragma once

#include "CrescendoEditor.h"

IMPLEMENT_GAME_MODULE(FCrescendoEditorModule, CrescendoEditor);
DEFINE_LOG_CATEGORY(LogCrsEditor)

#define LOCTEXT_NAMESPACE "CrescendoEditor"

void FCrescendoEditorModule::StartupModule()
{
	UE_LOG(LogCrsEditor, Warning, TEXT("CrescendoEditor: Log Started"));
}

void FCrescendoEditorModule::ShutdownModule()
{
	UE_LOG(LogCrsEditor, Warning, TEXT("CrescendoEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE
