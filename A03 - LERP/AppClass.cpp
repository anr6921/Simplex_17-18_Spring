#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Anna Rosenberg - anr6921@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager


		// MY CODE
		// generate vector for each LERP stop
		std::vector<vector3> shape;

		// loop to create each vector and add to shape vector
		for (int j = 0; j < i; j++)
		{
			//calculate angle
			float angle = (2 *PI)*j/i;
			//calculate x and y position of vector
			float x = fSize * cos(angle);
			float y = fSize * sin(angle);

			// push back vector with x, y, and 0 for z value
			shape.push_back(vector3(x, y, 0.0f));
		}

		// add zero to array keeping track of number of vertices
		vertices.push_back(0);

		// add shape vector to path vector
		path.push_back(shape);

		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	// set up timer
	static float timer = 0;
	static int clock = m_pSystem->GenClock();
	timer += m_pSystem->GetDeltaTime(clock); // add change in time for timer

	// set up frame rate
	float frame = MapValue(timer, 0.0f, 0.2f, 0.0f, 1.0f);

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos;

		//start point
		//std::vector<vector3> m_start = path[i];
		vector3 start = path[i][vertices[i]];

		// end point
		//std::vector<vector3> m_end = path[i];
		vector3 end = path[i][(vertices[i] + 1) % path[i].size()];

		// current position
		v3CurrentPos = glm::lerp(start, end, frame);

		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.2)), C_WHITE);

	}
	// end of path
	if (frame >= 1.0f)
	{
		for (uint j = 0; j <vertices.size(); j++)
		{
			// increment current vertex being calculated
			vertices[j]++;
			
			if (path[j].size() <= vertices[j])
			{
				vertices[j] = vertices[j]%path[j].size();
			}

		}
		//reset timer
		timer = m_pSystem->GetDeltaTime(clock);
	}

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}