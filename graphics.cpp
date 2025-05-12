#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// The Sun
	m_sphere = new Sphere(64, "assets\\2k_sun.jpg");

	// The Earth
	m_sphere2 = new Sphere(48, "assets\\2k_earth_daymap.jpg");
	
	// The moon
	m_sphere3 = new Sphere(48, "assets\\2k_moon.jpg");

	//mercury
	m_mercury = new Sphere(48, "assets\\Mercury.jpg");

	//venus
	m_venus = new Sphere(48, "assets\\Venus.jpg");

	//mars
	m_mars = new Sphere(48, "assets\\Mars.jpg");

	// Gas Giants
	m_jupiter = new Sphere(64, "assets\\Jupiter.jpg");
	m_saturn = new Sphere(64, "assets\\Saturn.jpg");

	// Ice Giants
	m_uranus = new Sphere(64, "assets\\Uranus.jpg");
	m_neptune = new Sphere(64, "assets\\Neptune.jpg");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {
	modelStack = std::stack<glm::mat4>(); // Reset the model stack
	glm::mat4 identity = glm::mat4(1.0f);
	modelStack.push(identity); // Root of the scene graph

	// ==== SUN ====
	glm::mat4 sunTranslate, sunRotate, sunScale;
	// Add rotation speed of 0.1 to make sun rotate slowly
	ComputeTransforms(dt, { 0,0,0 }, { 0,0,0 }, { 0.1 }, glm::vec3(0, 1, 0), { 2,2,2 }, sunTranslate, sunRotate, sunScale);
	glm::mat4 sunModel = modelStack.top() * sunTranslate * sunRotate * sunScale;
	m_sphere->Update(sunModel);

	// ==== EARTH ====
	modelStack.push(sunModel); // Parent is sun
	glm::mat4 earthTranslate, earthRotate, earthScale;
	// Change to {8,0,8} to orbit in X-Z plane
	ComputeTransforms(dt, { 0.5,0.5,0.5 }, { 8,0,8 }, { 1 }, glm::vec3(0, 1, 0), { 1,1,1 }, earthTranslate, earthRotate, earthScale);
	glm::mat4 earthModel = modelStack.top() * earthTranslate * earthRotate * earthScale;
	m_sphere2->Update(earthModel);

	// ==== MOON ====
	modelStack.push(earthModel); // Parent is earth
	glm::mat4 moonTranslate, moonRotate, moonScale;
	// Use {2,1,2} to create a tilted orbit (Y component creates the tilt)
	ComputeTransforms(dt, { 2,2,2 }, { 2,1,2 }, { 1 }, glm::vec3(0, 1, 0), { 0.5,0.5,0.5 }, moonTranslate, moonRotate, moonScale);
	glm::mat4 moonModel = modelStack.top() * moonTranslate * moonRotate * moonScale;
	m_sphere3->Update(moonModel);

	// ==== MERCURY ====
	modelStack.push(sunModel);
	glm::mat4 mercuryTranslate, mercuryRotate, mercuryScale;
	ComputeTransforms(dt, { 1.0f }, { 10.0f }, { 0.1f }, { 0,1,0 }, { 0.8f }, mercuryTranslate, mercuryRotate, mercuryScale);
	glm::mat4 mercuryModel = modelStack.top() * mercuryTranslate * mercuryRotate * mercuryScale;
	m_mercury->Update(mercuryModel);
	
	
	// ==== VENUS ====
	modelStack.push(sunModel);
	glm::mat4 venusTranslate, venusRotate, venusScale;
	ComputeTransforms(dt, { 0.8f }, { 15.0f }, { 0.1f }, { 0,1,0 }, { 0.9f }, venusTranslate, venusRotate, venusScale);
	glm::mat4 venusModel = modelStack.top() * venusTranslate * venusRotate * venusScale;
	m_venus->Update(venusModel);


	// ==== MARS ====
	modelStack.push(sunModel);
	glm::mat4 marsTranslate, marsRotate, marsScale;
	ComputeTransforms(dt, { 0.7f }, { 20.0f }, { 0.1f }, { 0,1,0 }, { 0.7f }, marsTranslate, marsRotate, marsScale);
	glm::mat4 marsModel = modelStack.top() * marsTranslate * marsRotate * marsScale;
	m_mars->Update(marsModel);
	

	// ==== JUPITER ====
	modelStack.push(sunModel);
	glm::mat4 jupiterTranslate, jupiterRotate, jupiterScale;
	ComputeTransforms(dt, { 0.5f }, { 30.0f }, { 0.1f }, { 0,1,0 }, { 2.0f }, jupiterTranslate, jupiterRotate, jupiterScale);
	glm::mat4 jupiterModel = modelStack.top() * jupiterTranslate * jupiterRotate * jupiterScale;
	m_jupiter->Update(jupiterModel);
	

	// ==== SATURN ====
	modelStack.push(sunModel);
	glm::mat4 saturnTranslate, saturnRotate, saturnScale;
	ComputeTransforms(dt, { 0.3f }, { 40.0f }, { 0.1f }, glm::vec3(0, 1, 0), { 3.0f }, saturnTranslate, saturnRotate, saturnScale);
	glm::mat4 saturnModel = modelStack.top() * saturnTranslate * saturnRotate * saturnScale;
	m_saturn->Update(saturnModel);


	// ==== URANUS ====
	modelStack.push(sunModel);
	glm::mat4 uranusTranslate, uranusRotate, uranusScale;
	ComputeTransforms(dt, { 0.2f }, { 50.0f }, { 0.1f }, glm::vec3(0, 1, 0), { 2.4f }, uranusTranslate, uranusRotate, uranusScale);
	glm::mat4 uranusModel = modelStack.top() * uranusTranslate * uranusRotate * uranusScale;
	m_uranus->Update(uranusModel);
	

	// ==== NEPTUNE ====
	modelStack.push(sunModel);
	glm::mat4 neptuneTranslate, neptuneRotate, neptuneScale;
	ComputeTransforms(dt, { 0.1f }, { 60.0f }, { 0.1f }, glm::vec3(0, 1, 0), { 1.4f }, neptuneTranslate, neptuneRotate, neptuneScale);
	glm::mat4 neptuneModel = modelStack.top() * neptuneTranslate * neptuneRotate * neptuneScale;
	m_neptune->Update(neptuneModel);
	

	// Pop everything
	modelStack.pop(); // moon
	modelStack.pop(); // earth
	modelStack.pop(); //mercury
	modelStack.pop(); //venus
	modelStack.pop(); //mars
	modelStack.pop(); //jupyter
	modelStack.pop(); //saturn
	modelStack.pop(); //uranus
	modelStack.pop(); //neptune


	// === STARSHIP ===
	// Change to {0,5,5} to orbit in Y-Z plane
	std::vector<float> speed = { 1, 1, 1 };
	std::vector<float> dist = { 0, 5, 5 };
	std::vector<float> scale = { 0.04f, 0.04f, 0.04f };
	glm::mat4 tmat, tempRmat, smat;

	// Calculate position only
	ComputeTransforms(dt, speed, dist, { 0 }, glm::vec3(0, 1, 0), scale, tmat, tempRmat, smat);

	// Extract position from translation matrix
	glm::vec3 shipPos = glm::vec3(tmat[3]);

	// Calculate dynamic rotation to always face the sun (at origin)
	glm::vec3 sunPos = glm::vec3(0.0f);
	glm::mat4 viewMat = glm::lookAt(shipPos, sunPos, glm::vec3(0.0f, 1.0f, 0.0f));

	// Inverse of view matrix gives us orientation facing sun
	glm::mat4 orientMat = glm::inverse(viewMat);

	// Extract rotation component only
	glm::mat4 rmat = glm::mat4(glm::mat3(orientMat));

	
	rmat *= glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1, 0));

	if (m_mesh != NULL)
		m_mesh->Update(modelStack.top() * tmat * rmat * smat);
}



void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.0, 0.0, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Render the objects
	/*if (m_cube != NULL){
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}*/

	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasTexture, true);
			m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	/*if (m_pyramid != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pyramid->GetModel()));
		m_pyramid->Render(m_positionAttrib, m_colorAttrib);
	}*/

	if (m_sphere != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere->GetModel()));
		if (m_sphere->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasTexture, true);
			m_sphere->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_sphere2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere2->GetModel()));
		if (m_sphere2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasTexture, true);
			m_sphere2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	// Render Moon
	if (m_sphere3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere3->GetModel()));
		if (m_sphere3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasTexture, true);
			m_sphere3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Mercury
	if (m_mercury) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Venus (fixed incorrect m_mercury reference)
	if (m_venus) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_venus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Mars
	if (m_mars) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_mars->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Jupiter
	if (m_jupiter) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_jupiter->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Saturn
	if (m_saturn) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_saturn->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Uranus
	if (m_uranus) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_uranus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Neptune
	if (m_neptune) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
		if (m_neptune->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_neptune->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler != INVALID_UNIFORM_LOCATION) {
				glUniform1i(sampler, 0);
			}
			glUniform1i(m_hasTexture, true);
			m_neptune->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

