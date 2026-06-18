/**
 * @file Soargraphy.h
 * @author Ravi Mohan (SunovatechProductions)
 * @brief This file contains native stuff. The file was generated from UE by Epic Games and modified by author.
 * @version 1.0
 * @date 15 June, 2026
 * @copyright Copyright Epic Games, Inc. All Rights Reserved. Ravi Mohan accepted UE(5) license.
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSoargraphyModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
