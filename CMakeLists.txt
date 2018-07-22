cmake_minimum_required(VERSION 3.7)
project(RaySplay)

FIND_PACKAGE(wxWidgets REQUIRED core base)
IF(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})

    set(CMAKE_CXX_STANDARD 14)
    #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    include_directories(src dependencies ${wxWidgets_INCLUDE_DIRS})

    set(SOURCE_FILES src/Ray.h src/Geometry.h src/SurfaceInfo.h src/geometries/PrimitivePlane.cpp src/geometries/PrimitivePlane.h src/RaySplayConst.h src/Scene.cpp src/Scene.h src/ImageOutput.h src/ImageOutputFile.cpp src/ImageOutputFile.h src/ViewDef.h src/RayTracer.cpp src/RayTracer.h src/SampleGenerator.cpp src/SampleGenerator.h src/Samplers.cpp src/Samplers.h src/LightSource.h src/SceneDef.h src/shaders/FlatColourShader.cpp src/shaders/FlatColourShader.h src/lights/AmbientLight.cpp src/lights/AmbientLight.h src/shaders/LambertianShader.cpp src/shaders/LambertianShader.h src/lights/PointLight.cpp src/lights/PointLight.h src/SampleGeneratorTests.cpp src/SampleGeneratorTests.h src/shaders/ReflectionShader.cpp src/shaders/ReflectionShader.h src/Camera.h src/Camera.cpp src/cameras/CameraOrthographic.cpp src/cameras/CameraOrthographic.h src/cameras/CameraPerspective.cpp src/cameras/CameraPerspective.h src/geometries/PrimitiveTriangle.cpp src/geometries/PrimitiveTriangle.h src/geometries/PrimitiveTriangleBase.cpp src/geometries/PrimitiveTriangleBase.h src/geometries/Mesh.cpp src/geometries/Mesh.h src/geometries/PrimitiveBoxAA.cpp src/geometries/PrimitiveBoxAA.h src/BoundingBox.cpp src/BoundingBox.h src/geometries/GeometrySet.cpp src/geometries/GeometrySet.h src/AccelerationStructure.h src/acceleration_structures/RegularGrid.cpp src/acceleration_structures/RegularGrid.h src/geometries/GeometryReference.cpp src/geometries/GeometryReference.h src/geometries/PrimitiveSphere.cpp src/geometries/PrimitiveSphere.h src/Shader.h src/lights/DirectionLight.cpp src/lights/DirectionLight.h src/shaders/PhongShader.cpp src/shaders/PhongShader.h src/lights/AmbientOcclusion.cpp src/lights/AmbientOcclusion.h src/Texture.h src/textures/ImageTexture.cpp src/textures/ImageTexture.h src/ResourceManager.cpp src/ResourceManager.h src/shaders/NormalsTestShader.cpp src/shaders/NormalsTestShader.h src/ImageOutputWindow.cpp src/ImageOutputWindow.h src/SceneLoader.cpp src/SceneLoader.h src/tinyxml2/tinyxml2.cpp src/tinyxml2/tinyxml2.h src/geometries/ObjMeshSet.h src/app/RSApp.cpp src/app/RSApp.h src/app/RSImagePanel.cpp src/app/RSImagePanel.h)
    add_executable(RaySplay ${SOURCE_FILES} )
    add_library(RaySplayLib STATIC ${SOURCE_FILES})


    set(LIB_MATHERNOGL ${PROJECT_BINARY_DIR}/../dependencies/libMathernoGL.a)
    set(LIBS ${LIB_MATHERNOGL} GL GLEW glfw freeimage ${wxWidgets_LIBRARIES})

    target_link_libraries(RaySplay ${LIBS})
    target_link_libraries(RaySplayLib ${LIBS})
ENDIF(wxWidgets_FOUND)