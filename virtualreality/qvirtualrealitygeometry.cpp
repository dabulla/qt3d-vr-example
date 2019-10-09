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

/*!
 * \class Qt3DExtras::QVirtualRealityGeometry
 * \inheaderfile Qt3DExtras/QVirtualRealityGeometry
 * \inmodule Qt3DExtras
 * \brief The QVirtualRealityGeometry class allows creation of a virtual reality representation of objects like controllers, hands, tracking cameras, lighthouses or tracking pucks in scenes.
 * \since 5.7
 * \ingroup geometries
 * \inherits Qt3DRender::QGeometry
 *
 * The QVirtualRealityGeometry class is most commonly used internally by the QVirtualrealityMesh
 * but can also be used in custom Qt3DRender::QGeometryRenderer subclasses.
 */

// TO DO: Refactor from Cone

#ifndef _USE_MATH_DEFINES
# define _USE_MATH_DEFINES // For MSVC
#endif

#include "qvirtualrealitygeometry.h"
#include "qvirtualrealitygeometry_p.h"
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>
#include <Qt3DRender/qattribute.h>
#include <QVector3D>
#include <cmath>
#include <qvirtualrealityapibackend.h>
#include <QOpenGLTexture>

QT_BEGIN_NAMESPACE

using namespace Qt3DRender;

namespace Qt3DVirtualReality {

class VirtualrealityVertexDataFunctor : public QBufferDataGenerator
{
public:
    VirtualrealityVertexDataFunctor(QVirtualRealityApiBackend *apibackend, int trackedObjectIndex)
        : m_apibackend(apibackend)
        , m_trackedObjectIndex(trackedObjectIndex)
    {}

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        QVector<float> vertices;
        QVector<int> indices;
        QOpenGLTexture tex(QOpenGLTexture::Target2D);
        m_apibackend->getTrackedObjectModel(m_trackedObjectIndex, vertices, indices, &tex);
        return QByteArray::fromRawData( reinterpret_cast<const char*>(vertices.constData()), sizeof(float) * vertices.size());
    }

    bool operator ==(const QBufferDataGenerator &other) const Q_DECL_OVERRIDE
    {
        const VirtualrealityVertexDataFunctor *otherFunctor = functor_cast<VirtualrealityVertexDataFunctor>(&other);
        if (otherFunctor != nullptr)
            return (otherFunctor->m_trackedObjectIndex == m_trackedObjectIndex
                 && otherFunctor->m_apibackend == m_apibackend);
        return false;
    }

    QT3D_FUNCTOR(VirtualrealityVertexDataFunctor)

private:
    QVirtualRealityApiBackend *m_apibackend;
    int m_trackedObjectIndex;
};

class VirtualrealityIndexDataFunctor : public QBufferDataGenerator
{
public:
    VirtualrealityIndexDataFunctor(QVirtualRealityApiBackend *apibackend, int trackedObjectIndex)
        : m_apibackend(apibackend)
        , m_trackedObjectIndex(trackedObjectIndex)
    {}

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        QVector<float> vertices;
        QVector<int> indices;
        QOpenGLTexture tex(QOpenGLTexture::Target2D);
        m_apibackend->getTrackedObjectModel(m_trackedObjectIndex, vertices, indices, &tex);
        return QByteArray::fromRawData( reinterpret_cast<const char*>(indices.constData()), sizeof(float) * indices.size());
    }

    bool operator ==(const QBufferDataGenerator &other) const Q_DECL_OVERRIDE
    {
        const VirtualrealityIndexDataFunctor *otherFunctor = functor_cast<VirtualrealityIndexDataFunctor>(&other);
        if (otherFunctor != nullptr)
            return (otherFunctor->m_trackedObjectIndex == m_trackedObjectIndex
                 && otherFunctor->m_apibackend == m_apibackend);
        return false;
    }

    QT3D_FUNCTOR(VirtualrealityIndexDataFunctor)

private:
    QVirtualRealityApiBackend *m_apibackend;
    int m_trackedObjectIndex;
};


QVirtualRealityGeometryPrivate::QVirtualRealityGeometryPrivate()
    : QGeometryPrivate()
    , m_trackedObjectIndex(-1)
    , m_positionAttribute(nullptr)
    , m_normalAttribute(nullptr)
    , m_texCoordAttribute(nullptr)
    , m_indexAttribute(nullptr)
    , m_positionBuffer(nullptr)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_apibackend(nullptr)
{
}

