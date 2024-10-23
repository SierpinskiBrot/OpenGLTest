#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "stb_image.h"

#include "shader.h"
#include "main.h"

#include "ft2build.h"
#include <map>
#include "circle.cpp"
#include "letterRenderer.cpp"
#include FT_FREETYPE_H

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view;
glm::mat4 projection = glm::mat4(1.0f);


int screenWidth = 1600;
int screenHeight = 900;


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// original camera angle values
float yaw = -90.0f;
float pitch = 0.0f;
float zoom = 60.0f;

// original mouse pos values
float lastX = 800, lastY = 450;


int main() 
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGLTest", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader programs
    // -----------------------------------------------------------------
    Shader ourShader("textureshader.vert", "textureshader.frag");
    Shader colorShader("shader1.vert", "shader1.frag");
    glEnable(GL_DEPTH_TEST);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    float vertices2[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW); //STATIC_DRAW - data defined once, used many times

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    
    

    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("resources/charles512.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    ourShader.setInt("texture1",0);

    
    glm::vec3 cubePositions[1];
    int currIdx = 0;

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            for (int k = 0; k < 1; k++) {
                cubePositions[currIdx] = glm::vec3(1.5*i - 5, 1.5*j - 5, -1.5*k - 5);
                currIdx++;
            }
        }
    };
    

    

    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
    ourShader.setMat4("projection", projection);

    LetterRenderer letters = LetterRenderer();

    std::vector<Circle> circles;



    Circle outerBorderCircle = Circle(1.0f, colorShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f,0.0f,0.0f));
    Circle innerBorderCircle = Circle(0.95f, colorShader, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f,0.0f,0.001f));

    
    for (int i = 0; i < 100; i++) {
        Circle physCircle = Circle(0.02f, colorShader, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3((0.65f*i*1.0f/100.0f+0.25f)*cos(i), (0.65f * i * 1.0f / 100.0f +0.25f)*sin(i), 0.002f));
        physCircle.setAcceleration(glm::vec3(0.0f, -1.0f, 0.0f));
        circles.push_back(physCircle);
    }






    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // uncomment this call to draw in wireframe polygons.
     //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // The render loop ;)
    // ------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        //std::cout << yaw << "\n";
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //std::cout << deltaTime << '\n';

        // create transformations
        glm::vec3 direction{};
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);

        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



        // draw our first triangle
        ourShader.use();
        ourShader.setMat4("view", view);
        projection = glm::perspective(glm::radians(zoom), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
        ourShader.setMat4("projection", projection);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 36);



        for (unsigned int i = 0; i < 1; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)currentFrame * 0.2f * (cubePositions[i].x+ cubePositions[i].y + cubePositions[i].z +0.5f), glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::translate(model, 0.5f* glm::vec3(0.5*sin(currentFrame * 2)+ 0.3*sin(currentFrame * 4), 0.2*sin(currentFrame * 1)+ 0.7*sin(currentFrame *5), 0.3*cos(currentFrame * 3) + 0.1*sin(currentFrame * 6)));
            ourShader.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        outerBorderCircle.draw(view, projection);
        innerBorderCircle.draw(view, projection);
        
        if (glfwGetTime()*20+95 > circles.size()) {
            Circle physCircle = Circle(0.02f, colorShader, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.5f*sin(glfwGetTime()*10), 0.5f-0.1f*cos(glfwGetTime()*10), 0.002f));
            physCircle.setAcceleration(glm::vec3(0.0f, -1.0f, 0.0f));
            circles.push_back(physCircle);
        }


        for (int i=0; i < circles.size(); i++) {

            circles[i].draw(view, projection);
            circles[i].update(0.005f);
            circles[i].applyConstraint();
            

            for (int k = i+1; k < circles.size(); k++) {
                const glm::vec2 collision_axis = glm::vec2(circles[i].pos.x - circles[k].pos.x, circles[i].pos.y - circles[k].pos.y);
                const float dist = pow(collision_axis.x * collision_axis.x + collision_axis.y * collision_axis.y, 0.5f);
                if (dist < circles[i].radius + circles[k].radius) {
                    const glm::vec2 n = collision_axis / dist;
                    const float delta = circles[i].radius + circles[k].radius - dist;
                    circles[i].pos += glm::vec3(0.5f * delta * n, 0.0f);
                    circles[k].pos -= glm::vec3(0.5f * delta * n, 0.0f);
                }

            }
  
            
        }
        
 
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // swap buffers and poll IO events
        // ----------------------------------------------------------------

        
        letters.RenderText("FPS: " + std::to_string(round(1 / deltaTime)), 25.0f, 850.0f, 0.9f, glm::vec3(0.8, 0.8f, 0.8f));
        letters.RenderText("dt: " + std::to_string(deltaTime), 25.0f, 800.0f, 0.9f, glm::vec3(0.8, 0.8f, 0.8f));
        letters.RenderText("circles: " + std::to_string(circles.size()), 25.0f, 750.0f, 0.9f, glm::vec3(0.8, 0.8f, 0.8f));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //dfg

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 30.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 0.5f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
        yaw = -90.0f;
        pitch = 0.0f;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    zoom += yoffset;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
}


// process mouse movements
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}




