#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_5.hpp"
#include "utils/random.hpp" //bib pour getrandomvec3f
#include "utils/read_file.hpp"
#include <iostream>
namespace M3D_ISICG
{
	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/shaders/";

	LabWork5::~LabWork5()
	{
		glDeleteProgram( Program ); // Destruction du programme
		bunny.cleanGL();
	}

	bool LabWork5::init()
	{
		const std::string vertexShaderStr
			= readFile( _shaderFolder + "meshtexture.vert" ); // lire le shader  et le stocker dans vertexShaderStr
		const std::string fragmentShaderStr
			= readFile( _shaderFolder + "meshtexture.frag" );			// lire le shader  et le stocker dans fragmentShaderStr
		vertexShader	  = glCreateShader( GL_VERTEX_SHADER ); // Construction du vertex shader
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER ); // Construction du fragment shader
		const char * VSrc = vertexShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		const char * FSrc = fragmentShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		glShaderSource( vertexShader, 1, &VSrc, NULL );	  // Initialisation du code source pour vertex Shader
		glShaderSource( fragmentShader, 1, &FSrc, NULL ); // Initialisation du code source pour fragment Shader
		glCompileShader( vertexShader );				  // Compilation pour recupérer les erreurs de compilation
		glCompileShader( fragmentShader );				  // Compilation pour recupérer les erreurs de compilation
		std::cout << "Initializing lab work 4..." << std::endl;
		// Set the color used by glClear to clear the color buffer
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

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

		Program = glCreateProgram(); // Création du programme
		// Attachage shaders/programme
		glAttachShader( Program, vertexShader );
		glAttachShader( Program, fragmentShader );
		// Liaison du programme
		glLinkProgram( Program );

		GLint linked;
		glGetProgramiv( Program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( Program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		glUseProgram( Program );
		_initCamera(); // appel de la fct pour l'initialisation du camera

		
		//bunny.load( "Bunny", "data/models/bunny2/bunny_2.obj" );
		bunny.load( "Bunny", "data/models/sponza/sponza.obj" );
		bunny._transformation = glm::scale( bunny._transformation, glm::vec3( 0.003f ) );
		////////////////////////////////////variables uniformes //////////////////////////
		location_MVP		  = glGetUniformLocation( Program, "MVPMatrix" );
		location_NormalMatrix = glGetUniformLocation( Program, "NormalMatrix" );
		location_ViewMatrix	  = glGetUniformLocation( Program, "ViewMatrix" );
		location_ModelMatrix  = glGetUniformLocation( Program, "ModelMatrix" );
		locationCameraPos	  = glGetUniformLocation( Program, "Camerapos" );
		///////////////////////////////////////////////////////////////////////////////////
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork5::animate( const float p_deltaTime ) {}

	void LabWork5::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable( GL_DEPTH_TEST );

		//glEnable( GL_BLEND );//Activer le mélange des couleurs dans le framebuffer
		//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );//Indiquer la fonction de mélange à utiliser

		/////////////////////////////////Calculer les matrices ///////////////////////////////////////////////////////////////////////
		MVPa = camera.getProjectionMatrix() * camera.getViewMatrix() * bunny._transformation;
		Mat3f Normal_Matrix = Mat3f( glm::transpose( glm::inverse( camera.getViewMatrix() * bunny._transformation ) ) );
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		

		//////////////////////Mettre a jour les variables uniformes/////////////////////////////////////////

		glProgramUniformMatrix4fv( Program, location_MVP, 1, GL_FALSE, glm::value_ptr( MVPa ) );
		glProgramUniformMatrix3fv( Program, location_NormalMatrix, 1, GL_FALSE, glm::value_ptr( Normal_Matrix ) );
		glProgramUniform3fv(Program, glGetUniformLocation( Program, "lightPos" ), 1, glm::value_ptr( camera.Positioncamera() ) );
		glProgramUniformMatrix4fv(Program, location_ViewMatrix, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
		glProgramUniformMatrix4fv(Program, location_ModelMatrix, 1, GL_FALSE, glm::value_ptr( bunny._transformation ) );
		glProgramUniform3fv( Program, locationCameraPos, 1, glm::value_ptr( camera.Positioncamera() ) );
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		
		bunny.render( Program );
	}
	void LabWork5::displayUI()
	{
		ImGui::Begin( "Settings lab work 5" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}
	void LabWork5::_initCamera()
	{
		camera.setPosition( Vec3f( 1.f, 2.f, 0 ) ); 
		camera.setScreenSize( _windowWidth, _windowHeight );
		camera.setLookAt( Vec3f(0.f, 0.f, 0.f ));
	}
	void LabWork5::_updateViewMatrix()
	{
		glProgramUniformMatrix4fv(Program, location_ViewMatrix, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
	}
	void LabWork5::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}
} // namespace M3D_ISICG
