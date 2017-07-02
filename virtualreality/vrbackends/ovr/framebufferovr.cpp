//****************************************************************************
//**
//** Author: Daniel Bulla
//** Contact: qt3d-vr@danielbulla.de
//**
//** GNU Lesser General Public License Usage
//** General Public License version 3 as published by the Free Software
//** Foundation and appearing in the file LICENSE.LGPL3 included in the
//** packaging of this file. Please review the following information to
//** ensure the GNU Lesser General Public License version 3 requirements
//** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
//**
//****************************************************************************/

#if(QT3DVR_COMPILE_WITH_OCULUSVR)
#include "framebufferovr.h"
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLFramebufferObject>
#include <QDebug>

class FramebufferOvr
{
    GLuint m_colorAttachment;
    GLuint m_depthBuffer;
    GLuint m_framebuffer;
    QOpenGLFunctions_3_2_Core *m_funcs;
public:
    FramebufferOvr(GLuint colorAttachment0TextureId, GLuint depthBufferRenderbufferId, QOpenGLFunctions_3_2_Core *funcs)
        : m_colorAttachment(colorAttachment0TextureId)
        , m_depthBuffer(depthBufferRenderbufferId)
        , m_funcs(funcs)
    {
        m_funcs->glGenFramebuffers(1, &m_framebuffer);
        m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        m_funcs->glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if(depthBufferRenderbufferId > 0) {
            Q_ASSERT(funcs->glIsRenderbuffer(m_depthBuffer));
            funcs->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                         GL_RENDERBUFFER, m_depthBuffer);
        }
        int sampleCount = 1;
        GLenum texTarget = (sampleCount > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        m_funcs->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texTarget, m_colorAttachment, 0);

        m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    ~FramebufferOvr() {

    }

    GLuint getTextureId() {
        return m_colorAttachment;
    }
    void bind() {
        m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    }
};

OvrSwapChain::OvrSwapChain( ovrSession session, QSize size)
    :m_session(session),
     m_texSize(size),
     m_textureChain(0)
{
    m_funcs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    if(!m_funcs)
    {
        qDebug() << "Could not get OpenGLFunctions 3.2";
        return;
    }
    ovrTextureSwapChainDesc desc = {};
    desc.Type = ovrTexture_2D;
    desc.ArraySize = 1;
    desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
    desc.Width = m_texSize.width();
    desc.Height = m_texSize.height();
    desc.MipLevels = 1;
    desc.SampleCount = 1;
    desc.StaticImage = ovrFalse;

    // We have to use the internally created texture from this method.
    // No way to use a native QOpenGLTexture for rendering, without copying it.
    ovrResult result = ovr_CreateTextureSwapChainGL(m_session, &desc, &m_textureChain);

    if(!OVR_SUCCESS(result))
    {
        ovrErrorInfo inf;
        ovr_GetLastErrorInfo(&inf);
        qDebug() << inf.ErrorString;
    }
    int length = 0;
    ovr_GetTextureSwapChainLength(m_session, m_textureChain, &length);

    if(OVR_SUCCESS(result))
    {
        GLuint depthBufferId;
        m_funcs->glGenRenderbuffers(1, &depthBufferId);
        m_funcs->glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
        Q_ASSERT(m_funcs->glIsRenderbuffer(depthBufferId));
        m_funcs->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_texSize.width(), m_texSize.height());

        for (int i = 0; i < length; ++i)
        {
            GLuint chainTextureId;
            ovr_GetTextureSwapChainBufferGL(m_session, m_textureChain, i, &chainTextureId);
            m_framebuffers.push_back(new FramebufferOvr(chainTextureId, depthBufferId, m_funcs));
        }
    }

    if (!m_textureChain)
    {
        ovrErrorInfo inf;
        ovr_GetLastErrorInfo(&inf);
        qDebug() << inf.ErrorString;
    }
}

OvrSwapChain::~OvrSwapChain()
{
    if (m_textureChain)
    {
        ovr_DestroyTextureSwapChain(m_session, m_textureChain);
        m_textureChain = nullptr;
    }
    for(QVector<FramebufferOvr *>::iterator iter(m_framebuffers.begin()) ; iter != m_framebuffers.end() ; ++iter) {
        delete *iter;
    }
}

QSize OvrSwapChain::size() const
{
    return m_texSize;
}

void OvrSwapChain::commit()
{
    Q_ASSERT(m_textureChain != nullptr);
    ovr_CommitTextureSwapChain(m_session, m_textureChain);
}

const ovrTextureSwapChain& OvrSwapChain::ovrTextureChain() const
{
    return m_textureChain;
}

void OvrSwapChain::bindCurrentChainIndexFramebuffer()
{
    Q_ASSERT(m_textureChain != nullptr);
    int curIndex;
    ovr_GetTextureSwapChainCurrentIndex(m_session, m_textureChain, &curIndex);
    m_framebuffers[curIndex]->bind();
}

void OvrSwapChain::bindFramebuffer(int index)
{
    //nyi
}

int OvrSwapChain::chainLength() const
{
    //nyi
    return 0;
}
#endif
