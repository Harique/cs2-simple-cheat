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
			"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
		float cs2Width = bottomRight.x - topLeft.x;
		float cs2Height = bottomRight.y - topLeft.y;
		float aspect = cs2Width / cs2Height;
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
		 
		for (int i = 0; i < 11; i++) {
			//sometimes when adding bots, first would be added 8 numbers away from 0 and THEN be incremented by 10 for each
			//no idea why, need investigation
			uintptr_t Entity = *(uintptr_t*)(base + 0x1BFF128 + (0x10 * i));
			if (!Entity) continue;
			
			int health = *(int*)(Entity + 0x34C);
			if (health <= 0 || health > 100) continue;

			float x = *(float*)(Entity + 0xF58);
			float z = *(float*)(Entity + 0xF5C);
			float legsY = *(float*)(Entity + 0xF60); // Z is used for up and down not Y so i renamed the variable to match normal XYZ, also this is the Z for the legs not the camera which is needed for the viewmatrix
			if (i == 0) {
				localPlayerCameraY = *(float*)(base + 0x1D16A08); // Z value for camera, possibly needed for headshot aimbot later, but needed for ESP.
				float* pitchPtr = (float*)(base + 0x1E1DC48);
				localYaw = *(pitchPtr + 1);
				localPitch = *pitchPtr;
				eye = { x,legsY,z };
				continue;
			} 
			Vector2 PixelCords;
			Matrix4x4 view = viewMatrix(eye, localYaw, localPitch);
			Matrix4x4 projection = getProjectionMatrix(fov, aspect);

			Matrix4x4 viewProjection = projection * view;
			Vector3 pos = { x,legsY ,z };
			Vector4 worldPos = {x, legsY, z, 1.0f};
			Vector4 entityPos = viewProjection * worldPos;

			if (WorldToScreen(pos, viewProjection, cs2Width, cs2Height, PixelCords)) {
				DrawBox(PixelCords.x, PixelCords.y, 70.f,70.f,1.f,0.f,0.f,1.f, shaderProgram,VAO,VBO,EBO, cs2Width,cs2Height);
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
