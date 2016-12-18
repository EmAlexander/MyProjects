#include "QOpenGl.h"

bool quebic::gl::initGl()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glewExperimental = GL_TRUE;

	return true;
}


quebic::gl::shader::shader()
{
}

quebic::gl::shader::~shader()
{
	free();
}

void quebic::gl::internals::printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void quebic::gl::internals::printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

bool quebic::gl::shader::setUpFromStr(const GLchar* vertexStr[], const GLchar* fragmentStr[], bool _hasVertexColor, bool _is3d)
{
	shaderId = glCreateProgram();

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(_vertexShader, 1, vertexStr, NULL);

	glCompileShader(_vertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		std::cout << "Unable to compile vertex shader " << _vertexShader << std::endl;
		quebic::gl::internals::printShaderLog(_vertexShader);
		return false;
	}
	else
	{
		glAttachShader(shaderId, _vertexShader);

		GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(_fragmentShader, 1, fragmentStr, NULL);

		glCompileShader(_fragmentShader);

		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			std::cout << "Unable to compile fragment shader " << _fragmentShader << std::endl;
			quebic::gl::internals::printShaderLog(_fragmentShader);
			return false;
		}
		else
		{
			glAttachShader(shaderId, _fragmentShader);

			glLinkProgram(shaderId);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(shaderId, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				std::cout << "Error linking program " << shaderId << std::endl;
				quebic::gl::internals::printProgramLog(shaderId);
				return false;
			}
			else
			{
				gVertexPosLocation = glGetAttribLocation(shaderId, "LVertexPos");
				if (gVertexPosLocation == -1)
				{
					std::cout << "LVertexPos is not a valid glsl program variable!" << std::endl;
					return false;
				}
				else
				{
					is3d = _is3d;
					hasVertexColor = _hasVertexColor;

					if (hasVertexColor)
					{
						gAttributeColor = glGetAttribLocation(shaderId, "v_color");
						if (gAttributeColor == -1)
						{
							std::cout << "Could not bind attribute " << gAttributeColor << std::endl;
							return false;
						}
					}

					glDetachShader(shaderId, _vertexShader);
					glDeleteShader(_vertexShader);
					glDetachShader(shaderId, _fragmentShader);
					glDeleteShader(_fragmentShader);

					return true;
				}
			}
		}
	}
}

void quebic::gl::shader::free()
{
	glUseProgram(NULL);
	glDeleteProgram(shaderId);
}

GLint quebic::gl::shader::getVertexLoc()
{
	return gVertexPosLocation;
}

GLuint quebic::gl::shader::getSId()
{
	return shaderId;
}

bool quebic::gl::shader::getIs3d()
{
	return is3d;
}

bool quebic::gl::shader::setUp(bool _is3d, bool vertxColor)
{
	/*
	if (hasVertexColor)
	{
		gAttributeColor = glGetAttribLocation(shaderId, "v_color");
		if (gAttributeColor == -1)
		{
			std::cout << "Could not bind attribute " << gAttributeColor << std::endl;
			return false;
		}
	}
	*/

	std::string vetexStr;
	std::string fragmentStr;

	//vertex shader setup
	vetexStr += "#version 440\n";
	
	if (_is3d)
		vetexStr += "in vec3 LVertexPos;";
	else
		vetexStr += "in vec2 LVertexPos;";

	if (vertxColor)
		vetexStr += "attribute vec3 v_color;varying vec3 f_color;";

	vetexStr += " void main() { gl_Position = vec4( LVertexPos.x, LVertexPos.y, ";

	if (_is3d)
		vetexStr += "LVertexPos.z, 1 ); ";
	else
		vetexStr += "0, 1 ); ";

	if (vertxColor)
		vetexStr += "f_color = v_color;}";
	else
		vetexStr += "}";

	//fragment shader setup
	fragmentStr += "#version 440\n";
	if (vertxColor)
		fragmentStr += "varying vec3 f_color;";
	fragmentStr += "out vec4 LFragment; void main() { ";
	if (vertxColor)
		fragmentStr += "LFragment = vec4( f_color.x, f_color.y, f_color.z, 1.0 ); }";
	else
		fragmentStr += "LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }";

	const GLchar* vertexShaderStr[] = { vetexStr.c_str() };
	const GLchar* fragmentShaderStr[] = { fragmentStr.c_str() };

	return setUpFromStr(vertexShaderStr, fragmentShaderStr, vertxColor, _is3d);
}

