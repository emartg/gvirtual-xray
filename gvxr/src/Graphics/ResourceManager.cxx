//******************************************************************************
//  Include
//******************************************************************************
#include <algorithm>

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#include "gVirtualXRay/framebufferObject.h"

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


ResourceManager::ResourceManager():
    m_p_context(0)
{}


void ResourceManager::releaseResources()
{
    // Release shaders
    for (std::vector<Shader*>::iterator ite = m_p_shader_set.begin();
        ite != m_p_shader_set.end();
        ++ite)
    {
        if (*ite)
        {
        	try
        	{
                (*ite)->release();
        	}
        	catch (...)
        	{
        		// Do nothing
        	}
        }
    }
    m_p_shader_set.clear();

    // Release FBOs
    while (m_p_FBO_set.size())
    {
        FramebufferObject* p_fbo = m_p_FBO_set.back();
        p_fbo->destroy();
    }

    // Release VBOs
    for (std::vector<OpenGL2VBO*>::iterator ite = m_p_gl2_vbo.begin();
        ite != m_p_gl2_vbo.end();
        ++ite)
    {
        if (*ite)
        {
            (*ite)->destroy();
        }
    }
    m_p_gl2_vbo.clear();

    for (std::vector<OpenGL3VBO*>::iterator ite = m_p_gl3_vbo.begin();
        ite != m_p_gl3_vbo.end();
        ++ite)
    {
        if (*ite)
        {
        	try
        	{
        		(*ite)->destroy();
        	}
        	catch (...)
        	{
        		// Do nothing
        	}
        }
    }
    m_p_gl3_vbo.clear();
}


void ResourceManager::addShader(Shader* apShader)
{
    // Only add the shader if it is not already found in the shader set
    std::vector<Shader*>::iterator ite(std::find(m_p_shader_set.begin(), m_p_shader_set.end(), apShader));
    if (ite == m_p_shader_set.end())
        m_p_shader_set.push_back(apShader);
}


void ResourceManager::addFBO(FramebufferObject* apFBO)
{
    // Only add the FBO if it is not already found in the FBO set
    std::vector<FramebufferObject*>::iterator ite(std::find(m_p_FBO_set.begin(), m_p_FBO_set.end(), apFBO));
    if (ite == m_p_FBO_set.end())
        m_p_FBO_set.push_back(apFBO);
}


void ResourceManager::addVBO(OpenGL2VBO* apVBO)
{
    // Only add the VBO if it is not already found in the VBO set
    std::vector<OpenGL2VBO*>::iterator ite(std::find(m_p_gl2_vbo.begin(), m_p_gl2_vbo.end(), apVBO));
    if (ite == m_p_gl2_vbo.end())
        m_p_gl2_vbo.push_back(apVBO);
}


void ResourceManager::addVBO(OpenGL3VBO* apVBO)
{
    // Only add the VBO if it is not already found in the VBO set
    std::vector<OpenGL3VBO*>::iterator ite(std::find(m_p_gl3_vbo.begin(), m_p_gl3_vbo.end(), apVBO));
    if (ite == m_p_gl3_vbo.end())
        m_p_gl3_vbo.push_back(apVBO);
}


void ResourceManager::destroyAndRemoveShader(Shader* apShader)
{
    std::vector<Shader*>::iterator ite(std::find(m_p_shader_set.begin(), m_p_shader_set.end(), apShader));
    if (ite != m_p_shader_set.end())
    {
        m_p_shader_set.erase(ite);
    }
}


void ResourceManager::destroyAndRemoveFBO(FramebufferObject* apFBO)
{
    std::vector<FramebufferObject*>::iterator ite(std::find(m_p_FBO_set.begin(), m_p_FBO_set.end(), apFBO));
    if (ite != m_p_FBO_set.end())
    {
        (*ite)->UnattachAll();
        m_p_FBO_set.erase(ite);
    }
}


void ResourceManager::destroyAndRemoveVBO(OpenGL2VBO* apVBO)
{
    std::vector<OpenGL2VBO*>::iterator ite(std::find(m_p_gl2_vbo.begin(), m_p_gl2_vbo.end(), apVBO));
    if (ite != m_p_gl2_vbo.end())
    {
        (*ite)->destroy();
        m_p_gl2_vbo.erase(ite);
    }
}


void ResourceManager::destroyAndRemoveVBO(OpenGL3VBO* apVBO)
{
    std::vector<OpenGL3VBO*>::iterator ite(std::find(m_p_gl3_vbo.begin(), m_p_gl3_vbo.end(), apVBO));
    if (ite != m_p_gl3_vbo.end())
    {
        (*ite)->destroy();
        m_p_gl3_vbo.erase(ite);
    }
}


void ResourceManager::setGraphicsContext(Context* apContext)
{
    m_p_context = apContext;
}


Context* ResourceManager::getGraphicsContext()
{
	return m_p_context;
}


void ResourceManager::destroyAndRemoveContext()
{
    releaseResources();

    if (m_p_context)
    {
    	m_p_context->release();
    }
    m_p_context = 0;
}
