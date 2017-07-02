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

class QT3DVR_EXPORT QVirtualRealityMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(int rings READ rings WRITE setRings NOTIFY ringsChanged)
    Q_PROPERTY(int slices READ slices WRITE setSlices NOTIFY slicesChanged)
    Q_PROPERTY( bool hasTopEndcap READ hasTopEndcap WRITE setHasTopEndcap NOTIFY hasTopEndcapChanged )
    Q_PROPERTY( bool hasBottomEndcap READ hasBottomEndcap WRITE setHasBottomEndcap NOTIFY hasBottomEndcapChanged )
    Q_PROPERTY( float topRadius READ topRadius WRITE setTopRadius NOTIFY topRadiusChanged )
    Q_PROPERTY( float bottomRadius READ bottomRadius WRITE setBottomRadius NOTIFY bottomRadiusChanged )
    Q_PROPERTY(float length READ length WRITE setLength NOTIFY lengthChanged)
public:
    explicit QVirtualRealityMesh(Qt3DCore::QNode *parent = nullptr);
    ~QVirtualRealityMesh();

    int rings() const;
    int slices() const;
    bool hasTopEndcap() const;
    bool hasBottomEndcap() const;
    float topRadius() const;
    float bottomRadius() const;
    float length() const;

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
};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QVIRTUALREALITYMESH_H
