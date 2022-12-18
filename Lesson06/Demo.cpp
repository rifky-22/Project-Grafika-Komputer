#include "Demo.h"



Demo::Demo() {
}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (posCamY < 90) {
			posCamY += 0.1;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (posCamY > -10) {
			posCamY -= 0.1f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 20) {
		viewCamY = posCamY + 20;
	}
	if ((viewCamY - posCamY) < -20) {
		viewCamY = posCamY - 20;
	}
	RotateCamera(-angleY);
}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY+15, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attribute

	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 30, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 0, 0);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
		glUniform3f(lightPosLoc, -45, 10, 20);
		GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
		glUniform3f(viewPosLoc, -45, 10, 20);
		GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
		glUniform3f(lightColorLoc, 166.0f/255.0f, 161.0f / 255.0f, 121.0f / 255.0f);
	}
	Wall();
	Table();
	Chair();
	Bed();
	Cupboard();
	Door();
	Monitor();
	Lamp(1);
	Lamp(2);
	Watch();
	Keyboard();
	Drawer();
	Carpet();
	Window();
	Shelf();
	DrawColoredPlane();
	glDisable(GL_DEPTH_TEST);
}

void Demo::Monitor() {
	DrawColoredCube(0.5,    12,  15,    -42,   15,    -18,  0.0f,  0.0f,  0.0f);
	DrawColoredCube(0.5,    12,  15, -42.75,   15,    -18, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  2,   0.5,  16, -42.25,  9.5,    -18, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  2,   0.5,  16, -42.25,   21,    -18, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  2, 11.75, 0.5, -42.25,   15, -10.25, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  2, 11.75, 0.5, -42.25,   15, -25.75, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  3,     3,   2,    -44,   15,    -18, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  2,    10,   2,    -45, 11.5,    -18, 51.0f, 51.0f, 51.0f);
	DrawColoredCube(  5,     1,   5,    -43,    8,    -18, 51.0f, 51.0f, 51.0f);
}

