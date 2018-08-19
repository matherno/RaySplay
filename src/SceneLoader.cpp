//
// Created by matt on 28/04/18.
//

#include <geometries/Mesh.h>
#include <shaders/PhongShader.h>
#include <shaders/LambertianShader.h>
#include <cameras/CameraPerspective.h>
#include <cameras/CameraOrthographic.h>
#include <geometries/PrimitiveBoxAA.h>
#include <geometries/GeometryReference.h>
#include <shaders/ReflectionShader.h>
#include <lights/AreaLight.h>
#include <shaders/EmissionShader.h>
#include "SceneLoader.h"
#include "RaySplayConst.h"
#include "tinyxml2/tinyxml2.h"
#include "lights/AmbientLight.h"
#include "lights/AmbientOcclusion.h"
#include "lights/DirectionLight.h"
#include "lights/PointLight.h"

#define CURRENT_XML_VERSION 1

#define ROOT_SCENE "RaySplayScene"
#define VERSION "Version"
#define TIMESTAMP "TimeSaved"

#define GEOMETRIES "Geometries"
#define LIGHTS "Lights"
#define MATERIALS "Materials"
#define PREFABEDGEOS "PrefabedGeometries"
#define CAMERA_PERSPECTIVE "CameraPerspective"
#define CAMERA_ORTHO "CameraOrthographic"
#define VIEWDEF "ViewDefinition"
#define EXTRACONFIG "ExtraConfig"

#define UNKNOWN_TYPE "UnknownType"
#define GEOMETRY_SET "GeometrySet"
#define GEOMETRY "Geometry"
#define MESH_SET "MeshSet"
#define REGULAR_GRID "RegularGrid"
#define MESH "Mesh"
#define SPHERE "Sphere"
#define AABOX "AABox"
#define PLANE "Plane"
#define PREFAB "Prefab"
#define AMBIENT_LIGHT "AmbientLight"
#define AMBIENT_OCCLUSION "AmbientOcclusion"
#define DIRECTION_LIGHT "DirectionLight"
#define POINT_LIGHT "PointLight"
#define AREA_LIGHT "AreaLight"
#define LAMBERTIAN "Lambertian"
#define PHONG "Phong"
#define REFLECTION "Reflection"

#define MATERIAL "Material"
#define PREFABREF "PrefabReference"
#define ID "ID"
#define FILE_PATH "FilePath"
#define COLOUR "Colour"
#define MIRRORCOLOUR "MirrorColour"
#define SPECULAR "Specular"
#define EXP "Exp"
#define FUZZINESS "Fuzziness"
#define VEC3D_X "X"
#define VEC3D_Y "Y"
#define VEC3D_Z "Z"
#define BGCOLOUR "BackgroundColour"
#define RADIUS "Radius"
#define DIRECTION "Direction"
#define POSITION "Position"
#define TRANSFORM "Transform"
#define NORMAL "Normal"
#define FALLOFF "Falloff"
#define VIEWDIR "ViewDir"
#define VIEWUP "ViewUp"
#define WIDTH "Width"
#define HEIGHT "Height"
#define FOV "FOV"
#define INVGAMMA "InvGamma"
#define ANTIALIASING "AntiAliasingDegree"
#define VIEWPLANEOFFSET "ViewPlaneOffset"
#define MAXRAYDEPTH "MaxRayDepth"
#define LOWERBOUND "LowerBound"
#define UPPERBOUND "UpperBound"
#define SMOOTHSHADING "SmoothShading"
#define SCALE "Scale"


using namespace tinyxml2;

