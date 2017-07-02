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

#include "QVirtualRealityMesh.h"

// TO DO: Refactor from Cone

#include "QVirtualRealityGeometry.h"
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>
#include <Qt3DRender/qattribute.h>
#include <qmath.h>
#include <QVector3D>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

#ifndef _USE_MATH_DEFINES
# define _USE_MATH_DEFINES // For MSVC
#endif

// TO DO: Refactor from Cone

#include "QVirtualRealityMesh.h"
#include "QVirtualRealityGeometry.h"
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>
#include <Qt3DRender/qattribute.h>
#include <qmath.h>
#include <QVector3D>

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
    QObject::connect(geometry, &QVirtualRealityGeometry::hasTopEndcapChanged, this, &QVirtualRealityMesh::hasTopEndcapChanged);
    QObject::connect(geometry, &QVirtualRealityGeometry::hasBottomEndcapChanged, this, &QVirtualRealityMesh::hasBottomEndcapChanged);
    QObject::connect(geometry, &QVirtualRealityGeometry::topRadiusChanged, this, &QVirtualRealityMesh::topRadiusChanged);
    QObject::connect(geometry, &QVirtualRealityGeometry::bottomRadiusChanged, this, &QVirtualRealityMesh::bottomRadiusChanged);
    QObject::connect(geometry, &QVirtualRealityGeometry::ringsChanged, this, &QVirtualRealityMesh::ringsChanged);
    QObject::connect(geometry, &QVirtualRealityGeometry::slicesChanged, this, &QVirtualRealityMesh::slicesChanged);
    QObject::connect(geometry, &QVirtualRealityGeometry::lengthChanged, this, &QVirtualRealityMesh::lengthChanged);

    QGeometryRenderer::setGeometry(geometry);
}

/*! \internal */
QVirtualRealityMesh::~QVirtualRealityMesh()
{
}

void QVirtualRealityMesh::setHasTopEndcap(bool hasTopEndcap)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setHasTopEndcap(hasTopEndcap);
}

void QVirtualRealityMesh::setHasBottomEndcap(bool hasBottomEndcap)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setHasBottomEndcap(hasBottomEndcap);
}

void QVirtualRealityMesh::setTopRadius(float topRadius)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setTopRadius(topRadius);
}

void QVirtualRealityMesh::setBottomRadius(float bottomRadius)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setBottomRadius(bottomRadius);
}

void QVirtualRealityMesh::setRings(int rings)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setRings(rings);
}

void QVirtualRealityMesh::setSlices(int slices)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setSlices(slices);
}

void QVirtualRealityMesh::setLength(float length)
{
    static_cast<QVirtualRealityGeometry *>(geometry())->setLength(length);
}

/*!
 * \property QVirtualRealityMesh::hasTopEndcap
 *
 * Determines if the virtualreality top is capped or open.
 */
bool QVirtualRealityMesh::hasTopEndcap() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->hasTopEndcap();
}

/*!
 * \property QVirtualRealityMesh::hasBottomEndcap
 *
 * Determines if the virtualreality bottom is capped or open.
 */
bool QVirtualRealityMesh::hasBottomEndcap() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->hasBottomEndcap();
}

/*!
 * \property QVirtualRealityMesh::topRadius
 *
 * Holds the top radius of the virtualreality.
 */
float QVirtualRealityMesh::topRadius() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->topRadius();
}

/*!
 * \property QVirtualRealityMesh::bottomRadius
 *
 * Holds the bottom radius of the virtualreality.
 */
float QVirtualRealityMesh::bottomRadius() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->bottomRadius();
}

/*!
 * \property QVirtualRealityMesh::rings
 *
 * Holds the number of rings in the mesh.
 */
int QVirtualRealityMesh::rings() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->rings();
}

/*!
 * \property QVirtualRealityMesh::slices
 *
 * Holds the number of slices in the mesh.
 */
int QVirtualRealityMesh::slices() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->slices();
}

/*!
 * \property QVirtualRealityMesh::length
 *
 * Holds the length of the virtualreality.
 */
float QVirtualRealityMesh::length() const
{
    return static_cast<QVirtualRealityGeometry *>(geometry())->length();
}

}

// namespace Qt3DVirtualReality
QT_END_NAMESPACE
