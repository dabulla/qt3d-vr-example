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

#ifndef QVIRTUALREALITYMESH_H
#define QVIRTUALREALITYMESH_H

#include <qt3dvr_global.h>
#include <Qt3DRender/qgeometryrenderer.h>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class QVirtualRealityApiBackend; //TO DO: temp

class QT3DVR_EXPORT QVirtualRealityMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(int trackedObjectId READ trackedObjectId WRITE setTrackedObjectId NOTIFY trackedObjectIdChanged)
public:
    explicit QVirtualRealityMesh(Qt3DCore::QNode *parent = nullptr);
    ~QVirtualRealityMesh();

    int trackedObjectId() const;

    void setVrApiBackendTmp(QVirtualRealityApiBackend *apibackend); //TO DO: temp

public Q_SLOTS:

    void setTrackedObjectId(int trackedObjectId);

Q_SIGNALS:

    void trackedObjectIdChanged(int trackedObjectId);

private:
    // As this is a default provided geometry renderer, no one should be able
    // to modify the QGeometryRenderer's properties

    void setInstanceCount(int instanceCount);
    void setVertexCount(int vertexCount);
    void setIndexOffset(int indexOffset);
    void setFirstInstance(int firstInstance);
    void setRestartIndexValue(int index);
    void setPrimitiveRestartEnabled(bool enabled);
    void setGeometry(Qt3DRender::QGeometry *geometry);
    void setPrimitiveType(PrimitiveType primitiveType);
    int m_trackedObjectId;
};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QVIRTUALREALITYMESH_H