std::shared_ptr<ObjMeshSet> SceneLoader::loadSceneFromOBJ(const std::string& filePath)
  {
  std::shared_ptr<ObjMeshSet> objMeshSet (new ObjMeshSet(filePath));
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string error;

  string objDirectory = mathernogl::getDirectoryFromPath(filePath);
  if (!tinyobj::LoadObj(shapes, materials, error, filePath.c_str(), objDirectory.c_str()))
    throw std::runtime_error("Couldn't load obj at '" + filePath + "' ");

  PhongShader* phongShader;
  std::vector<ShaderPtr> shaders;
  for (int matNum = 0; matNum < materials.size(); ++matNum)
    {
    const tinyobj::material_t& material = materials[matNum];
    const Vector3D diffuse(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
    const Vector3D specular(material.specular[0], material.specular[1], material.specular[2]);
    const float specularExp = 500;
    phongShader = new PhongShader(diffuse, specular, specularExp);
    shaders.push_back(ShaderPtr(phongShader));
    }
  ShaderPtr defaultShader(new LambertianShader(0, 0, 0));

  std::shared_ptr<Mesh> meshObject;
  for(int shapeNum = 0; shapeNum < shapes.size(); ++shapeNum)
    {
    std::vector<uint>& inIndices = shapes[shapeNum].mesh.indices;
    std::vector<float>& inVerts = shapes[shapeNum].mesh.positions;
    std::vector<float>& inNorms = shapes[shapeNum].mesh.normals;
    std::vector<float>& inTexCoords = shapes[shapeNum].mesh.texcoords;
    std::vector<int>& inMaterialIDs = shapes[shapeNum].mesh.material_ids;

    const int materialID = inMaterialIDs[0];
    meshObject.reset(new Mesh());
    meshObject->setSmoothShading(true);
    meshObject->setMaterial(materialID >= 0 && materialID < shaders.size() ? shaders[materialID] : defaultShader);

    //  vertex indices
    for (uint vertexIndex : inIndices)
      {
      meshObject->indices.emplace_back(vertexIndex);
      }

    //  vertex positions
    if (inVerts.size() > 0)
      {
      for (int vertexNum = 0; vertexNum < inVerts.size(); vertexNum += 3)
        {
        meshObject->vertices.emplace_back(inVerts[vertexNum], inVerts[vertexNum + 1], inVerts[vertexNum + 2]);
        }
      }

    //  vertex normals
    if (inNorms.size() > 0)
      {
      for (int vertexNum = 0; vertexNum < inNorms.size(); vertexNum += 3)
        {
        meshObject->normals.emplace_back(inNorms[vertexNum], inNorms[vertexNum + 1], inNorms[vertexNum + 2]);
        }
      }

    // vertex texture coordinates
    if (inTexCoords.size() > 0)
      {
      for (int vertexNum = 0; vertexNum < inTexCoords.size(); vertexNum += 2)
        {
        meshObject->texCoords.emplace_back(inTexCoords[vertexNum], inTexCoords[vertexNum + 1]);
        }
      }

    objMeshSet->push_back(meshObject);
    }

  return objMeshSet;
  }

static Vector3D getVector3DValue(XMLDocument& doc, XMLElement* parent)
  {
  Vector3D value;
  double scale = 1;
  if (parent)
    {
    parent->QueryAttribute(VEC3D_X, &value.x);
    parent->QueryAttribute(VEC3D_Y, &value.y);
    parent->QueryAttribute(VEC3D_Z, &value.z);
    parent->QueryAttribute(SCALE, &scale);
    }
  value *= scale;
  return value;
  }

static mathernogl::Matrix4 getMatrixValue(XMLDocument& doc, XMLElement* parent)
  {
  mathernogl::Matrix4 matrix(1);
  for (int i = 0; i < 4; ++i)
    {
    for (int j = 0; j < 4; ++j)
      {
      string name = "M" + std::to_string(i) + std::to_string(j);
      matrix.setAt(parent->FloatAttribute(name.c_str(), 0), i, j);
      }
    }
  return matrix;
  }

static string getStringAttribute(XMLElement* element, const string& name)
  {
  const char* attribute;
  element->QueryStringAttribute(name.c_str(), &attribute);
  return string(attribute);
  }

static void loadMaterials(XMLDocument& doc, XMLElement* materialsElement, std::map<uint, ShaderPtr>* shaders)
  {
  XMLElement* materialElement = materialsElement->FirstChildElement();
  while (materialElement)
    {
    ShaderPtr shader;
    const uint id = materialElement->UnsignedAttribute(ID, 0);

    const string geometryType = materialElement->Value();
    if (geometryType == LAMBERTIAN)
      {
      const Vector3D colour = getVector3DValue(doc, materialElement->FirstChildElement(COLOUR));
      shader.reset(new LambertianShader(colour));
      }
    else if (geometryType == PHONG)
      {
      const Vector3D colour = getVector3DValue(doc, materialElement->FirstChildElement(COLOUR));
      const Vector3D specular = getVector3DValue(doc, materialElement->FirstChildElement(SPECULAR));
      const float exp = materialElement->FloatAttribute(EXP, 1);
      shader.reset(new PhongShader(colour, specular, exp));
      }
    else if (geometryType == REFLECTION)
      {
      const Vector3D colour = getVector3DValue(doc, materialElement->FirstChildElement(COLOUR));
      const Vector3D specular = getVector3DValue(doc, materialElement->FirstChildElement(SPECULAR));
      const Vector3D mirror = getVector3DValue(doc, materialElement->FirstChildElement(MIRRORCOLOUR));
      const float exp = materialElement->FloatAttribute(EXP, 1);
      const float fuzziness = materialElement->FloatAttribute(FUZZINESS, 0);
      auto reflectionShader = new ReflectionShader(colour, specular, exp, mirror);
      reflectionShader->setFuzziness(fuzziness);
      shader.reset(reflectionShader);
      }

    if (shader && id > 0)
      (*shaders)[id] = shader;
    materialElement = materialElement->NextSiblingElement();
    }
  }

typedef std::function<void(XMLElement* geometryElement, GeometryPtr geometry)> AddGeometryFunc;

static GeometryPtr loadSingularGeometry(XMLDocument& doc, XMLElement* geometryElement, std::map<uint, GeometryPtr>* prefabedGeometries)
  {
  if (!geometryElement)
    return nullptr;

  const string geometryType = geometryElement->Value();
  GeometryPtr geometry;
  if (geometryType == SPHERE)
    {
    const float radius = geometryElement->FloatAttribute(RADIUS, 1);
    const Vector3D centre = getVector3DValue(doc, geometryElement->FirstChildElement(POSITION));
    geometry.reset(new PrimitiveSphere(centre, radius));
    }
  else if (geometryType == AABOX)
    {
    const Vector3D lb = getVector3DValue(doc, geometryElement->FirstChildElement(LOWERBOUND));
    const Vector3D ub = getVector3DValue(doc, geometryElement->FirstChildElement(UPPERBOUND));
    geometry.reset(new PrimitiveBoxAA(lb, ub));
    }
  else if (geometryType == PLANE)
    {
    const Vector3D pos = getVector3DValue(doc, geometryElement->FirstChildElement(POSITION));
    const Vector3D norm = getVector3DValue(doc, geometryElement->FirstChildElement(NORMAL));
    geometry.reset(new PrimitivePlane(pos, norm));
    }
  else if (geometryType == PREFAB)
    {
    if (!prefabedGeometries)
      return nullptr;
    const int prefabID = geometryElement->IntAttribute(PREFABREF, -1);
    const mathernogl::Matrix4 transform = getMatrixValue(doc, geometryElement->FirstChildElement(TRANSFORM));
    if (prefabID >= 0 && prefabID < prefabedGeometries->size())
      {
      GeometryReference* geoRef = new GeometryReference(prefabedGeometries->at(prefabID));
      geoRef->setTransform(transform);
      geometry.reset(geoRef);
      }
    }
  else if (geometryType == MESH)
    {
    const string filePath = getStringAttribute(geometryElement, FILE_PATH);
    if (!filePath.empty())
      {
      Mesh* mesh = new Mesh(filePath);
      mesh->ensureNormalsNormalised();
      mesh->setSmoothShading(geometryElement->BoolAttribute(SMOOTHSHADING, false));
      geometry.reset(mesh);
      }
    }
  return geometry;
  }

static void loadGeometries(XMLDocument& doc, XMLElement* geometriesElement, std::map<uint, GeometryPtr>* prefabedGeometries,
                           std::map<uint, ShaderPtr>* shaders, AddGeometryFunc addGeometryFunc)
  {
  XMLElement* geometryElement = geometriesElement->FirstChildElement();
  while (geometryElement)
    {
    GeometryPtr geometry;

    const string geometryType = geometryElement->Value();
    if (geometryType == MESH_SET)
      {
      const string filePath = getStringAttribute(geometryElement, FILE_PATH);
      try
        {
        if (!filePath.empty())
          geometry = SceneLoader::loadSceneFromOBJ(filePath);
        }
      catch (std::runtime_error& err)
        {
        mathernogl::logWarning(err.what());
        }
      }
    else if (geometryType == GEOMETRY_SET || geometryType == REGULAR_GRID)
      {
      GeometrySet* geoSet = nullptr;
      if (geometryType == REGULAR_GRID)
        geoSet = new RegularGrid();
      else
        geoSet = new GeometrySet();

      geometry.reset(geoSet);
      loadGeometries(doc, geometryElement, prefabedGeometries, shaders,
        [geoSet](XMLElement* geometryElement, GeometryPtr geometry)
        {
        geoSet->push_back(geometry);
        });
      }
    else
      {
      geometry = loadSingularGeometry(doc, geometryElement, prefabedGeometries);
      }

    if (geometry)
      {
      const int materialID = geometryElement->UnsignedAttribute(MATERIAL, 0);
      const int prefabID = geometryElement->UnsignedAttribute(ID, 0);
      if (materialID > 0 && shaders->count(materialID) > 0)
        geometry->setMaterial(shaders->at(materialID));
      addGeometryFunc(geometryElement, geometry);
      }
    geometryElement = geometryElement->NextSiblingElement();
    }
  }

static void loadLightSources(XMLDocument& doc, XMLElement* lightSourcesElement, Scene* scene)
  {
  XMLElement* lightElement = lightSourcesElement->FirstChildElement();
  while (lightElement)
    {
    LightSourcePtr lightSource;

    const string lightSourceType = lightElement->Value();
    if (lightSourceType == AMBIENT_LIGHT)
      {
      Vector3D colour = getVector3DValue(doc, lightElement->FirstChildElement(COLOUR));
      lightSource.reset(new AmbientLight(colour));
      }
    else if (lightSourceType == AMBIENT_OCCLUSION)
      {
      Vector3D colour = getVector3DValue(doc, lightElement->FirstChildElement(COLOUR));
      float radius = lightElement->FloatAttribute(RADIUS, 4);
      lightSource.reset(new AmbientOcclusion(colour, radius));
      }
    else if (lightSourceType == DIRECTION_LIGHT)
      {
      Vector3D colour = getVector3DValue(doc, lightElement->FirstChildElement(COLOUR));
      Vector3D direction = getVector3DValue(doc, lightElement->FirstChildElement(DIRECTION));
      lightSource.reset(new DirectionLight(colour, direction));
      }
    else if (lightSourceType == POINT_LIGHT)
      {
      Vector3D colour = getVector3DValue(doc, lightElement->FirstChildElement(COLOUR));
      Vector3D position = getVector3DValue(doc, lightElement->FirstChildElement(POSITION));
      float radius = lightElement->FloatAttribute(RADIUS, 3);
      float falloff = lightElement->FloatAttribute(FALLOFF, 1);
      lightSource.reset(new PointLight(colour, position, radius, falloff));
      }
    else if (lightSourceType == AREA_LIGHT)
      {
      Vector3D colour = getVector3DValue(doc, lightElement->FirstChildElement(COLOUR));
      float radius = lightElement->FloatAttribute(RADIUS, 3);
      float falloff = lightElement->FloatAttribute(FALLOFF, 1);

      XMLElement* lightGeometryElement = lightElement->FirstChildElement(GEOMETRY);
      if (lightGeometryElement)
        {
        GeometryPtr geometry = loadSingularGeometry(doc, lightGeometryElement->FirstChildElement(), nullptr);
        if (geometry)
          {
          geometry->setMaterial(ShaderPtr(new EmissionShader(colour)));
          AreaLight* areaLight = new AreaLight(geometry);
          areaLight->setColour(colour);
          areaLight->setRadius(radius);
          areaLight->setFallOffExp(falloff);
          lightSource.reset(areaLight);
          }
        scene->getSceneDef()->geometries.push_back(geometry);
        }
      }

    if (lightSource)
      scene->getSceneDef()->lights.push_back(lightSource);
    lightElement = lightElement->NextSiblingElement();
    }
  }

static void loadView(XMLDocument& doc, XMLElement* viewDefElement, Scene* scene)
  {
  scene->getViewDef()->width = viewDefElement->UnsignedAttribute(WIDTH, 800);
  scene->getViewDef()->height = viewDefElement->UnsignedAttribute(HEIGHT, 600);
  scene->getViewDef()->fov = viewDefElement->FloatAttribute(FOV, 1);
  scene->getViewDef()->antiAliasingDegree = viewDefElement->IntAttribute(ANTIALIASING, 1);
  scene->getViewDef()->invGamma = viewDefElement->FloatAttribute(INVGAMMA, 1);
  }

static void loadCamera(XMLDocument& doc, XMLElement* root, Scene* scene)
  {
  std::shared_ptr<Camera> camera;
  bool perspective = true;
  XMLElement* cameraElement = root->FirstChildElement(CAMERA_PERSPECTIVE);
  if (!cameraElement)
    {
    perspective = false;
    cameraElement = root->FirstChildElement(CAMERA_ORTHO);
    ASSERT(cameraElement, "What type of camera is this?");
    }
  if (cameraElement)
    {
    if (perspective)
      {
      CameraPerspective* perspectiveCamera = new CameraPerspective();
      perspectiveCamera->setViewPlaneOffset(cameraElement->FloatAttribute(VIEWPLANEOFFSET, 1));
      camera.reset(perspectiveCamera);
      }
    else
      {
      camera.reset(new CameraOrthographic());
      }
    camera->setPosition(getVector3DValue(doc, cameraElement->FirstChildElement(POSITION)));
    camera->setViewDirection(getVector3DValue(doc, cameraElement->FirstChildElement(VIEWDIR)));
    camera->setViewUp(getVector3DValue(doc, cameraElement->FirstChildElement(VIEWUP)));
    }
  if (camera)
    scene->setCamera(camera);
  }

static void loadExtraConfig(XMLDocument& doc, XMLElement* configElement, Scene* scene)
  {
  scene->getSceneDef()->maxRayDepth = (int)configElement->UnsignedAttribute(MAXRAYDEPTH, 3);
  scene->getSceneDef()->bgColour = getVector3DValue(doc, configElement->FirstChildElement(BGCOLOUR));
  }

bool SceneLoader::loadSceneFromXML(const std::string& filePath, Scene* scene)
  {
  XMLDocument doc;
  XMLError xmlError = doc.LoadFile(filePath.c_str());
  if (xmlError > 0)
    {
    mathernogl::logError("Failed to load XML scene '" + filePath + "', error code: " + std::to_string(xmlError));
    return false;
    }

  XMLElement* rootScene = doc.FirstChildElement(ROOT_SCENE);
  if (!rootScene)
    {
    mathernogl::logError("Failed to load XML scene '" + filePath + "', unrecognised format");
    return false;
    }

  XMLElement* geometries = rootScene->FirstChildElement(GEOMETRIES);
  XMLElement* lights = rootScene->FirstChildElement(LIGHTS);
  XMLElement* prefabed = rootScene->FirstChildElement(PREFABEDGEOS);
  XMLElement* materials = rootScene->FirstChildElement(MATERIALS);
  XMLElement* viewDef = rootScene->FirstChildElement(VIEWDEF);
  XMLElement* config = rootScene->FirstChildElement(EXTRACONFIG);
  if (!geometries || !lights || !viewDef)
    {
    mathernogl::logError("Failed to load XML scene '" + filePath + "', unrecognised format");
    return false;
    }

  std::map<uint, ShaderPtr> shaders;
  std::map<uint, GeometryPtr> prefabedGeometries;

  loadView(doc, viewDef, scene);
  loadCamera(doc, rootScene, scene);
  if (config)
    loadExtraConfig(doc, config, scene);

  //  load materials
  if (materials)
    loadMaterials(doc, materials, &shaders);

  //  load prefabed geometries
  if (prefabed)
    {
    loadGeometries(doc, prefabed, nullptr, &shaders, [&prefabedGeometries](XMLElement* geometryElement, GeometryPtr geometry)
      {
      const int prefabID = geometryElement->UnsignedAttribute(ID, -1);
      if (prefabID >= 0)
        prefabedGeometries[prefabID] = geometry;
      });
    }

  //  load geometries
  loadGeometries(doc, geometries, &prefabedGeometries, &shaders, [scene](XMLElement* geometryElement, GeometryPtr geometry)
    {
    scene->getSceneDef()->geometries.push_back(geometry);
    });

  //  load lights
  loadLightSources(doc, lights, scene);

  return true;
  }

static string getTimeFormatted()
  {
  std::time_t rawtime;
  std::tm* timeinfo;
  char buffer [30];
  std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);
  std::strftime(buffer,30,"%d/%m/%Y %H:%M:%S",timeinfo);
  return buffer;
  }

