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
#include "math.cpp"
using namespace std;


static void HackThread(HMODULE instance) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	printf("Cheat injected successfully!\n\n");
	
	uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");
	bool toggle = false;
	

	while (!GetAsyncKeyState(VK_RETURN))
	{
		if (GetAsyncKeyState('J') & 1) {
			toggle = !toggle;
			std::cout << "Feature is now " << (toggle ? "ON" : "OFF") << "\n";
		}

		if (toggle == false)   // <-- FIXED: comparison, not assignment
			continue;
		Vector3 local_player;
		Vector3 enemy;
		ViewAngles angles;
		float localPlayerCameraY;
		float* localYaw = reinterpret_cast<float*>(base + 0x1E3DC24);
		float* localPitch = reinterpret_cast<float*>(base + 0x1E3DC20);
		 
		 
		for (int i = 0; i < 11; i++) {
			//sometimes when adding bots, first would be added 8 numbers away from 0 and THEN be incremented by 10 for each
			//no idea why, need investigation
			uintptr_t Entity = *(uintptr_t*)(base + 0x1BFF128 + (0x10 * i));
			if (!Entity) continue;
			
			int health = *(int*)(Entity + 0x34C);
			if (health <= 0 || health > 100) continue;

			float x = *(float*)(Entity + 0xF58);
			float z = *(float*)(Entity + 0xF5C);
			float y = *(float*)(Entity + 0xF60);// up
			if (i == 0) {
				localPlayerCameraY = *(float*)(base + 0x1D16A08);
				local_player = { x,z,localPlayerCameraY };
				continue;
			}
			else {
				enemy = { x,z,y };
			}
			Vector3 direction = enemy - local_player;
			angles.yaw = to_degrees(std::atan2(direction.y, direction.x));
			angles.pitch = to_degrees(std::atan2(-direction.z - 63.f, std::hypot(direction.x, direction.y))); // remove hard coded subtration when you get enemy camera Z
			*localYaw = angles.yaw;
			*localPitch = angles.pitch;

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