GLint quebic::gl::shader::getColorLoc()
{
	return gAttributeColor;
}

bool quebic::gl::shader::getVertexColor()
{
	return hasVertexColor;
}

void quebic::gl::p2d::createCirclePolys(std::vector<GLfloat> * vboArray, std::vector<GLfloat> * iboArray, int num_segments, float cX, float cY, float raidus, bool soild)
{
	if (soild)
	{
		vboArray->push_back(cX);
		vboArray->push_back(cY);
	}

	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * M_PI * float(ii) / float(num_segments);//get the current angle 

		float x = raidus * cosf(theta);//calculate the x component 
		float y = raidus * sinf(theta);//calculate the y component 

		vboArray->push_back(x + cX);
		vboArray->push_back(y + cY);

		if (soild)
		{
			iboArray->push_back(0);
			iboArray->push_back(1 + ii);
			if (!(ii + 2 >= num_segments + 1))
				iboArray->push_back(2 + ii);
			else
				iboArray->push_back(1);
		}
		else
		{
			iboArray->push_back(ii);
		}
	}
}

void quebic::gl::p2d::createTorusPolys(std::vector<GLfloat> * vboArray, std::vector<GLfloat> * iboArray, int num_segments, float cX, float cY, float innerR, float outterR)
{
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * M_PI * float(ii) / float(num_segments);//get the current angle 

		float x = outterR * cosf(theta);//calculate the x component 
		float y = outterR * sinf(theta);//calculate the y component 

		vboArray->push_back(x + cX);
		vboArray->push_back(y + cY);

		float _theta = 2.0f * M_PI * float(ii) / float(num_segments);//get the current angle 

		float _x = innerR * cosf(_theta);//calculate the x component 
		float _y = innerR * sinf(_theta);//calculate the y component 

		vboArray->push_back(_x + cX);
		vboArray->push_back(_y + cY);

		iboArray->push_back(ii);
		iboArray->push_back(ii + 1);
		iboArray->push_back(ii + 2);

		iboArray->push_back(num_segments + ii);
		if (!(num_segments + ii + 1 >= num_segments * 2))
			iboArray->push_back(num_segments + ii + 1);
		else
			iboArray->push_back(1);
		if (!(num_segments + ii + 2 >= num_segments * 2))
			iboArray->push_back(num_segments + ii + 2);
		else
			iboArray->push_back(0);
	}
}

bool quebic::gl::p3d::loadOBJ(const char * path, std::vector < GLfloat > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals, std::vector < GLfloat > & ibo, bool hasUvs)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		return false;
	}
	while (true)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

			out_vertices.push_back(vertex.x);
			out_vertices.push_back(vertex.y);
			out_vertices.push_back(vertex.z);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			if (!hasUvs)
			{
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				if (matches != 6){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				ibo.push_back(vertexIndex[0] - 1);
				ibo.push_back(vertexIndex[1] - 1);
				ibo.push_back(vertexIndex[2] - 1);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else
			{
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				ibo.push_back(vertexIndex[0] - 1);
				ibo.push_back(vertexIndex[1] - 1);
				ibo.push_back(vertexIndex[2] - 1);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}
		/*for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			out_vertices.push_back(vertex);
		}*/
	}

	return true;
}

quebic::gl::poly::poly(){}

quebic::gl::poly::~poly()
{
	free();
}

