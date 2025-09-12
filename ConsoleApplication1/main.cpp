#include <Windows.h>
#include <thread>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "math.h"
#include "esp.h"
#pragma once

static void HackThread(HMODULE instance) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	printf("Cheat injected successfully!\n\n");
	if (!glfwInit()) return;
	HWND hwnd = FindWindowA(NULL, "Counter-Strike 2");
	RECT rect;
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 uColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
		"}\0";
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); //controls transparency
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // no decoractions
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE); // always on top

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Overlay", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	//makes it click-through
	HWND hwndOverlay = glfwGetWin32Window(window);
	LONG exStyle = GetWindowLong(hwndOverlay, GWL_EXSTYLE);
	SetWindowLong(hwndOverlay, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}
	glClearColor(0.f, 0.f, 0.f, 0.f); // RGBA -> using all 0s will make it fully transparent
	//we need to use blending here so when i draw the ESP it will be draw in a non transparent state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");

	//making a VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//making VBO
	unsigned int VBO;
	unsigned int vertexShader;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, nullptr, GL_DYNAMIC_DRAW);

	//EBO for indexing the order of which verteces are drawn.
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, nullptr, GL_DYNAMIC_DRAW);
	
	//creating and assigning a vertex shader object here
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // (shaderobject,how many strings were passing as a source...usually one, source code for shader, no idea)
	glCompileShader(vertexShader); 
	//checking if successful.
	int  success; 
	char infoLog[512]; //stroage for error message
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//checks if the shader process worked, will fill the success int if yes, if not then failed
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); //(shader object that we did the check for, the buffer size, since we put 512 in infoLog than we use that, size but idk why we keep it null, the storage for the error to fill)
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	
	}
	//exact same process as vertext shader, but we use GL_FRAGMENT_SHADER instead
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//checking if successful.
	int  success2;
	char infoLog2[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);//checks if the shader process worked, will fill the success int if yes, if not then failed
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2); //(shader object that we did the check for, the buffer size, since we put 512 in infoLog than we use that, size but idk why we keep it null, the storage for the error to fill)
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;

	}
	//creating shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//combining and linking the shaders we created
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//checking if successful.
	int  success3;
	char infoLog3[512];
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);
	if (!success3) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog3 << std::endl;
	}
	//deleting shaders as we dont need them anymore, every render call after this will use the new program thus using the shaders we linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//telling OpenGL how to read the vertex data, in here since we only pass XYZ as data, we only need to multiply 3 * sizeof(float), if we add RGB later, than it would become 6
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window) && !GetAsyncKeyState(VK_RETURN))
	{
		
		
		//get the cs2 window x,y and height and width to make overlay follow it

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // width/height of CS2's rendering area

		POINT topLeft = { clientRect.left, clientRect.top };
		ClientToScreen(hwnd, &topLeft);     // convert client coords to screen coords

		POINT bottomRight = { clientRect.right, clientRect.bottom };
		ClientToScreen(hwnd, &bottomRight);
		int cs2X = topLeft.x;
		int cs2Y = topLeft.y;
		int cs2Width = bottomRight.x - topLeft.x;
		int cs2Height = bottomRight.y - topLeft.y;
		float aspect = (float)cs2Width / (float)cs2Height;
		float fov = 90.f; //hard coded cause its the default and cant be changed in cs2 legitamitely 

		//  whatever background color i chose in glClearColor
		glClear(GL_COLOR_BUFFER_BIT);

		// Move/resize overlay, overlay seems to be couple pixels bigger for some reason.
		SetWindowPos(
			hwndOverlay, HWND_TOPMOST,
			cs2X, cs2Y,
			cs2Width, cs2Height,
			SWP_NOACTIVATE | SWP_SHOWWINDOW
		);
		 float localPlayerCameraY;
		 float localYaw;
		 float localPitch;
		 Vector3 eye;

		 Matrix4x4 viewProj((float*)(base + 0x1E330F0));  // ptr to view matrix
		 Matrix4x4 DXtoGL;
		 DXtoGL.m[2][2] = 2.0f;     // scale Z
		 DXtoGL.m[2][3] = -1.0f;    // shift Z

		 Matrix4x4 corrected = viewProj * DXtoGL;

		for (int i = 0; i < 11; i++) {
			//sometimes when adding bots, first would be added 8 numbers away from 0 and THEN be incremented by 10 for each
			//no idea why, need investigation
			uintptr_t Entity = *(uintptr_t*)(base + 0x1BFF128 + (0x10 * i));
			if (!Entity) continue;
			
			int health = *(int*)(Entity + 0x34C);
			if (health <= 0 || health > 100) continue;

			float x = *(float*)(Entity + 0xF58);
			float y = *(float*)(Entity + 0xF5C);
			float z = *(float*)(Entity + 0xF60); // Z is used for up and down not Y so i renamed the variable 
			// to match normal XYZ, also this is the Z for the legs not the camera which is needed for the viewmatrix
			
			Vector4 feet(x,y,z,1.0f);
			Vector4 head = feet + Vector4(0,0,70,0.0f);

			if (i == 0) {
				localPlayerCameraY = *(float*)(base + 0x1D16A08); // Z value for camera, possibly needed for headshot aimbot later, but needed for ESP.
				float* pitchPtr = (float*)(base + 0x1E1DC48);
				localYaw = *(pitchPtr + 1);
				localPitch = *pitchPtr;
				eye = { x,y,z };
				continue;
			} 
			Vector2 screenFeet, screenHead;

			if (WorldToScreen(feet, viewProj, screenFeet) && WorldToScreen(head, viewProj, screenHead)) {


				float height = abs(screenFeet.y - screenHead.y);
				float width = height * 0.5f;

				float left = screenHead.x - width / 2;
				float right = screenHead.x + width / 2;
				float top = screenHead.y;
				float bottom = screenFeet.y;

				//std::cout << "head vec2: " << screenHead << std::endl;
				//std::cout << "feet vec2: " << screenFeet << std::endl;
				//std::cout << "height: " << height << std::endl;
				//std::cout << "width: " << width << std::endl;
				//std::cout << "left: " << left << std::endl;
				//std::cout << "right: " << right << std::endl;
				//std::cout << "top: " << top << std::endl;
				//std::cout << "bottom: " << bottom << std::endl;
				//std::cout << "------------------------------" << std::endl;

				Vector3 corners[4] = {};
				corners[0] = Vector3(left, bottom, 0.0f);
				corners[1] = Vector3(left, top, 0.0f);
				corners[2] = Vector3(right, bottom, 0.0f);
				corners[3] = Vector3(right, top, 0.0f);

				DrawBox(corners, 70.f,70.f,1.f,1.f, shaderProgram,VAO,VBO,EBO);
			}

		}
		
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(16);
	}

	if (f)
	{
		fclose(f);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
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
