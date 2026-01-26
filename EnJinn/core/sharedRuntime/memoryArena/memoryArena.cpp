#include <memoryArena/memoryArena.h>
#include <malloc.h>

//todo remove
void enjinn::memory::MemoryArena::allocateStaticMemory(const ContainerInformation &containerInfo)
{
	containerStructMemory.size = containerInfo.containerStructBaseSize;
	containerStructMemory.block = malloc(containerInfo.containerStructBaseSize);
}

void enjinn::memory::MemoryArena::dealocateStaticMemory()
{
	containerStructMemory.size = 0;
	free(containerStructMemory.block);
	containerStructMemory.block = 0;
}

