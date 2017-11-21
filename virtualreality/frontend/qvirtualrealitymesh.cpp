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

#include "qvirtualrealitymesh.h"

#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>
#include <Qt3DRender/qattribute.h>
#include <qmath.h>
#include <QVector3D>
#include "qvirtualrealitygeometry.h"

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

#ifndef _USE_MATH_DEFINES
# define _USE_MATH_DEFINES // For MSVC
#endif


/*!
 * \qmltype VirtualrealityMesh
 * \instantiates Qt3DVirtualReality::QVirtualRealityMesh
 * \inqmlmodule Qt3D.VirtualReality
 * \brief A conical mesh.
 */

/*!
 * \qmlproperty real VirtualrealityMesh::length
 *
 * Holds the length.
 */

/*!
 * \class Qt3DVirtualReality::QVirtualrealityMesh
 * \inheaderfile Qt3DVirtualReality/QVirtualrealityMesh
 * \inmodule Qt3DVirtualReality
 *
 * \inherits Qt3DRender::QGeometryRenderer
 *
 * \brief A mesh queried from VR SDK.
 */

QVirtualRealityMesh::QVirtualRealityMesh(QNode *parent)
    : QGeometryRenderer(parent)
{
    QVirtualRealityGeometry *geometry = new QVirtualRealityGeometry(this);
    QObject::connect(geometry, &QVirtualRealityGeometry::trackedObjectIndexChanged, this, &QVirtualRealityMesh::trackedObjectIdChanged);

    QGeometryRenderer::setGeometry(geometry);
}

/*! \internal */
QVirtualRealityMesh::~QVirtualRealityMesh()
{
}

int QVirtualRealityMesh::trackedObjectId() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->trackedObjectIndex();
}

void QVirtualRealityMesh::setVrApiBackendTmp(QVirtualRealityApiBackend *apibackend)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setVrApiBackendTmp(apibackend);
}

void QVirtualRealityMesh::setTrackedObjectId(int trackedObjectId)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setTrackedObjectIndex(trackedObjectId);
}

}

// namespace Qt3DVirtualReality
QT_END_NAMESPACE
