#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;

Tema2::Tema2() {}

Tema2::~Tema2() {}

// create random number
int Tema2::RandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void Tema2::Init()
{
    //  glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // create camera and set position |  (x, y, z) body tank position -> (x, y + ?, z - ?) camera position
    camera = new implemented::Camera();
    camera->Set(player.body + glm::vec3(0, 1.5f, -2.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // set minimap viewport
    glm::ivec2 resolution = window->GetResolution();
    minimap = ViewportArea(resolution.x * 0.8f - 50, resolution.y * 0.8f - 50, resolution.x / 5.f, resolution.y / 5.f);

    // render tank components
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("wheels");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "wheels.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    // render buildings
    {
        Mesh* mesh = new Mesh("type1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "building"), "type1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("type2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "building"), "type2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // render ground
	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

    // render star
    {
        Mesh* mesh = new Mesh("star");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "star.obj");
		meshes[mesh->GetMeshID()] = mesh;
    }
  
    // create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // create matrics
    minimapProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 200.0f);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    viewMatrix = camera->GetViewMatrix();

    // create enemies (25)
    for(int i = 0; i < 25; i++) {
		EnemyTank enemy;
		enemies.push_back(enemy);
	}
    
	// create buildings (25)
    for (int i = 0; i < 25; i++) {
		Building building;
		buildings.push_back(building);
    }

    // create stars
    for (int i = 0; i < 10; i++) {
		Elements element(glm::vec3(i * 5, 0, 0), i);
        elements.push_back(element);
	}

}

void Tema2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::FrameEnd() {}

void Tema2::RenderPlayerTank() {
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player.body);
        modelMatrix = glm::rotate(modelMatrix, player.body_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["wheels"], shaders["TankShader"], modelMatrix, glm::vec3(0.60f,0.61f,0.62f), 3, 0);
        RenderSimpleMesh(meshes["body"], shaders["TankShader"], modelMatrix, glm::vec3(0.22f, 0.26f, 0.73f), 3, 0);


        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player.body);
        modelMatrix = glm::rotate(modelMatrix, player.body_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, player.turret_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["turret"], shaders["TankShader"], modelMatrix, glm::vec3(0.22f, 0.26f, 0.73), 3, 0);
        RenderSimpleMesh(meshes["cannon"], shaders["TankShader"], modelMatrix, glm::vec3(0.60f, 0.61f, 0.62f), 3, 0);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2,2,2));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        RenderSimpleMesh(meshes["box"], shaders["TankShader"], modelMatrix, glm::vec3(1, 1, 0), 3, 0);
    }

}

void Tema2::RenderProjectile(float deltaTime) {
    
    // Render projectiles
    for (Projectile &projectile : projectiles) {
        // cout << "number " << projectiles.size() << endl;
		glm::mat4 modelMatrix = projectile.GetModelMatrix();
        projectile.position += projectile.forward * deltaTime * 6.0f;
        RenderSimpleMesh(meshes["box"], shaders["TankShader"], modelMatrix, glm::vec3(0, 1, 1), 3, 0);
	}
    
    // add out of map collision
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& elem) {
        return abs(elem.position.x) > 50 || abs(elem.position.z) > 50;
        }), projectiles.end());

    // add collision with enemies (check function for delete)
    for (EnemyTank& enemy : enemies) {
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [&enemy](Projectile& elem) {
			return elem.CheckHit(enemy.position, enemy.health);
			}), projectiles.end());
	}

    // add collision with buildings
    for (Building& building : buildings) {
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [&building](Projectile& elem) {
            return building.CheckHitProjectile(elem.position);
            }), projectiles.end());
    }

    // delete enemies with health 0
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyTank& elem) {
        return elem.ShouldDelete();
		}), enemies.end());

}

void Tema2::RenderEnemyTank(float deltaTime) {

    // Render enemies ?? change with iterator ??
    for (EnemyTank &enemy : enemies) {
        
        // move enemy
        enemy.RandomMove(deltaTime);

        // check collision with player
        glm::vec3 collision = enemy.CheckCollision(player.body);
        if (collision != glm::vec3(0, 0, 0)) {
			player.body += collision * 0.5f;
            camera->position += collision * 0.5f;
        }

        // render components
        glm::mat4 modelMatrix = enemy.GetModelMatrix();
        RenderSimpleMesh(meshes["body"], shaders["TankShader"], modelMatrix, glm::vec3(0.69f, 0.13f, 0.13f), enemy.health, 0);
        RenderSimpleMesh(meshes["wheels"], shaders["TankShader"], modelMatrix, glm::vec3(0.60f, 0.61f, 0.62f), enemy.health, 0);
        modelMatrix = glm::rotate(modelMatrix, enemy.turret_angle - enemy.body_angle, glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["turret"], shaders["TankShader"], modelMatrix, glm::vec3(0.69f, 0.13f, 0.13f), enemy.health, 0);
		RenderSimpleMesh(meshes["cannon"], shaders["TankShader"], modelMatrix, glm::vec3(0.60f, 0.61f, 0.62f), enemy.health, 0);
    }

}

