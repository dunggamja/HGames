// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PhysicsAPI.h"

#include "EngineMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(Client, Log, All);

#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_S(Verbosity) UE_LOG(Client, Verbosity, TEXT("%s"), *LOG_CALLINFO);
#define LOG(Verbosity, Format, ...) UE_LOG(Client, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, \
        *FString::Printf(Format, ##__VA_ARGS__))