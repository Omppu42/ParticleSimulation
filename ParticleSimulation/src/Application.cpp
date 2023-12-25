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
#define VISUALIZE_QUADTREE true

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

        std::vector<Vertex> positionsParticles = {  };
        std::vector<SquareIndicies> squareIndiciesParticles = {  };
        std::vector<Particle> particlesVector = {  };

        std::vector<Vertex> VerticiesQuadtree = {  };
        std::vector<SquareIndicies> squareIndiciesQuadtree = {  };


        //Spawn particles
        ParticleSpawner spawner;

        //TODO: Add quadtree visualization

        //fill screen with particles
        //spawner.SpawnSquare(300, particlesVector);
        
        //                Start pos -1.0f to 1.0f          particle count    particle mass                          IMPORTANT: When editing this, make sure to adjust TIMESTEP in 
        //                          |         start velocity      |  center mass   |            spin speed           scr\Simulation\Particle.cpp for approprate simulation speed
        //spawner.SpawnGalaxy({ -0.3f,  0.3f }, { 0.0f, -300.0f }, 1000, 100000.0f, 100.5f, .1f, 1000.0f, particlesVector);
        //spawner.SpawnGalaxy({  0.3f, -0.3f }, { 0.0f,  300.0f }, 20000, 100000.0f,    .5f, .1f, 1000.0f, particlesVector);
        spawner.SpawnGalaxy({  0.0f, 0.0f },  { 0.0f,  -300.0f }, 20000, 100000.0f,    .5f, .3f, 1000.0f, particlesVector);
        //                                                                                  |              DONT CHANGE                                             
        //                                                                            galaxy radius
        
        std::cout << "Number of particles: " << particlesVector.size() << std::endl;

        //create verticies for particles
        for (Particle p : particlesVector) {
            p.createParticleVerticies(positionsParticles, squareIndiciesParticles);
        }

        VertexArray vaoParticles;
        VertexBuffer vboParticles(positionsParticles, 3 * sizeof(float), GL_DYNAMIC_DRAW);

        VertexBufferLayout layoutParticles;
        layoutParticles.Push(GL_FLOAT, 2, GL_FALSE); // 1st 2 floats are position
        //layout.Push(GL_FLOAT, 4, GL_FALSE); // next 4 floats are color
        layoutParticles.Push(GL_FLOAT, 1, GL_FALSE); // next 1 float is alpha
        
        vaoParticles.AddBuffer(vboParticles, layoutParticles);

        IndexBuffer iboParticles(squareIndiciesParticles, GL_STATIC_DRAW);

        //Create particleShaders
        ShaderProgramSource particleSource = ParseShader("../ParticleSimulation/res/shaders/Particle.shader");
        unsigned int particleShader = CreateShader(particleSource.VertexSource, particleSource.FragmentSource);
        
        //GLCall(glUseProgram(particleShader));

        //set particle color
        //GLCall(int location = glGetUniformLocation(particleShader, "u_Color"));
        //ASSERT(location != -1);
        //GLCall(glUniform4f(location, 0.1f, 0.3f, 0.8f, 1.0f));

        //GLCall(glUseProgram(0));
        vboParticles.Unbind();
        iboParticles.Unbind();
        vaoParticles.Unbind();
        

        // Create quadtree draw buffers
        tree.Draw(VerticiesQuadtree, squareIndiciesQuadtree);
        
        VertexArray vaoQuadtree;
        VertexBuffer vboQuadtree(VerticiesQuadtree, 5 * sizeof(float), GL_DYNAMIC_DRAW);

        VertexBufferLayout layoutQuadtree;
        layoutQuadtree.Push(GL_FLOAT, 2, GL_FALSE); // 1st 2 floats are position
        layoutQuadtree.Push(GL_FLOAT, 3, GL_FALSE); // next 3 float are for color

        vaoQuadtree.AddBuffer(vboQuadtree, layoutQuadtree);

        IndexBuffer iboQuadtree(squareIndiciesQuadtree, GL_DYNAMIC_DRAW);

        ShaderProgramSource quadtreeSource = ParseShader("../ParticleSimulation/res/shaders/Quadtree.shader");
        unsigned int quadtreeShader = CreateShader(quadtreeSource.VertexSource, quadtreeSource.FragmentSource);

        vboQuadtree.Unbind();
        iboQuadtree.Unbind();
        vaoQuadtree.Unbind();


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
            positionsParticles = {};
            VerticiesQuadtree = {};
            squareIndiciesQuadtree = {};
            
            if (CAP_FPS) {
                double currentTime = glfwGetTime();
                double elapsedTime = currentTime - previousTime;

                if (elapsedTime < frameTime) continue;

                previousTime = currentTime;
            }

            //std::cout << std::endl;

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            ASSERT(tree.GetTotalMass() == 0);
            for (Particle p : particlesVector) { //insert particles into quadtree
                tree.InsertParticle(p);
            }


            //UPDATING PARTICLES
            tree.CalculateForceOnParticles(particlesVector);

            for (Particle& p : particlesVector) {
                p.UpdatePosition();
                p.createParticleVerticies(positionsParticles);
            }

            GLCall(glUseProgram(particleShader));


            vaoParticles.Bind();
            iboParticles.Bind();
            vboParticles.Bind();

            vboParticles.UpdateBuffer(positionsParticles);

            //std::cout << "color location: " << glGetAttribLocation(particleShader, "color") << std::endl;
            
            // draw particles
            GLCall(glDrawElements(GL_TRIANGLES, squareIndiciesParticles.size() * 6, GL_UNSIGNED_INT, nullptr));
            
            

            if (VISUALIZE_QUADTREE) {
                // Draw quadtree visualization
                tree.Draw(VerticiesQuadtree, squareIndiciesQuadtree);

                GLCall(glUseProgram(quadtreeShader));

                vaoQuadtree.Bind();
                iboQuadtree.Bind();
                vboQuadtree.Bind();

                vboQuadtree.UpdateBuffer(VerticiesQuadtree);
                iboQuadtree.UpdateIndicies(squareIndiciesQuadtree);

                // draw quadtree
                GLCall(glDrawElements(GL_TRIANGLES, squareIndiciesQuadtree.size() * 6, GL_UNSIGNED_INT, nullptr)); 
            }


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
                if (frameNum == 100) {
                    std::cout << "Average Framerate: " << 1 / ((glfwGetTime() - startTime) / frameNum) << ", current time: " << glfwGetTime() - startTime << std::endl;
                }
            }
        }
        GLCall(glDeleteProgram(particleShader));
        GLCall(glDeleteProgram(quadtreeShader));
    }
    glfwTerminate();
    return 0;
}