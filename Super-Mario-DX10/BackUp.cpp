#include "BackUp.h"
#include <iostream>
#include <fstream>
#include "Utils.h"

BackUp* BackUp::__instance = NULL;

BackUp* BackUp::GetInstance()
{
	if (__instance == NULL) __instance = new BackUp();
	return __instance;
}
