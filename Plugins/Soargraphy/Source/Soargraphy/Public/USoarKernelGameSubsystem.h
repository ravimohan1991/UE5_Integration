/**
 * @file USoarKernelGameSubsystem.h
 * @author Ravi Mohan (SunovatechProductions)
 * @brief This file contains native stuff. The file was generated from UE by Epic Games and modified by author.
 * @version 1.0
 * @date 15 June, 2026
 * @copyright Copyright Epic Games, Inc. All Rights Reserved. Ravi Mohan accepted UE(5) license.
 */
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "sml_ClientKernel.h"

#include "USoarKernelGameSubsystem.generated.h"

/**
 * @brief A game instance subsystem for managing the Soar kernel.
 */
UCLASS()
class SOARGRAPHY_API UUSoarKernelGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/**
	 * @brief Initializes the subsystem.
	 * @param Collection The collection of subsystems.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Deinitializes the subsystem.
	 */
	virtual void Deinitialize() override;

	sml::Kernel* GetSoarKernel() const { return SoarKernel.get(); }

private:
	/** The Soar kernel instance. */
	std::unique_ptr<sml::Kernel> SoarKernel;

};
