#pragma once

#include <vector>

#include "lab_m1/lab5/lab_camera.h"
#include "components/simple_scene.h"
#include "components/camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:

         // functions for helping
         int RandomNumber(int min, int max);

         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };

         struct MyTank
         {  
             MyTank() : body(glm::vec3(0, 0, 0)), turret(glm::vec3(0, 0, 0)), cannon(glm::vec3(0, 0, 0)), wheels(glm::vec3(0, 0, 0)), body_forward(glm::vec3(0, 0, 0)),
                 turret_forward(glm::vec3(0, 0, 0)) {}
             MyTank(glm::vec3 body, glm::vec3 turret, glm::vec3 cannon, glm::vec3 wheels, glm::vec3 body_forward, glm::vec3 turret_forward)
				 : body(body), turret(turret), cannon(cannon), wheels(wheels), body_forward(body_forward), turret_forward(turret_forward) {}
             
             // vector of position
             glm::vec3 body;
             glm::vec3 turret;
             glm::vec3 cannon;
             glm::vec3 wheels;
             
             glm::vec3 body_forward;
             float body_angle = 0;

             glm::vec3 turret_forward;
             float turret_angle = 0;

             bool cooldown = false;
             float time = 0;

             int hp = 10;
            
             // calculate forward
             void calculateBodyForward()
             {
				 body_forward = glm::vec3(glm::sin(body_angle), 0, glm::cos(body_angle));
			 }

             // remain in map
             void RemainInMap(implemented::Camera **camera, float deltaTime) {
                 

                 calculateBodyForward();
                 float map = 50.0f;
                 float speed = 10.0f;
                 float ratio = deltaTime * speed;
                 glm::vec3 forwardHere = body_forward * ratio;

                 
                 if (body.x > map) {
                     if ((body + forwardHere).x < map) {
                         body += forwardHere;
                         (*camera)->MoveForward(ratio);
                     }
                     else
                     {
                         body -= forwardHere;
						 (*camera)->MoveForward(-ratio);
                     }
				 }
				 if (body.x < -map) {
                     if ((body + forwardHere).x > -map) {
                         body += forwardHere;
                         (*camera)->MoveForward(ratio);
                     }
                     else
                     {
                         body -= forwardHere;
                         (*camera)->MoveForward(-ratio);
                     }
				 }
                 if (body.z > map) {
                     if ((body + forwardHere).z < map) {
                         body += forwardHere;
                         (*camera)->MoveForward(ratio);
                     }
                     else
                     {
                         body -= forwardHere;
                         (*camera)->MoveForward(-ratio);
                     }
                 }
                 if (body.z < -map) {
                     if ((body + forwardHere).z > -map) {
                         body += forwardHere;
                         (*camera)->MoveForward(ratio);
                     }
                     else
                     {
                         body -= forwardHere;
                         (*camera)->MoveForward(-ratio);
                     }
                 }
             }

             void SetShoot() {
				 cooldown = true;
				 time = Engine::GetElapsedTime() + 1.0f;
			 }


             void CheckCooldown() {
                 if (cooldown == true) {
                     if (Engine::GetElapsedTime() > time) {
						 cooldown = false;
					 }
				 }
			 }

		 };

         struct EnemyTank {

             EnemyTank() : position(GenerateRandomPosition()), modelMatrix(GetModelMatrix()), health(3) {}
             EnemyTank(glm::vec3 position, glm::mat4 modelMatrix, int health) : position(position), modelMatrix(modelMatrix), health(health) {}

             // general position
             glm::vec3 position;
             // model Matrix
             glm::mat4 modelMatrix;
             // forward
             glm::vec3 forward;

             // angle turret
             float turret_angle = 0;

             // angle body  
             float body_angle = 0;

             // health
             int health;

             // clock for shooting
             float clock = 0;
             bool set = false;

             // flag for random move
             int flag = 0;

             // generate random position
             int RandomCoordonates(int min, int max)
             {
                 return rand() % (max - min + 1) + min;
             }

             // generate random coordinates between ? and ?
             glm::vec3 GenerateRandomPosition()
			 {  
				 int x = RandomCoordonates(-45, 45);
				 int z = RandomCoordonates(-45, 45);

                 // not in the player spawn (10, 10)
                 // cant detect him on start
                 if(abs(x) < 10.0f + 2.0f || abs(z) < 10.0f + 2.0f)
					 return GenerateRandomPosition();
                 
				 return glm::vec3(x, 0, z);
			 }

             // get model matrix
             glm::mat4 GetModelMatrix()
			 {   
                 // set to 1
                 modelMatrix = glm::mat4(1);
				 // translate
                 modelMatrix = glm::translate(modelMatrix, position);
                 // scale   
                 modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
                 // rotate
                 modelMatrix = glm::rotate(modelMatrix, body_angle, glm::vec3(0, 1, 0));
                 
                 return modelMatrix;
			 }

             glm::vec3 GetForward() {
				 forward = glm::vec3(glm::sin(body_angle), 0, glm::cos(body_angle));
                 return glm::vec3(glm::sin(body_angle), 0, glm::cos(body_angle));
			 }

             // check collision
             glm::vec3 CheckCollision(glm::vec3 playerPosition) {
                 
                 glm::vec3 movePosition = glm::vec3(0, 0, 0);
                 // dif = distance between player and enemy
                 // c1 = center of player
                 // c2 = center of enemy
                 // r1 = radius of player
                 // r2 = radius of enemy
                 // if dif < r1 + r2 => collision
                 if(glm::distance(playerPosition, position) < 0.65f * 2.0f)
				 {  
                     float P = abs(0.65f * 2.0f - glm::distance(playerPosition, position));
                     movePosition = P * glm::normalize(playerPosition - position);
                     position += movePosition * - 0.5f;
                     return movePosition;
				 }

                 return movePosition;
             }

             // check if player is in range
             bool PlayerRange(glm::vec3 playerPosition) {
                 glm::vec3 distanceToPlayer = playerPosition - position;
                 if (glm::length(distanceToPlayer) < 10.0f) {
                     // shoot after 1 second
                     if (set == false) {
						 clock = Engine::GetElapsedTime() + 1.0f;
						 set = true;
					 }
                     return true;
                 }
                 // shoot disabled
                 set = false;
                 return false;
             }

             // compute angle between enemy and player & and set the angle
             void ComputeAngle(glm::vec3 playerPosition) {                 
                 if(!PlayerRange(playerPosition))
					 return;
                 
                 glm::vec3 distanceToPlayer = playerPosition - position;
				 float angle = -atan2(distanceToPlayer.z, distanceToPlayer.x) + glm::radians(90.0f);
				 turret_angle = angle;
			 }

             // shoot on player
             bool ShootOnPlayer(glm::vec3 playerPosition) {
				 if (!PlayerRange(playerPosition))
					 return false;

                 if (Engine::GetElapsedTime() > clock) {
                     clock = Engine::GetElapsedTime() + 3;
                     return true;
                 }

                 return false;
			 }

             // check if enemy is dead
             bool ShouldDelete() {
                 return health <= 0;
			 }

             // random move
             void RandomMove(float deltaTime) {
				 if (flag == 1)
                     position += GetForward() * deltaTime * 0.5f;
				 if (flag == 2)
                     position -= GetForward() * deltaTime * 0.5f;
                 if (flag == 3)
                     body_angle += deltaTime * 0.2f;
                 if (flag == 4)
                     body_angle -= deltaTime * 0.2f;
             }

             // remain in the map
             void RemainInMap(float deltaTime) {
                 float map = 50.0f;
                 float speed = 10.0f;
                 float ratio = deltaTime * speed;
                 if (position.x > map) {
                     // adjust in function of enemy speed
                     position.x = map - ratio;
                 }
                 if (position.x < -map) {
                     position.x = -map + ratio;
                 }

                 if (position.z > map) {
                      position.z = map - ratio;
                 }
                 if (position.z < -map) {
                    position.z = -map + ratio;
                 }
			 }

         };

         struct Projectile {

			 Projectile() : position(glm::vec3(0, 0, 0)), modelMatrix(glm::mat4(1)), forward(glm::vec3(0, 0, 0)), angle(0) {}
			 Projectile(glm::vec3 position, glm::mat4 modelMatrix, glm::vec3 forward, float angle) : position(position), modelMatrix(modelMatrix), forward(forward), angle(angle) {}

			 // general position
			 glm::vec3 position;
			 // model Matrix
			 glm::mat4 modelMatrix;
			 // forward
			 glm::vec3 forward;
			 // angle
			 float angle;

             // calculate forward
             void CalculateForward() {
                 forward = glm::vec3(glm::sin(angle), 0, glm::cos(angle));
             }

			 // get model matrix
             glm::mat4 GetModelMatrix() {
                 // set to 1
                 modelMatrix = glm::mat4(1);
                 // translate
                 modelMatrix = glm::translate(modelMatrix, position);
                 // scale   
                 modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

                 return modelMatrix;
             };

             bool CheckHit(glm::vec3 enemyPosition, int& enemyHealth) {
                 if (glm::distance(position, enemyPosition) < 0.65f * 2.0f) {
					 enemyHealth--;
                     return true;
				 }
				 return false;
			 }
         
         };

         // buildings
         struct Building {

             Building() : position(GenerateRandomPosition()), modelMatrix(GetModelMatrix()), type(GenerateType()) {}
             Building(glm::vec3 position, glm::mat4 modelMatrix, std::string type) : position(position), modelMatrix(modelMatrix), type(type) {}

             // general position
             glm::vec3 position;
             // model Matrix
             glm::mat4 modelMatrix;
             // type of building
             std::string type;


             std::string GenerateType() {
				 int i = RandomCoordonates(1, 2);
                 if(i == 1)
                    return "type1";
                 if(i == 2)
					return "type2";
			 }

             // generate random position
             int RandomCoordonates(int min, int max)
             {
                 return rand() % (max - min + 1) + min;
             }

             // generate random coordinates between ? and ?
             glm::vec3 GenerateRandomPosition()
             {   
                 float map = 50.0f;
                 int x = RandomCoordonates(-map, map);
                 int z = RandomCoordonates(-map, map);

                 if (abs(x) < 5.0f || abs(z) < 8.0f)
                     return GenerateRandomPosition();

                 return glm::vec3(x, 0, z);
             }

             // get model matrix
             glm::mat4 GetModelMatrix()
             {
                 // set to 1
                 modelMatrix = glm::mat4(1);
                 // translate
                 modelMatrix = glm::translate(modelMatrix, position);
                 // scale   
                 modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

                 return modelMatrix;
             }

             // check collision
             glm::vec3 CheckCollision(glm::vec3 playerPosition) {

                 glm::vec3 type1 = glm::vec3(1.5f, 0, 3.0f) * 2.5f;
                 glm::vec3 type2 = glm::vec3(1.0f, 0, 1.0f) * 2.5f;
                 glm::vec3 movePosition = glm::vec3(0, 0, 0);
                 glm::vec3 distanceToPlayer = abs(playerPosition - position);
                 


                 // collision with type 2
                 if (distanceToPlayer.x < type2.x + 0.65f && distanceToPlayer.z < type2.z + 0.65f && type == "type2")
                 {      
                     glm::vec3 collisionDirection = glm::normalize(position - playerPosition);
                     float normalDistanceX = type2.x + 0.65f;
                     float normalDistanceZ = type2.z + 0.65f;
                     
                     float moveDistanceX = normalDistanceX - abs(position.x - playerPosition.x);
                     float moveDistanceZ = normalDistanceZ - abs(position.z - playerPosition.z);
                     
                     float distance = moveDistanceX < moveDistanceZ ? moveDistanceX : moveDistanceZ;
                     float calibration = 0.90f;
                     movePosition = collisionDirection * distance * calibration;
				 }

                 // collision with type 1
                 if (distanceToPlayer.x < type1.x + 0.65f && distanceToPlayer.z < type1.z + 0.65f  && type == "type1")
                 {
					 glm::vec3 collisionDirection = glm::normalize(position - playerPosition);
					 float normalDistanceX = type1.x + 0.65f;
					 float normalDistanceZ = type1.z + 0.65f;

					 float moveDistanceX = normalDistanceX - abs(position.x - playerPosition.x);
					 float moveDistanceZ = normalDistanceZ - abs(position.z - playerPosition.z);

					 float distance = moveDistanceX < moveDistanceZ ? moveDistanceX : moveDistanceZ;
					 float calibration = 0.90f;
					 movePosition = collisionDirection * distance * calibration;
				 }

                 return movePosition;
             }


             bool CheckHitProjectile(glm::vec3 playerPosition) {

                 glm::vec3 type1 = glm::vec3(1.5f, 0, 3.0f);
                 glm::vec3 type2 = glm::vec3(1.0f, 0, 1.0f);
                 glm::vec3 distanceToPlayer = abs(playerPosition - position);

                 // collision with type 2
                 if (distanceToPlayer.x < type2.x && distanceToPlayer.z < type2.z && type == "type2")
                 {
                     return true;
                 }

                 // collision with type 1
                 if (distanceToPlayer.x < type1.x && distanceToPlayer.z < type1.z && type == "type1")
                 {
                     return true;
                 }

                 return false;
             }

         };

         // elements
         struct Elements {
             Elements() : position(glm::vec3(0, 0, 0)), id(0) {}
             Elements(glm::vec3 position, int id) : position(position), id(id) {}
             
             glm::vec3 position;
             int id;

             glm::mat4 GetModelMatrix(glm::vec3 elemPos) {
				 glm::mat4 modelMatrix = glm::mat4(1);
				 modelMatrix = glm::translate(modelMatrix, position + elemPos);
				 modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
                 modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
				 return modelMatrix;
			 }
         };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3& color, int health, int type);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // functions for rendering
        void RenderScene();
        void RenderPlayerTank();
        void RenderEnemyTank(float deltaTime);
        void RenderBuilding();
        void RenderProjectile(float deltaTime);
        void Mechanics(float deltaTime);

        // viewport + projection matrix
        ViewportArea minimap;
        glm::mat4 minimapProjectionMatrix;
        int typeProjection = 0;

        // position tank
        glm::vec3 tank_pos;

        // create camera for game 
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 modelMatrix;
        bool onCamera = false;
        bool onTurret = true;
        float lastTurret = 0;

        glm::ivec2 initialMousePosition;
        bool isInitialMousePositionSet = false;

        // create the player
        MyTank player;
        // create the enemies
        std::vector<EnemyTank> enemies;
        // create the projectiles
        std::vector<Projectile> projectiles;
        // create the buildings
        std::vector<Building> buildings;
        // create the elements
        std::vector<Elements> elements;

        float generationTime = 0;
        float repeatMove = 0;

    };
}   // namespace m1
