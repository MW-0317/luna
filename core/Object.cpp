#include "Object.h"

#include <thirdparty/glad/glad.h>

#include "Camera.h"
#include "Space.h"
#include "Engine.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace luna
{
	int Texture::loadImage(const char* path)
	{
		this->image = new Image(path, true);
		return 0;
	}

	int Texture::loadCubeMap(const char* path, int index)
	{
		loadImage(path);
		if (image->width != image->height)
		{
			int newSize = std::max(image->width, image->height);
			newSize--;
			newSize |= newSize >> 1;
			newSize |= newSize >> 2;
			newSize |= newSize >> 4;
			newSize |= newSize >> 8;
			newSize |= newSize >> 16;
			newSize |= newSize >> 32;
			newSize++;
			image->resize(newSize, newSize);
		}
;		this->index = index;

		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		for (int i = 0; i < 6; i++)
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, 
				image->format, 
				image->width, 
				image->height, 
				0, 
				image->format, 
				GL_UNSIGNED_BYTE, 
				image->data
			);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return id;
	}

	int Texture::loadTexture(const char* path, int index)
	{
		loadImage(path);
		this->index = index;

		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			image->format, 
			image->width, 
			image->height, 
			0, 
			image->format, 
			GL_UNSIGNED_BYTE, 
			image->data
		);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		return id;
	}

	Texture Texture::getDefault()
	{
		return Texture("defaultTexture", "resources/textures/default.png");
	}

	void Texture::bind()
	{
		glActiveTexture(GL_TEXTURE0 + this->index);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}

	MaxSizeVector<Texture*, 16>
		Texture::generateFromPaths(std::vector<const char*> paths,
			std::vector<const char*> names)
	{

		// Todo try and make Texture a pointer so that bind can be called if
		// the texture is a cubemap
		MaxSizeVector<Texture*, 16> textures;
		for (int i = 0; i < paths.size(); i++)
		{
			// Setting CM_ to be a prefix for a cube map
			if (names[i][0] != '\0' && names[i][0] == 'C' &&
				names[i][1] != '\0' && names[i][1] == 'M' &&
				names[i][2] != '\0' && names[i][2] == '_')
				textures.push_back(new CubeMapTexture(names[i], paths[i], i));
			else
				textures.push_back(new Texture(names[i], paths[i], i));
		}

		return textures;
	}

	void CubeMapTexture::bind()
	{
		glActiveTexture(GL_TEXTURE0 + this->index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}

	void CubeMapTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE0);
	}

	void Line::init(std::vector<PrimitiveVertex> points, std::vector<unsigned int> indices)
	{
		this->basicShader = Shader("shaders/line.glsl");
		this->points = points;
		this->indices = indices;

		float* flatarray = getFlatArray();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,
			6 * sizeof(float) * points.size(), flatarray, GL_STATIC_DRAW);

		if (!indices.empty())
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	Line::Line(std::vector<PrimitiveVertex> points)
	{
		init(points, {});
	}

	Line::Line(std::vector<glm::vec3> positions)
	{
		std::vector<PrimitiveVertex> points;
		for (int i = 0; i < positions.size(); i++)
		{
			PrimitiveVertex line;
			line.color = glm::vec3(0.1f, 0.2f, 0.3f);
			line.position = positions[i];
			points.push_back(line);
		}
		init(points, {});
	}

	Line::Line(std::vector<glm::vec3> positions, std::vector<unsigned int> indices)
	{
		std::vector<PrimitiveVertex> points;
		for (int i = 0; i < positions.size(); i++)
		{
			PrimitiveVertex line;
			line.color = glm::vec3(0.1f, 0.2f, 0.3f);
			line.position = positions[i];
			points.push_back(line);
		}
		init(points, indices);
	}

	// TODO __
	void Line::draw(Frame frame, Shader shader)
	{
		shader.use();
		shader.setFloat("time", glfwGetTime());
		shader.setFloat("deltatime", frame.deltatime);

		shader.setInt("width", frame.width);
		shader.setInt("height", frame.height);

		//renderProps.engine->log.log("%s", );
		frame.model = glm::mat4(1.0f);
		//std::cout << glm::to_string(renderProps.model) << std::endl;
		shader.setMat4("model", frame.model);
		shader.setMat4("view", frame.view);
		shader.setMat4("projection", frame.proj);
		
		glBindVertexArray(VAO);
		if (indices.empty())
			glDrawArrays(GL_LINES, 0, points.size());
		else
			glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		//shader.disable();
		glBindVertexArray(0);
		shader.disable();
	}

	void Mesh::init(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		MaxSizeVector<Texture*, 16> textures)
	{
		this->vertices = vertices;
		this->textures = textures;
		float* flatArray = getFlatArray();
		this->center = getCenter();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,
			VERTEX_SIZE * sizeof(float) * vertices.size(), flatArray, GL_STATIC_DRAW);


		if (indices.empty())
			this->indices = triangulateMesh(vertices);
		else
			this->indices = indices;

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * this->indices.size(), &this->indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
			(void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Texture Coordinates
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
			(void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
	}

	Mesh::Mesh()
	{}

	Mesh::Mesh(const char* objPath)
	{
		std::vector<Vertex> vertices = loadObjMesh(objPath).vertices;
		std::vector<unsigned int> indices;
		int size = vertices.size() / 3;
		for (unsigned int i = 0; i < size; i++)
		{
			indices.push_back(i);
		}
		init(vertices, indices, MaxSizeVector<Texture*, 16>());
	}

	Mesh::Mesh(std::vector<float> vertices)
	{
		std::vector<Vertex> newVertices = floatToVertex(vertices);
		init(newVertices, std::vector<unsigned int>(), MaxSizeVector<Texture*, 16>());
	}

	Mesh::Mesh(std::vector<Vertex> vertices)
	{
		init(vertices, std::vector<unsigned int>(), MaxSizeVector<Texture*, 16>());
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		init(vertices, indices, MaxSizeVector<Texture*, 16>());
	}

	Mesh::Mesh(std::vector<Vertex> vertices, MaxSizeVector<Texture*, 16>	textures)
	{
		init(vertices, std::vector<unsigned int>(), textures);
	}

	void Mesh::setTextures(MaxSizeVector<Texture*, 16> textures, float mix)
	{
		this->textures = textures;
		this->mix = mix;
	}

	void Mesh::draw(Frame frame, Shader shader)
	{
		shader.use();
		shader.setFloat("time", glfwGetTime());
		shader.setFloat("deltatime", frame.deltatime);
		
		shader.setInt("width", frame.width);
		shader.setInt("height", frame.height);

		shader.setVec3("camera.position", frame.camera->getPosition());
		
		frame.model = glm::translate(frame.model, -center);
		shader.setMat4("model", frame.model);
		shader.setMat4("view", frame.view);
		shader.setMat4("projection", frame.proj);

		shader.setFloat("mixAmount", this->mix);
		glBindVertexArray(VAO);
		for (int i = 0; i < textures.size(); i++)
		{
			textures[i]->bind();
			shader.setInt(textures[i]->name, i);
		}
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader.disable();
	}

	float* Mesh::getFlatArray()
	{
		if (vertices.size() <= 0)
			return nullptr;
		return &vertices[0].position.x;
	}

	glm::vec3 Mesh::getCenter()
	{
		int vSize = vertices.size();
		glm::vec3 max = glm::vec3(-INFINITY);
		glm::vec3 min = glm::vec3(INFINITY);
		for (int i = 0; i < vSize; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (vertices[i].position[j] > max[j])
					max[j] = vertices[i].position[j];
				if (vertices[i].position[j] < min[j])
					min[j] = vertices[i].position[j];
			}
		}

		glm::vec3 center;
		for (int i = 0; i < 3; i++)
		{
			center[i] = (max[i] + min[i]) / 2;
		}
		return center;
	}

	std::vector<Vertex> Mesh::floatToVertex(std::vector<float> vertices)
	{
		std::vector<Vertex> newVertices;
		int rows = vertices.size() / VERTEX_SIZE;
		for (int i = 0; i < rows; i++)
		{
			Vertex v;
			float* floatArray = &v.position.x;
			for (int j = 0; j < VERTEX_SIZE; j++)
			{
				floatArray[j] = vertices[i * VERTEX_SIZE + j];
			}
			newVertices.push_back(v);
		}
		return newVertices;
	}

	std::vector<Vertex> Mesh::createSquareArray()
	{
		std::vector<float> vertices = {
		// Position            | Color              | Normal             | Texture Coordinates
			-0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		};

		return floatToVertex(vertices);
	}

	Mesh Mesh::createSquare()
	{
		std::vector<Vertex> vertices = createSquareArray();
		return Mesh(vertices);
	}

	Mesh Mesh::loadObjMesh(const char* filename)
	{
		std::vector<Vertex>			vertices;

		std::vector<glm::vec3>		vList;
		std::vector<glm::vec2>		vtList;
		std::vector<glm::vec3>		vnList;

		std::string line;
		std::ifstream meshFile(filename);

		if (!meshFile.is_open())
		{
			std::cout << "loadObjMesh\n\t^Could not open file" << std::endl;
			return Mesh();
		}

		while (std::getline(meshFile, line))
		{
			if (line[0] == '\0' || line[0] == '#') continue;
			std::vector<std::string> lineData;
			size_t i = 0;
			size_t next = line.find(' ');
			while (next != std::string::npos)
			{	
				std::string sub = line.substr(i, next - i);
				if (sub != "" && sub != " ")
					lineData.push_back(sub);
				i = next + 1;
				next = line.find(' ', i);
			}
			std::string sub = line.substr(i, line.size() - i);
			if (sub != "" && sub != " ")
				lineData.push_back(line.substr(i, line.size() - i));

			// if v, vn, vt, f...

			if (lineData.size() < 1)
				continue;

			if (lineData[0] == "v")
			{
				glm::vec3 v = glm::vec3(1.0f);
				for (int j = 1; j < lineData.size(); j++)
				{
					v[j - 1] = std::stof(lineData[j]);
				}
				vList.push_back(v);
			}

			else if (lineData[0] == "vt")
			{
				glm::vec3 vt = glm::vec3(0.0f);
				for (int j = 1; j < lineData.size(); j++)
				{
					vt[j - 1] = std::stof(lineData[j]);
				}
				vtList.push_back(vt);
			}

			else if (lineData[0] == "vn")
			{
				glm::vec3 vn = glm::vec3(1.0f);
				for (int j = 1; j < lineData.size(); j++)
				{
					vn[j - 1] = std::stof(lineData[j]);
				}
				vnList.push_back(vn);
			}

			else if (lineData[0] == "f")
			{
				std::vector< std::vector<std::string>> verticesArgs;
				for (int k = 1; k < lineData.size(); k++)
				{
					std::vector<std::string> vertexArgs;
					size_t i = 0;
					size_t next = lineData[k].find('/');
					while (next != std::string::npos)
					{
						vertexArgs.push_back(lineData[k].substr(i, next - i));
						i = next + 1;
						next = lineData[k].find('/', i);
					}
					vertexArgs.push_back(lineData[k].substr(i, lineData[k].size() - i));

					verticesArgs.push_back(vertexArgs);
				}

				for (int l = 0; l < verticesArgs.size() - 2; l++)
				{
					Vertex v;
					std::vector<std::string> vertexArgs = verticesArgs[0];
					if (vertexArgs[0] != "")
						v.position = vList[std::stoi(vertexArgs[0]) - 1];
					if (vertexArgs[1] != "")
						v.texcoords = vtList[std::stoi(vertexArgs[1]) - 1];
					if (vertexArgs[2] != "")
						v.normal = vnList[std::stoi(vertexArgs[2]) - 1];
					vertices.push_back(v);
					for (int m = 1; m < 3; m++)
					{
						Vertex v;
						std::vector<std::string> vertexArgs = verticesArgs[l + m];
						if (vertexArgs[0] != "")
							v.position = vList[std::stoi(vertexArgs[0]) - 1];
						if (vertexArgs[1] != "")
							v.texcoords = vtList[std::stoi(vertexArgs[1]) - 1];
						if (vertexArgs[2] != "")
							v.normal = vnList[std::stoi(vertexArgs[2]) - 1];
						vertices.push_back(v);
					}
				}
			}
		}
		meshFile.close();

		std::vector<unsigned int> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);
		return Mesh(vertices, indices);
	}

	// This is a mess!
	std::vector<unsigned int> Mesh::triangulateMesh(std::vector<Vertex> vertices)
	{
		struct Triangle
		{
			glm::vec3 indices;

			glm::vec3 getPosition(int index, std::vector<Vertex> vers)
			{
				float inf = 10.0f;
				if (index == -3)
				{
					return glm::vec3(inf, -inf, 0);
				}
				else if (index == -2)
				{
					return glm::vec3(0, inf, 0);
				}
				else if (index == -1)
				{
					return glm::vec3(-inf, -inf, 0);
				}
				return vers[index].position;
			}
		};

		std::vector<Triangle> triangulation = {};
		Triangle superTriangle;
		superTriangle.indices = glm::vec3(-1.0f, -2.0f, -3.0f);
		triangulation.push_back(superTriangle);
		for (int i = 0; i < vertices.size(); i++)
		{
			Vertex ver = vertices[i];

			std::vector<Triangle> badTriangles = {};
			for (Triangle tri : triangulation)
			{
				glm::vec3 A = tri.getPosition(tri.indices[0], vertices);
				glm::vec3 B = tri.getPosition(tri.indices[1], vertices);
				glm::vec3 C = tri.getPosition(tri.indices[2], vertices);
				glm::vec3 D = ver.position;
				float circumcircleDeterminantArray[] = {
					A.x, A.y, A.x * A.x + A.y * A.y, 1.0f,
					B.x, B.y, B.x * B.x + B.y * B.y, 1.0f,
					C.x, C.y, C.x * C.x + C.y * C.y, 1.0f,
					D.x, D.y, D.x * D.x + D.y * D.y, 1.0f
				};
				glm::mat4 circumcircleDeterminant = glm::make_mat4(circumcircleDeterminantArray);
				float determinant = glm::determinant(circumcircleDeterminant);
				if (determinant < 0)
					badTriangles.push_back(tri);
			}

			std::vector<glm::vec2> polygon;
			for (int t1 = 0; t1 < badTriangles.size(); t1++)
			{
				Triangle tri = badTriangles[t1];
				for (int j = 0; j < 3; j++)
				{
					bool isShared = false;
					int start1 = j;
					int end1 = (j + 1) % 3;
					glm::vec2 currentEdge = glm::vec2(tri.indices[start1], tri.indices[end1]);
					for (int t2 = 0; t2 < badTriangles.size(); t2++)
					{
						if (t1 == t2)
							continue;
						Triangle aTri = badTriangles[t2];
						for (int k = 0; k < 3; k++)
						{
							int start2 = k;
							int end2 = (k + 1) % 3;
							if ((tri.indices[start1] == aTri.indices[start2] && tri.indices[end1] == aTri.indices[end2]) ||
								(tri.indices[start1] == aTri.indices[end2] && aTri.indices[start2] == tri.indices[end1]))
							{
								isShared = true;
							}
						}
					}
					if (!isShared)
						polygon.push_back(currentEdge);
				}
			}
			for (Triangle tri : badTriangles)
			{
				int end = triangulation.size();
				for (int j = 0; j < end; j++)
				{
					if (tri.indices == triangulation[j].indices)
					{
						triangulation.erase(triangulation.begin() + j);
						end--;
					}
				}
			}
			for (glm::vec2 edge : polygon)
			{
				Triangle newTri;
				newTri.indices = glm::vec3(edge[0], edge[1], i);
				triangulation.push_back(newTri);
			}

		}

		int end = triangulation.size();
		for (int i = 0; i < end; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (triangulation[i].indices[j] < 0.0f)
				{
					triangulation.erase(triangulation.begin() + i);
					i--;
					end--;
					break;
				}
			}
		}

		std::vector<unsigned int> triangulated;
		for (int i = 0; i < triangulation.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				triangulated.push_back(triangulation[i].indices[j]);
			}
		}
		return triangulated;
	}

	Model::Model(Shader shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		this->shader = shader;
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;

		model = glm::mat4(1.0f);
		model = glm::scale(model, scale);
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	Model::~Model() {}

	glm::mat4 Model::getModelMatrix()
	{
		model = glm::mat4(1.0f);
		model = glm::scale(model, scale);
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		return model;
	}

	void Model::setRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
	}

	void Model::setRotation(float xRot, float yRot, float zRot)
	{
		this->rotation = glm::vec3(xRot, yRot, zRot);
	}

	void Model::setPosition(glm::vec3 position)
	{
		this->position = position;
	}

	void Model::setPosition(float x, float y, float z)
	{
		this->position = glm::vec3(x, y, z);
	}

	void Model::draw(Frame frame) { }

	Shader* Model::getShader()
	{
		return &shader;
	}

	Object::Object(Mesh mesh, Shader shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
		: Model(shader, position, scale, rotation)
	{
		this->mesh = mesh;
	}

	Object::~Object()
	{
	}

	void Object::frameUpdate(Frame frame)
	{
		this->draw(frame);
		this->frameSystems(frame);
	}

	void Object::tickUpdate(Tick tick)
	{
		this->tickSystems(tick);
	}

	void Object::draw(Frame frame)
	{
		frame.model = getModelMatrix();
		mesh.draw(frame, shader);
	}

	Object Object::createSquare()
	{
		Mesh mesh = Mesh::createSquare();
		Shader shader = Shader::getDefaultShader();

		return Object(mesh, shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
	}

	Object Object::createCube()
	{
		Mesh mesh = Mesh::loadObjMesh("resources/objects/b_cube.obj");
		Shader shader = Shader::getDefaultShader();

		return Object(mesh, shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
	}

	Object Object::createSphere()
	{
		Mesh mesh = Mesh::loadObjMesh("resources/objects/sphere.obj");
		Shader shader = Shader::getDefaultShader();

		return Object(mesh, shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
	}

	void Object::setTexture(std::string filename)
	{
		std::vector<const char*> paths = {
			filename.c_str(),
		};
		std::vector<const char*> names = {
			"defaultTexture",
		};
		this->mesh.setTextures(Texture::generateFromPaths(paths, names), 0.0f);
	}

	void Object::setCubeMap(std::string filename)
	{
		std::vector<const char*> paths = {
			filename.c_str(),
		};
		std::vector<const char*> names = {
			"CM_defaultTexture",
		};
		this->mesh.setTextures(Texture::generateFromPaths(paths, names), 1.0f);
	}

	void Sprite::draw(Frame frame)
	{

	}
}