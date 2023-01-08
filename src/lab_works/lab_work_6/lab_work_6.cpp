#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_6.hpp"
#include "utils/random.hpp" //bib pour getrandomvec3f
#include "utils/read_file.hpp"
#include <iostream>
namespace M3D_ISICG
{
	const std::string LabWork6::_shaderFolder = "src/lab_works/lab_work_6/shaders/";

	LabWork6::~LabWork6()
	{
		glDeleteProgram( _geometryPassProgram ); // Destruction du programme
		
		glDeleteProgram( _shadingPassProgram );// Destruction du programme
		bunny.cleanGL();
	}

	bool LabWork6::init()
	{
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		initialiser_geometryPassProgram();
		initialiser_shadingPassProgram();
		_initCamera(); // appel de la fct pour l'initialisation du camera
		glEnable( GL_DEPTH_TEST );
		// bunny.load( "Bunny", "data/models/bunny2/bunny_2.obj" );
		bunny.load( "Bunny", "data/models/sponza/sponza.obj" );
		bunny._transformation = glm::scale( bunny._transformation, glm::vec3( 0.003f ) );
		////////////////////////////////////variables uniformes //////////////////////////
		location_MVP		  = glGetUniformLocation( _geometryPassProgram, "MVPMatrix" );
		location_NormalMatrix = glGetUniformLocation( _geometryPassProgram, "NormalMatrix" );
		location_ViewMatrix	  = glGetUniformLocation( _geometryPassProgram, "ViewMatrix" );
		location_ModelMatrix  = glGetUniformLocation( _geometryPassProgram, "ModelMatrix" );
		locationCameraPos	  = glGetUniformLocation( _geometryPassProgram, "Camerapos" );
		///////////////////////////////////////////////////////////////////////////////////
		PreparerGBuffuer();
		
		CreateCube();
		
		std::cout << "Done!" << std::endl;
		return true;
	}
	void LabWork6::initialiser_geometryPassProgram() {
		const std::string vertexShaderStr
			= readFile( _shaderFolder + "geometry_pass.vert" ); // lire le shader  et le stocker dans vertexShaderStr
		const std::string fragmentShaderStr
			= readFile( _shaderFolder + "geometry_pass.frag" ); // lire le shader  et le stocker dans fragmentShaderStr
		vertexShader	  = glCreateShader( GL_VERTEX_SHADER ); // Construction du vertex shader
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER ); // Construction du fragment shader
		const char * VSrc = vertexShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		const char * FSrc = fragmentShaderStr.c_str();	  // variable intermédiaire pour l'utiliser dans glCreateShader
		glShaderSource( vertexShader, 1, &VSrc, NULL );	  // Initialisation du code source pour vertex Shader
		glShaderSource( fragmentShader, 1, &FSrc, NULL ); // Initialisation du code source pour fragment Shader
		glCompileShader( vertexShader );				  // Compilation pour recupérer les erreurs de compilation
		glCompileShader( fragmentShader );				  // Compilation pour recupérer les erreurs de compilation
		std::cout << "Initializing lab work 6..." << std::endl;
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
			return;
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
			return;
		}

		_geometryPassProgram = glCreateProgram(); // Création du programme
		// Attachage shaders/programme
		glAttachShader( _geometryPassProgram, vertexShader );
		glAttachShader( _geometryPassProgram, fragmentShader );
		// Liaison du programme
		glLinkProgram( _geometryPassProgram );

		GLint linked;
		glGetProgramiv( _geometryPassProgram, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _geometryPassProgram, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return;
		}

		
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
	}
	void LabWork6::animate( const float p_deltaTime ) {}

	void LabWork6::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		/////////////////////////////////Calculer les matrices
		//////////////////////////////////////////////////////////////////////////
		MVPa				= camera.getProjectionMatrix() * camera.getViewMatrix() * bunny._transformation;
		Mat3f Normal_Matrix = Mat3f( glm::transpose( glm::inverse( camera.getViewMatrix() * bunny._transformation ) ) );
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//////////////////////Mettre a jour les variables uniformes////////////////////////////////////////

		glProgramUniformMatrix4fv( _geometryPassProgram, location_MVP, 1, GL_FALSE, glm::value_ptr( MVPa ) );
		glProgramUniformMatrix3fv(
			_geometryPassProgram, location_NormalMatrix, 1, GL_FALSE, glm::value_ptr( Normal_Matrix ) );
		glProgramUniform3fv( _geometryPassProgram,
							 glGetUniformLocation( _geometryPassProgram, "lightPos" ),
							 1,
							 glm::value_ptr( Vec3f( 0.f, 0.f, 0.f ) ) );
		glProgramUniformMatrix4fv(
			_geometryPassProgram, location_ViewMatrix, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
		glProgramUniformMatrix4fv(
			_geometryPassProgram, location_ModelMatrix, 1, GL_FALSE, glm::value_ptr( bunny._transformation ) );
		glProgramUniform3fv( _geometryPassProgram, locationCameraPos, 1, glm::value_ptr( camera.Positioncamera() ) );
		_geometryPass();
		_shadingPass();
		
	}
	void LabWork6::initialiser_shadingPassProgram()
	{
		const std::string fragmentShaderStr = readFile( _shaderFolder + "shading_pass.frag" );
		const GLuint	  fragmentShader	= glCreateShader( GL_FRAGMENT_SHADER );
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();

		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		glCompileShader( fragmentShader );

		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling shading pass fragment shader : " << log << std ::endl;
			return;
		}

		_shadingPassProgram = glCreateProgram();
		glAttachShader( _shadingPassProgram, fragmentShader );
		glLinkProgram( _shadingPassProgram );

		GLint linked;
		glGetProgramiv( _shadingPassProgram, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _shadingPassProgram, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return ;
		}
	 LightPOS = glGetUniformLocation( _shadingPassProgram, "LightPOS" );
		glDeleteShader( fragmentShader );
	}
	void LabWork6::_geometryPass() {
		glUseProgram( _geometryPassProgram );
		glEnable( GL_DEPTH_TEST );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FBO );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		bunny.render( _geometryPassProgram );
		//////////////////////////////////////////////////////////////////////////////////////////////////
		glNamedFramebufferReadBuffer( FBO, texture_à_afficher ); // Sélection le buffer à lire
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FBO );//Le FS va écrire dans les textures attachées au FBO
		for ( size_t i = 0; i < 5; i++ )
		{
			glBindTextureUnit( i, _gBufferTextures[ i ] );
		}

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ); // remettre le frame buffer par défaut
		
		
		

		glBlitNamedFramebuffer( FBO,
								0,
								0,
								0,
								_windowWidth,
								_windowHeight,
								0,
								0,
								_windowWidth,
								_windowHeight,
								GL_COLOR_BUFFER_BIT,
								GL_NEAREST );

		
	}
	
	void LabWork6::PreparerGBuffuer() {
		//--------------------------Création de FBO-----------------------------------------
		glCreateFramebuffers( 1, &FBO );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FBO );
		//------------------------------------------------------------------------------------
		//---------------------Création des textures-----------------------------------------------
		glCreateTextures( GL_TEXTURE_2D, 1, &_gBufferTextures[0] );
		glCreateTextures( GL_TEXTURE_2D, 1, &_gBufferTextures[ 1 ] );
		glCreateTextures( GL_TEXTURE_2D, 1, &_gBufferTextures[ 2 ] );
		glCreateTextures( GL_TEXTURE_2D, 1, &_gBufferTextures[ 3 ] );
		glCreateTextures( GL_TEXTURE_2D, 1, &_gBufferTextures[ 4 ] );
		glCreateTextures( GL_TEXTURE_2D, 1, &_gBufferTextures[ 5 ] );
		GLenum drawBuffers[] = {
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4,
		};

		glNamedFramebufferTexture( FBO, GL_COLOR_ATTACHMENT0, _gBufferTextures[ 0 ], 0 );
		glNamedFramebufferTexture( FBO, GL_COLOR_ATTACHMENT1, _gBufferTextures[ 1 ], 0 );
		glNamedFramebufferTexture( FBO, GL_COLOR_ATTACHMENT2, _gBufferTextures[ 2 ], 0 );
		glNamedFramebufferTexture( FBO, GL_COLOR_ATTACHMENT3, _gBufferTextures[ 3 ], 0 );
		glNamedFramebufferTexture( FBO, GL_COLOR_ATTACHMENT4, _gBufferTextures[ 4 ], 0 );
		glNamedFramebufferTexture( FBO, GL_DEPTH_ATTACHMENT, _gBufferTextures[ 5 ], 0 );
		glTextureStorage2D( _gBufferTextures[ 0 ], 1, GL_RGB32F, _windowWidth, _windowHeight );
		glTextureStorage2D( _gBufferTextures[ 1 ], 1, GL_RGB32F, _windowWidth, _windowHeight );
		glTextureStorage2D( _gBufferTextures[ 2 ], 1, GL_RGB32F, _windowWidth, _windowHeight );
		glTextureStorage2D( _gBufferTextures[ 3 ], 1, GL_RGB32F, _windowWidth, _windowHeight );
		glTextureStorage2D( _gBufferTextures[ 4 ], 1, GL_RGB32F, _windowWidth, _windowHeight );
		glTextureStorage2D( _gBufferTextures[ 5 ], 1, GL_DEPTH_COMPONENT32F, _windowWidth, _windowHeight );
		
		glTextureParameteri( _gBufferTextures[ 0 ], GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( _gBufferTextures[ 0 ], GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( _gBufferTextures[ 1 ], GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( _gBufferTextures[ 1 ], GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( _gBufferTextures[ 2 ], GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( _gBufferTextures[ 2 ], GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( _gBufferTextures[ 3 ], GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( _gBufferTextures[ 3 ], GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( _gBufferTextures[ 4 ], GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( _gBufferTextures[ 4 ], GL_TEXTURE_MAG_FILTER, GL_NEAREST );


		glTextureParameteri( _gBufferTextures[ 5 ], GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( _gBufferTextures[ 5 ], GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		
		
		
		glNamedFramebufferDrawBuffers( FBO, 5, drawBuffers );//associez les textures à la sortie du fragement shader
		//-----------------------------------------------------------------------------------------
		//-------Check FBO-----------------------------------------------------------------
		if ( glCheckNamedFramebufferStatus( FBO, GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		{
			std::cout << "Err: Gbuffer n'est pas complet" << std::endl;
		}
		//----------------------------------------------------------------------------------
	}
	void LabWork6::displayUI()
	{
		ImGui::Begin( "Settings lab work 6" );
		ImGui::Text( "No setting available!" );
		if ( ImGui::BeginListBox( "Chosisir textures à afficher:" ) )
		{
			if ( ImGui::Selectable( "PosFragments" ) )
			{
				texture_à_afficher = GL_COLOR_ATTACHMENT0;
			}
			else if ( ImGui::Selectable( "Normales" ) )
			{
				texture_à_afficher = GL_COLOR_ATTACHMENT1;
			}
			else if ( ImGui::Selectable( "Color_Ambient" ) )
			{
				texture_à_afficher = GL_COLOR_ATTACHMENT2;
			}
			else if ( ImGui::Selectable( "Color_Diffuse" ) )
			{
				texture_à_afficher = GL_COLOR_ATTACHMENT3;
			}
			else if ( ImGui::Selectable( "Color_Spec" ) )
			{
				texture_à_afficher = GL_COLOR_ATTACHMENT4;
			}
			ImGui::EndListBox();
		}
		ImGui::End();
	}
	void LabWork6::_initCamera()
	{
		camera.setPosition( Vec3f( 1.f, 2.f, 0 ) );
		camera.setScreenSize( _windowWidth, _windowHeight );
		camera.setLookAt( Vec3f( 0.f, 0.f, 0.f ) );
	}
	void LabWork6::CreateCube() {
		GLuint vbo;
		GLuint ebo;

		glCreateBuffers( 1, &vbo );
		glNamedBufferData( vbo, PositionsSommet.size() * sizeof( Vec2f ), PositionsSommet.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &ebo );
		glNamedBufferData( ebo, Indices.size() * sizeof( int ), Indices.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &_Vao );

		glEnableVertexArrayAttrib( _Vao, 0 );
		glVertexArrayAttribFormat( _Vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _Vao, 0, vbo, 0, sizeof( Vec2f ) );

		glVertexArrayElementBuffer( _Vao, ebo );

		glVertexArrayAttribBinding( _Vao, 0, 0 );

		
	}
	void LabWork6::_updateViewMatrix()
	{
		glProgramUniformMatrix4fv(_geometryPassProgram, location_ViewMatrix, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
	}
	void LabWork6::_shadingPass()
	{
		glUseProgram( _shadingPassProgram );
		glDisable( GL_DEPTH_TEST );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		// Quad
		glBindVertexArray( _Vao );
		glProgramUniform3fv( _shadingPassProgram, LightPOS, 1, glm::value_ptr( camera.Positioncamera( ) ));
		glDrawElements( GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0 );
		for ( int i = 0; i < 6; i++ )
		{
			glBindTextureUnit( i, _gBufferTextures[ i ] );
			glBindTextureUnit( i, 0 );
		}
		glBindVertexArray( 0 );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
	void LabWork6::handleEvents( const SDL_Event & p_event )
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