static XMLElement* createElement(XMLDocument& doc, XMLElement* parent, string name)
  {
  XMLElement* element = doc.NewElement(name.c_str());
  parent->InsertEndChild(element);
  return element;
  }

static void setVector3DValue(XMLDocument& doc, XMLElement* parent, const Vector3D& value)
  {
  parent->SetAttribute(VEC3D_X, value.x);
  parent->SetAttribute(VEC3D_Y, value.y);
  parent->SetAttribute(VEC3D_Z, value.z);
  }

static void setMatrixValue(XMLDocument& doc, XMLElement* parent, const mathernogl::Matrix4* value)
  {
  for (int i = 0; i < 4; ++i)
    {
    for (int j = 0; j < 4; ++j)
      {
      string name = "M" + std::to_string(i) + std::to_string(j);
      parent->SetAttribute(name.c_str(), value->getAt(i, j));
      }
    }
  }

typedef std::function<void(XMLElement* geometryElement, Geometry* geometry)> GeometryElementCreated;

static void saveGeometry(XMLDocument&, XMLElement*, GeometryPtr, std::vector<GeometryPtr>*, GeometryElementCreated);

static void saveGeometrySet(XMLDocument& doc, XMLElement* parent, GeometrySet* geometrySet, std::vector<GeometryPtr>* prefabedGeometries, GeometryElementCreated geoElementCreatedFunc)
  {
  XMLElement* geometrySetElement;
  geoElementCreatedFunc(geometrySetElement, geometrySet);

  if (ObjMeshSet* meshSet = dynamic_cast<ObjMeshSet*>(geometrySet))
    {
    geometrySetElement = createElement(doc, parent, MESH_SET);
    geometrySetElement->SetAttribute(FILE_PATH, meshSet->filePath.c_str());
    return;   // don't save children, can reload them from obj file later
    }
  else if (RegularGrid* grid = dynamic_cast<RegularGrid*>(geometrySet))
    {
    geometrySetElement = createElement(doc, parent, REGULAR_GRID);
    }
  else
    {
    geometrySetElement = createElement(doc, parent, GEOMETRY_SET);
    }

  for (GeometryPtr child : *geometrySet)
    saveGeometry(doc, geometrySetElement, child, prefabedGeometries, geoElementCreatedFunc);
  return;
  }