void Demo::Door() {
	DrawColoredCube(1, 25, 17, 49, 12, 0, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(2, 25, 2.5, 48, 12, 8.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(2, 25, 2.5, 48, 12, -8.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(2, 2.5, 20, 48, 24, 0, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(2, 0.5, 0.5, 48, 12, -5.5, 169.0f, 169.0f, 169.0f);
	DrawColoredCube(0.5, 0.5, 2, 47.25, 12, -5.25, 169.0f, 169.0f, 169.0f);
}

void Demo::Watch() {
	DrawColoredCube(2, 1.2, 3, -40, 8.4, -29, 69.0f, 69.0f, 69.0f);
	DrawColoredCube(2, 1.2, 3, -40.2, 8.4, -29, 169.0f, 169.0f, 169.0f);
	DrawColoredCube(2.5, 0.4, 3.5, -40, 7.7, -29, 169.0f, 169.0f, 169.0f);
	DrawColoredCube(2.5, 0.4, 3.5, -40, 8.9, -29, 169.0f, 169.0f, 169.0f);
	DrawColoredCube(2.5, 2, 0.4, -40, 8, -30.6, 169.0f, 169.0f, 169.0f);
	DrawColoredCube(2.5, 2, 0.4, -40, 8, -27.4, 169.0f, 169.0f, 169.0f);
}

void Demo::Cupboard() {
	DrawColoredCube( 20,  25, 10,   0, 13, -44.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(9.5,  25, 10,   5, 13, -43.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(9.5,  25, 10,  -5, 13, -43.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(1.5,  25, 11, -10, 13, -43.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(1.5,  25, 11,  10, 13, -43.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube( 23, 1.5, 11,   0, 25, -43.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(3, 3, 3, 8, 0, -48.49, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(3, 3, 3, 8, 0, -41, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(3, 3, 3, -8, 0, -41, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(3, 3, 3, -8, 0, -48.49, 140.0f, 102.0f, 69.0f);
}

void Demo::Keyboard() {
	DrawColoredCube(4.25, 0.25, 10.5, -37, 7.5, -18.15, 255.0f, 255.0f, 255.0f);
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 5; j++) {
			DrawColoredCube(0.75, 0.25, 0.75, -38.5 + (0.8 * j), 7.75, -13.5-(0.85*i), 165.0f, 165.0f, 165.0f);
		}
	}
}

void Demo::Drawer() {
	DrawColoredCube(  6, 2.5,   6, -45, 4.25, 20, 168.0f, 168.0f, 167.0f);
	DrawColoredCube(0.5, 0.5, 0.5, -42, 4.25, 20, 125.0f, 105.0f, 61.0f);
	DrawColoredCube(  6, 2.5,   6, -45, 1.5, 20, 168.0f, 168.0f, 167.0f);
	DrawColoredCube(0.5, 0.5, 0.5, -42, 1.5, 20, 125.0f, 105.0f, 61.0f);
	DrawColoredCube(7, 5.75, 0.5, -45, 2.75, 23, 125.0f, 105.0f, 61.0f);
	DrawColoredCube(7, 5.75, 0.5, -45, 2.75, 17, 125.0f, 105.0f, 61.0f);
	DrawColoredCube(7, 0.5, 5.5, -45, 5.5, 20, 125.0f, 105.0f, 61.0f);
	DrawColoredCube(7, 0.5, 5.5, -45, 0.25, 20, 125.0f, 105.0f, 61.0f);
}

void Demo::Table() {
	DrawColoredCube(15, 1, 35, -40, 7, -18, 140.0f, 102.0f, 69.0f);
	DrawColoredCube( 1, 7,  1, -35, 3, -4, 140.0f, 102.0f, 69.0f);
	DrawColoredCube( 1, 7,  1, -45, 3, -4, 140.0f, 102.0f, 69.0f);
	DrawColoredCube( 1, 7,  1, -35, 3, -32, 140.0f, 102.0f, 69.0f);
	DrawColoredCube( 1, 7,  1, -45, 3, -32, 140.0f, 102.0f, 69.0f);
}

void Demo::Chair() {
	DrawColoredCube(7.75, 1, 7.75, -25, 5, -18, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(1, 15, 1, -21.5, 7, -14.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(1, 6, 1, -28.5, 2.5, -14.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(0.5, 7, 7, -21.5, 10, -18, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(1, 15, 1, -21.5, 7, -21.5, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(1, 6, 1, -28.5, 2.5, -21.5, 140.0f, 102.0f, 69.0f);
}

void Demo::Wall() {
	DrawColoredCube(  1, 50, 100, -49.5, 10,     0, 0.0f, 200.0f, 255.0f);
	DrawColoredCube(100, 50,   1,     0, 10, -49.5, 0.0f, 200.0f, 255.0f);
	DrawColoredCube(  1, 50, 100,  49.5, 10,     0, 0.0f, 200.0f, 255.0f);
	DrawColoredCube(100, 50,   1,     0, 10,  49.5, 0.0f, 200.0f, 255.0f);
	DrawColoredCube(100,  1, 100,     0, 35,     0, 122.0f, 122.0f, 122.0f);
}

void Demo::Bed() {
	DrawColoredCube( 3,   8,   20, -47,   1, 35, 125.0f, 105.0f,  61.0f);
	DrawColoredCube(34,   2,   21, -30,   1, 35, 125.0f, 105.0f,  61.0f);
	DrawColoredCube(31,   3,   18, -30, 2.5, 35,  89.0f,  87.0f,  84.0f);
	DrawColoredCube(26, 3.1, 18.5, -27, 2.5, 35, 168.0f, 168.0f, 167.0f);
}

void Demo::Window() {
	DrawColoredCube(  20,   15, 0.5,   0,   20,    49, 163.0f, 247.0f, 255.0f);
	DrawColoredCube(  20, 0.25, 0.5,   0,   20, 48.95,   0.0f,   0.0f,   0.0f);
	DrawColoredCube(0.25,   15, 0.5,   0,   20, 48.95,   0.0f,   0.0f,   0.0f);
	DrawColoredCube(   1,   15, 2.5,  10,   20,    49, 140.0f, 102.0f,  69.0f);
	DrawColoredCube(   1,   15, 2.5, -10,   20,    49, 140.0f, 102.0f,  69.0f);
	DrawColoredCube(  21,    1, 2.5,   0, 27.5,    49, 140.0f, 102.0f,  69.0f);
	DrawColoredCube(  21,    1, 2.5,   0, 12.5,    49, 140.0f, 102.0f,  69.0f);
}

void Demo::Shelf() {
	DrawColoredCube(21, 1, 5, 25, 29, 48, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(21, 1, 5, 25, 22, 48, 140.0f, 102.0f, 69.0f);

	for (int i = 0; i < 6; i++) {
		float color1;
		float color2;
		float color3;
		if (i == 0) {
			color1 = 0.0f;
			color2 = 4.0f;
			color3 = 255.0f;
		}
		else if (i == 1) {
			color1 = 255.0f;
			color2 = 111.0f;
			color3 = 0.0f;
		}
		else if (i == 2) {
			color1 = 255.0f;
			color2 = 0.0f;
			color3 = 212.0f;
		}
		else if (i == 3) {
			color1 = 43.0f;
			color2 = 143.0f;
			color3 = 66.0f;
		}
		else if (i == 4) {
			color1 = 221.0f;
			color2 = 255.0f;
			color3 = 0.0f;
		}
		else if (i == 5) {
			color1 = 0.0f;
			color2 = 255.0f;
			color3 = 136.0f;
		}
		DrawColoredCube(0.2, 5.5, 4.5, 15.9 + (i * 3.1), 25.25, 48, color1, color2, color3);
		DrawColoredCube(0.2, 5.5, 4.5, 18.1 + (i * 3.1), 25.25, 48, color1, color2, color3);
		DrawColoredCube(  2,   5,   4,   17 + (i * 3.1), 25.25, 48, 255.0f, 255.0f, 255.0f);
	}

	DrawColoredCube(5, 1, 21, 48, 25, 35, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(4, 3,  3, 48, 26.5, 35, 255.0f, 111.0f, 0.0f);
	DrawColoredCube(2, 4,  2, 48, 27, 30, 0.0f, 4.0f, 255.0f);

	DrawColoredCube(5, 1, 21, 48, 21, 31, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(4, 2, 5, 48, 22.5, 38, 43.0f, 143.0f, 66.0f);
	DrawColoredCube(2, 3, 2, 48, 22.5, 25, 255.0f, 111.0f, 0.0f);

	DrawColoredCube(5, 1, 21, 48, 17, 27, 140.0f, 102.0f, 69.0f);
	DrawColoredCube(4, 2, 5, 48, 18.5, 34, 0.0f, 255.0f, 136.0f);
}

void Demo::Lamp(int draw) {
	if (draw == 1) {
		for (int i = 0; i < 5; i++) {
			DrawColoredCube(0.5 + (i * 0.2), 0.25, 0.5 + (i * 0.2), 0, 32 + (i * 0.25), 0, 255.0f, 255.0f, 255.0f);
		}
		DrawColoredCube(1.5, 0.5, 1.5, 0, 33.35, 0, 255.0f, 255.0f, 255.0f);
		DrawColoredCube(0.75, 0.75, 0.75, 0, 34, 0, 0.0f, 0.0f, 0.0f);
	}
	else {
		DrawColoredCube(1, 3, 1, -45, 7, 20, 0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 15; i++) {
			DrawColoredCube(4 - (i * 0.2), 0.25, 4 - (i * 0.2), -45, 8.5 + (i * 0.25), 20, 255.0f, 255.0f, 255.0f);
		}
	}
}

void Demo::Carpet() {
	DrawColoredCube(45, 0.25, 45, 10, 0, 0, 41.0f, 60.0f, 107.0f);
}

void Demo::BuildColoredCube() {
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5,  0.0f,  0.0f,  1.0f, // 0
		0.5, -0.5, 0.5,   0.0f,  0.0f,  1.0f, // 1
		0.5,  0.5, 0.5,   0.0f,  0.0f,  1.0f, // 2
		-0.5,  0.5, 0.5,  0.0f,  0.0f,  1.0f, // 3

		// right
		0.5,  0.5,  0.5, 1.0f,  0.0f,  0.0f, // 4
		0.5,  0.5, -0.5, 1.0f,  0.0f,  0.0f, // 5
		0.5, -0.5, -0.5, 1.0f,  0.0f,  0.0f, // 6
		0.5, -0.5,  0.5, 1.0f,  0.0f,  0.0f, // 7

		// back
		-0.5, -0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 8 
		0.5,  -0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 9
		0.5,   0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 10
		-0.5,  0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-0.5, -0.5, -0.5, -1.0f,  0.0f,  0.0f, // 12
		-0.5, -0.5,  0.5, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  0.5,  0.5, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  0.5, -0.5, -1.0f,  0.0f,  0.0f, // 15

		// upper
		0.5, 0.5,  0.5, 0.0f,  1.0f,  0.0f, // 16
		-0.5, 0.5, 0.5, 0.0f,  1.0f,  0.0f, // 17
		-0.5, 0.5, -0.5,0.0f,  1.0f,  0.0f, // 18
		0.5, 0.5, -0.5, 0.0f,  1.0f,  0.0f, // 19

		// bottom
		-0.5, -0.5, -0.5, 0.0f,  -1.0f,  0.0f, // 20
		0.5, -0.5, -0.5,  0.0f,  -1.0f,  0.0f, // 21
		0.5, -0.5,  0.5,  0.0f,  -1.0f,  0.0f, // 22
		-0.5, -0.5,  0.5, 0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define normal pointer layout 2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube(float s1, float s2, float s3, float t1, float t2, float t3, float c1, float c2, float c3)
{
	UseShader(shaderProgram);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, c1 / 255.0f, c2 / 255.0f, c3 / 255.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(t1, t2, t3));

	model = glm::scale(model, glm::vec3(s1, s2, s3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0.0f,  1.0f,  0.0f,
		 50.0, -0.5, -50.0,  0.0f,  1.0f,  0.0f,
		 50.0, -0.5,  50.0,  0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0.0f,  1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	UseShader(shaderProgram);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 179.0f/255.0f, 179.0f / 255.0f, 179.0f / 255.0f);

	glm::mat4 model;

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 5.0f;
	posCamZ = 18.0f;
	viewCamX = 0.0f;
	viewCamY = 500.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 25.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.1f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Basic Lighting: Phong Lighting Model", 1920, 1080, true, true);
}