#ifndef __LAB_WORK_1_HPP__
#define __LAB_WORK_1_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork1 : public BaseLabWork
	{
	  public:
		LabWork1() : BaseLabWork() {}
		~LabWork1();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		std::vector<Vec2f> Position_sommets; // vector contiendra la position des sommets du triangle en 2D
		// ================
		const std::string vertexShaderStr;	 // Déclaration de variable pour lire et stocker vertex shader
		const std::string fragmentShaderStr; // Déclaration de variable pour lire et stocker fragment shader
		// ================ GL data.
		GLuint vertexShader, fragmentShader; // Déclaration des shaders
		GLuint Program;						 // pour stocker l’identifiant de l’objet Program
		GLuint VBO;							 //  Stocker l’identifiant du VBO
		GLuint VAO;							 // Stocker l’identifiant du VAO
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_1_HPP__
