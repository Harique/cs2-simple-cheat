#include <Windows.h>
#include <thread>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include "vector.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "esp.cpp"
#include "math.h"
using namespace std;


static void HackThread(HMODULE instance) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	printf("Cheat injected successfully!\n\n");
	
	uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");

	

	while (!GetAsyncKeyState(VK_RETURN))
	{
		
		 float localPlayerCameraY;
		 float localYaw;
		 float localPitch;
		 
		 
		for (int i = 0; i < 11; i++) {
			//sometimes when adding bots, first would be added 8 numbers away from 0 and THEN be incremented by 10 for each
			//no idea why, need investigation
			uintptr_t Entity = *(uintptr_t*)(base + 0x1BFF128 + (0x10 * i));
			if (!Entity) continue;
			
			int health = *(int*)(Entity + 0x34C);
			if (health <= 0 || health > 100) continue;

			float x = *(float*)(Entity + 0xF58);
			float z = *(float*)(Entity + 0xF5C);
			float y = *(float*)(Entity + 0xF60); // Z is used for up and down not Y so i renamed the variable to match normal XYZ, also this is the Z for the legs not the camera which is needed for the viewmatrix
			if (i == 0) {
				localPlayerCameraY = *(float*)(base + 0x1D16A08); // Z value for camera, possibly needed for headshot aimbot later, but needed for ESP.
				float* pitchPtr = (float*)(base + 0x1E1DC48);
				localYaw = *(pitchPtr + 1);
				localPitch = *pitchPtr;
				
				continue;
			} 
			

		}


		Sleep(16);
	}

	if (f)
	{
		fclose(f);
	}

	FreeConsole();
	FreeLibraryAndExitThread(instance, 0);
}

// Entry:

extern "C" BOOL WINAPI DllMain(HMODULE const instance, DWORD const reason, LPVOID lpvReserved) {

	// DLL Process attach
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);


		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(HackThread),
			instance,
			0,
			nullptr
		);

		if (thread) {
			CloseHandle(thread);
		}
	}

	return TRUE;
}
