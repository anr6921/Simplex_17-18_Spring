#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------
	//vector3 v3Value = a_v3Dimensions * 0.5f;
	float pi = 3.14159f;
	vector3 origin = vector3(0, 0, 0);
	vector3 top = vector3(0,0,a_fHeight);

	for (int i = 0; i <= a_nSubdivisions; ++i)
	{
		//generate first tri vector
		float angle = 2 * pi*((float)i / (float)a_nSubdivisions);
		float s = sin(angle);
		float c = cos(angle);
		vector3 side1(c, s, 0.0f);
		side1 = side1*a_fRadius;

		//generate second tri vector
		float angle2 = 2 * pi*((float)(i + 1) / (float)a_nSubdivisions);
		float s2 = sin(angle2);
		float c2 = cos(angle2);
		vector3 side2(c2, s2, 0.0f);
		side2 = side2*a_fRadius;

		//bottom
		AddTri(side2, side1, origin);

		//AddVertexPosition(side2);
		//AddVertexPosition(side1);
		//AddVertexPosition(origin);

		//add side tri
		AddTri(side1, side2, top);
	}
	
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------
	float pi = 3.14159f;
	vector3 origin = vector3(0, 0, 0);
	vector3 top=vector3(0,0,a_fHeight);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//generate first bottom tri vector
		float angle = 2 * pi*((float)i / (float)a_nSubdivisions);
		float s = sin(angle);
		float c = cos(angle);
		vector3 side1(c, s, 0.0f);
		side1 = side1*a_fRadius;
		
		//generate second bottom tri vector
		float angle2 = 2 * pi*((float)(i+1) / (float)a_nSubdivisions);
		float s2 = sin(angle2);
		float c2 = cos(angle2);
		vector3 side2(c2, s2, 0.0f);
		side2 = side2*a_fRadius;

		//bottom 
		//AddVertexPosition(side2);
		//AddVertexPosition(side1);
		//AddVertexPosition(origin);
		AddTri(side2, side1, origin);

		//top
		vector3 top1(c*a_fRadius, s*a_fRadius, a_fHeight);
		vector3 top2(c2*a_fRadius, s2*a_fRadius, a_fHeight);
		//AddVertexPosition(top1);
		//AddVertexPosition(top2);
		//AddVertexPosition(top);
		AddTri(top1, top2, top);

		//sides
		AddQuad(top2, top1, side2, side1);
		
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------
	float pi = 3.14159f;
	vector3 origin = vector3(0, 0, 0);
	vector3 top = vector3(0, 0, a_fHeight);
	float a_fRadius = a_fOuterRadius; //set outerRadius to a_fRadius to reuse cylinder code

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//generate first bottom outter vector
		float angle = 2 * pi*((float)i / (float)a_nSubdivisions);
		float s = sin(angle);
		float c = cos(angle);
		vector3 side1(c, s, 0.0f);
		side1 = side1*a_fRadius;

		//generate second bottom outter vector
		float angle2 = 2 * pi*((float)(i + 1) / (float)a_nSubdivisions);
		float s2 = sin(angle2);
		float c2 = cos(angle2);
		vector3 side2(c2, s2, 0.0f);
		side2 = side2*a_fRadius;

		//first bottom inner vector
		float s3 = sin(angle);
		float c3 = cos(angle);
		vector3 side3(c, s, 0.0f);
		side3 = side3*a_fInnerRadius;

		//generate second bottom inner vector
		float s4 = sin(angle2);
		float c4 = cos(angle2);
		vector3 side4(c2, s2, 0.0f);
		side4 = side4*a_fInnerRadius;

		//bottom 
		AddQuad(side3, side4, side1, side2);

		//top
		vector3 top1(c*a_fRadius, s*a_fRadius, a_fHeight);
		vector3 top2(c2*a_fRadius, s2*a_fRadius, a_fHeight);
		vector3 top3(c*a_fInnerRadius, s*a_fInnerRadius, a_fHeight);
		vector3 top4(c2*a_fInnerRadius, s2*a_fInnerRadius, a_fHeight);
		AddQuad(top1, top2, top3, top4);

		//outter sides
		AddQuad(top2, top1, side2, side1);

		//inner sides
		AddQuad(top3, top4, side3, side4);

	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// -------------------------------
	float pi = 3.14159f;
	vector3 origin = vector3(0, 0, 0);
	float a_fHeight = a_fRadius;
	vector3 top = vector3(0, 0, a_fHeight); 
	float radius = a_fRadius / 2;

	for (int i = 0; i <= a_nSubdivisions; ++i)
	{
		/* SPHERE
		//get number of rows
		float rows = a_fRadius / a_nSubdivisions;

		//generate first bottom tri vector
		float angle = 2 * pi*((float)i / (float)a_nSubdivisions);
		float s = sin(angle);
		float c = cos(angle);
		vector3 side1(c, s, 0.0f);
		side1 = side1*a_fRadius;

		//generate second bottom tri vector
		float angle2 = 2 * pi*((float)(i + 1) / (float)a_nSubdivisions);
		float s2 = sin(angle2);
		float c2 = cos(angle2);
		vector3 side2(c2, s2, 0.0f);
		side2 = side2*a_fRadius;

		//bottom 
		//AddVertexPosition(side2);
		//AddVertexPosition(side1);
		//AddVertexPosition(origin);
		AddTri(side2, side1, origin);

		//top
		vector3 top1(c*a_fRadius, s*a_fRadius, a_fHeight);
		vector3 top2(c2*a_fRadius, s2*a_fRadius, a_fHeight);
		//AddVertexPosition(top1);
		//AddVertexPosition(top2);
		//AddVertexPosition(top);
		AddTri(top1, top2, top);

		//sides
		AddQuad(top2, top1, side2, side1);*/

		//DODECAHEDRON
		//generate first bottom tri vector
		float angle = 2 * pi*((float)i / (float)a_nSubdivisions);
		float s = sin(angle);
		float c = cos(angle);
		vector3 side1(c, s, 0.0f);
		side1 = side1*a_fRadius;

		//generate second bottom tri vector
		float angle2 = 2 * pi*((float)(i + 1) / (float)a_nSubdivisions);
		float s2 = sin(angle2);
		float c2 = cos(angle2);
		vector3 side2(c2, s2, 0.0f);
		side2 = side2*a_fRadius;

		// get vector between side1 and side2
		vector3 mid1 = (side2 + side1)/2.0f;
		mid1 = mid1 + mid1*radius;
		
		//vector between side1 and top
		vector3 mid2 = (top + side1)/2.0f;
		mid2 = mid2 + mid2*radius;
		
		//vec between side2 and top
		vector3 mid3 = (top + side2)/2.0f;
		mid3 = mid3 + mid3*radius;

		//top center tri
		AddTri(mid3, mid2, mid1);
		//top top tri
		AddTri(top, mid2, mid3);
		//top left tri
		AddTri(mid3, mid1, side2);
		//top right tri
		AddTri(mid2, side1, mid1);

		//vector between side1 and -top
		vector3 mid4 = (-top + side1) / 2.0f;
		mid4 = mid4 + mid4*radius;

		//vec between side2 and -top
		vector3 mid5 = (-top + side2) / 2.0f;
		mid5 = mid5 + mid5*radius;

		//bottom center tri
		AddTri(mid1, mid4, mid5);
		//bottom top tri
		AddTri(mid5, mid4, -top);
		//bottom left tri
		AddTri(side2, mid1, mid5);
		//bottom right tri
		AddTri(mid1, side1, mid4);
		
	}


	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}