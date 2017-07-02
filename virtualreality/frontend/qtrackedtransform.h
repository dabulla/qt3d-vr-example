#ifndef QTRACKEDTRANSFORM_H
#define QTRACKEDTRANSFORM_H

#include <Qt3DCore/qcomponent.h>

#include <QtGui/qmatrix4x4.h>
#include <QtGui/qquaternion.h>
#include <QtGui/qvector3d.h>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class QTrackedTransformPrivate;

//class QTrackedTransform : public Qt3DCore::QComponent
//{
//public:
//    QTrackedTransform();
    
//    // QNode interface
//protected:
//    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &change)
//    {
//    }
    
//private:
//    Qt3DCore::QNodeCreatedChangeBasePtr createNodeCreationChange() const
//    {
//    }
//};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QTRACKEDTRANSFORM_H