void QVirtualRealityGeometryPrivate::init()
{
    Q_Q(QVirtualRealityGeometry);
    m_positionAttribute = new QAttribute(q);
    m_normalAttribute = new QAttribute(q);
    m_texCoordAttribute = new QAttribute(q);
    m_indexAttribute = new QAttribute(q);
    m_vertexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, q);
    m_indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, q);

    //TO DO: Vertx count and index count lazy. do not retrieve object for this.
    // vec3 pos, vec2 tex, vec3 normal
    const quint32 elementSize = 3 + 2 + 3;
    const quint32 stride = elementSize * sizeof(float);
//    QVector<float> vertices;
//    QVector<int> indices;
//    QOpenGLTexture tex(QOpenGLTexture::Target2D);
//    m_apibackend->getTrackedObjectModel(m_trackedObjectIndex, vertices, indices, &tex);
    const int faces = 1;//indices.count()/3;
    const int nVerts = 3;//vertices.count()/elementSize;

    m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setVertexBaseType(QAttribute::Float);
    m_positionAttribute->setVertexSize(3);
    m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexBuffer);
    m_positionAttribute->setByteStride(stride);
    m_positionAttribute->setCount(nVerts);

    m_normalAttribute->setName(QAttribute::defaultNormalAttributeName());
    m_normalAttribute->setVertexBaseType(QAttribute::Float);
    m_normalAttribute->setVertexSize(3);
    m_normalAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_normalAttribute->setBuffer(m_vertexBuffer);
    m_normalAttribute->setByteStride(stride);
    m_normalAttribute->setByteOffset(3 * sizeof(float));
    m_normalAttribute->setCount(nVerts);

    m_texCoordAttribute->setName(QAttribute::defaultTextureCoordinateAttributeName());
    m_texCoordAttribute->setVertexBaseType(QAttribute::Float);
    m_texCoordAttribute->setVertexSize(2);
    m_texCoordAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_texCoordAttribute->setBuffer(m_vertexBuffer);
    m_texCoordAttribute->setByteStride(stride);
    m_texCoordAttribute->setByteOffset(6 * sizeof(float));
    m_texCoordAttribute->setCount(nVerts);

    m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    m_indexAttribute->setVertexBaseType(QAttribute::UnsignedInt);
    m_indexAttribute->setBuffer(m_indexBuffer);

    m_indexAttribute->setCount(faces * 3);

    //m_vertexBuffer->setDataGenerator(QSharedPointer<VirtualrealityVertexDataFunctor>::create(m_apibackend, m_trackedObjectIndex));
    //m_indexBuffer->setDataGenerator(QSharedPointer<VirtualrealityIndexDataFunctor>::create(m_apibackend, m_trackedObjectIndex));

    q->addAttribute(m_positionAttribute);
    q->addAttribute(m_normalAttribute);
    q->addAttribute(m_texCoordAttribute);
    q->addAttribute(m_indexAttribute);
}

/*!
 * \qmltype VirtualrealityGeometry
 * \instantiates Qt3DExtras::QVirtualRealityGeometry
 * \inqmlmodule Qt3D.Extras
 * \brief VirtualrealityGeometry allows creation of a virtualreality in 3D space.
 *
 * The VirtualrealityGeometry type is most commonly used internally by the VirtualrealityMesh type
 * but can also be used in custom GeometryRenderer types..
 */

/*!
 * \qmlproperty Attribute VirtualrealityGeometry::positionAttribute
 *
 * Holds the geometry position attribute.
 */

/*!
 * \qmlproperty Attribute VirtualrealityGeometry::normalAttribute
 *
 * Holds the geometry normal attribute.
 */

/*!
 * \qmlproperty Attribute VirtualrealityGeometry::texCoordAttribute
 *
 * Holds the geometry texture coordinate attribute.
 */

/*!
 * \qmlproperty Attribute VirtualrealityGeometry::indexAttribute
 *
 * Holds the geometry index attribute.
 */

