#include "task15.h"

// ОНОВЛЕНИЙ Вершинний шейдер: тепер він приймає колір (layout 1) і передає його далі
const char* vertexShaderSource15 = R"(
#version 330 core
layout (location = 0) in vec3 aPos;   // Позиція вершини
layout (location = 1) in vec3 aColor; // Колір вершини

out vec3 vertexColor; // Змінна для передачі кольору у фрагментний шейдер

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
    vertexColor = aColor; // Передаємо колір без змін
}
)";

// ОНОВЛЕНИЙ Фрагментний шейдер: отримує інтерпольований колір
const char* fragmentShaderSource15 = R"(
#version 330 core
out vec4 FragColor;
in vec3 vertexColor; // Отримуємо колір від вершинного шейдера

void main() {
    // OpenGL автоматично змішує (інтерполює) кольори між вершинами
    FragColor = vec4(vertexColor, 1.0); 
}
)";

void runTask15() {
    if (!glfwInit()) return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Final Task: Color Interpolation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;

    // Компіляція шейдерів
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource15, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource15, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // Вершина 1: Нижня ліва  (Червона)
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f, // Вершина 2: Нижня права (Жовта)
         0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // Вершина 3: Верхня права (Зелена)
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // Вершина 4: Верхня ліва  (Синя)
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Атрибут 0: Позиції
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Атрибут 1: Кольори
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 projection = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * model;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        
        GLint mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}