static void saveGeometry(XMLDocument& doc, XMLElement* parent, GeometryPtr geometry, std::vector<GeometryPtr>* prefabedGeometries, GeometryElementCreated geoElementCreatedFunc)
  {
  //  geometry set
  if (GeometrySet* geometrySet = dynamic_cast<GeometrySet*>(geometry.get()))
    {
    saveGeometrySet(doc, parent, geometrySet, prefabedGeometries, geoElementCreatedFunc);
    return;
    }

  //  geometry reference (prefab)
  if (GeometryReference* geoReference = dynamic_cast<GeometryReference*>(geometry.get()))
    {
    if (!prefabedGeometries)
      return;

    XMLElement* element = createElement(doc, parent, PREFAB);
    geoElementCreatedFunc(element, geometry.get());
    int index;
    const int numPrefabed = (int)prefabedGeometries->size();
    for (index = 0; index < numPrefabed; ++index)
      {
      if ((*prefabedGeometries)[index] == geoReference->getGeometry())
        break;
      }
    if (index >= numPrefabed)
      {
      (*prefabedGeometries).push_back(geoReference->getGeometry());
      index = numPrefabed;
      }
    element->SetAttribute(PREFABREF, index);
    setMatrixValue(doc, createElement(doc, element, TRANSFORM), geoReference->getTransform()->getTransformMatrix());
    return;
    }

  //  mesh
  if (Mesh* mesh = dynamic_cast<Mesh*>(geometry.get()))
    {
    const string filePath = mesh->getSourceFilePath();
    if (!filePath.empty())
      {
      XMLElement* element = createElement(doc, parent, MESH);
      geoElementCreatedFunc(element, geometry.get());
      element->SetAttribute(FILE_PATH, filePath.c_str());
      element->SetAttribute(SMOOTHSHADING, mesh->smoothShadingEnabled());
      }

    return;
    }

  //  sphere
  if (PrimitiveSphere* sphere = dynamic_cast<PrimitiveSphere*>(geometry.get()))
    {
    XMLElement* element = createElement(doc, parent, SPHERE);
    geoElementCreatedFunc(element, geometry.get());
    setVector3DValue(doc, createElement(doc, element, POSITION), sphere->getCentre());
    element->SetAttribute(RADIUS, sphere->getRadius());
    return;
    }

  //  axis aligned box
  if (PrimitiveBoxAA* box = dynamic_cast<PrimitiveBoxAA*>(geometry.get()))
    {
    XMLElement* element = createElement(doc, parent, AABOX);
    geoElementCreatedFunc(element, geometry.get());
    setVector3DValue(doc, createElement(doc, element, LOWERBOUND), box->getLowerBound());
    setVector3DValue(doc, createElement(doc, element, UPPERBOUND), box->getUpperBound());
    return;
    }

  //  plane
  if (PrimitivePlane* plane = dynamic_cast<PrimitivePlane*>(geometry.get()))
    {
    XMLElement* element = createElement(doc, parent, PLANE);
    geoElementCreatedFunc(element, geometry.get());
    setVector3DValue(doc, createElement(doc, element, POSITION), plane->getPoint());
    setVector3DValue(doc, createElement(doc, element, NORMAL), plane->getNormal());
    return;
    }

  //  unknown type
  createElement(doc, parent, UNKNOWN_TYPE);
  }

