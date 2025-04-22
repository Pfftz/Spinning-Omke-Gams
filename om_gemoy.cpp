#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION // Define this in exactly one .c or .cpp file
#include <stb_image.h>           // Include stb_image header for texture loading

#define SDL_MAIN_HANDLED       // Prevent SDL from redefining main
#include <SDL2/SDL.h>          // Include SDL main header
#include <SDL2/SDL_mixer.h>    // Include SDL_mixer header

#include <iostream>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function prototypes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int createShaderProgram(const char *vertexSource, const char *fragmentSource);
unsigned int loadTexture(const char *path); // Texture loading function prototype

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader source code
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord; // Texture coordinate attribute

    out vec2 TexCoord; // Output texture coordinate to fragment shader

    uniform mat4 transform;

    void main()
    {
        gl_Position = transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
        TexCoord = aTexCoord; // Pass texture coordinate to fragment shader
    }
)";

// Fragment Shader source code
const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord; // Input texture coordinate from vertex shader

    uniform sampler2D ourTexture; // Texture sampler

    void main()
    {
        FragColor = texture(ourTexture, TexCoord); // Sample texture at TexCoord
    }
)";

// Global variable for music
Mix_Music *backgroundMusic = NULL;
// Flag to track successful mixer initialization
bool mixerInitialized = false;

int main(int argc, char *argv[])
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rotating Pak Vramroro", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize SDL Audio and SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }
    else
    {
        int flags = MIX_INIT_MP3;
        if ((Mix_Init(flags) & flags) != flags)
        {
            std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
            SDL_Quit();
        }
        else
        {
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                std::cerr << "Failed to open audio device: " << Mix_GetError() << std::endl;
                Mix_Quit();
                SDL_Quit();
            }
            else
            {
                mixerInitialized = true; // Set flag here after successful OpenAudio
                backgroundMusic = Mix_LoadMUS("Pak_Vramroro.mp3");
                if (!backgroundMusic)
                {
                    std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
                    mixerInitialized = false;
                    Mix_CloseAudio();
                    Mix_Quit();
                    SDL_Quit();
                }
                else
                {
                    if (Mix_PlayMusic(backgroundMusic, -1) == -1)
                    {
                        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
                        mixerInitialized = false;
                        Mix_FreeMusic(backgroundMusic);
                        backgroundMusic = NULL;
                        Mix_CloseAudio();
                        Mix_Quit();
                        SDL_Quit();
                    }
                }
            }
        }
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0)
    {
        glfwTerminate();
        return -1; // Shader creation failed
    }

    // load texture
    // ------------
    unsigned int texture = loadTexture("gemoy.png");
    if (texture == 0)
    {
        std::cerr << "Failed to load texture" << std::endl;
        glDeleteProgram(shaderProgram);
        glfwTerminate();
        return -1;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices;
    float radius = 0.5f;
    int numSides = 8; // Octagon

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.5f);

    for (int i = 0; i < numSides; ++i)
    {
        float angle = i * (2.0f * M_PI / numSides);
        vertices.push_back(radius * cos(angle));
        vertices.push_back(radius * sin(angle));
        vertices.push_back(0.5f + 0.5f * cos(angle));
        vertices.push_back(0.5f + 0.5f * sin(angle));
    }

    vertices.push_back(vertices[4]);
    vertices.push_back(vertices[5]);
    vertices.push_back(vertices[6]);
    vertices.push_back(vertices[7]);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSides + 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);

    if (backgroundMusic)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }
    if (mixerInitialized)
    {
        Mix_CloseAudio();
        Mix_Quit();
    }
    if (SDL_WasInit(SDL_INIT_AUDIO))
    {
        SDL_Quit();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int createShaderProgram(const char *vertexSource, const char *fragmentSource)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return 0;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int loadTexture(const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
        {
            std::cerr << "Texture format not supported: " << nrComponents << " components" << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        return 0;
    }

    return textureID;
}
