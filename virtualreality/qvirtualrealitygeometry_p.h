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


#ifndef QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_P_H
#define QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_P_H

#include <Qt3DRender/private/qgeometry_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QAttribute;
class QBuffer;

} // namespace Qt3DRender

namespace Qt3DVirtualReality {

class QVirtualRealityGeometryPrivate : public Qt3DRender::QGeometryPrivate
{
public:
    QVirtualRealityGeometryPrivate();

    void init();

    Q_DECLARE_PUBLIC(QVirtualRealityGeometry)

    bool m_hasTopEndcap;
    bool m_hasBottomEndcap;
    int m_rings;
    int m_slices;
    float m_topRadius;
    float m_bottomRadius;
    float m_length;
    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_normalAttribute;
    Qt3DRender::QAttribute *m_texCoordAttribute;
    Qt3DRender::QAttribute *m_indexAttribute;
    Qt3DRender::QBuffer *m_positionBuffer;
    Qt3DRender::QBuffer *m_vertexBuffer;
    Qt3DRender::QBuffer *m_indexBuffer;
};

} // Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_P_H