void quebic::gl::poly::free()
{
	if (_vbo != 0)
	{
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}
	if (_ibo != 0)
	{
		glDeleteBuffers(1, &_ibo);
		_ibo = 0;
	}
}

void quebic::gl::poly::setVertexes(std::vector<GLfloat> vboVec, std::vector<GLfloat> iboVec)
{
	GLuint *iboAr = new GLuint[iboVec.size()];
	for (unsigned int i = 0; i < iboVec.size(); i++)
	{
		iboAr[i] = iboVec.at(i);
	}

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vboVec.size() * sizeof(GLfloat), &vboVec[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboVec.size() * sizeof(GLuint), iboAr, GL_STATIC_DRAW);

	delete iboAr;
}

void quebic::gl::poly::startRender(shader* _shader)
{
	glUseProgram(_shader->getSId());

	glEnableVertexAttribArray(_shader->getVertexLoc());
}

void quebic::gl::poly::render(shader* _shader, GLenum renderMode, bool dontUseIbo, bool D3d)
{
	if (!dontUseIbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		if (!D3d)
			glVertexAttribPointer(_shader->getVertexLoc(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
		else
			glVertexAttribPointer(_shader->getVertexLoc(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glDrawElements(renderMode, size / sizeof(GLushort), GL_UNSIGNED_INT, NULL);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		if (!D3d)
			glVertexAttribPointer(_shader->getVertexLoc(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
		else
			glVertexAttribPointer(_shader->getVertexLoc(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

		int size;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glDrawArrays(GL_TRIANGLE_FAN, 0, size / sizeof(GLfloat));
	}
}

void quebic::gl::poly::endRender(shader* _shader)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(_shader->getVertexLoc());

	glUseProgram(NULL);
}

int quebic::gl::poly::getVboLoc()
{
	return _vbo;
}

int quebic::gl::poly::getIboLoc()
{
	return _ibo;
}

quebic::gl::asset::asset(){};

quebic::gl::asset::~asset()
{
	_free();
};

void quebic::gl::asset::_free()
{
	assetPolygon.free();
	assetShader.free();
	texture._free();
}

enum arrayTypes
{
	AT_NULL = 0,
	AT_VBO = 1,
	AT_IBO = 2,
	AT_TEXCO = 3,
	AT_TEXSRC = 4
};

bool quebic::gl::asset::loadFromFile(std::string filePath)
{
	std::fstream file;
	string line;
	int linenum = 1;
	bool startArrayInput = false;
	int currentArrayIn = 0;
	string arrays[4];
	bool pxIsInF = false;
	string texLoc = "";

	file.open(filePath.c_str(), ios::in | ios::binary);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			vector<string> splited = quebic::stringS::split(line, ' ');
			if (splited.size() >= 2)
			{
				if (splited.at(0) != "#")
				{
					//var parse
					if (splited.at(0) == "%")
					{
						if (splited.at(1) == "is3d")
						{
							if (splited.at(2) == "0\r" || splited.at(2) == "0" || splited.at(2) == "false\r" || splited.at(2) == "false")
							{
								is3d = false;
							}
							else if (splited.at(2) == "1\r" || splited.at(2) == "1" || splited.at(2) == "true\r" || splited.at(2) == "true")
							{
								is3d = true;
							}
							else
							{
								cout << "is3d take a bool value!" << endl;
							}
						}
						else if (splited.at(1) == "hasIboArray")
						{
							if (splited.at(2) == "0\r" || splited.at(2) == "0" || splited.at(2) == "false\r" || splited.at(2) == "false")
							{
								hasIbo = false;
							}
							else if (splited.at(2) == "1\r" || splited.at(2) == "1" || splited.at(2) == "true\r" || splited.at(2) == "true")
							{
								hasIbo = true;
							}
							else
							{
								cout << "LN: " << linenum << " ERROR: " << "hasIboArray takes a bool value!" << endl;
							}
						}
						else if (splited.at(1) == "txtdiment")
						{
							if (splited.size() >= 4)
							{
								texDiment.x = atoi(splited.at(2).c_str());
								texDiment.y = atoi(splited.at(3).c_str());
							}
							else
							{
								cout << "LN: " << linenum << " ERROR: " << "txtdiment takes two pramiters!" << endl;
							}
						}
						else if (splited.at(1) == "textureSrcIsInFile")
						{
							if (splited.at(2) == "0\r" || splited.at(2) == "0" || splited.at(2) == "false\r" || splited.at(2) == "false")
							{
								pxIsInF = false;
							}
							else if (splited.at(2) == "1\r" || splited.at(2) == "1" || splited.at(2) == "true\r" || splited.at(2) == "true")
							{
								pxIsInF = true;
							}
							else
							{
								cout << "LN: " << linenum << " ERROR: " << "textureSrcIsInFile takes a bool value!" << endl;
							}
						}
						else if (splited.at(1) == "texturePath")
						{
							if (!pxIsInF)
							{
								/*texLoc = splited.at(2);
								texLoc.erase(texLoc.end() - 1);*/

								vector<string> linePToQ = quebic::stringS::split(line, '\"');

								if (linePToQ.size() == 3)
								{
									texLoc = linePToQ.at(1);
								}
								else
								{
									cout << "LN: " << linenum << " ERROR: " << "texturePath pramiter is never closed or defined!" << endl;
								}
							}
							else
							{
								cout << "LN: " << linenum << " WARNING: " << "textureSrcIsInFile need to be false for this var to work!" << endl;
							}
						}
						else if (splited.at(1) == "renderType")
						{
							if (splited.size() >= 3)
							{
								string toUpperCase = quebic::stringS::stringToUpper(splited.at(2));

								if (toUpperCase == "GL_TRIANGLES\r" || toUpperCase == "GL_TRIANGLES")
								{
									renderType = GL_TRIANGLES;
								}
								else if (toUpperCase == "GL_TRIANGLE_FAN\r" || toUpperCase == "GL_TRIANGLE_FAN")
								{
									renderType = GL_TRIANGLE_FAN;
								}
								else if (toUpperCase == "GL_TRIANGLE_STRIP\r" || toUpperCase == "GL_TRIANGLE_STRIP")
								{
									renderType = GL_TRIANGLE_STRIP;
								}
								else if (toUpperCase == "GL_QUADS\r" || toUpperCase == "GL_QUADS")
								{
									renderType = GL_QUADS;
								}
								else if (toUpperCase == "GL_QUAD_STRIP\r" || toUpperCase == "GL_QUAD_STRIP")
								{
									renderType = GL_QUAD_STRIP;
								}
								else if (toUpperCase == "GL_LINES\r" || toUpperCase == "GL_LINES")
								{
									renderType = GL_LINES;
								}
								else if (toUpperCase == "GL_LINE_LOOP\r" || toUpperCase == "GL_LINE_LOOP")
								{
									renderType = GL_LINE_LOOP;
								}
								else if (toUpperCase == "GL_POINTS\r" || toUpperCase == "GL_POINTS")
								{
									renderType = GL_POINTS;
								}
								else
								{
									cout << "LN: " << linenum << " ERROR: " << "renderType " << splited.at(2) << " does not exist!" << endl;
								}
							}
							else
							{
								cout << "LN: " << linenum << " ERROR: " << "renderType takes one pramiters!" << endl;
							}
						}
						else
						{
							string varName = splited.at(1);
							if (!varName.empty() && varName[varName.size() - 1] == '\r')
								varName.erase(varName.size() - 1);

							cout << "LN: " << linenum << " ERROR: " << "Bad var name! \"" << varName << "\"" << endl;
						}
					}
					//array parce
					else if (splited.at(0) == "@")
					{
						if (splited.at(1) == "vbo\r" || splited.at(1) == "vbo")
						{
							startArrayInput = true;
							currentArrayIn = AT_VBO;
						}
						else if (splited.at(1) == "ibo\r" || splited.at(1) == "ibo")
						{
							startArrayInput = true;
							currentArrayIn = AT_IBO;
						}
						else if (splited.at(1) == "texCords\r" || splited.at(1) == "texCords")
						{
							startArrayInput = true;
							currentArrayIn = AT_TEXCO;
						}
						else if (splited.at(1) == "texSrc\r" || splited.at(1) == "texSrc")
						{
							if (pxIsInF)
							{
								startArrayInput = true;
								currentArrayIn = AT_TEXSRC;
							}
							else
							{
								cout << "LN: " << linenum << " ERROR: " << "textureSrcIsInFile need to be true for texSrc to work!" << endl;
							}
						}
						else if (splited.at(1) == "end\r" || splited.at(1) == "end")
						{
							if (startArrayInput == false)
							{
								cout << "LN: " << linenum << " ERROR: " << "Can not end array!" << endl;
							}
							else
							{
								startArrayInput = false;
								currentArrayIn = AT_NULL;
							}
						}
						else
						{
							string arrayName = splited.at(1);
							if (!arrayName.empty() && arrayName[arrayName.size() - 1] == '\r')
								arrayName.erase(arrayName.size() - 1);

							cout << "LN: " << linenum << " ERROR: " << "Bad array name! \"" << arrayName << "\"" << endl;
						}
					}
					//array input parce
					else if (startArrayInput)
					{
						arrays[currentArrayIn - 1] += line;
					}
				}
			}
			linenum++;
		}
	}
	else
	{
		cout << "Can not open the asset file!" << endl;
		return false;
	}
	file.close();

	vector<GLfloat> vboParced;
	vector<GLfloat> iboParced;

	vector<string> vboSplited = quebic::stringS::split(arrays[0], ' ');
	for (unsigned int i = 0; i < vboSplited.size(); i++)
	{
		if (vboSplited.at(i) != "\r")
		{
			vboParced.push_back(atof(vboSplited.at(i).c_str()));
		}
	}

	vector<string> ibosplited = quebic::stringS::split(arrays[1], ' ');
	if (hasIbo)
	{
		for (unsigned int i = 0; i < ibosplited.size(); i++)
		{
			iboParced.push_back(atof(ibosplited.at(i).c_str()));
		}
	}
	else
	{
		iboParced.push_back(0);
	}

	string verTexStr = "";
	string frgTexStr = "";

	verTexStr += "#version 440\r\n";
	if (is3d)
		verTexStr += "in vec3 LVertexPos;uniform mat4 mvp;attribute vec2 texcoord;varying vec2 f_texcoord;void main(){ gl_Position = mvp * vec4(LVertexPos.x, LVertexPos.y, LVertexPos.z, 1); f_texcoord = texcoord;}";
	else
		verTexStr += "in vec2 LVertexPos;uniform mat4 mvp;attribute vec2 texcoord;varying vec2 f_texcoord;void main(){gl_Position = mvp * vec4( LVertexPos.x, LVertexPos.y, 0, 1 ); f_texcoord = texcoord;}";

	frgTexStr += "#version 440\r\nout vec4 LFragment;varying vec2 f_texcoord;uniform sampler2D mytexture;void main(){ LFragment = texture2D(mytexture, f_texcoord); }";

	const GLchar* vertexS[] = { verTexStr.c_str() };
	const GLchar* fragmentS[] = { frgTexStr.c_str() };

	assetShader.setUpFromStr(vertexS, fragmentS, false, is3d);
	assetPolygon.setVertexes(vboParced, iboParced);

	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(assetShader.getSId(), uniform_name);
	if (uniform_mvp == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	vector<GLfloat> texcoord;

	vector<string> texSplited = quebic::stringS::split(arrays[2], ' ');
	for (unsigned int i = 0; i < texSplited.size(); i++)
	{
		if (texSplited.at(i) != "\r")
		{
			texcoord.push_back(atof(texSplited.at(i).c_str()));
		}
	}

	if (!pxIsInF)
	{
		texture.loadFromFile(texLoc, &assetShader, texcoord);
	}
	else
	{
		vector<string> pxSplited = quebic::stringS::split(arrays[AT_TEXSRC - 1], ' ');

		Uint32* px = new Uint32[pxSplited.size()];

		for (unsigned int i = 0; i < pxSplited.size(); i++)
		{
			if (pxSplited.at(i) != "\r")
			{
				px[i] = atof(pxSplited.at(i).c_str());
			}
		}

		texture.loadFromPx(px, &assetShader, texcoord, texDiment.x, texDiment.y);
	}

	return true;
}

void quebic::gl::asset::setWorldVars(glm::mat4 _model, glm::mat4 _camera, glm::mat4 _projection)
{
	model = _model;
	view = _camera;
	projection = _projection;
}

void quebic::gl::asset::render(glm::mat4 tranlate)
{
	assetPolygon.startRender(&assetShader);

	glm::mat4 mvp;
	if (is3d)
		mvp = projection * view * model * tranlate;
	else
		mvp = tranlate;

	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	texture.bindTex();

	assetPolygon.render(&assetShader, renderType, !hasIbo, is3d);

	texture.unBindTex();

	assetPolygon.endRender(&assetShader);
}

quebic::gl::QGlTexture::QGlTexture()
{

}

quebic::gl::QGlTexture::~QGlTexture()
{
	_free();
}

GLuint quebic::gl::QGlTexture::getTextureId()
{
	return textureId;
}

bool quebic::gl::QGlTexture::loadFromFile(string path, shader* _shader, vector<GLfloat> texcoords)
{
	GLuint retval;
	SDL_Surface *sdlimage;
	void *raw;
	int w, h, i, j, bpp;
	Uint8 *srcPixel, *dstPixel;
	Uint32 truePixel;
	GLenum errorCode;

	sdlimage = IMG_Load(path.c_str());

	if (!sdlimage)
	{
		printf("SDL_Image load error: %s\n", IMG_GetError());
		return false;
	}
	if (sdlimage->format->BytesPerPixel < 2)
	{
		printf("Bad image -- not true color!\n");
		SDL_FreeSurface(sdlimage);
		return false;
	}

	w = sdlimage->w;
	h = sdlimage->h;

	raw = (void *)malloc(w * h * 4);
	dstPixel = (Uint8 *)raw;

	SDL_LockSurface(sdlimage);

	bpp = sdlimage->format->BytesPerPixel;

	for (i = h - 1; i >= 0; i--)
	{
		for (j = 0; j < w; j++)
		{
			srcPixel = (Uint8 *)sdlimage->pixels + i * sdlimage->pitch + j * bpp;
			switch (bpp) {
			case 1:
				truePixel = *srcPixel;
				break;

			case 2:
				truePixel = *(Uint16 *)srcPixel;
				break;

			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					truePixel = srcPixel[0] << 16 | srcPixel[1] << 8 | srcPixel[2];
				}
				else
				{
					truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16;
				}
				break;

			case 4:
				truePixel = *(Uint32 *)srcPixel;
				break;

			default:
				printf("Image bpp of %d unusable\n", bpp);
				SDL_UnlockSurface(sdlimage);
				SDL_FreeSurface(sdlimage);
				free(raw);
				return false;
			}

			SDL_GetRGBA(truePixel, sdlimage->format, &(dstPixel[0]), &(dstPixel[1]), &(dstPixel[2]), &(dstPixel[3]));
			dstPixel++;
			dstPixel++;
			dstPixel++;
			dstPixel++;
		}
	}

	SDL_UnlockSurface(sdlimage);
	SDL_FreeSurface(sdlimage);

	while (glGetError()) { ; }

	glGenTextures(1, &retval);
	glBindTexture(GL_TEXTURE_2D, retval);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	errorCode = glGetError();
	if (errorCode != 0)
	{
		if (errorCode == GL_OUT_OF_MEMORY)
		{
			printf("Out of texture memory!\n");
		}

		glDeleteTextures(1, &retval);
		free(raw);
		return false;
	}

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (Uint8 *)raw);

	errorCode = glGetError();
	if (errorCode != 0)
	{
		if (errorCode == GL_OUT_OF_MEMORY)
		{
			printf("Out of texture memory!\n");
		}

		glDeleteTextures(1, &retval);
		free(raw);
		return false;
	}

	textureId = retval;

	/*GLfloat _texcoords[] = {
		// front
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};*/

	glGenBuffers(1, &vbo_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GLfloat), &texcoords[0], GL_STATIC_DRAW);

	attribute_texcoord = glGetAttribLocation(_shader->getSId(), "texcoord");
	if (attribute_texcoord == -1)
	{
		cout << "Could not bind attribute texcoord" << endl;
		return false;
	}

	return true;
}

bool quebic::gl::QGlTexture::loadFromSdlSurface(SDL_Surface* suf)
{
	GLuint retval;
	SDL_Surface *sdlimage;
	void *raw;
	int w, h, i, j, bpp;
	Uint8 *srcPixel, *dstPixel;
	Uint32 truePixel;
	GLenum errorCode;

	sdlimage = suf;

	if (!sdlimage)
	{
		printf("SDL_Image load error: %s\n", IMG_GetError());
		return false;
	}
	if (sdlimage->format->BytesPerPixel < 2)
	{
		printf("Bad image -- not true color!\n");
		SDL_FreeSurface(sdlimage);
		return false;
	}

	w = sdlimage->w;
	h = sdlimage->h;

	raw = (void *)malloc(w * h * 4);
	dstPixel = (Uint8 *)raw;

	SDL_LockSurface(sdlimage);

	bpp = sdlimage->format->BytesPerPixel;

	for (i = h - 1; i >= 0; i--)
	{
		for (j = 0; j < w; j++)
		{
			srcPixel = (Uint8 *)sdlimage->pixels + i * sdlimage->pitch + j * bpp;
			switch (bpp) {
			case 1:
				truePixel = *srcPixel;
				break;

			case 2:
				truePixel = *(Uint16 *)srcPixel;
				break;

			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					truePixel = srcPixel[0] << 16 | srcPixel[1] << 8 | srcPixel[2];
				}
				else
				{
					truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16;
				}
				break;

			case 4:
				truePixel = *(Uint32 *)srcPixel;
				break;

			default:
				printf("Image bpp of %d unusable\n", bpp);
				SDL_UnlockSurface(sdlimage);
				SDL_FreeSurface(sdlimage);
				free(raw);
				return false;
			}

			SDL_GetRGBA(truePixel, sdlimage->format, &(dstPixel[0]), &(dstPixel[1]), &(dstPixel[2]), &(dstPixel[3]));
			dstPixel++;
			dstPixel++;
			dstPixel++;
			dstPixel++;
		}
	}

	SDL_UnlockSurface(sdlimage);
	SDL_FreeSurface(sdlimage);

	while (glGetError()) { ; }

	glGenTextures(1, &retval);
	glBindTexture(GL_TEXTURE_2D, retval);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	errorCode = glGetError();
	if (errorCode != 0)
	{
		if (errorCode == GL_OUT_OF_MEMORY)
		{
			printf("Out of texture memory!\n");
		}

		glDeleteTextures(1, &retval);
		free(raw);
		return false;
	}

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (Uint8 *)raw);

	errorCode = glGetError();
	if (errorCode != 0)
	{
		if (errorCode == GL_OUT_OF_MEMORY)
		{
			printf("Out of texture memory!\n");
		}

		glDeleteTextures(1, &retval);
		free(raw);
		return false;
	}

	textureId = retval;

	return true;
}

bool quebic::gl::QGlTexture::loadFromPx(Uint32 *pixels, shader* _shader, vector<GLfloat> texcoords, int _w, int _h)
{
	GLuint retval;
	SDL_Surface *sdlimage;
	void *raw;
	int w, h, i, j, bpp;
	Uint8 *srcPixel, *dstPixel;
	Uint32 truePixel;
	GLenum errorCode;

	sdlimage = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);

	Uint32 *_pixels = (Uint32 *)sdlimage->pixels;

	_pixels = pixels;

	if (!sdlimage)
	{
		printf("SDL_Image load error: %s\n", IMG_GetError());
		return false;
	}
	if (sdlimage->format->BytesPerPixel < 2)
	{
		printf("Bad image -- not true color!\n");
		SDL_FreeSurface(sdlimage);
		return false;
	}

	w = sdlimage->w;
	h = sdlimage->h;

	raw = (void *)malloc(w * h * 4);
	dstPixel = (Uint8 *)raw;

	SDL_LockSurface(sdlimage);

	bpp = sdlimage->format->BytesPerPixel;

	for (i = h - 1; i >= 0; i--)
	{
		for (j = 0; j < w; j++)
		{
			srcPixel = (Uint8 *)sdlimage->pixels + i * sdlimage->pitch + j * bpp;
			switch (bpp) {
			case 1:
				truePixel = *srcPixel;
				break;

			case 2:
				truePixel = *(Uint16 *)srcPixel;
				break;

			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					truePixel = srcPixel[0] << 16 | srcPixel[1] << 8 | srcPixel[2];
				}
				else
				{
					truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16;
				}
				break;

			case 4:
				truePixel = *(Uint32 *)srcPixel;
				break;

			default:
				printf("Image bpp of %d unusable\n", bpp);
				SDL_UnlockSurface(sdlimage);
				SDL_FreeSurface(sdlimage);
				free(raw);
				return false;
			}

			SDL_GetRGBA(truePixel, sdlimage->format, &(dstPixel[0]), &(dstPixel[1]), &(dstPixel[2]), &(dstPixel[3]));
			dstPixel++;
			dstPixel++;
			dstPixel++;
			dstPixel++;
		}
	}

	SDL_UnlockSurface(sdlimage);
	SDL_FreeSurface(sdlimage);

	while (glGetError()) { ; }

	glGenTextures(1, &retval);
	glBindTexture(GL_TEXTURE_2D, retval);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	errorCode = glGetError();
	if (errorCode != 0)
	{
		if (errorCode == GL_OUT_OF_MEMORY)
		{
			printf("Out of texture memory!\n");
		}

		glDeleteTextures(1, &retval);
		free(raw);
		return false;
	}

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (Uint8 *)raw);

	errorCode = glGetError();
	if (errorCode != 0)
	{
		if (errorCode == GL_OUT_OF_MEMORY)
		{
			printf("Out of texture memory!\n");
		}

		glDeleteTextures(1, &retval);
		free(raw);
		return false;
	}

	textureId = retval;

	glGenBuffers(1, &vbo_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GLfloat), &texcoords[0], GL_STATIC_DRAW);

	attribute_texcoord = glGetAttribLocation(_shader->getSId(), "texcoord");
	if (attribute_texcoord == -1)
	{
		cout << "Could not bind attribute texcoord" << endl;
		return false;
	}

	return true;
}

void quebic::gl::QGlTexture::bindTex()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(uniform_texture, /*GL_TEXTURE*/0);

	glEnableVertexAttribArray(attribute_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
	glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void quebic::gl::QGlTexture::unBindTex()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(attribute_texcoord);
}

void quebic::gl::QGlTexture::_free()
{
	glDeleteTextures(1, &textureId);
}