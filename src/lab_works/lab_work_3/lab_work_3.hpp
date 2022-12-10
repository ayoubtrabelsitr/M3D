#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "common/camera.hpp"

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
		
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();
		

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void handleEvents( const SDL_Event & p_event ) override;
		//void handleEvents( const SDL_Event & p_event ) override;
		
		void displayUI() override;

		
	  private:
		struct Mesh             //structure Mesh
		{
			std::vector<Vec3f>		  positions_sommets; //vector pour les positions des sommets
			std::vector<Vec3f>		  couleurs_sommets;  //vector pour les couleurs des sommets
			std::vector<unsigned int> indices_sommets;//vector pour les indices des sommets
			Mat4f					  matrice;        //Matrice pour la transformation de l’objet
			GLuint					  VBO_s, VBO_c, VAO, EBO; // les buffers et le VAO
			Mat4f					  transformation ;
		};
		Mesh _cube;
		Mesh _createCube();
		void _initBuffers(); 
		void _updateViewMatrix();//Méthode pour mettre à jour la variable uniforme Viewmatrix
		void _updateProjectionMatrix();//Méthode pour mettre à jour la variable uniforme Projectionmatrix
		void _initCamera();
		// ================ Scene data.
		
		Camera			   camera;
		
		Mat4f			   MVPMatrix;
		float fovy = 60;
		float			   _cameraSpeed		  = 0.1f;
		float			   _cameraSensitivity = 0.1f;
		const std::string  vertexShaderStr;	  // Déclaration de variable pour lire et stocker vertex shader
		const std::string  fragmentShaderStr; // Déclaration de variable pour lire et stocker fragment shader
		// ================ GL data.
		GLuint vertexShader, fragmentShader; // Déclaration des shaders
		GLuint Program;						 // pour stocker l’identifiant de l’objet Program
		//GLint  location_ModelMatrix;							 // Pour l'adresse de la variable uniforme modelmatrix
		//GLint  location_ViewMatrix;						 // Pour l'adresse de la variable uniforme viewmatrix
		//GLint  location_ProjectionMatrix;						 // Pour l'adresse de la variable uniforme Projectionmatrix
		GLint  location_MVP;
		// ================
		
		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