void Tema2::RenderBuilding() {
    
    for (Building& building : buildings) {

        // check collision with player
        glm::vec3 collision = building.CheckCollision(player.body);
        if (collision != glm::vec3(0, 0, 0)) {
            player.body -= collision;
            camera->position -= collision;
        }

        // check collision with enemies
        for (EnemyTank& enemy : enemies) {
			glm::vec3 collision = building.CheckCollision(enemy.position);
            if (collision != glm::vec3(0, 0, 0)) {
				enemy.position -= collision;
			}
		}

        glm::mat4 modelMatrix = building.GetModelMatrix();
        RenderSimpleMesh(meshes[building.type], shaders["TankShader"], modelMatrix, glm::vec3(0.97f, 0.97f, 0.97f), 3, 0);
    }
}

void Tema2::Mechanics(float deltaTime) {

    for(EnemyTank& enemy : enemies) {
        // calculate enemy angle for turret
        enemy.ComputeAngle(player.body);

        // calculate shoot on player
        if (enemy.ShootOnPlayer(player.body)) {            
            Projectile projectile(enemy.position + glm::vec3(0, 0.3f, 0), glm::mat4(1), glm::vec3(0, 0, 0) , enemy.turret_angle);
			projectile.CalculateForward();
            projectile.position += 1.5f * projectile.forward;
			projectiles.push_back(projectile);
		}
    }

    // generation move for enemies
    if (generationTime > repeatMove) {
        repeatMove += 5.0f;

        // set a random move for each enemy
        for (EnemyTank& enemy : enemies) {
            int random = RandomNumber(1, 4);
            enemy.flag = random;
		}
    }

    // set default move to none
    if (repeatMove - generationTime < 0.2f) {
        for (EnemyTank& enemy : enemies) {
			enemy.flag = -1;
        }
    }

    // enemies remain in map
    for (EnemyTank& enemy : enemies) {
		enemy.RemainInMap(deltaTime);
	}

    // player remain in map
    player.RemainInMap(&camera, deltaTime);

    // check cooldown for player
    player.CheckCooldown();

    // check hp for player
    MyTank* tank = &player;
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [&tank](Projectile& elem) {
        return elem.CheckHit(tank->body, tank->hp);
        }), projectiles.end());
}

