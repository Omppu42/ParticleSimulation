#include "GLUtils.h"
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
#include "Shader.h"

#include "ShapeGenerator.h"

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

        std::vector<Particle> particlesVector = {  };

        //Spawn particles
        ParticleSpawner spawner;

        //fill screen with particles
        //spawner.SpawnSquare(300, particlesVector);
        
        //                Start pos -1.0f to 1.0f          particle count    particle mass                          IMPORTANT: When editing this, make sure to adjust TIMESTEP in 
        //                          |         start velocity      |  center mass   |            spin speed           scr\Simulation\Particle.cpp for approprate simulation speed
        //spawner.SpawnGalaxy({ -0.3f,  0.3f }, { 0.0f, -300.0f }, 1000, 100000.0f, 100.5f, .1f, 1000.0f, particlesVector);
        //spawner.SpawnGalaxy({  0.3f, -0.3f }, { 0.0f,  300.0f }, 20000, 100000.0f,    .5f, .1f, 1000.0f, particlesVector);
        spawner.SpawnGalaxy({  0.0f, 0.0f },  { 0.0f,  -300.0f }, 10000, 100000.0f,    .5f, .3f, 1000.0f, particlesVector);
        //                                                                                  |              DONT CHANGE                                             
        //                                                                            galaxy radius
        
        std::cout << "Number of particles: " << particlesVector.size() << std::endl;


        // PARTICLES RENDERING SETUP
        RendererState rendererParticles(GL_DYNAMIC_DRAW, GL_STATIC_DRAW, "../ParticleSimulation/res/shaders/Particle.shader");

        rendererParticles.AddVBLayoutElement(GL_FLOAT, 2);
        rendererParticles.AddVBLayoutElement(GL_FLOAT, 1);

        for (Particle& p : particlesVector) {
            p.createParticleVerticies(rendererParticles.vbo, rendererParticles.ibo);
        }

        rendererParticles.UpdateVBOandIBO();
        rendererParticles.UnBindAll();


        // QUADTREE RENDERING SETUP
        RendererState rendererQuadtree(GL_DYNAMIC_DRAW, GL_DYNAMIC_DRAW, "../ParticleSimulation/res/shaders/Quadtree.shader");

        rendererQuadtree.AddVBLayoutElement(GL_FLOAT, 2);
        rendererQuadtree.AddVBLayoutElement(GL_FLOAT, 3);

        tree.Draw(rendererQuadtree.vbo, rendererQuadtree.ibo);
        
        rendererQuadtree.UpdateVBOandIBO();
        rendererQuadtree.UnBindAll();


        // SHAPES RENDERING SETUP
        RendererState rendererShapes(GL_STATIC_DRAW, GL_STATIC_DRAW, "../ParticleSimulation/res/shaders/Basic.shader");

        rendererShapes.AddVBLayoutElement(GL_FLOAT, 2);
        rendererShapes.AddVBLayoutElement(GL_FLOAT, 4);

        //ShapeGenerator::DrawHorizontalLine({ 0.9f, -0.5f }, { -0.9f, 0.5f }, 0.1f, { 0.0f, 1.0f, 0.0f, 0.5f }, rendererShapes.vbo, rendererShapes.ibo);
        //ShapeGenerator::DrawVerticalLine({ -0.9f, -0.5f }, { 0.9f, 0.5f }, 0.1f, { 0.0f, 1.0f, 0.0f, 0.5f }, rendererShapes.vbo, rendererShapes.ibo);
        //ShapeGenerator::DrawCircle({ 0.0f, 0.2f }, 0.2f, 10, { 1.0f, 0.0f, 0.0f, 1.0f }, rendererShapes.vbo, rendererShapes.ibo);

        rendererShapes.UpdateVBOandIBO();
        rendererShapes.UnBindAll();


        //Framerate calculation variables
        unsigned int frameNum = 0;
        double startTime = glfwGetTime();

        double previousTime = glfwGetTime();
        const double targetFrameTime = 1.0 / TARGET_FPS;

        // Enable blending
        GLCall(glEnable(GL_BLEND)); 
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        while (!glfwWindowShouldClose(window))
        {
            rendererParticles.vbo.ClearVerticiesData();
            rendererQuadtree.vbo.ClearVerticiesData();
            rendererQuadtree.ibo.ClearIndicies();
            
            if (CAP_FPS) {
                double currentTime = glfwGetTime();
                double elapsedTime = currentTime - previousTime;

                if (elapsedTime < targetFrameTime) continue;
            }

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            ASSERT(tree.GetTotalMass() == 0);
            for (Particle& p : particlesVector) { //insert particles into quadtree
                tree.InsertParticle(p);
            }


            // UPDATE PARTICLES
            {
                tree.CalculateForceOnParticles(particlesVector);

                for (Particle& p : particlesVector) {
                    p.UpdatePosition();
                    p.createParticleVerticies(rendererParticles.vbo);
                }

                rendererParticles.UpdateVBO();
                Renderer::Draw(rendererParticles);
            }
            

            if (VISUALIZE_QUADTREE) {
                // Draw quadtree visualization
                tree.Draw(rendererQuadtree.vbo, rendererQuadtree.ibo);

                rendererQuadtree.UpdateVBOandIBO();
                Renderer::Draw(rendererQuadtree);
            }

            // Draw shapes
            Renderer::Draw(rendererShapes);

            glfwSwapBuffers(window);
            glfwPollEvents();

            tree.ClearTree();

            if (CAP_FPS) {

                // Calculate the time taken to render the frame
                double renderTime = glfwGetTime() - previousTime;

                // Sleep for the remaining time
                double sleepTime = targetFrameTime - renderTime;
                if (sleepTime > 0.0f) {
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
                }
                previousTime = glfwGetTime() + sleepTime;
            }

            if (CALCULATE_FRAMERATE) {
                frameNum++;
                if (frameNum == 1000) {
                    std::cout << "After " << frameNum << " frames: " << std::endl <<
                        "  " << ROUND_TO_DECIMAL(1 / ((glfwGetTime() - startTime) / frameNum), 1) << " fps (" << ROUND_TO_DECIMAL((glfwGetTime() - startTime) / frameNum * 1000.0f, 2) << "ms)" << std::endl;
                }
            }
        }
    }
    glfwTerminate();
    return 0;
}