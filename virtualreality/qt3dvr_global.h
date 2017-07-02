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

#ifndef QT3DVR_GLOBAL_H
#define QT3DVR_GLOBAL_H

#include <Qt3DCore/qt3dcore_global.h>

QT_BEGIN_NAMESPACE

#if defined(QT_SHARED) || !defined(QT_STATIC)
#  if defined(QT3DVR_LIBRARY)
#    define QT3DVR_EXPORT Q_DECL_EXPORT
#  else
#    define QT3DVR_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QT3DVR_EXPORT
#endif

QT_END_NAMESPACE

#endif // QT3DVR_GLOBAL_H
