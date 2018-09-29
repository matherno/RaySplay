//
// Created by matt on 23/09/18.
//

#include <SceneLoader.h>
#include <cameras/CameraPerspective.h>
#include "RSWalkthroughWindow.h"

static const string vertexShader = ""
  "#version 330\n"
  ""
  "layout(location = 0) in vec4 inVert;"
  ""
  "out vec3 pixelPos;"
  ""
  "uniform int screenWidth = 800;"
  "uniform int screenHeight = 800;"
  ""
  "void main()"
  "  {"
  "  pixelPos = vec3(inVert.x, inVert.y, inVert.z);"
  "  pixelPos *= 0.5;"
  "  pixelPos += 0.5;"
  "  pixelPos.x *= screenWidth;"
  "  pixelPos.y = 1 - pixelPos.y;"
  "  pixelPos.y *= screenHeight;"
  "  gl_Position = inVert;"
  "  }";

static const string fragShaderMain = ""
  "#version 330\n"
  ""
  "#define EPSILON 1e-2\n"
  ""
  "out vec4 outputColour;"
  ""
  "in vec3 pixelPos;"
  ""
  "uniform int screenWidth = 800;"
  "uniform int screenHeight = 800;"
  "uniform vec3 camPosition = vec3(0, 0, 0);"
  "uniform vec3 camDirection = vec3(0, 0, -1);"
  "uniform vec3 camUp = vec3(0, 1, 0);"
  "uniform float fov = 1;"
  "uniform float viewPlaneOffset = 1;"
  "uniform float invGamma = 1;"
  "uniform vec3 backgroundCol = vec3(0, 0, 0);"
  ""
  "float hitTestScene(vec3 rayOrigin, vec3 rayDir, inout vec3 hitNormal, inout vec3 hitColour); "
  "vec3 shadeSurface(vec3 surfacePos, vec3 surfaceNormal, vec3 viewDir); "
  ""
  "void main()"
  "  {"
  ""
  "  vec3 rayOrigin;"
  "  vec3 rayDir;"
  ""
  "  float pixelSize = fov / screenWidth;\n"
  "  rayOrigin.x = pixelPos.x - (screenWidth / 2.0f);\n"
  "  rayOrigin.y = float(screenHeight) - pixelPos.y - (screenHeight / 2.0f);\n"
  "  rayOrigin *= pixelSize;\n"
  ""
  "  vec3 u, v, w;"
  "  w = camDirection * -1;\n"
  "  w = normalize(w);\n"
  "  u = cross(camUp, w);\n"
  "  u = normalize(u);\n"
  "  v = cross(w, u);\n"
  ""
  "  rayOrigin = u * rayOrigin.x + v * rayOrigin.y;\n"
  "  rayOrigin += camPosition;\n"
  "  rayOrigin += camDirection * viewPlaneOffset;\n"
  ""
  "  rayDir = normalize(rayOrigin - camPosition);"
  ""
  "  vec3 hitColour, hitNormal;"
  "  float tValue = hitTestScene(rayOrigin, rayDir, hitNormal, hitColour);"
  "  if (tValue > 0)"
  "    outputColour = vec4(hitColour * shadeSurface(rayOrigin + rayDir * tValue, hitNormal, rayDir * -1), 1);"
  "  else"
  "    outputColour = vec4(backgroundCol, 1);"
  "  outputColour.x = pow(outputColour.x, invGamma);"
  "  outputColour.y = pow(outputColour.y, invGamma);"
  "  outputColour.z = pow(outputColour.z, invGamma);"
  "  }"
  ""
  "";

static const string triHitTestFunc = "\n"
  "float hitTestTriangle(vec3 rayOrigin, vec3 rayDir, vec3 cornerA, vec3 cornerB, vec3 cornerC, vec3 normalA, vec3 normalB, vec3 normalC, inout vec3 triNormal)\n"
  "{"
  "float triTValue = -1;"
  "float a = cornerA.x - cornerB.x;\n"
  "float b = cornerA.x - cornerC.x;\n"
  "float c = rayDir.x;\n"
  "float d = cornerA.x - rayOrigin.x;\n"
  "float e = cornerA.y - cornerB.y;\n"
  "float f = cornerA.y - cornerC.y;\n"
  "float g = rayDir.y;\n"
  "float h = cornerA.y - rayOrigin.y;\n"
  "float i = cornerA.z - cornerB.z;\n"
  "float j = cornerA.z - cornerC.z;\n"
  "float k = rayDir.z;\n"
  "float l = cornerA.z - rayOrigin.z;\n"
  ""
  "float m = f*k - g*j;\n"
  "float n = h*k - g*l;\n"
  "float p = f*l - h*j;\n"
  "float q = g*i - e*k;\n"
  "float s = e*j - f*i;\n"
  ""
  "float invDenominator = 1.0f / (a*m + b*q + c*s);\n"
  "float e1 = d*m - b*n - c*p;\n"
  "float beta = e1 * invDenominator;\n"
  ""
  "if(beta >= 0)\n"
  "  {\n"
  "  float r = e*l - h*i;\n"
  "  float e2 = a*n + d*q + c*r;\n"
  "  float gamma = e2 * invDenominator;\n"
  "  "
  "  if(gamma >= 0 && beta + gamma <= 1)"
  "    {"
  "    float e3 = a*p - b*r + d*s;\n"
  "    float tValue = e3 * invDenominator;\n"
  "    "
  "    if(tValue > EPSILON)"
  "      {"
  "      triTValue = tValue;"
  "      triNormal = vec3(0, 0, 0);"
  "      triNormal += normalA * 1.0 - beta - gamma;"
  "      triNormal += normalB * beta;"
  "      triNormal += normalC * gamma;"
  "      triNormal = normalize(triNormal);"
  "      }"
  "    }"
  " }"
  ""
  "return triTValue;"
  "}\n";

