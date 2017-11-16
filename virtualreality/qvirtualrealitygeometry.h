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

#ifndef QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_H
#define QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_H

#include <qt3dvr_global.h>
#include <Qt3DRender/qgeometry.h>
#include <qvirtualrealityapibackend.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
class QAttribute;
} // Render

namespace Qt3DVirtualReality {

class QVirtualRealityGeometryPrivate;

class QT3DVR_EXPORT QVirtualRealityGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT
    Q_PROPERTY(int trackedObjectIndex READ trackedObjectIndex WRITE setTrackedObjectIndex NOTIFY trackedObjectIndexChanged)
    Q_PROPERTY(Qt3DRender::QAttribute *positionAttribute READ positionAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *normalAttribute READ normalAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *texCoordAttribute READ texCoordAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *indexAttribute READ indexAttribute CONSTANT)

public:
    explicit QVirtualRealityGeometry(QNode *parent = nullptr);
    ~QVirtualRealityGeometry();

    void updateVertices();
    void updateIndices();

    //TO DO: this should be there e.g. through backend/aspects
    void setVrApiBackendTmp(QVirtualRealityApiBackend *apibackend);

    int trackedObjectIndex() const;
    Qt3DRender::QAttribute *positionAttribute() const;
    Qt3DRender::QAttribute *normalAttribute() const;
    Qt3DRender::QAttribute *texCoordAttribute() const;
    Qt3DRender::QAttribute *indexAttribute() const;

public Q_SLOTS:
    void setTrackedObjectIndex(int trackedObjectIndex);

Q_SIGNALS:
    void trackedObjectIndexChanged(int trackedObjectIndex);

protected:
    QVirtualRealityGeometry(QVirtualRealityGeometryPrivate &dd, QNode *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(QVirtualRealityGeometry)
};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_H
