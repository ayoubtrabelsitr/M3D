#ifndef __LAB_WORK_6_HPP__
#define __LAB_WORK_6_HPP__
#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "common/models/triangle_mesh.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork6 : public BaseLabWork
	{
	  public:
		LabWork6() : BaseLabWork() {}
		~LabWork6();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.

		Camera			  camera;
		TriangleMeshModel bunny;

		void			  _initCamera();
		void			  _updateViewMatrix();
		
		float			  _cameraSpeed		 = 0.1f;
		float			  _cameraSensitivity = 0.1f;
		const std::string vertexShaderStr;	 // D�claration de variable pour lire et stocker vertex shader
		const std::string fragmentShaderStr; // D�claration de variable pour lire et stocker fragment shader
		// ================ GL data.
		GLuint vertexShader, fragmentShader; // D�claration des shaders
		GLuint _geometryPassProgram;		 // pour stocker l�identifiant de l�objet Program
		GLuint _shadingPassProgram;
		GLint  location_MVP;				 // Pour l'adresse de la variable uniforme modelmatrix
		GLuint location_ViewMatrix, location_ModelMatrix, location_ProjectionMatrix;
		GLuint location_NormalMatrix;
		GLuint locationCameraPos;
		GLenum texture_�_afficher = GL_COLOR_ATTACHMENT1;
		GLuint _Vao;
		GLuint LightPOS;
		//-----------------Les attributs pour les textures ---------------------------------------------------
		GLuint _gBufferTextures[6]; //6 attributs 
		GLuint FBO;

		void   PreparerGBuffuer();
		
		void   _geometryPass();
		void   _shadingPass();

		void   initialiser_geometryPassProgram();
		void   initialiser_shadingPassProgram();

		void   CreateCube();
		
		// ================
		// Mat4f modelView = glm::mat4( 1.f );
		Mat4f MVPa;
		std::vector<int> Indices = { 0, 1, 2, 0, 1, 3 };
		
		std::vector<Vec2f> PositionsSommet= { Vec2f( 5.0f, 5.0f ), Vec2f( -5.0f, -5.0f ), Vec2f( 5.0f, -5.0f ), Vec2f( -5.0f, 5.0f )};
		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_6_HPP__
