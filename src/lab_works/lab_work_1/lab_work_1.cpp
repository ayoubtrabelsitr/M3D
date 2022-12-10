#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1()
	{
		glDeleteShader( vertexShader );		  // Destruction VERTEX Shader
		glDeleteShader( fragmentShader );	  // Destruction FRAGMENT Shader
		glDeleteProgram( Program );			  // Destruction du programme
		glDeleteBuffers( 1, &VBO );			  // Destruction du VBO
		glDisableVertexArrayAttrib( VAO, 0 ); // Désactiver les attributs du VAO
		glDeleteVertexArrays( 1, &VAO );	  // En détruisant le VAO
	}

	bool LabWork1::init()
	{
		// Les Shaders
		const std::string vertexShaderStr
			= readFile( _shaderFolder + "lw1.vert" ); // lire le shader  et le stocker dans vertexShaderStr
		const std::string fragmentShaderStr
			= readFile( _shaderFolder + "lw1.frag" );			// lire le shader  et le stocker dans fragmentShaderStr
		vertexShader	  = glCreateShader( GL_VERTEX_SHADER ); // Construction du vertex shader
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER ); // Construction du fragment shader
		const char * VSrc = vertexShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		const char * FSrc = fragmentShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		glShaderSource( vertexShader, 1, &VSrc, NULL );	  // Initialisation du code source pour vertex Shader
		glShaderSource( fragmentShader, 1, &FSrc, NULL ); // Initialisation du code source pour fragment Shader
		glCompileShader( vertexShader );				  // Compilation pour recupérer les erreurs de compilation
		glCompileShader( fragmentShader );				  // Compilation pour recupérer les erreurs de compilation
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		// Check if compilation is ok vertex.

		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}
		// Check if compilation is ok fragment.
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}
		Program = glCreateProgram(); // Création du programme
		// Attachage shaders/programme
		glAttachShader( Program, vertexShader );
		glAttachShader( Program, fragmentShader );
		// Liaison du programme
		glLinkProgram( Program );
		// Check if link is ok.
		GLint linked;
		glGetProgramiv( Program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( Program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}
		Position_sommets = {
			Vec2f( -0.5, 0.5 ), Vec2f( 0.5, 0.5 ), Vec2f( 0.5, -0.5 )
		};							// initialisation le vecteur avec les coordonnees des sommets de triangle
		glCreateBuffers( 1, &VBO ); // Création du VBO
		// Remplissage du buffer
		glNamedBufferData( VBO,
						   Position_sommets.size() * sizeof( Vec2f ),
						   Position_sommets.data(),
						   GL_STATIC_DRAW ); // Nous utiliserons un buffer statique car les donnéees des sommets ne
											 // seront jamais modifiées
		// Création du VAO
		glCreateVertexArrays( 1, &VAO );
		glEnableVertexArrayAttrib( VAO, 0 );						   // Activez l’attribut 0 du VAO
		glVertexArrayAttribFormat( VAO, 0, 2, GL_FLOAT, GL_FALSE, 0 ); // Définissez le format de l’attribut
		glVertexArrayVertexBuffer( VAO, 0, VBO, 0, sizeof( Vec2f ) );  // Liez le VBO et le VAO
		glVertexArrayAttribBinding( VAO, 0, 0 );					   // connectez le VAO avec le Vertex shader

		glUseProgram( Program ); // Spécifier le programme à utiliser

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Effacer le frame buffer
		glBindVertexArray( VAO );							  // Liaison entre VAO et le programme
		glDrawArrays( GL_TRIANGLES, 0, 3 );					  // Lancez le pipeline
		glBindVertexArray( 0 );								  // Déliez le VAO et le programme
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
