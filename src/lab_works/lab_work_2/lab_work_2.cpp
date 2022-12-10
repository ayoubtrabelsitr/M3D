#include "lab_work_2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>
namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteShader( vertexShader );		  // Destruction VERTEX Shader
		glDeleteShader( fragmentShader );	  // Destruction FRAGMENT Shader
		glDeleteProgram( Program );			  // Destruction du programme
		glDeleteBuffers( 1, &VBO );			  // Destruction du VBO
		glDisableVertexArrayAttrib( VAO, 0 ); // Désactiver les attributs du VAO
		glDeleteVertexArrays( 1, &VAO );	  // En détruisant le VAO
		glDeleteBuffers( 1, &VBO_color );	  //// Destruction du VBO pour les couleurs
	}

	bool LabWork2::init()
	{
		// Les Shaders
		const std::string vertexShaderStr
			= readFile( _shaderFolder + "lw2.vert" ); // lire le shader  et le stocker dans vertexShaderStr
		const std::string fragmentShaderStr
			= readFile( _shaderFolder + "lw2.frag" );			// lire le shader  et le stocker dans fragmentShaderStr
		vertexShader	  = glCreateShader( GL_VERTEX_SHADER ); // Construction du vertex shader
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER ); // Construction du fragment shader
		const char * VSrc = vertexShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		const char * FSrc = fragmentShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		glShaderSource( vertexShader, 1, &VSrc, NULL );	  // Initialisation du code source pour vertex Shader
		glShaderSource( fragmentShader, 1, &FSrc, NULL ); // Initialisation du code source pour fragment Shader
		glCompileShader( vertexShader );				  // Compilation pour recupérer les erreurs de compilation
		glCompileShader( fragmentShader );				  // Compilation pour recupérer les erreurs de compilation
		std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		// Check if compilation is ok
		// vertex.---------------------------------------------------------------------------------------------

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
		// Check if compilation is ok
		// fragment.----------------------------------------------------------------------------
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
		//------------------------------------------------------------------------------------------------------------------------------------
		Program = glCreateProgram(); // Création du programme
		// Attachage shaders/programme
		glAttachShader( Program, vertexShader );
		glAttachShader( Program, fragmentShader );
		// Liaison du programme
		glLinkProgram( Program );
		// Check if link is
		// ok.-----------------------------------------------------------------------------------------------------------------------
		GLint linked;
		glGetProgramiv( Program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( Program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}
		// Initialisation------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Position_sommets = {Vec2f( -0.5, 0.5 ),Vec2f( 0.5, 0.5 ),Vec2f( 0.5, -0.5 ),Vec2f( -0.5, 0.5 ),Vec2f(0.5,
		// -0.5),Vec2f( -0.5, -0.5 )}; // initialisation le vecteur avec les coordonnees des sommets de quad composée de
		// deux triangles
		Position_sommets = {
			Vec2f( -0.5, 0.5 ), Vec2f( 0.5, 0.5 ), Vec2f( 0.5, -0.5 ), Vec2f( -0.5, -0.5 )
		}; // initialisation le vecteur avec les coordonnees des sommets de quad sans les deux sommets redondants
		indices_sommets = { 0, 1, 2, 0, 3, 2 }; // initialisation des indices des sommets formant les deux triangles
		color_sommets	= {
			  Vec3f( 1.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ), Vec3f( 0.f, 0.f, 1.f ), Vec3f( 1.f, 0.f, 1.f )
		}; // initialisation des couleurs des sommets
		//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		glCreateBuffers( 1, &VBO );		  // Création du VBO pour les position des sommets
		glCreateBuffers( 1, &VBO_color ); // Création du VBO pour les couleurs des sommets

		// Remplissage du
		// buffer-----------------------------------------------------------------------------------------------
		glNamedBufferData( VBO,
						   Position_sommets.size() * sizeof( Vec2f ),
						   Position_sommets.data(),
						   GL_STATIC_DRAW ); // Nous utiliserons un buffer statique car les donnéees des sommets ne
											 // seront jamais modifiée
		glNamedBufferData( VBO_color, color_sommets.size() * sizeof( Vec3f ), color_sommets.data(), GL_STATIC_DRAW );

		// Création du
		// VAO-------------------------------------------------------------------------------------------------------
		glCreateVertexArrays( 1, &VAO );
		glEnableVertexArrayAttrib( VAO, 0 );						   // Activez l’attribut 0 du VAO
		glVertexArrayAttribFormat( VAO, 0, 3, GL_FLOAT, GL_FALSE, 0 ); // Définissez le format de l’attribut
		glVertexArrayVertexBuffer( VAO, 0, VBO, 0, sizeof( Vec2f ) );  // Liez le VBO et le VAO
		glVertexArrayAttribBinding( VAO, 0, 0 );					   // connectez le VAO avec le Vertex shader
		// ------------------------------------------------------------------------------------------
		glEnableVertexArrayAttrib( VAO, 1 );								// Activez l’attribut 1 du VAO
		glVertexArrayVertexBuffer( VAO, 1, VBO_color, 0, sizeof( Vec3f ) ); // Liez le VBO_color et le VAO
		glVertexArrayAttribBinding( VAO, 1, 1 );							// Connectez le VAO avec le Vertex shader

		//...........................................................................................................
		// Création et remplissage d'EBO--------------------------------------------------------------------------------
		glCreateBuffers( 1, &EBO );
		glNamedBufferData( EBO, indices_sommets.size() * sizeof( int ), indices_sommets.data(), GL_STATIC_DRAW );
		//----------------------------------------------------------------------------------------------------------

		// Liez l’EBO au VAO
		glVertexArrayElementBuffer( VAO, EBO );
		loc_translation = glGetUniformLocation( Program, "uTranslationX" ); // récuperer l’adresse de uTranslationX
		// glProgramUniform1f( Program, loc_translation, 0.5 ); //essayer avec valeur aléatoire mais pas trop pour nous
		// puissions toujours voir la geométrie dans la fenetre
		loc_luminosité = glGetUniformLocation(
			Program, "c_luminosite" ); // Lier loc_luminosité avec c_luminosite du fragment shader

		////////////////////////////////////////////////////////////////////////////////////////
		glProgramUniform1f( Program, loc_luminosité, Luminosité ); // Pour la valeur initiale 1
		/////////////////////////////////////////////////////////////////////////////////////////

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::animate( const float p_deltaTime )
	{
		_time		  = _time + p_deltaTime;
		double valeur = glm::sin( _time );
		glProgramUniform1f(
			Program, loc_translation, valeur ); // donner une valeur à la variable uTranslationX en fonction de _time
	}

	void LabWork2::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Effacer le frame buffer
		glBindVertexArray( VAO );							  // Liaison entre VAO et le programme
		// glDrawArrays( GL_TRIANGLES, 0, 6);	// Lancez le pipeline avec 6 sommets pour repréesenter le quad

		glUseProgram( Program ); // Spécifier le programme à utiliser

		glDrawElements( GL_TRIANGLES,
						indices_sommets.size(),
						GL_UNSIGNED_INT,
						0 );	// Lancez le pipeline avec 4 sommets pour repréesenter le quad à l'aide d'EBO
		glBindVertexArray( 0 ); // Déliez le VAO et le programme
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		if ( ImGui::SliderFloat( "Luminosite", &Luminosité, 0.f, 1.f ) )
		{
			glProgramUniform1f( Program, loc_luminosité, Luminosité ); // modifier la valeur de variable Luminosité
		}
		if ( ImGui::ColorEdit3( "Baground color", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x,
						  _bgColor.y,
						  _bgColor.z,
						  _bgColor.w ); // créer une palette de couleurs permettant de choisir la couleur de fond
		}
		ImGui::Begin( "Settings lab work 2" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
