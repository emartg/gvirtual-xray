/**
********************************************************************************
*
*	@file		LungsMesh.cxx
*
*	@brief      Class to handle a polygon mesh for lungs.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_LUNG_MESH_H
#include "gVirtualXRay/LungsMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
LungsMesh::LungsMesh(bool x_mirror, bool y_mirror, bool z_mirror,
		bool optimize_texture_coordinates, unsigned int texture_2D_size, unsigned int texture_3D_size):
//----------------------------------------------
		SoftTissueMesh(optimize_texture_coordinates, texture_2D_size, texture_3D_size),
		m_x_mirror(x_mirror),
		m_y_mirror(y_mirror),
		m_z_mirror(z_mirror)
//----------------------------------------------
{
	getMaterial().setDiffuseColour(1.,0.71,0.66,1.);
	getMaterial().setAmbientColour(1.,0.71,0.66,1.);
	m_is_lung=1;
	m_use_3D_colour_texture = 1;
	m_label = "lungs";
}


//----------------------------------------------
LungsMesh::~LungsMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void LungsMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	GLuint handle(glGetUniformLocation(shader_id, "g_min_corner"));
	glUniform3fv(handle, 1,(const GLfloat*) &m_local_min_corner);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	float max_corner[] = {m_local_max_corner.getX(), m_local_max_corner.getY(), m_local_max_corner.getZ()};

	handle = glGetUniformLocation(shader_id, "g_max_corner");
	glUniform3fv(handle, 1, &max_corner[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//matrix to rotate in case the model is upside down
	Matrix4x4<GLfloat> rotation_matrix(Matrix4x4<GLfloat>::buildIdentityMatrix());

	if(m_x_mirror)
	{
		rotation_matrix *= 	rotation_matrix*Matrix4x4<GLfloat>::buildRotationMatrix(180,VEC3(0.,1.,0.));
	}
	if(m_y_mirror)
	{
		rotation_matrix *= 	rotation_matrix*Matrix4x4<GLfloat>::buildRotationMatrix(180,VEC3(0.,0.,1.));
	}
	if(m_z_mirror)
	{
		rotation_matrix *= 	rotation_matrix*Matrix4x4<GLfloat>::buildRotationMatrix(180,VEC3(1.,0.,0.));
	}


	//matrix for the bump map (to create the line on the right)
	VEC3 translation_vector = getLocalMinCorner() + (getLocalMaxCorner() - getLocalMinCorner()) / 2.0;
	Matrix4x4<GLfloat> matrix(Matrix4x4<GLfloat>::buildTranslationMatrix(translation_vector));
	matrix *= rotation_matrix*Matrix4x4<GLfloat>::buildRotationMatrix(10,VEC3(1.,0.,0.));
	matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-translation_vector);

	handle = glGetUniformLocation(shader_id, "g_right_line_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	//matrix for the bump map (to create the lines on the sides)
	translation_vector = getLocalMinCorner() + (getLocalMaxCorner() - getLocalMinCorner()) / 2.0;
	matrix = Matrix4x4<GLfloat>::buildTranslationMatrix(translation_vector);
	matrix *= rotation_matrix*Matrix4x4<GLfloat>::buildRotationMatrix(30,VEC3(1.,0.,0.));
	matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-translation_vector);

	handle = glGetUniformLocation(shader_id, "g_side_lines_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineBumpMapShaderInput(shader_id);
}

//----------------------------------------------
void LungsMesh::defineColourTextureShaderInput(int shader_id)
//----------------------------------------------
{
	GLfloat second_colour[] = {0.4,0.03,0.03,1.};

	GLuint handle(glGetUniformLocation(shader_id, "g_second_colour"));
	glUniform4fv(handle, 1, &second_colour[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineColourTextureShaderInput(shader_id);
}

} // namespace gVirtualXRay
