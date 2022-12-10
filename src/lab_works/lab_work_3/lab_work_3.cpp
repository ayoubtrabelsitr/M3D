#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_3.hpp"
#include "utils/read_file.hpp"

#include <iostream>
#include"utils/random.hpp"//bib pour getrandomvec3f
namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3()
	{
		glDeleteShader( vertexShader );		  // Destruction VERTEX Shader
		glDeleteShader( fragmentShader );	  // Destruction FRAGMENT Shader
		glDeleteProgram( Program );			  // Destruction du programme
		glDeleteBuffers( 1, &_cube.VBO_s );		  // Destruction du VBO
		glDisableVertexArrayAttrib( _cube.VAO, 0 ); // Désactiver les attributs du VAO
		glDisableVertexArrayAttrib( _cube.VAO, 1 ); // Désactiver les attributs du VAO
		glDeleteVertexArrays( 1, &_cube.VAO );		// En détruisant le VAO
		glDeleteBuffers( 1, &_cube.VBO_c );			//// Destruction du VBO pour les couleurs
	}

	bool LabWork3::init()
	{
		// Les Shaders
		const std::string vertexShaderStr
			= readFile( _shaderFolder + "lw3.vert" ); // lire le shader  et le stocker dans vertexShaderStr
		const std::string fragmentShaderStr
			= readFile( _shaderFolder + "lw3.frag" );			// lire le shader  et le stocker dans fragmentShaderStr
		vertexShader	  = glCreateShader( GL_VERTEX_SHADER ); // Construction du vertex shader
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER ); // Construction du fragment shader
		const char * VSrc = vertexShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		const char * FSrc = fragmentShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		glShaderSource( vertexShader, 1, &VSrc, NULL );	  // Initialisation du code source pour vertex Shader
		glShaderSource( fragmentShader, 1, &FSrc, NULL ); // Initialisation du code source pour fragment Shader
		glCompileShader( vertexShader );				  // Compilation pour recupérer les erreurs de compilation
		glCompileShader( fragmentShader );				  // Compilation pour recupérer les erreurs de compilation
		std::cout << "Initializing lab work 3..." << std::endl;
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
		_cube = _createCube();
		
		_initBuffers();   
		_initCamera(); //appel de la fct pour l'initialisation du camera  
		//_updateViewMatrix(); // appel de la fct
		

		//_updateProjectionMatrix(); // appel de la fct
		//location_ModelMatrix = glGetUniformLocation(Program, "Modelmatrix" ); // Récupérez l’adresse de modelmatrix et le stocker dans location_ModelMatrix
		//location_ViewMatrix = glGetUniformLocation(Program, "Viewmatrix" ); // Récupérez l’adresse de ViewMatrix et le stocker dans location_ViewMatrix
		//location_ProjectionMatrix=glGetUniformLocation( Program, "Projectionmatrix" ); // Récupérez l’adresse de ProjectionMatrix et le stocker dans location_ProjectionMatrix
		location_MVP = glGetUniformLocation(Program, "MVP" ); // Récupérez l’adresse de modelmatrix et le stocker dans location_MVP
		
		
		//glProgramUniformMatrix4fv(Program,location_ModelMatrix,1,GL_FALSE,glm::value_ptr( _cube.matrice ) );  

		
		// glProgramUniformMatrix4fv(Program, location_ViewMatrix, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
		//glProgramUniformMatrix4fv(Program, location_ProjectionMatrix, 1, GL_FALSE, glm::value_ptr( camera.getProjectionMatrix() ) );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork3::animate( const float p_deltaTime ) 
	{ 
		// rotation d’angle p_deltaTime radians autour de l’axe( 0, 1, 1 )
		_cube.matrice = glm::rotate( _cube.matrice, glm::radians( p_deltaTime ), glm::vec3( 0.0f, 1.0f, 1.0f ) );
		
		
		glProgramUniformMatrix4fv( Program, location_MVP, 1, GL_FALSE, glm::value_ptr( MVPMatrix ) );//Mettre à jour la variable uniforme MVP dans location_MVP  
		
	
	}

	void LabWork3::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Effacer le frame buffer
		glEnable( GL_DEPTH_TEST ); //activer le test de profondeur 
		glBindVertexArray( _cube.VAO );							  // Liaison entre VAO et le programme

		glUseProgram( Program ); // Spécifier le programme à utiliser
		 MVPMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * _cube.matrice;

		glProgramUniformMatrix4fv( Program, location_MVP, 1, GL_FALSE, glm::value_ptr( MVPMatrix ) ); 
	
		glDrawElements( GL_TRIANGLES,_cube.indices_sommets.size(),GL_UNSIGNED_INT,0 );	// Lancez le pipeline
		glBindVertexArray( 0 ); // Déliez le VAO et le programme
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );
		if ( ImGui::ColorEdit3( "Baground color", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x,
						  _bgColor.y,
						  _bgColor.z,
						  _bgColor.w ); // créer une palette de couleurs permettant de choisir la couleur de fond
		}
		if ( ImGui::SliderFloat( "fovy", &fovy, 5.f, 160.f, "%1.0f" ) )
		{
			camera.setFovy( fovy ); // changer la variable Fovy avec fovy avec la fct setFovy
			//_updateProjectionMatrix();
		}
		
		ImGui::End();
	}
	//fonction qui crée un cube unitaire (Mesh)----------------------------------------------------------
	LabWork3::Mesh LabWork3::_createCube()
	{ 
			
		_cube.positions_sommets
			= { Vec3f( -0.5, -0.5, 0 ), Vec3f( -0.5, 0.5, 0 ), Vec3f( 0.5, 0.5, 0 ), Vec3f( 0.5, -0.5, 0 ),
				Vec3f( -0.5, -0.5, 1 ), Vec3f( -0.5, 0.5, 1 ), Vec3f( 0.5, 0.5, 1 ), Vec3f( 0.5, -0.5, 1 ) };
		_cube.couleurs_sommets = { getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f(),
								   getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f() };  //les couleurs sont aléatoirement sélectionnées.
		_cube.indices_sommets = { 0, 1, 4, 1, 4, 5, 1, 2, 5, 2, 5, 6, 0, 3, 4, 4, 3, 7,
								  2, 3, 7, 2, 6, 7, 0, 3, 1, 1, 2, 3, 5, 4, 7, 5, 6, 7 };
		_cube.matrice		   = glm::mat4( 1.f ); //matrice identité

		_cube.matrice = glm::scale( _cube.matrice, glm::vec3( 0.8f ) );//homotéthie avec 0.8 sur chaque axe
		return _cube;
		} 
		//initialisez les buffers et le VAO-----------------------------------------------------------------------------------------------
		void LabWork3::_initBuffers()
		{
			glCreateBuffers( 1, &_cube.VBO_s);		  // Création du VBO pour les position des sommets
			glCreateBuffers( 1, &_cube.VBO_c );      // Création du VBO pour les couleurs des sommets
			glNamedBufferData( _cube.VBO_s,
							   _cube.positions_sommets.size() * sizeof( Vec3f ),
							   _cube.positions_sommets.data(),
							   GL_STATIC_DRAW ); // Nous utiliserons un buffer statique car les donnéees des sommets ne
												 // seront jamais modifiée
			glNamedBufferData(
				_cube.VBO_c, _cube.couleurs_sommets.size() * sizeof( Vec3f ), _cube.couleurs_sommets.data(), GL_STATIC_DRAW );

			// VAO-------------------------------------------------------------------------------------------------------
			glCreateVertexArrays( 1, &_cube.VAO );
			glEnableVertexArrayAttrib( _cube.VAO, 0 );							// Activez l’attribut 0 du VAO
			glEnableVertexArrayAttrib( _cube.VAO, 1 );							 // Activez l’attribut 1 du VAO
			glVertexArrayAttribFormat( _cube.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0 ); // Définissez le format de l’attribut
			glVertexArrayVertexBuffer( _cube.VAO, 0, _cube.VBO_s, 0, sizeof( Vec3f ) );	 // Liez le VBO_sommets et le VAO
			glVertexArrayAttribBinding( _cube.VAO, 0, 0 ); // connectez le VAO avec le Vertex shader 
			// ------------------------------------------------------------------------------------------
			glVertexArrayVertexBuffer(_cube.VAO, 1, _cube.VBO_c, 0, sizeof( Vec3f )); // Liez le VBO_color et le VAO
			glVertexArrayAttribBinding( _cube.VAO, 1, 1 ); // Connectez le VAO avec le Vertex shader
			
			//...........................................................................................................
 
			// Création et remplissage d'EBO--------------------------------------------------------------------------------
			glCreateBuffers( 1, &_cube.EBO );
			glNamedBufferData(
				_cube.EBO, _cube.indices_sommets.size() * sizeof( unsigned int ), _cube.indices_sommets.data(), GL_STATIC_DRAW );
			//----------------------------------------------------------------------------------------------------------
			// Liez l’EBO au VAO
			glVertexArrayElementBuffer( _cube.VAO, _cube.EBO );
		}
		//------------------------------------------------------------------------------------------------------------------------------------------------
		void LabWork3::_updateViewMatrix()
		{
			//glProgramUniformMatrix4fv(Program, location_ViewMatrix, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
			
		}
		void LabWork3::_updateProjectionMatrix() {
			//glProgramUniformMatrix4fv(Program, location_ProjectionMatrix, 1, GL_FALSE, glm::value_ptr( camera.getProjectionMatrix() ) );

		}
		void LabWork3::_initCamera()
		{
			camera.setPosition( Vec3f( 0, 1, 3 ) ); // Placez la caméra en (0, 1, 3)
			camera.setScreenSize( _windowWidth, _windowHeight );//Initialisez les attributs de la camera relatifs a la taille de la fenetre
			//camera.setLookAt( Vec3f( 0, 0, 0 ) ); //Direction camera vers (0,0,0)
		}
		void LabWork3::handleEvents( const SDL_Event & p_event ) {
			if ( p_event.type == SDL_KEYDOWN )
			{
				switch ( p_event.key.keysym.scancode )
				{
				case SDL_SCANCODE_W: // Front
					camera.moveFront( _cameraSpeed );
					//_updateViewMatrix();
					break;
				case SDL_SCANCODE_S: // Back
					camera.moveFront( -_cameraSpeed );
					//_updateViewMatrix();
					break;
				case SDL_SCANCODE_A: // Left
					camera.moveRight( -_cameraSpeed );
					//_updateViewMatrix();
					break;
				case SDL_SCANCODE_D: // Right
					camera.moveRight(_cameraSpeed );
					//_updateViewMatrix();
					break;
				case SDL_SCANCODE_R: // Up
					camera.moveUp( _cameraSpeed );
					//_updateViewMatrix();
					break;
				case SDL_SCANCODE_F: // Bottom
					camera.moveUp( -_cameraSpeed );
					//_updateViewMatrix();
					break;
				default: break;
				}
			}

			// Rotate when left click + motion (if not on Imgui widget).
			if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
				 && !ImGui::GetIO().WantCaptureMouse )
			{
				camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
				//_updateViewMatrix();
			}
		}
		} // namespace M3D_ISICG
