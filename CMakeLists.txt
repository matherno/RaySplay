cmake_minimum_required(VERSION 3.7)
project(RaySplay)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pg ")

include_directories(src ../MathernoGL/include)

set(SOURCE_FILES src/Main.cpp src/Ray.h src/Geometry.h src/SurfaceInfo.h src/geometries/PrimitivePlane.cpp src/geometries/PrimitivePlane.h src/RaySplayConst.h src/Scene.cpp src/Scene.h src/ImageOutput.h src/ImageOutputFile.cpp src/ImageOutputFile.h src/ViewDef.h src/RayTracer.cpp src/RayTracer.h src/SampleGenerator.cpp src/SampleGenerator.h src/Samplers.cpp src/Samplers.h src/LightSource.h src/SceneDef.h src/shaders/FlatColourShader.cpp src/shaders/FlatColourShader.h src/lights/AmbientLight.cpp src/lights/AmbientLight.h src/shaders/LambertianShader.cpp src/shaders/LambertianShader.h src/lights/PointLight.cpp src/lights/PointLight.h src/SampleGeneratorTests.cpp src/SampleGeneratorTests.h src/shaders/ReflectionShader.cpp src/shaders/ReflectionShader.h src/Camera.h src/Camera.cpp src/cameras/CameraOrthographic.cpp src/cameras/CameraOrthographic.h src/cameras/CameraPerspective.cpp src/cameras/CameraPerspective.h src/geometries/PrimitiveTriangle.cpp src/geometries/PrimitiveTriangle.h src/geometries/PrimitiveTriangleBase.cpp src/geometries/PrimitiveTriangleBase.h src/geometries/Mesh.cpp src/geometries/Mesh.h src/geometries/PrimitiveBoxAA.cpp src/geometries/PrimitiveBoxAA.h src/BoundingBox.cpp src/BoundingBox.h src/geometries/GeometrySet.cpp src/geometries/GeometrySet.h src/AccelerationStructure.h src/acceleration_structures/RegularGrid.cpp src/acceleration_structures/RegularGrid.h src/geometries/GeometryReference.cpp src/geometries/GeometryReference.h src/geometries/PrimitiveSphere.cpp src/geometries/PrimitiveSphere.h src/Shader.h src/lights/DirectionLight.cpp src/lights/DirectionLight.h src/shaders/PhongShader.cpp src/shaders/PhongShader.h src/lights/AmbientOcclusion.cpp src/lights/AmbientOcclusion.h src/Texture.h src/textures/ImageTexture.cpp src/textures/ImageTexture.h src/ResourceManager.cpp src/ResourceManager.h)
add_executable(RaySplay ${SOURCE_FILES} )

target_link_libraries(RaySplay /home/matherno/SoftwareProjects/C++/MathernoGL/cmake-build-debug/libMathernoGL.a freeimage)