QVirtualRealityGeometry::QVirtualRealityGeometry(QNode *parent)
    : QGeometry(*new QVirtualRealityGeometryPrivate, parent)
{
    Q_D(QVirtualRealityGeometry);
    d->init();
}

QVirtualRealityGeometry::QVirtualRealityGeometry(QVirtualRealityGeometryPrivate &dd, QNode *parent)
    :QGeometry(dd, parent)
{
    Q_D(QVirtualRealityGeometry);
    d->init();
}


/*! \internal */
QVirtualRealityGeometry::~QVirtualRealityGeometry()
{
}

/*!
 * Updates vertices based on geometry properties.
 */
void QVirtualRealityGeometry::updateVertices()
{
    Q_D(QVirtualRealityGeometry);

    if(!d->m_apibackend) return;
    //TO DO: do not query whole object here. lazy loading of vertex count
    QVector<float> vertices;
    QVector<int> indices;
    QOpenGLTexture tex(QOpenGLTexture::Target2D);
    d->m_apibackend->getTrackedObjectModel(d->m_trackedObjectIndex, vertices, indices, &tex);

    d->m_positionAttribute->setCount(vertices.count()/8);
    d->m_texCoordAttribute->setCount(vertices.count()/8);
    d->m_normalAttribute->setCount(vertices.count()/8);
    d->m_vertexBuffer->setDataGenerator(QSharedPointer<VirtualrealityVertexDataFunctor>::create(d->m_apibackend, d->m_trackedObjectIndex));
}

/*!
 * Updates indices based on geometry properties.
 */
void QVirtualRealityGeometry::updateIndices()
{
    Q_D(QVirtualRealityGeometry);

    if(!d->m_apibackend) return;
    //TO DO: do not query whole object here. lazy loading of vertex count
    QVector<float> vertices;
    QVector<int> indices;
    QOpenGLTexture tex(QOpenGLTexture::Target2D);
    d->m_apibackend->getTrackedObjectModel(d->m_trackedObjectIndex, vertices, indices, &tex);

    d->m_indexAttribute->setCount(indices.count());
    d->m_indexBuffer->setDataGenerator(QSharedPointer<VirtualrealityIndexDataFunctor>::create(d->m_apibackend, d->m_trackedObjectIndex));
}

void QVirtualRealityGeometry::setVrApiBackendTmp(QVirtualRealityApiBackend *apibackend)
{
    Q_D(QVirtualRealityGeometry);
    d->m_apibackend = apibackend;
    updateVertices();
    updateIndices();
}

int QVirtualRealityGeometry::trackedObjectIndex() const
{
    const Q_D(QVirtualRealityGeometry);
    return d->m_trackedObjectIndex;
}

/*!
 * \property QVirtualRealityGeometry::positionAttribute
 *
 * Holds the geometry position attribute.
 */

/*!
 * \property QVirtualRealityGeometry::normalAttribute
 *
 * Holds the geometry normal attribute.
 */

/*!
 * \property QVirtualRealityGeometry::texCoordAttribute
 *
 * Holds the geometry texture coordinate attribute.
 */

/*!
 * \property QVirtualRealityGeometry::indexAttribute
 *
 * Holds the geometry index attribute.
 */


QAttribute *QVirtualRealityGeometry::positionAttribute() const
{
    Q_D(const QVirtualRealityGeometry);
    return d->m_positionAttribute;
}

QAttribute *QVirtualRealityGeometry::normalAttribute() const
{
    Q_D(const QVirtualRealityGeometry);
    return d->m_normalAttribute;
}

QAttribute *QVirtualRealityGeometry::texCoordAttribute() const
{
    Q_D(const QVirtualRealityGeometry);
    return d->m_texCoordAttribute;
}

QAttribute *QVirtualRealityGeometry::indexAttribute() const
{
    Q_D(const QVirtualRealityGeometry);
    return d->m_indexAttribute;
}

void QVirtualRealityGeometry::setTrackedObjectIndex(int trackedObjectIndex)
{
    Q_D(QVirtualRealityGeometry);
    if (trackedObjectIndex != d->m_trackedObjectIndex) {
        d->m_trackedObjectIndex = trackedObjectIndex;
        updateVertices();
        updateIndices();
        Q_EMIT trackedObjectIndexChanged(trackedObjectIndex);
    }
}

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE
