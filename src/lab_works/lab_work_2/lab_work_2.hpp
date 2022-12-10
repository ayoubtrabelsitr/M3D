#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		void			   Exercice_Optionnel( Vec2f C, int N, float R ); // Fonction pour l'exercice Optionnel
		std::vector<Vec3f> color_sommets;	  // vector pour stocker les couleurs des sommets
		std::vector<Vec2f> Position_sommets;  // vector contiendra la position des sommets du triangle en 2D
		std::vector<int>   indices_sommets;	  // vector pour stocker les indices des sommets formant les deux triangle
		const std::string  vertexShaderStr;	  // Déclaration de variable pour lire et stocker vertex shader
		const std::string  fragmentShaderStr; // Déclaration de variable pour lire et stocker fragment shader
		// ================ GL data.
		GLuint vertexShader, fragmentShader; // Déclaration des shaders
		GLuint Program;						 // pour stocker l’identifiant de l’objet Program
		GLuint VBO;							 // Déclaration pour l’identifiant du VBO
		GLuint VAO;							 // Déclaration pour l’identifiant du VAO
		GLuint EBO;							 // Déclaration pour l’identifiant du EBO
		GLuint VBO_color;					 // VBO pour les couleurs
		GLint  loc_translation;				 // Pour la variable uniforme de translation
		GLint  loc_luminosité;				 // Pour la variable uniforme de luminosite
		double _time	  = 0;				 // pour stocker le temps d’exécution
		float  Luminosité = 1;				 // Pour stocker la luminosité
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
