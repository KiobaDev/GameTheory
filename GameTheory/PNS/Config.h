#pragma once

int maxResourcesCount = 1000;
int usedResourcesCount = 0;

bool resourcesAvailable()
{
	return usedResourcesCount++ < maxResourcesCount;
}

void resetResources()
{
	usedResourcesCount = 0;
}