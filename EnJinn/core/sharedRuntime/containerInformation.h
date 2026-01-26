#pragma once
#include <string>
#include <baseContainer.h>

namespace enjinn
{

struct ContainerInformation
{
	ContainerInformation() {};
	ContainerInformation(
		size_t containerStructBaseSize,
		const char *containerName,
		const ContainerStaticInfo& containerStaticInfo):
		containerStructBaseSize(containerStructBaseSize), containerName(containerName),
		containerStaticInfo(containerStaticInfo)
	{
		useDefaultAllocator = containerStaticInfo.useDefaultAllocator;
	};

	bool operator==(const ContainerInformation &other)
	{
		if (this == &other) { return true; }

		return
			this->containerStructBaseSize == other.containerStructBaseSize &&
			this->containerName == other.containerName &&
			this->containerStaticInfo == other.containerStaticInfo &&
			this->useDefaultAllocator == other.useDefaultAllocator;

	}

	bool operator!=(const ContainerInformation &other)
	{ 
		return !(*this == other);
	}

	size_t containerStructBaseSize = 0; //static memory
	std::string containerName = "";
	ContainerStaticInfo containerStaticInfo = {};
	bool useDefaultAllocator = 0; //move into container static info when implementing

	size_t calculateMemoryRequirements()
	{
		if (useDefaultAllocator) { return 0; }

		size_t size = 0;

		size += containerStructBaseSize;
		enjinn::align64(size);

		size += containerStaticInfo.defaultHeapMemorySize;

		for (auto i : containerStaticInfo.bonusAllocators)
		{
			enjinn::align64(size);
			size += i;
		}

		return size;
	}
};

}