void Tema2::RenderScene() {
    
    // render stars after 60s
    if (Engine::GetElapsedTime() > 60) {
        // render elements
        for (Elements& element : elements) {
            glm::mat4 modelMatrix = element.GetModelMatrix(glm::vec3(0, 6.0f, 49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(0, 12.0f, 49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(-51.0f, 6.0f, 49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(-51.0f, 12.0f, 49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(0.0f, 6.0f, -49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(0.0f, 12.0f, -49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(-51.0f, 6.0f, -49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
            modelMatrix = element.GetModelMatrix(glm::vec3(-51.0f, 12.0f, -49.5f));
            RenderSimpleMesh(meshes["star"], shaders["TankShader"], modelMatrix, glm::vec3(1.0f, 1.0f, 0.0f), 3, 3);
        }
    }

    // render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        RenderSimpleMesh(meshes["plane"], shaders["TankShader"], modelMatrix, glm::vec3(0.49f, 0.65f, 0.25f), 3, 2);
    }

    // render sky
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 15.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        RenderSimpleMesh(meshes["plane"], shaders["TankShader"], modelMatrix, glm::vec3(0.67f, 0.95f, 0.99f), 3, 1);
    }


    // render borders
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 50.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(270.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["plane"], shaders["TankShader"], modelMatrix, glm::vec3(0.67f, 0.95f, 0.99f), 3, 1);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, -50.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(270.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["plane"], shaders["TankShader"], modelMatrix, glm::vec3(0.67f, 0.95f, 0.99f), 3, 1);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(50.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(270.0f), glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["plane"], shaders["TankShader"], modelMatrix, glm::vec3(0.67f, 0.95f, 0.99f), 3, 1);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-50.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(270.0f), glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["plane"], shaders["TankShader"], modelMatrix, glm::vec3(0.67f, 0.95f, 0.99f), 3, 1);
    }

}

void Tema2::Update(float deltaTimeSeconds) {   
    
    // used for mechanics
    generationTime += deltaTimeSeconds;

    // screen render
    typeProjection = 0;
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    // render all 
    RenderScene();
    RenderPlayerTank();
    RenderEnemyTank(deltaTimeSeconds);
    RenderBuilding();
    RenderProjectile(deltaTimeSeconds);
    // add mechanics
    Mechanics(deltaTimeSeconds);

    // minimap render
    typeProjection = 1;
    minimap = ViewportArea(resolution.x * 0.8f - 50, resolution.y * 0.8f - 50, resolution.x / 5.f, resolution.y / 5.f);
    glViewport(minimap.x, minimap.y, minimap.width, minimap.height);

    // render all
    RenderPlayerTank();
    RenderEnemyTank(deltaTimeSeconds);
    RenderBuilding();
    RenderScene();
    RenderProjectile(deltaTimeSeconds);

    // end game
    if (Engine::GetElapsedTime() > 120 || !player.hp) {
        window->Close();
    }

    // check time
	// cout << "time " << Engine::GetElapsedTime() << endl;
    // check hp
    // cout << "hp " << player.hp << endl;
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3& color, int health, int type)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // render using shader program
    glUseProgram(shader->program);

    // modelMatrix location
    int M = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(M, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // viewMatrix location
    int V = glGetUniformLocation(shader->program, "View");
    
    // type projection
    if (typeProjection == 0) {
        viewMatrix = camera->GetViewMatrix();
    }
    else
        viewMatrix = glm::lookAt(player.body + glm::vec3(0, 5.0f, 0), player.body, glm::vec3(0, 0, -1));

    glUniformMatrix4fv(V, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // projectionMatrix location
    int P = glGetUniformLocation(shader->program, "Projection");
    if(typeProjection == 0)
        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    else
        projectionMatrix = minimapProjectionMatrix;
    glUniformMatrix4fv(P, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    // time location
    int time = glGetUniformLocation(shader->program, "Time");
    glUniform1f(time, Engine::GetElapsedTime());

    // state location
    int state = glGetUniformLocation(shader->program, "State");
    glUniform1i(state, health);

    // color location
    int location_color = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(location_color, 1, glm::value_ptr(color));

    // type location
    int location_type = glGetUniformLocation(shader->program, "Type");
    glUniform1i(location_type, type);
    
    // eye location
    glm::vec3 eyePosition = camera->position;
    int location_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(location_eye_position, 1, glm::value_ptr(eyePosition));

    // light location
    glm::vec3 lightPosition = player.body + player.body_forward * 0.65f + glm::vec3(0, 1.5f, 0);
    int location_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(location_light_position, 1, glm::value_ptr(lightPosition));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    // set parameters
    float forwardSpeed = 5.0f;
    float rotationSpeed = 2.0f;
    
    // type projection
    if (window->KeyHold(GLFW_KEY_1)) {
        typeProjection = 0;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        typeProjection = 1;
    }

    // movement
    if (!onCamera) {
        if (window->KeyHold(GLFW_KEY_W)) {
            player.calculateBodyForward();
            player.body += forwardSpeed * deltaTime * player.body_forward;
            camera->MoveForward(forwardSpeed * deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            player.calculateBodyForward();
            player.body -= forwardSpeed * deltaTime * player.body_forward;
            camera->MoveForward(-forwardSpeed * deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            player.body_angle += rotationSpeed * deltaTime;
            player.calculateBodyForward();
            camera->RotateThirdPerson_OY(rotationSpeed * deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            player.body_angle -= rotationSpeed * deltaTime;
            player.calculateBodyForward();
            camera->RotateThirdPerson_OY(-rotationSpeed * deltaTime);
        }
    }
}

void Tema2::OnKeyPress(int key, int mods) {}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // move camera
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && onCamera == true) {   
        float sensivityOY = 0.001f;
        if (window->GetSpecialKeyState() == 0) {
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
    // move turret
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && onCamera == false && onTurret == true) {
        float sensivityOY = 0.001f;
        if (isInitialMousePositionSet) {
            isInitialMousePositionSet = false;
            return;
        }
        if (window->GetSpecialKeyState() == 0 && onTurret == true) {
            player.turret_angle -= sensivityOY * deltaX * 2.0f;
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // create a projectile
    if(button == GLFW_MOUSE_BUTTON_2 && player.cooldown == false) {
        Projectile projectile(player.body + glm::vec3(0, 0.3f, 0), glm::mat4(1), player.body_forward, player.body_angle + player.turret_angle);
        projectile.CalculateForward();
        projectile.position += 1.5f * projectile.forward;
		projectiles.push_back(projectile);
        player.SetShoot();
	}

    // rotate camera
    if (button == GLFW_MOUSE_BUTTON_3) {
        // initial mouse position
        initialMousePosition = glm::ivec2(mouseX, mouseY);
        isInitialMousePositionSet = true;

        // lock camera & turret
        onCamera = true;
        onTurret = false;
        camera->SaveState();
        lastTurret = player.turret_angle;
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // unlock camera
    if (button == GLFW_MOUSE_BUTTON_3) {
        onCamera = false;
        onTurret = true;
        camera->LoadState();
        player.turret_angle = lastTurret;
    }
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}
