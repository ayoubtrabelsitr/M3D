#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		
		
		glProgramUniform3fv(p_glProgram,glGetUniformLocation( p_glProgram, "ambient_color" ),1,glm::value_ptr( _material._ambient ) ); // Passage du couleur ambient de la maillage au programme GLSL
		glProgramUniform3fv(p_glProgram,glGetUniformLocation( p_glProgram, "diffuse_color" ),1,glm::value_ptr( _material._diffuse ) ); // Passage du couleur diffuse de la maillage au programme GLSL
		glProgramUniform3fv(p_glProgram,glGetUniformLocation( p_glProgram, "speculaire_color" ),1,glm::value_ptr( _material._specular ) ); // Passage du couleur diffuse de la maillage au programme GLSL
		
		glProgramUniform1f(p_glProgram, glGetUniformLocation( p_glProgram, "shininess" ),_material._shininess );
		
			glProgramUniform1i(p_glProgram, glGetUniformLocation( p_glProgram, "uHasDiffuseMap" ), _material._hasDiffuseMap );
		if (_material._hasDiffuseMap) 
		{
			glBindTextureUnit( 1, _material._diffuseMap._id ); //Lier binding=1
		}

		glProgramUniform1i(p_glProgram, glGetUniformLocation( p_glProgram, "uHasAmbientMap" ), _material._hasAmbientMap );
		if ( _material._hasAmbientMap )
		{
			glBindTextureUnit( 2, _material._ambientMap._id ); // Lier binding=1
		}
		glProgramUniform1i(p_glProgram, glGetUniformLocation( p_glProgram, "uHasSpecularMap" ), _material._hasSpecularMap );
		if ( _material._hasSpecularMap )
		{
			glBindTextureUnit( 3, _material._specularMap._id ); // Lier binding=1
		}
		glProgramUniform1i(p_glProgram, glGetUniformLocation( p_glProgram, "uHasShininess" ), _material._hasShininessMap );
		if ( _material._hasShininessMap )
		{
			glBindTextureUnit( 4, _material._shininessMap._id );
		}  
		glBindVertexArray(_vao);//Liaison entre VAO et le programme
		glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,0); // Lancez le pipeline
		glBindVertexArray(0);// Déliez le VAO et le programme

		
	}

	void TriangleMesh::cleanGL()
	{
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
		glBindTextureUnit( 0, _material._diffuseMap._id ); //délier la texture en passant la valeur 0
		glBindTextureUnit( 0, _material._ambientMap._id ); // délier la texture en passant la valeur 0
		glBindTextureUnit( 0, _material._specularMap._id ); // délier la texture en passant la valeur 0
		glBindTextureUnit( 0, _material._shininessMap._id ); // délier la texture en passant la valeur 0
	}

	void TriangleMesh::_setupGL()
	{	
		glCreateBuffers( 1, &_vbo ); // Création du VBO pour les position des sommets
		glNamedBufferData( _vbo,_vertices.size() * sizeof( Vertex ),_vertices.data(),GL_STATIC_DRAW); // Nous utiliserons un buffer statique car les donnéees des sommets ne seront jamais modifiée
		// VAO-------------------------------------------------------------------------------------------------------
		glCreateVertexArrays( 1, &_vao );
		glEnableVertexArrayAttrib( _vao, 0 ); // Activez l’attribut 0 du VAO
		glEnableVertexArrayAttrib( _vao, 1 ); // Activez l’attribut 1 du VAO
		glEnableVertexArrayAttrib( _vao, 2 ); // Activez l’attribut 2 du VAO
		glEnableVertexArrayAttrib( _vao, 3 ); // Activez l’attribut 3 du VAO
		glEnableVertexArrayAttrib( _vao, 4 ); // Activez l’attribut 4 du VAO
		glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _position));// Définissez le format de l’attribut
		glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _normal));// Définissez le format de l’attribut
		glVertexArrayAttribFormat(_vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _texCoords));// Définissez le format de l’attribut
		glVertexArrayAttribFormat(_vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _tangent));//Définissez le format de l’attribut
		glVertexArrayAttribFormat(_vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _bitangent));// Définissez le format de l’attribut
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vec3f ) );// Liez le VBO_sommets et le VAO
		glVertexArrayAttribBinding( _vao, 0, 0 );// connectez le VAO avec le Vertex shader
		glVertexArrayAttribBinding( _vao, 1, 0 );// connectez le VAO avec le Vertex shader
		glVertexArrayAttribBinding( _vao, 2, 0 );// connectez le VAO avec le Vertex shader
		glVertexArrayAttribBinding( _vao, 3, 0 );// connectez le VAO avec le Vertex shader
		glVertexArrayAttribBinding( _vao, 4, 0 );// connectez le VAO avec le Vertex shader
		// ------------------------------------------------------------------------------------------
		glVertexArrayVertexBuffer(_vao,0,_vbo,0,sizeof(Vertex));//Liez le VBO_color et le VAO
		// Création et remplissage d'EBO--------------------------------------------------------------------------------
		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );
		//----------------------------------------------------------------------------------------------------------
		// Liez l’EBO au VAO
		glVertexArrayElementBuffer( _vao, _ebo );
		///////////////////////

		}
} // namespace M3D_ISICG