static void saveLightSource(XMLDocument& doc, XMLElement* parent, LightSourcePtr light)
  {
  // ambient light
  if (AmbientLight* ambientLight = dynamic_cast<AmbientLight*>(light.get()))
    {
    XMLElement* element = createElement(doc, parent, AMBIENT_LIGHT);
    setVector3DValue(doc, createElement(doc, element, COLOUR), ambientLight->getColour());
    return;
    }

  //  ambient occlusion
  if (AmbientOcclusion* ambientLight = dynamic_cast<AmbientOcclusion*>(light.get()))
    {
    XMLElement* element = createElement(doc, parent, AMBIENT_OCCLUSION);
    setVector3DValue(doc, createElement(doc, element, COLOUR), ambientLight->getAmbientColour());
    element->SetAttribute(RADIUS, ambientLight->getSampleRadius());
    return;
    }

  //  directional light
  if (DirectionLight* directionalLight = dynamic_cast<DirectionLight*>(light.get()))
    {
    XMLElement* element = createElement(doc, parent, DIRECTION_LIGHT);
    setVector3DValue(doc, createElement(doc, element, COLOUR), directionalLight->getColour());
    setVector3DValue(doc, createElement(doc, element, DIRECTION), directionalLight->getDirection());
    return;
    }

  //  point light
  if (PointLight* pointLight = dynamic_cast<PointLight*>(light.get()))
    {
    XMLElement* element = createElement(doc, parent, POINT_LIGHT);
    setVector3DValue(doc, createElement(doc, element, COLOUR), pointLight->getColour());
    setVector3DValue(doc, createElement(doc, element, POSITION), pointLight->getPosition());
    element->SetAttribute(RADIUS, pointLight->getRadius());
    element->SetAttribute(FALLOFF, pointLight->getFallOffExp());
    return;
    }

  //  unknown type
  createElement(doc, parent, UNKNOWN_TYPE);
  }

