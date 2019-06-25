#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.h"
#include "componentPhysicsBullet.h"
#include "systemPhysicsBullet.h"
#include "componentRenderOpenGL.h"
#include "shader.h"

int main ()
{
    auto entity = new Entity();
	auto sys = new BulletPhysicsSystem();
    
    {
    btCollisionShape* colShape = new btSphereShape(btScalar(1.));
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0,50,0));
    btScalar mass(1.f);

    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
       colShape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo (mass, myMotionState, colShape, localInertia);

    auto body = new btRigidBody(rbInfo);
    auto comp = std::unique_ptr<BulletPhysicsComponent>(new BulletPhysicsComponent(body));
    sys->RegisterComponent(comp.get());
    entity->AddComponent(std::move(comp));
    }

    for (int i = 0; i < 100; i++)
    {
        sys->Tick(1);
        //std::cout << entity->GetLocation().y << "\n";
    }

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
	{
		// TODO: Error
		return EXIT_FAILURE;
	}

	SDL_Window *win = SDL_CreateWindow("Hello", 100, 100, 512, 512, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	SDL_GLContext con = SDL_GL_CreateContext(win);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);
	
	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// TODO: handle errors
	}	


	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(win);

	glEnable(GL_DEPTH_TEST);

	Shader shader("shader.vert", "shader.frag");
	Model model("/home/teagan/Downloads/Nanosuit/Nanosuit.obj");

	bool loop = true;
	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				loop = false;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 27)
			{
				loop = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		glm::mat4 projection = glm::perspective(100.0, 1.0, 0.1, 100.0);
		glm::mat4 view = glm::lookAt(
				glm::vec3(0.0,1.0,0.0),
				glm::vec3(0.0,1.0,-1.0),
				glm::vec3(0.0,-1.0,0.0)
				);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(0.0,0.0,-10.0));
		//modelMat = glm::scale(modelMat, glm::vec3(0.2,0.2,0.2));
		shader.SetMat4("model", modelMat);
		model.Draw(shader);

		SDL_GL_SwapWindow(win);
	}
	
}
