#include <glm/gtx/string_cast.hpp>
glm::mat4 getVulkanProjectionMatrix(float fovy, float aspectRatio, float zNear, float zFar){
    return glm::scale(glm::mat4(1), glm::vec3(1,-1,1)) * glm::perspective(fovy, aspectRatio, zNear, zFar);
}

void GameLogic(Assignment07 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World) {
    /*
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(45.0f);
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;
	
	// Player starting point
	const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);
	
	// Camera target height and distance
	const float camHeight = 0.25;
	const float camDist = 1.5;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(120.0f);
	const float MOVE_SPEED = 2.0f;

    const glm::mat4 I = glm::mat4(1);
    const glm::vec3 X = glm::vec3 (1, 0, 0);
    const glm::vec3 Y = glm::vec3(0, 1, 0);
    const glm::vec3 Z = glm::vec3(0, 0, 1);
	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the _eulerVector axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assginment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	bool fire = false;
	//A->getSixAxis(deltaT, m, r, fire);
    fmwk::GameEngine::getInstance() -> handleInputs(deltaT, m, r, fire);
	// Game Logic implementation
	// Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
	static glm::vec3 Pos = StartingPosition;
    static float pitch = 0;
    static float yaw = 0;
    static glm::mat4 currentCameraMatrix = glm::translate(I, StartingPosition + glm::vec3(0, camHeight, camDist));
    static glm::vec3 characterDirection = -1.0f * Z;
    static float characterYaw = 0;

    auto localXGround = glm::normalize(glm::vec3(currentCameraMatrix[0][0], 0, currentCameraMatrix[0][2]));
    auto localZGround = glm::normalize(glm::vec3(currentCameraMatrix[2][0], 0, currentCameraMatrix[2][2]));

    auto localDirection = (m.x == 0 && m.z == 0)? glm::vec3(0) : glm::normalize(m.x * localXGround + m.z * localZGround);
    float cosDeltaCharacterDirection = glm::dot(characterDirection, localDirection);
    cosDeltaCharacterDirection = glm::clamp(cosDeltaCharacterDirection, -1.0f, 1.0f);
    float deltaCharacterYaw = 0;
    if(cosDeltaCharacterDirection == -1.0f)
        deltaCharacterYaw = -glm::radians(180.0f);
    else{
        deltaCharacterYaw = ((float)glm::sign(glm::cross(characterDirection, localDirection).y)) * glm::acos(cosDeltaCharacterDirection);
    }
    if(isnan(deltaCharacterYaw)) {
        std::cout << "dot: " << glm::dot(characterDirection, localDirection) << "\n";
        std::cout << "acos: " << glm::acos(glm::dot(characterDirection, localDirection)) << "\n";
        std::cout << "sign: " << (float)glm::sign(glm::cross(characterDirection, localDirection).y) << "\n";
    }

    characterYaw += deltaCharacterYaw;
    Pos += deltaT * MOVE_SPEED * localDirection;

    auto cameraTarget = Pos + glm::vec3(0, camHeight, 0);
    auto cameraCenter = cameraTarget + glm::vec3(0, 0, camDist);

    float deltaPitch = r.x * deltaT * ROT_SPEED;
    float deltaYaw = r.y * deltaT * ROT_SPEED;
    pitch += deltaPitch;
    yaw += deltaYaw;
    pitch = fmax(minPitch, fmin(pitch, maxPitch));

    auto cameraCenterRotationMatrix =
            glm::translate(I, cameraTarget)
            * glm::rotate(I, yaw, Y)
            * glm::rotate(I, -pitch, X)
            * glm::translate(I, -1.0f * cameraTarget)
            ;


    cameraCenter = cameraCenterRotationMatrix * glm::vec4(cameraCenter, 1);

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(cameraCenter), cameraTarget, glm::vec3(0,1,0));

    //used for the next frame
    currentCameraMatrix = glm::inverse(viewMatrix);
    if(glm::length(localDirection) > 0)
        characterDirection = localDirection;

	ViewPrj =
            getVulkanProjectionMatrix(FOVy, Ar, nearPlane, farPlane)
            * viewMatrix
            ;
	World = glm::translate(glm::mat4(1), Pos) * glm::rotate(I, characterYaw, Y);
     */
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->logicUpdate();
    auto& cameraComponent = dynamic_cast<fmwk::Camera&>(gameEngine->getEntityByName("Camera").getComponentByName("MainCamera"));
    auto& characterTransform = dynamic_cast<fmwk::Transform&>(gameEngine->getEntityByName("Character").getComponentByName("Transform"));


    ViewPrj = cameraComponent.getProjectionMatrix() * cameraComponent.getViewMatrix();
    World = characterTransform.getWorldMatrix();

}