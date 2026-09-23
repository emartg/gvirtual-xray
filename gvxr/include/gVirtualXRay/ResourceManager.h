#ifndef GVXR_RESOURCE_MANAGER_H
#define GVXR_RESOURCE_MANAGER_H


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>

#ifndef GVXR_SHADER_H
#include <gVirtualXRay/Shader.h>
#endif

#ifndef GVXR_OPENGL2_VBO_H
#include <gVirtualXRay/OpenGL2VBO.h>
#endif

#ifndef GVXR_OPENGL3_VBO_H
#include <gVirtualXRay/OpenGL3VBO.h>
#endif

#ifndef GVXR_CONTEXT_H
#include <gVirtualXRay/Context.h>
#endif


//******************************************************************************
//	namespace
//******************************************************************************
//using namespace gVirtualXRay;


class FramebufferObject;


//******************************************************************************
//  Namespace
//******************************************************************************
namespace gVirtualXRay {


class ResourceManager
{
    public:
        static ResourceManager& getInstance()
        {
            static ResourceManager    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

        ResourceManager(ResourceManager const&) = delete;
        void operator=(ResourceManager const&)  = delete;

        void releaseResources();

        void addShader(Shader* apShader);
        void addFBO(FramebufferObject* apFBO);
        void addVBO(OpenGL2VBO* apVBO);
        void addVBO(OpenGL3VBO* apVBO);

        void destroyAndRemoveShader(Shader* apShader);
        void destroyAndRemoveFBO(FramebufferObject* apFBO);
        void destroyAndRemoveVBO(OpenGL2VBO* apVBO);
        void destroyAndRemoveVBO(OpenGL3VBO* apVBO);

        void setGraphicsContext(Context* apContext);
        Context* getGraphicsContext();
        void destroyAndRemoveContext();

      private:
        ResourceManager();

        std::vector<Shader*> m_p_shader_set;

        std::vector<FramebufferObject*> m_p_FBO_set;

        std::vector<OpenGL2VBO*> m_p_gl2_vbo;
        std::vector<OpenGL3VBO*> m_p_gl3_vbo;

        Context* m_p_context;
};
};

#endif
