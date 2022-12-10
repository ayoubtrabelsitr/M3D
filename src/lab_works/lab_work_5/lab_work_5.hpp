#ifndef __LAB_WORK_5_HPP__
#define __LAB_WORK_5_HPP__
#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "common/models/triangle_mesh.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork5 : public BaseLabWork
	{
	  public:
		LabWork5() : BaseLabWork() {}
		~LabWork5();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.

		Camera			  camera;
		TriangleMeshModel bunny;

		void _initCamera();
		void			  _updateViewMatrix();
		float			  fovy				= 60;
		float			  _cameraSpeed		= 0.1f;
		float			  _cameraSensitivity = 0.1f;
		const std::string vertexShaderStr;	 // Déclaration de variable pour lire et stocker vertex shader
		const std::string fragmentShaderStr; // Déclaration de variable pour lire et stocker fragment shader
		// ================ GL data.
		GLuint vertexShader, fragmentShader; // Déclaration des shaders
		GLuint Program;						 // pour stocker l’identifiant de l’objet Program
		GLint  location_MVP;				 // Pour l'adresse de la variable uniforme modelmatrix
		GLuint location_ViewMatrix, location_ModelMatrix, location_ProjectionMatrix;
		GLuint location_NormalMatrix;
		GLuint locationCameraPos;
		// ================
		// Mat4f modelView = glm::mat4( 1.f );
		Mat4f MVPa;

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_4_HPP__
