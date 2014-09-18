TEMPLATE = lib
CONFIG  += shared
CONFIG  -= app_bundle
QT      += core gui

DESTDIR = "C:/Program Files (x86)/Vireio/Perception/bin"

INCLUDEPATH +=            \
    ../../common          \
    ../../VRboostReferee  \
    ../src/Streaming      \
    $$(DXSDK_DIR)/Include \
    $$(FREESPACE)/include  \
    $$(LIBOVR)/Include     \
    $$(FFMPEG)/include


LIBS += \
    -L$$(DXSDK_DIR)/Lib/x86 \
    -L$$(FREESPACE)/lib \
    -L$$(FFMPEG)/lib \
    -L$$(LIBOVR)/Lib/Win32/VS2012 \
    -L$$(WIN_DDK_DIR)/lib/ATL/i386 \
    -lAdvapi32 \
    -ld3dx9 \
    -ld3d9 \
    -ldinput8 \
    -ldxerr \
    -ldxguid \
    -ldinput8 \
    -lxinput \
    -lavcodec \
    -lavformat \
    -lavutil \
    -lswscale \
    -lws2_32 \
    -llibfreespace \
    -llibovr \
    -lwinmm \
    -lgdi32

SOURCES += \
    ../src/Main.cpp \
    ../../common/cStereoMode.cpp \
    ../../common/cPropsFile.cpp \
    ../../common/cGameProfile.cpp \
    ../../common/cUser.cpp \
    ../../common/cSettings.cpp \
    ../../common/cGame.cpp \
    ../../common/cConfig.cpp \
    ../src/D3D9ProxyCubeTexture.cpp \
    ../src/D3D9ProxyDirect3D.cpp \
    ../src/D3D9ProxyIndexBuffer.cpp \
    ../src/D3D9ProxyPixelShader.cpp \
    ../src/D3D9ProxyQuery.cpp \
    ../src/D3D9ProxyStateBlock.cpp \
    ../src/D3D9ProxySurface.cpp \
    ../src/D3D9ProxySwapChain.cpp \
    ../src/D3D9ProxyTexture.cpp \
    ../src/D3D9ProxyVertexBuffer.cpp \
    ../src/D3D9ProxyVertexDeclaration.cpp \
    ../src/D3D9ProxyVertexShader.cpp \
    ../src/D3D9ProxyVolume.cpp \
    ../src/D3D9ProxyVolumeTexture.cpp \
    ../src/D3DProxyDevice.cpp \
    ../src/D3DProxyDeviceAdv.cpp \
    ../src/D3DProxyDeviceDebug.cpp \
    ../src/D3DProxyDeviceEgo.cpp \
    ../src/D3DProxyDeviceFactory.cpp \
    ../src/DataGatherer.cpp \
    ../src/DirectInput.cpp \
    ../src/FreeSpaceTracker.cpp \
    ../src/FreeTrackTracker.cpp \
    ../src/GameHandler.cpp \
    ../src/InputControls.cpp \
    ../src/Main.cpp \
    ../src/MatrixNoShowModification.cpp \
    ../src/MotionTracker.cpp \
    ../src/MotionTrackerFactory.cpp \
    ../src/MurmurHash3.cpp \
    ../src/OculusTracker.cpp \
    ../src/pugixml.cpp \
    ../src/ShaderModificationRepository.cpp \
    ../src/ShaderRegisters.cpp \
    ../src/SharedMemoryTracker.cpp \
    ../src/SocketTracker.cpp \
    ../src/StereoBackbuffer.cpp \
    ../src/StereoView.cpp \
    ../src/ViewAdjustment.cpp \
    ../src/Vireio.cpp \
    ../src/Streaming/Streamer.cpp \
    ../src/Streaming/Streamer-enc-ffmpeg.cpp \
    ../src/Streaming/Streamer-net-udp.cpp \
    ../src/D3D9ProxyDeviceEx.cpp \
    ../src/D3D9ProxyDirect3DEx.cpp

HEADERS += \
    ../src/D3D9ProxyCubeTexture.h \
    ../src/D3D9ProxyDirect3D.h \
    ../src/D3D9ProxyIndexBuffer.h \
    ../src/D3D9ProxyPixelShader.h \
    ../src/D3D9ProxyQuery.h \
    ../src/D3D9ProxyStateBlock.h \
    ../src/D3D9ProxySurface.h \
    ../src/D3D9ProxySwapChain.h \
    ../src/D3D9ProxyTexture.h \
    ../src/D3D9ProxyVertexBuffer.h \
    ../src/D3D9ProxyVertexDeclaration.h \
    ../src/D3D9ProxyVertexShader.h \
    ../src/D3D9ProxyVolume.h \
    ../src/D3D9ProxyVolumeTexture.h \
    ../src/D3DProxyDevice.h \
    ../src/D3DProxyDeviceAdv.h \
    ../src/D3DProxyDeviceDebug.h \
    ../src/D3DProxyDeviceEgo.h \
    ../src/D3DProxyDeviceFactory.h \
    ../src/DataGatherer.h \
    ../src/Direct3D9Ex.h \
    ../src/Direct3DDevice9Ex.h \
    ../src/DirectInput.h \
    ../src/FreeSpaceTracker.h \
    ../src/FreeTrackTracker.h \
    ../src/GameHandler.h \
    ../src/InputControls.h \
    ../src/IStereoCapableWrapper.h \
    ../src/MatrixConvOffsetAdjustment.h \
    ../src/MatrixDoNothing.h \
    ../src/MatrixGatheredOrhoSquash.h \
    ../src/MatrixGuiSquash.h \
    ../src/MatrixHudSlide.h \
    ../src/MatrixHudSquash.h \
    ../src/MatrixIgnoreOrtho.h \
    ../src/MatrixNoRoll.h \
    ../src/MatrixNoShowModification.h \
    ../src/MatrixOrthoSquash.h \
    ../src/MatrixOrthoSquashHud.h \
    ../src/MatrixOrthoSquashShifted.h \
    ../src/MatrixRollOnly.h \
    ../src/MatrixRollOnlyHalf.h \
    ../src/MatrixRollOnlyNegative.h \
    ../src/MatrixSurfaceRefractionTransform.h \
    ../src/MotionTracker.h \
    ../src/MotionTrackerFactory.h \
    ../src/MurmurHash3.h \
    ../src/OculusTracker.h \
    ../src/pugiconfig.hpp \
    ../src/pugixml.hpp \
    ../src/ShaderConstantModification.h \
    ../src/ShaderConstantModificationFactory.h \
    ../src/ShaderMatrixModification.h \
    ../src/ShaderModificationRepository.h \
    ../src/ShaderRegisters.h \
    ../src/SharedMemoryTracker.h \
    ../src/SocketTracker.h \
    ../src/StereoBackbuffer.h \
    ../src/StereoShaderConstant.h \
    ../src/StereoView.h \
    ../src/Vector4EyeShiftUnity.h \
    ../src/Vector4SimpleTranslate.h \
    ../src/Version.h \
    ../src/ViewAdjustment.h \
    ../src/Vireio.h \
    ../src/Streaming/inttypes.h \
    ../src/Streaming/Streamer.h \
    ../../common/cStereoMode.h \
    ../../common/cGame.h \
    ../../common/cGameProfile.h \
    ../../common/cPropsFile.h \
    ../../common/cSettings.h \
    ../../common/cConfig.h \
    ../../common/cUser.h \
    ../../common/VireIO.h

OTHER_FILES += \
    ../src/Exports.def \
    ../src/SideBySideRiftPartialCrop.fx