static void saveMaterial(XMLDocument& doc, XMLElement* materialsElement, Shader* materialShader)
  {
  XMLElement* element = nullptr;

  if (LambertianShader* shader = dynamic_cast<LambertianShader*>(materialShader))
    {
    element = createElement(doc, materialsElement, LAMBERTIAN);
    setVector3DValue(doc, createElement(doc, element, COLOUR), shader->getDiffuseColour());
    }
  else if (ReflectionShader* shader = dynamic_cast<ReflectionShader*>(materialShader))
    {
    element = createElement(doc, materialsElement, REFLECTION);
    setVector3DValue(doc, createElement(doc, element, COLOUR), shader->getDiffuseColour());
    setVector3DValue(doc, createElement(doc, element, SPECULAR), shader->getSpecularColour());
    setVector3DValue(doc, createElement(doc, element, MIRRORCOLOUR), shader->getMirrorCol());
    element->SetAttribute(EXP, shader->getSpecularExp());
    element->SetAttribute(FUZZINESS, shader->getFuzziness());
    }
  else if (PhongShader* shader = dynamic_cast<PhongShader*>(materialShader))
    {
    element = createElement(doc, materialsElement, PHONG);
    setVector3DValue(doc, createElement(doc, element, COLOUR), shader->getDiffuseColour());
    setVector3DValue(doc, createElement(doc, element, SPECULAR), shader->getSpecularColour());
    element->SetAttribute(EXP, shader->getSpecularExp());
    }

  if (element)
    {
    element->SetAttribute(ID, materialShader->id);
    }
  else
    {
    //  unknown type
    createElement(doc, materialsElement, UNKNOWN_TYPE);
    }
  }