void RSWalkthroughWindow::startWalkthrough(const string& sceneFilePath)
  {
  mathernogl::logInfo("Starting walkthrough: " + sceneFilePath);
  Scene scene;
  mathernogl::logInfo("-- Building Scene...");
  if(!SceneLoader::loadSceneFromXML(sceneFilePath, &scene))
    return;

  ViewDef* viewDef = scene.getViewDef();
  for(GeometryPtr geometry : scene.getSceneDef()->geometries)
    geometry->init();
  mathernogl::logInfo("-- Creating shader code...");
  const string fragmentShader = fragShaderMain + triHitTestFunc + createGLSLHitTestScene(&scene) + createGLSLShadeSurface(&scene);

  try
    {
    mathernogl::logInfo("-- Initialising window and OpenGL...");
    mathernogl::clearGLErrors();
    window.reset(mathernogl::initGL("RaySplay Walkthrough: " + sceneFilePath, viewDef->width, viewDef->height, false, false));

    mathernogl::logInfo("-- Compiling Shaders...");
    shaderProgram.initFromSrc(vertexShader, fragmentShader);
    shaderProgram.enable();
    shaderProgram.setVarInt("screenWidth", viewDef->width);
    shaderProgram.setVarInt("screenHeight", viewDef->height);
    shaderProgram.setVarFloat("fov", viewDef->fov);
    shaderProgram.setVarFloat("invGamma", viewDef->invGamma);
    shaderProgram.setVarVec3("backgroundCol", scene.getSceneDef()->bgColour);
    CameraPerspective* cam = dynamic_cast<CameraPerspective*> (scene.getCamera().get());
    if (cam->getViewPlaneOffset())
      {
      camPosition = cam->getPosition();
      setCamRotation(cam->getViewDirection());
      shaderProgram.setVarFloat("viewPlaneOffset", cam->getViewPlaneOffset());
      }

    mathernogl::GPUBufferStatic buffer;
    buffer.init();
    buffer.copyDataFloat(
      {
        -1, -1, 0,
        1, -1, 0,
        1, 1, 0,
        -1, -1, 0,
        1, 1, 0,
        -1, 1, 0,
      });

    vao.init();
    vao.bind();
    vao.linkBufferAsFloats(buffer, 3, 0, false);
    vao.unbind();
    buffer.cleanUp();
    vao.bind();

    inputHandler.init(window->getGLFWWindow());
    inputHandler.setCursorMode(mathernogl::CursorMode::CURSOR_DISABLED);

    ASSERT_NO_GL_ERROR();
    mathernogl::logInfo("Finished preparing walkthrough");
    }
  catch (std::runtime_error err)
    {
    mathernogl::logError(err.what());
//    mathernogl::logInfo("\nFragment Shader Code: \n" + fragmentShader);
    if (window && window->isOpen())
      window->close();
    window.reset();
    }
  }

bool RSWalkthroughWindow::onUpdate()
  {
  if (window && window->isOpen())
    {
    mathernogl::clearGLErrors();

    inputHandler.checkHeldButtons();

    if(inputHandler.isKeyAction(GLFW_KEY_ESCAPE, mathernogl::InputAction::INPUT_PRESSED))
      {
      endWalkthrough();
      return false;
      }

    static const double moveSpeed = 0.5;
    Vector3D camMovement;
    if(inputHandler.isKeyAction(GLFW_KEY_W, mathernogl::InputAction::INPUT_HELD))
      camMovement.z -= moveSpeed;
    if(inputHandler.isKeyAction(GLFW_KEY_S, mathernogl::InputAction::INPUT_HELD))
      camMovement.z += moveSpeed;
    if(inputHandler.isKeyAction(GLFW_KEY_A, mathernogl::InputAction::INPUT_HELD))
      camMovement.x -= moveSpeed;
    if(inputHandler.isKeyAction(GLFW_KEY_D, mathernogl::InputAction::INPUT_HELD))
      camMovement.x += moveSpeed;

    static const double lookSpeed = 0.2;
    const Vector2D mouseOffset = inputHandler.getMouseOffset();
    yaw -= mouseOffset.x * lookSpeed;
    pitch -= mouseOffset.y * lookSpeed;

    mathernogl::Matrix4 rotation = mathernogl::matrixRotate(0, 1, 0, yaw);
    camMovement *= rotation;

    rotation *= mathernogl::matrixRotate(Vector3D(1, 0, 0) * rotation, pitch);
    Vector3D camDirection = Vector3D(0, 0, -1) * rotation;
    camDirection.makeUniform();

    if(inputHandler.isKeyAction(GLFW_KEY_SPACE, mathernogl::InputAction::INPUT_HELD))
      camMovement.y += moveSpeed * 0.5;
    if(inputHandler.isKeyAction(GLFW_KEY_LEFT_CONTROL, mathernogl::InputAction::INPUT_HELD))
      camMovement.y -= moveSpeed * 0.5;
    if(inputHandler.isKeyAction(GLFW_KEY_LEFT_SHIFT, mathernogl::InputAction::INPUT_HELD))
      camMovement *= 2;

    camPosition += camMovement;

    shaderProgram.setVarVec3("camPosition", camPosition);
    shaderProgram.setVarVec3("camDirection", camDirection);

    inputHandler.clearEvents();
    window->clear();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    window->update();
    ASSERT_NO_GL_ERROR();
    return true;
    }

  if (window)
    endWalkthrough();

  return false;
  }

