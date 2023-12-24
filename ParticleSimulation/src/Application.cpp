#include "Renderer.h"

#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <locale>
#include <codecvt>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "SquareIndicies.h"
#include "ShaderCompiler.h"

#include "Simulation/Particle.h"
#include "Simulation/Quadtree.h"
#include "Simulation/ParticleSpawner.h"

#define SCR_W 800
#define SCR_H 800

#define TARGET_FPS 60.0f //Caps at about half this
#define CAP_FPS true

#define CALCULATE_FRAMERATE true

// Constant color:
// 8K particles at 17.5fps
// 
// color changing with speed:
// 8K particles at 15.9fps

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_W, SCR_H, "Particle Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing glew" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        std::srand(0);
        Quadtree tree(Container(-1.0f, -1.0f, 2.0f, 2.0f), 0);
        ASSERT(tree.GetContainer()->Intersects(1.0f, 0.0f));

        std::vector<Vertex> positions = {  };
        std::vector<SquareIndicies> squareIndicies = {  };
        std::vector<Particle> particlesVector = {  };


        //Spawn particles
        ParticleSpawner spawner;

        //TODO: Add quadtree visualization

        //fill screen with particles
        //spawner.SpawnSquare(300, particlesVector);
        
        //                Start pos -1.0f to 1.0f          particle count    particle mass                          IMPORTANT: When editing this, make sure to adjust TIMESTEP in 
        //                          |         start velocity      |  center mass   |            spin speed           scr\Simulation\Particle.cpp for approprate simulation speed
        //spawner.SpawnGalaxy({ -0.3f,  0.3f }, { 0.0f, -300.0f }, 3000, 100000.0f, 100.5f, .1f, 1000.0f, particlesVector);
        //spawner.SpawnGalaxy({  0.3f, -0.3f }, { 0.0f,  300.0f }, 3000, 100000.0f,    .5f, .1f, 1000.0f, particlesVector);
        spawner.SpawnGalaxy({  0.0f, 0.0f },  { 0.0f,  -300.0f }, 200000, 100000.0f,    .5f, .3f, 1000.0f, particlesVector);
        //                                                                                  |              DONT CHANGE                                             
        //                                                                            galaxy radius
        
        std::cout << "Number of particles: " << particlesVector.size() << std::endl;

        //create verticies for particles
        for (Particle p : particlesVector) {
            p.createParticleVerticies(positions, squareIndicies);
        }

        VertexArray va;
        VertexBuffer vb(positions, 3 * sizeof(float), GL_DYNAMIC_DRAW);

        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 2, GL_FALSE); // 1st 2 floats are position
        //layout.Push(GL_FLOAT, 4, GL_FALSE); // next 4 floats are color
        layout.Push(GL_FLOAT, 1, GL_FALSE); // next 1 float is alpha
        
        va.AddBuffer(vb, layout);

        IndexBuffer ib(squareIndicies, GL_STATIC_DRAW);

        //Create shaders
        ShaderProgramSource source = ParseShader("../ParticleSimulation/res/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        //set particle color
        /*GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);*/
        //GLCall(glUniform4f(location, 0.1f, 0.3f, 0.8f, 1.0f));

        GLCall(glUseProgram(0));
        vb.Unbind();
        ib.Unbind();

        //Framerate calculation variables
        unsigned int frameNum = 0;
        double startTime = glfwGetTime();

        double previousTime = glfwGetTime();
        const double frameTime = 1.0 / TARGET_FPS;

        // Enable blending
        GLCall(glEnable(GL_BLEND)); 
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        while (!glfwWindowShouldClose(window))
        {
            if (CAP_FPS) {
                double currentTime = glfwGetTime();
                double elapsedTime = currentTime - previousTime;

                if (elapsedTime < frameTime) continue;

                previousTime = currentTime;
            }

            //std::cout << std::endl;

            positions = {};
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            ASSERT(tree.GetTotalMass() == 0);
            for (Particle p : particlesVector) { //insert particles into quadtree
                tree.InsertParticle(p);
            }

            //UPDATING PARTICLES
            tree.CalculateForceOnParticles(particlesVector);

            for (Particle& p : particlesVector) {
                p.UpdatePosition();
                p.createParticleVerticies(positions);
            }

            GLCall(glUseProgram(shader));


            va.Bind();
            ib.Bind();
            vb.Bind();

            vb.UpdateBuffer(positions);

            //std::cout << "color location: " << glGetAttribLocation(shader, "color") << std::endl;
            
            // draw particles
            GLCall(glDrawElements(GL_TRIANGLES, squareIndicies.size() * 6, GL_UNSIGNED_INT, nullptr)); 

            glfwSwapBuffers(window);
            glfwPollEvents();

            tree.ClearTree();


            if (CAP_FPS) {
                // Calculate the time taken to render the frame
                double renderTime = glfwGetTime() - previousTime;

                // Sleep for the remaining time
                double sleepTime = frameTime - renderTime;
                if (sleepTime > 0.0f) {
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
                }
            }

            if (CALCULATE_FRAMERATE) {
                frameNum++;
                if (frameNum == 1000) {
                    std::cout << "Average Framerate: " << 1 / ((glfwGetTime() - startTime) / frameNum) << ", current time: " << glfwGetTime() - startTime << std::endl;
                }
            }
        }
        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}