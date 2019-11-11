#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::octantCount = 0;
uint MyOctant::maxLevel = 3;
uint MyOctant::idealEntityCount = 5;

uint MyOctant::GetOctantCount(void) 
{ 
	return octantCount; 
}

MyOctant* MyOctant::GetParent(void) 
{ 
	return parent; 
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return children == 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	int count = 0;
	int objectCount = entityManage->GetEntityCount();
	for (int i = 0; i < objectCount; i++)
	{
		if (IsColliding(i))
		{
			count++;
		}
		if (count > a_nEntities)
		{
			return true;
		}
	}

	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	for (int i = 0; i < children; i++)
	{
		child[i]->KillBranches();
		delete child[i];
		child[i] = nullptr;
	}

	children = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	if (level != 0)
	{
		return;
	}

	maxLevel = a_nMaxLevel;
	octantCount = 1;
	entityList.clear();

	KillBranches();
	rootChild.clear();

	if (ContainsMoreThan(idealEntityCount))
	{
		Subdivide();
	}

	AssignIDtoEntity();

	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	for (int i = 0; i < children; i++)
	{
		child[i]->AssignIDtoEntity();
	}

	if (children == 0)
	{
		int entities = entityManage->GetEntityCount();
		for (int j = 0; j < entities; j++)
		{
			if (IsColliding(j))
			{
				entityList.push_back(j);
				entityManage->AddDimension(j, id);
			}
		}
	}

}

void MyOctant::Init(void)
{
	children = 0;
	size = 0.0f;
	id = octantCount;
	level = 0;
	center = vector3(0.0f);
	min = vector3(0.0f);
	max = vector3(0.0f);
	meshManage = MeshManager::GetInstance();
	entityManage = MyEntityManager::GetInstance();
	root = nullptr;
	parent = nullptr;
	for (int i = 0; i < 8; i++)
	{
		child[i] = nullptr;
	}
}

void Simplex::MyOctant::ConstructList(void)
{
	for (int i = 0; i < children; i++)
	{
		child[i]->ConstructList();
	}

	if (entityList.size() > 0)
	{
		root->rootChild.push_back(this);
	}
}

void MyOctant::Swap(MyOctant& other)
{
	std::swap(children, other.children);
	std::swap(size, other.size);
	std::swap(id, other.id);
	std::swap(level, other.level);
	std::swap(center, other.center);
	std::swap(min, other.min);
	std::swap(max, other.max);
	std::swap(root, other.root);
	std::swap(parent, other.parent);

	meshManage = MeshManager::GetInstance();
	entityManage = MyEntityManager::GetInstance();

	for (int i = 0; i < 8; i++)
	{
		std::swap(child[i], other.child[i]);
	}
}

void MyOctant::Release(void)
{
	if (level == 0)
	{
		KillBranches();
	}

	children = 0;
	size = 0.0f;
	entityList.clear();
	rootChild.clear();
}

MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();
	octantCount = 0;
	maxLevel = a_nMaxLevel;
	idealEntityCount = a_nIdealEntityCount;
	id = octantCount;

	root = this;
	rootChild.clear();

	std::vector<vector3> boundMinMax;
	uint objects = entityManage->GetEntityCount();
	for (int i = 0; i < objects; i++)
	{
		MyEntity* tempEntity = entityManage->GetEntity(i);
		MyRigidBody* tempBody = tempEntity->GetRigidBody();
		boundMinMax.push_back(tempBody->GetMinGlobal());
		boundMinMax.push_back(tempBody->GetMaxGlobal());
	}

	MyRigidBody* tempBody = new MyRigidBody(boundMinMax);

	vector3 halfWidth = tempBody->GetHalfWidth();
	float floatMax = halfWidth.x;
	for (int i = 1; i < 3; i++)
	{
		if (floatMax < halfWidth[i])
		{
			floatMax = halfWidth[i];
		}
	}

	vector3 bodyCenter = tempBody->GetCenterLocal();
	boundMinMax.clear();
	SafeDelete(tempBody);

	size = floatMax * 2.0f;
	center = bodyCenter;
	min = center - (vector3(floatMax));
	max = center + (vector3(floatMax));

	octantCount++;
	ConstructTree(maxLevel);
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();
	center = a_v3Center;
	size = a_fSize;
	min = center - (vector3(size) / 2.0f);
	max = center + (vector3(size) / 2.0f);
}

