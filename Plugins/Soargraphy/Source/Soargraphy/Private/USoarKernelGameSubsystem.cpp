#include "USoarKernelGameSubsystem.h"
#include "Soargraphy.h"

DEFINE_LOG_CATEGORY(LogSoargraphy);

void UUSoarKernelGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogSoargraphy, Log, TEXT("Initializing Soar Kernel Game Subsystem"));

    // Initialize the Soar kernel
    SoarKernel.reset(sml::Kernel::CreateKernelInNewThread());
}

void UUSoarKernelGameSubsystem::Deinitialize()
{
    UE_LOG(LogSoargraphy, Log, TEXT("Deinitializing Soar Kernel Game Subsystem"));

    // Clean up the Soar kernel
    SoarKernel->Shutdown();
    SoarKernel.reset();

    Super::Deinitialize();
}