void RSWalkthroughWindow::endWalkthrough()
  {
  inputHandler.setCursorMode(mathernogl::CursorMode::CURSOR_ENABLED);
  inputHandler.cleanUp();
  vao.cleanUp();
  shaderProgram.cleanUp();
  window->close();
  window.reset();
  }

static string constructGLSLHitTestSingleGeometry(GeometryPtr geometry)
  {
  string glsl;
  glsl += "{"
    "float thisTValue = -1;"
    "vec3 thisNormal;";

  glsl += geometry->constructGLSLHitTest();

  ShaderPtr shader = geometry->getMaterial();
  if (shader && shader->isEmissive())
    return "";
  const Vector3D colour = shader ? shader->getSimplifiedSurfaceColour() : Vector3D(1, 0, 1);

  glsl += ""
    "if (thisTValue > 0 && (hitTValue < 0 || thisTValue < hitTValue))"
    "  {"
    "  hitTValue = thisTValue;"
    "  hitNormal = thisNormal;"
    "  hitColour = vec3(" + std::to_string(colour.x) + ", " + std::to_string(colour.y) + ", " + std::to_string(colour.z) + ");"
    "  }"
    "}";
  return glsl;
  }

static string constructGLSLHitTestGeometry(GeometryPtr geometry)
  {
  string glsl;
  GeometrySet* geometrySet = dynamic_cast<GeometrySet*> (geometry.get());
  if (geometrySet)
    {
    for (GeometryPtr geometry : *geometrySet)
      glsl += constructGLSLHitTestGeometry(geometry);
    }
  else
    {
    glsl += constructGLSLHitTestSingleGeometry(geometry);
    }
  return glsl;
  }

string RSWalkthroughWindow::createGLSLHitTestScene(Scene* scene)
  {
  string glsl;
  glsl += ""
    "float hitTestScene(vec3 rayOrigin, vec3 rayDir, inout vec3 hitNormal, inout vec3 hitColour)"
    "  {"
    "  float hitTValue = -1;";

  for (GeometryPtr geometry : scene->getSceneDef()->geometries)
    glsl += constructGLSLHitTestGeometry(geometry);

  glsl += ""
    "return hitTValue;"
    "}";
  return glsl;
  }

void RSWalkthroughWindow::setCamRotation(const Vector3D& camViewDirection)
  {
  Vector3D rotatedZ(camViewDirection.x, 0, camViewDirection.z);
  if (camViewDirection.y == 1)
    rotatedZ.set(0, 0, 1);
  rotatedZ.makeUniform();
  yaw = acos(mathernogl::dotProduct(rotatedZ, Vector3D(0, 0, -1)));
  pitch = acos(mathernogl::dotProduct(rotatedZ, camViewDirection));
  yaw = -1 * mathernogl::radToDeg(yaw);
  pitch = -1 * mathernogl::radToDeg(pitch);
  }

string RSWalkthroughWindow::createGLSLShadeSurface(Scene* scene)
  {
  string glsl = ""
    "vec3 shadeSurface(vec3 surfacePos, vec3 surfaceNormal, vec3 viewDir) "
    "  {"
    "  vec3 surfaceCol = vec3(0, 0, 0);";

  for (LightSourcePtr lightSource : scene->getSceneDef()->lights)
    {
    glsl += ""
      "{"
        "vec3 lightCol = vec3(0, 0, 0);"
        "vec3 lightDir = vec3(0, 0, 0);"
        "{";
      glsl += lightSource->constructGLSLLightSurface();
      glsl += ""
        "}"
        "float lightFactor = 1;"
        "if (lightDir.x != 0 || lightDir.y != 0 || lightDir.z != 0)"
        "  lightFactor = clamp(dot(lightDir, surfaceNormal), 0, 1);"
        "surfaceCol += lightCol * lightFactor;"
      "}";
    }

  glsl += ""
    "  return surfaceCol;"
    "  }"
    "";

  return glsl;
  }