static void saveView(XMLDocument& doc, XMLElement* parent, Scene* scene)
  {
  XMLElement* viewElement = createElement(doc, parent, VIEWDEF);
  viewElement->SetAttribute(WIDTH, scene->getViewDef()->width);
  viewElement->SetAttribute(HEIGHT, scene->getViewDef()->height);
  viewElement->SetAttribute(FOV, scene->getViewDef()->fov);
  viewElement->SetAttribute(ANTIALIASING, scene->getViewDef()->antiAliasingDegree);
  viewElement->SetAttribute(INVGAMMA, scene->getViewDef()->invGamma);
  }

static void saveCamera(XMLDocument& doc, XMLElement* parent, Scene* scene)
  {
  XMLElement* cameraElement = createElement(doc, parent, CAMERA_ORTHO);
  setVector3DValue(doc, createElement(doc, cameraElement, POSITION), scene->getCamera()->getPosition());
  setVector3DValue(doc, createElement(doc, cameraElement, VIEWDIR), scene->getCamera()->getViewDirection());
  setVector3DValue(doc, createElement(doc, cameraElement, VIEWUP), scene->getCamera()->getViewUp());
  if (CameraPerspective* camera = dynamic_cast<CameraPerspective*>(scene->getCamera().get()))
    {
    cameraElement->SetValue(CAMERA_PERSPECTIVE);
    cameraElement->SetAttribute(VIEWPLANEOFFSET, camera->getViewPlaneOffset());
    }
  }

