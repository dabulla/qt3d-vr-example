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
#ifndef QVRRENDERTARGET
#define QVRRENDERTARGET

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLFramebufferObject>
#include <QVector>
#include <iostream>

//#include "GL/CAPI_GLE.h"
#include "Extras/OVR_Math.h"
#include "OVR_CAPI_GL.h"
#include <assert.h>

#if defined(_WIN32)
    #include <dxgi.h> // for GetDefaultAdapterLuid
    //#pragma comment(lib, "dxgi.lib")
#endif

using namespace OVR;

class FramebufferOvr;

class OvrSwapChain
{
    QSize               m_texSize;
    ovrSession          m_session;
    //GLuint              m_texIdDepthBuffer;
    QOpenGLFunctions_3_2_Core *m_funcs;
    QVector<FramebufferOvr *> m_framebuffers;
    ovrTextureSwapChain m_textureChain;
public:
    OvrSwapChain(ovrSession session, QSize size);
    ~OvrSwapChain();

    QSize size() const;
    void   bindCurrentChainIndexFramebuffer();
    void   bindFramebuffer(int index);
    int    chainLength() const;
    void   commit();
    const ovrTextureSwapChain &ovrTextureChain() const;
};

#endif
#endif
