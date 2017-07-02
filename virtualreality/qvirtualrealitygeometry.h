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

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
class QAttribute;
} // Render

namespace Qt3DVirtualReality {

class QVirtualRealityGeometryPrivate;

class QT3DVR_EXPORT QVirtualRealityGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT
    Q_PROPERTY( bool hasTopEndcap READ hasTopEndcap WRITE setHasTopEndcap NOTIFY hasTopEndcapChanged )
    Q_PROPERTY( bool hasBottomEndcap READ hasBottomEndcap WRITE setHasBottomEndcap NOTIFY hasBottomEndcapChanged )
    Q_PROPERTY(int rings READ rings WRITE setRings NOTIFY ringsChanged)
    Q_PROPERTY(int slices READ slices WRITE setSlices NOTIFY slicesChanged)
    Q_PROPERTY( float topRadius READ topRadius WRITE setTopRadius NOTIFY topRadiusChanged )
    Q_PROPERTY( float bottomRadius READ bottomRadius WRITE setBottomRadius NOTIFY bottomRadiusChanged )
    Q_PROPERTY(float length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(Qt3DRender::QAttribute *positionAttribute READ positionAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *normalAttribute READ normalAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *texCoordAttribute READ texCoordAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *indexAttribute READ indexAttribute CONSTANT)

public:
    explicit QVirtualRealityGeometry(QNode *parent = nullptr);
    ~QVirtualRealityGeometry();

    void updateVertices();
    void updateIndices();

    bool hasTopEndcap() const;
    bool hasBottomEndcap() const;
    float topRadius() const;
    float bottomRadius() const;
    int rings() const;
    int slices() const;
    float length() const;

    Qt3DRender::QAttribute *positionAttribute() const;
    Qt3DRender::QAttribute *normalAttribute() const;
    Qt3DRender::QAttribute *texCoordAttribute() const;
    Qt3DRender::QAttribute *indexAttribute() const;

public Q_SLOTS:
    void setHasTopEndcap( bool hasTopEndcap );
    void setHasBottomEndcap( bool hasBottomEndcap );
    void setTopRadius( float topRadius );
    void setBottomRadius( float bottomRadius );
    void setRings( int rings );
    void setSlices( int slices );
    void setLength( float length );

Q_SIGNALS:
    void hasTopEndcapChanged( bool hasTopEndcap );
    void hasBottomEndcapChanged( bool hasBottomEndcap );
    void topRadiusChanged( float topRadius );
    void bottomRadiusChanged( float bottomRadius );
    void ringsChanged( int rings );
    void slicesChanged( int slices );
    void lengthChanged( float length );

protected:
    QVirtualRealityGeometry(QVirtualRealityGeometryPrivate &dd, QNode *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(QVirtualRealityGeometry)
};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QT3DEXTRAS_QVIRTUALREALITYGEOMETRY_H
