#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			return i;
		}
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	if (a_uIndex < 0 || a_uIndex > m_uEntityCount)
	{
		return nullptr;
	}

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			return m_entityList[i]->GetModel();
		}
	}
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	if (a_uIndex < 0 || a_uIndex > m_uEntityCount)
	{
		return nullptr;
	}
	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			return m_entityList[i]->GetRigidBody();
		}
	}
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (m_entityList.size() == 0)
	{
		return IDENTITY_M4;
	}
	if (a_uIndex < 0 || a_uIndex > m_uEntityCount)
	{
		return IDENTITY_M4;
	}

	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			return m_entityList[i]->GetModelMatrix();
		}
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			m_entityList[i]->SetModelMatrix(a_m4ToWorld);
		}
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (m_entityList.size() == 0)
	{
		return;
	}

	if (a_uIndex < 0 || a_uIndex > m_uEntityCount)
	{
		return;
	}

	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{

}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* temp = new MyEntity(a_sFileName, a_sUniqueID);

	m_entityList.push_back(temp);
	m_uEntityCount = m_entityList.size();
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (m_entityList.size() == 0)
		return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{

}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	return "";
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{

}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{

}