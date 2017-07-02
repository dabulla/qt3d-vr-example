# Custom Configuration

###### OpenVR ######
# Put openvr_api.dll in executable folder
# Incompatible configuration of the dll can be visualized using the tool "dependency walker"

WITH_VR_SDK_OPENVR = true
VR_SDK_OPENVR_INCLUDE = "D:/devel/openvr/headers"
VR_SDK_OPENVR_LIB_PATH = "D:/devel/openvr/lib/win64"


###### OculusVR ######
# LibOVR.lib compiles with static runtime by default (/MT, /MTd).
# This can be changed by recompiling the lib using visual studio (simple task)
# For 32bit you might add "_ITERATOR_DEBUG_LEVEL=0" under C/C++ -> Preprocessor -> Preprocessordefinitions

WITH_VR_SDK_OCULUSVR = true
VR_SDK_OCULUSVR_INCLUDE = "D:/devel/OculusSDK/LibOVR/Include"
#VR_SDK_OCULUSVR_LIB_PATH = "D:/devel/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2015"
VR_SDK_OCULUSVR_LIB_PATH = "D:/devel/OculusSDK/LibOVR/Lib/Windows/x64/Debug/VS2015"


###### OSVR ######
# Not yet implemented

WITH_VR_SDK_OSVR = false

###### OpenXR ######

WITH_VR_OPENXR = false
