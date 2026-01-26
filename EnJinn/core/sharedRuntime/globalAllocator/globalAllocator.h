#pragma once

#include <enjinnAllocator/freeListAllocator.h>

namespace enjinn
{
namespace memory
{
	void setGlobalAllocatorToStandard();
	
	void setGlobalAllocator(enjinn::memory::FreeListAllocator *allocator);

	void dissableAllocators();

	void pushCustomAllocatorsToStandard();

	void popCustomAllocatorsToStandard();

}
}
