#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_WHITE);

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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = -20.0f;
	static float value2 = 0.0f;
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 46; i++)
	{

		matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value + i*2, value2, -20.0f));
		matrix4 m4Model = m4Scale * m4Translate;
		m_pMesh->Render(m4Projection, m4View, m4Model);

	}
	
	value += 0.01f;
	value2 += 0.001f;
	

	
	
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
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}