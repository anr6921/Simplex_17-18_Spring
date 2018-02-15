#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
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

	static float fPos = 0.0f;

	m_m4Cube = glm::translate(vector3(fPos, 0.0f, 0.0f));

	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall)*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 2.0f, 0.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, 3.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 6.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 3.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, -1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 5.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, -1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 3.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, -1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 3.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 3.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, -1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, 5.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, -1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 4.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 6.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 3.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 0.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 0.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 1.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-7.0f, 2.0f, 0.0f))*m_m4Cube);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-7.0f, 3.0f, 0.0f))*m_m4Cube);

	fPos += 0.1f;

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}