static void saveExtraConfig(XMLDocument& doc, XMLElement* parent, Scene* scene)
  {
  XMLElement* configElement = createElement(doc, parent, EXTRACONFIG);
  configElement->SetAttribute(MAXRAYDEPTH, scene->getSceneDef()->maxRayDepth);
  setVector3DValue(doc, createElement(doc, configElement, BGCOLOUR), scene->getSceneDef()->bgColour);
  }

void obtainShader(XMLElement* element, Geometry* geometry, std::map<uint, ShaderPtr>* shaders)
  {
  ShaderPtr shader = geometry->getMaterial();
  if (shader)
    {
    element->SetAttribute(MATERIAL, shader->id);
    if (shaders->count(shader->id) == 0)
      (*shaders)[shader->id] = shader;
    }
  }

bool SceneLoader::saveSceneToXML(const std::string& filePath, Scene* scene)
  {
  XMLDocument doc;

  XMLElement* root = doc.NewElement(ROOT_SCENE);
  root->SetAttribute(TIMESTAMP, getTimeFormatted().c_str());
  root->SetAttribute(VERSION, CURRENT_XML_VERSION);
  doc.InsertEndChild(root);

  saveView(doc, root, scene);
  saveCamera(doc, root, scene);
  saveExtraConfig(doc, root, scene);

  std::map<uint, ShaderPtr> shaders;
  std::vector<GeometryPtr> prefabedGeometries;

  GeometryElementCreated geometryElementCreated = [&shaders](XMLElement* element, Geometry* geometry)
    {
    obtainShader(element, geometry, &shaders);
    };

  XMLElement* geometries = createElement(doc, root, GEOMETRIES);
  for (GeometryPtr geometry : scene->getSceneDef()->geometries)
    saveGeometry(doc, geometries, geometry, &prefabedGeometries, geometryElementCreated);

  XMLElement* prefabed = createElement(doc, root, PREFABEDGEOS);
  for (int prefabIndex = 0; prefabIndex < prefabedGeometries.size(); ++prefabIndex)
    {
    GeometryPtr geo = prefabedGeometries[prefabIndex];
    saveGeometry(doc, prefabed, geo, nullptr,
      [&shaders, prefabIndex](XMLElement* element, Geometry* geometry)
      {
      obtainShader(element, geometry, &shaders);
      element->SetAttribute(ID, prefabIndex);
      });
    }

  XMLElement* materials = createElement(doc, root, MATERIALS);
  for (auto pair : shaders)
    saveMaterial(doc, materials, pair.second.get());

  XMLElement* lights = createElement(doc, root, LIGHTS);
  for (LightSourcePtr light : scene->getSceneDef()->lights)
    saveLightSource(doc, lights, light);

  return doc.SaveFile(filePath.c_str(), false) == 0;
  }