MyOctant::MyOctant(MyOctant const& other)
{
	children = other.children;
	center = other.center;
	min = other.min;
	max = other.max;
	size = other.size;
	id = other.id;
	level = other.level;
	parent = other.parent;
	root = other.root;
	rootChild = other.rootChild;
	meshManage = MeshManager::GetInstance();
	entityManage = MyEntityManager::GetInstance();

	for (int i = 0; i < 8; i++)
	{
		child[i] = other.child[i];
	}
}

MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temporary(other);
		Swap(temporary);
	}

	return *this;
}

MyOctant::~MyOctant()
{
	Release();
}

float MyOctant::GetSize() 
{ 
	return size; 
}

vector3 MyOctant::GetCenterGlobal(void)
{
	return center;
}

vector3 MyOctant::GetMinGlobal(void)
{
	return min;
}

vector3 MyOctant::GetMaxGlobal(void)
{
	return max;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	int objectCount = entityManage->GetEntityCount();
	if (a_uRBIndex >= objectCount)
	{
		return false;
	}

	MyEntity* temp = entityManage->GetEntity(a_uRBIndex);
	MyRigidBody* tempBody = temp->GetRigidBody();
	vector3 tempMin = tempBody->GetMinGlobal();
	vector3 tempMax = tempBody->GetMaxGlobal();


	if (max.x < tempMin.x)
	{
		return false;
	}

	if (max.x > tempMax.x)
	{
		return false;
	}

	if (max.y < tempMin.y)
	{
		return false;
	}

	if (max.y > tempMax.y)
	{
		return false;
	}

	if (max.z < tempMin.z)
	{
		return false;
	}

	if (max.z > tempMax.z)
	{
		return false;
	}

	return true;
}

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	if (id == a_nIndex)
	{
		meshManage->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(size)), a_v3Color, RENDER_WIRE);
		return;
	}
	for (int i = 0; i < children; i++)
	{
		child[i]->Display(a_nIndex);
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	for (int i = 0; i < children; i++)
	{
		child[i]->Display(a_v3Color);
	}
	meshManage->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(size)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	int leaves = rootChild.size();
	for (int i = 0; i < leaves; i++)
	{
		rootChild[i]->DisplayLeafs(a_v3Color);
	}
	meshManage->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(size)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList(void)
{
	for (int i = 0; i < children; i++)
	{
		child[i]->ClearEntityList();
	}

	entityList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	if (level >= maxLevel)
	{
		return;
	}

	if (children != 0)
	{
		return;
	}

	children = 8;

	float floatSize = size / 4.0f;
	float floatSize2 = floatSize * 2.0f;
	vector3 v3Center = center;

	v3Center.x -= floatSize;
	v3Center.y -= floatSize;
	v3Center.z -= floatSize;

	child[0] = new MyOctant(v3Center, floatSize2);

	v3Center.x += floatSize2;
	child[1] = new MyOctant(v3Center, floatSize2);

	v3Center.z += floatSize2;
	child[2] = new MyOctant(v3Center, floatSize2);

	v3Center.x += floatSize2;
	child[3] = new MyOctant(v3Center, floatSize2);

	v3Center.y += floatSize2;
	child[4] = new MyOctant(v3Center, floatSize2);

	v3Center.z += floatSize2;
	child[5] = new MyOctant(v3Center, floatSize2);

	v3Center.x += floatSize2;
	child[6] = new MyOctant(v3Center, floatSize2);

	v3Center.z += floatSize2;
	child[7] = new MyOctant(v3Center, floatSize2);


	for (int i = 0; i < 8; i++)
	{
		child[i]->root = root;
		child[i]->parent = this;
		child[i]->level = level + 1;
		if (child[i]->ContainsMoreThan(idealEntityCount))
		{
			child[i]->Subdivide();
		}
	}

}

MyOctant* Simplex::MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild >= 8)
	{
		return nullptr;
	}
	return child[a_nChild];
}



