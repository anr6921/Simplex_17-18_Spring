#include "MyOctant.h"



/*MyOctant::MyOctant()
{
}*/

// CONSTRUCTORS
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	// set unsigned int values from arguments
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	m_pRoot = this; // set this to root
	m_uOctantCount++; // increase oct count by 1
	m_uID = m_uOctantCount;
	m_v3Center = m_pEntityMngr->GetEntity()->GetRigidBody()->GetCenterGlobal; // set center of tree
	m_fSize = m_pEntityMngr->GetEntity()->GetRigidBody()->GetHalfWidth().x; //get size from half width
	m_fSize = m_fSize * 2; // get full size by multiplying half width by two
	m_v3Max = m_pEntityMngr->GetEntity()->GetRigidBody()->GetHalfWidth() * 2.0f;
	m_v3Min = m_pEntityMngr->GetEntity()->GetRigidBody()->GetHalfWidth() - m_pEntityMngr->GetEntity()->GetRigidBody()->GetHalfWidth();
	
	ConstructTree(m_uMaxLevel);
	
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	// set values from arguments
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_uOctantCount++; // increase by 1
	m_v3Max = m_v3Center + (vector3(m_fSize));
	m_v3Min = m_v3Center - (vector3(m_fSize));
	

}

// COPY CONSTRUCTORS
Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	// copy values from referenced octree
	m_uID = m_uID; //Will store the current ID for this octant
	m_uLevel = m_uLevel; //Will store the current level of the octant
	m_uChildren = m_uChildren;// Number of children on the octant (either 0 or 8)

	m_fSize = 0.0f; //Size of the octant

	m_v3Center = m_v3Center; //Will store the center point of the octant
	m_v3Min = m_v3Min; //Will store the minimum vector of the octant
	m_v3Max = m_v3Max; //Will store the maximum vector of the octant

	m_pParent = other.m_pParent; // Will store the parent of current octant
	//m_pChild = other.m_pChild; //Will store the children of the current octant
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}


	m_pRoot = other.m_pRoot; //Root octant
	m_lChild = other.m_lChild; //list of nodes that contain objects (this will be applied to root only)
}

// COPY ASSIGNMENT OPERATOR
Simplex::MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	// TODO: insert return statement here
}

// DESRUCTOR
Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

// Changes object contents for other object's
void Simplex::MyOctant::Swap(MyOctant & other)
{

}

// GET ACCESSORS
float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

Simplex::vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

Simplex::vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

Simplex::vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

// CHECK FOR COLLISION
bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	
	// Get vectors from global 
	//MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);
	//MyRigidBody* rb = entity->GetRigidBody();
	vector3 otherMin = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetMinGlobal();//= rb->GetMinGlobal();
	vector3 otherMax = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetMaxGlobal();

	// x
	if (m_v3Max.x < otherMin.x || m_v3Min.x > otherMax.x)
	{
		return false;
	}

	// y
	if (m_v3Max.y < otherMin.y || m_v3Min.y > otherMax.y)
	{
		return false;
	}

	// z
	if (m_v3Min.z > otherMax.z || m_v3Max.z < otherMin.z)
	{
		return false;
	}

	// else return true
	return true;
}


// Displays the MyOctant volume specified by index including the objects underneath
void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{

}

// Displays the MyOctant volume in the color specified
void Simplex::MyOctant::Display(vector3 a_v3Color)
{
}

// Displays the non empty leafs in the octree
void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{

}


// clear entity list
void Simplex::MyOctant::ClearEntityList(void)
{
	// loop through list of children and clear individually
	for (int i = 0; i < m_uChildren; i++)
	{
		m_lChild[i]->ClearEntityList();
	}
}

// SUBDIVIDE
void Simplex::MyOctant::Subdivide(void)
{
	// check if thingy is out of kiddos
	if (m_uChildren >= 0)
	{
		return;
	}

	// check if thingy is at max lvl
	if (m_uMaxLevel <= m_uLevel)
	{
		return;
	}

	// congrats, you are now the proud parent of octuplets!
	m_uChildren = 8;

	// artificial incemination of the octuplets

	// Bottom Left Back
	// Bottom Right Back 
	// Bottom Right Front 
	// Bottom Left Front 
	// Top Left Front 
	// Top Left Back 
	// Top Right Back 
	// Top Right Front 
}


//MORE GETTERS
Simplex::MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	// return child in array
	return m_pChild[a_nChild];
}

Simplex::MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	if (m_uChildren == 0)
	{
		return true;
	}
	else{
		return false;
	}

}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	// go through children and crush thier dreams
	for (int i = 0; i < m_uChildren; i++)
	{
		if (m_pChild[i]->IsLeaf())
		{
			delete m_pChild[i];
		}
	}
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	m_uMaxLevel = a_nMaxLevel;

	if (m_uLevel != 0)
	{
		return;
	}

	// go through children and build thier dreams
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;	// grow the childs roots
		m_pChild[i]->m_pParent = this;	// new mom
		m_pChild[i]->m_uLevel++; // increment level by 1
		m_pChild[i]->ConstructTree(a_nMaxLevel); // they grow up so fast (': aka new tree for each kiddo
	}

}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
}

// MORE GETTERS?
Simplex::uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
	KillBranches(); // rip tree
}

void Simplex::MyOctant::Init(void)
{
}

void Simplex::MyOctant::ConstructList(void)
{
}
