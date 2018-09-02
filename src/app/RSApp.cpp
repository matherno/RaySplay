//
// Created by matt on 17/07/18.
//

#include <Scene.h>
#include "RSApp.h"
#include <wx/filepicker.h>
#include <ImageOutputFile.h>
#include <thread>

#define IMAGE_PANEL_SIZE wxSize(1366, 768)
#define CONTROL_PANEL_SIZE wxSize(IMAGE_PANEL_SIZE.x, 60)

/*
 *  RSMainFrame
 */

wxBEGIN_EVENT_TABLE(RSMainFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  RSMainFrame::onExit)
    EVT_BUTTON(ID_RENDER,  RSMainFrame::onRender)
    EVT_BUTTON(ID_CANCEL,  RSMainFrame::onCancel)
    EVT_BUTTON(ID_SAVE,  RSMainFrame::onSave)
wxEND_EVENT_TABLE()

RSMainFrame::RSMainFrame()
    : wxFrame(NULL, wxID_ANY, "RaySplay", wxDefaultPosition, wxSize(800, 600))
  {
  CreateStatusBar();
  createMenu();
  createControls();
  SetMinSize(GetSize());
  SetMaxSize(GetSize());
  }

void RSMainFrame::onExit(wxCommandEvent& event)
  {
  Close(true);
  }

void RSMainFrame::createMenu()
  {
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_EXIT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  SetMenuBar(menuBar);
  }

void RSMainFrame::createControls()
  {
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  imagePanel = new RSImagePanel(this, wxDefaultPosition, IMAGE_PANEL_SIZE);
  imagePanel->SetBackgroundColour(COLOUR_DARK_GREY);
  mainSizer->Add(imagePanel, 0, 0, 0);

  wxPanel* sidePanel = new wxPanel(this, wxID_ANY, wxPoint(0, IMAGE_PANEL_SIZE.y), CONTROL_PANEL_SIZE);
  sidePanel->SetBackgroundColour(COLOUR_LIGHT_GREY);
  mainSizer->Add(sidePanel, 0, 0, 0);

  wxBoxSizer* controlPanelSizer = new wxBoxSizer(wxHORIZONTAL);
  controlPanelSizer->AddSpacer(50);
  controlPanelSizer->Add(new wxStaticText(sidePanel, -1, "XML Scene File"), 0, wxALIGN_CENTRE_VERTICAL);
  controlPanelSizer->AddSpacer(10);
  controlPanelSizer->Add(new wxFilePickerCtrl(sidePanel, ID_INPUTFILE, "", "XML Scene File", "*.xml", wxDefaultPosition, wxSize(200, -1)), 0, wxALIGN_CENTRE_VERTICAL);
  controlPanelSizer->AddSpacer(40);
  controlPanelSizer->Add(new wxButton(sidePanel, ID_RENDER, "Render"), 0, wxALIGN_CENTRE_VERTICAL);
  controlPanelSizer->AddSpacer(20);
  controlPanelSizer->Add(new wxButton(sidePanel, ID_CANCEL, "Cancel"), 0, wxALIGN_CENTRE_VERTICAL);
  controlPanelSizer->AddSpacer(200);

  wxCheckBox* checkBox = new wxCheckBox(sidePanel, ID_DRAFTBOOL, "Draft");
  checkBox->SetValue(true);
  controlPanelSizer->Add(checkBox, 0, wxALIGN_CENTRE_VERTICAL);

  controlPanelSizer->AddSpacer(20);
  controlPanelSizer->Add(new wxStaticText(sidePanel, -1, "Threads"), 0, wxALIGN_CENTRE_VERTICAL);
  controlPanelSizer->AddSpacer(10);
  controlPanelSizer->Add(new wxTextCtrl(sidePanel, ID_NUMTHREADS, "4", wxDefaultPosition, wxSize(40, -1)), 0, wxALIGN_CENTRE_VERTICAL);

  controlPanelSizer->AddSpacer(250);
  controlPanelSizer->Add(new wxButton(sidePanel, ID_SAVE, "Save Image"), 0, wxALIGN_CENTRE_VERTICAL);

  sidePanel->SetSizer(controlPanelSizer);
  mainSizer->SetSizeHints(this);
  }

void RSMainFrame::onRender(wxCommandEvent& event)
  {
  if (rendering)
    return;

  string inputFilePath = "";
  wxFilePickerCtrl* inputFilePicker = dynamic_cast<wxFilePickerCtrl*>(FindWindowById(ID_INPUTFILE));
  if (inputFilePicker)
    inputFilePath = inputFilePicker->GetPath();

  int numThreads = 4;
  wxTextCtrl* numThreadsCtrl = dynamic_cast<wxTextCtrl*>(FindWindowById(ID_NUMTHREADS));
  if (numThreadsCtrl)
    numThreads = atoi(numThreadsCtrl->GetValue());
  numThreads = mathernogl::clampi(numThreads, 1, 16);

  wxCheckBox* draftBool = dynamic_cast<wxCheckBox*>(FindWindowById(ID_DRAFTBOOL));
  bool draftMode = false;
  if (draftBool)
   draftMode = draftBool->GetValue();

  const long startTime = mathernogl::getTimeMS();
  bool renderSuccess = false;
  setRendering(true);
  cancelRender = false;
  progressString = "Rendering... 0%";

  std::thread thread([&renderSuccess, inputFilePath, draftMode, numThreads, this]()
    {
    Scene scene((uint)numThreads);
    if (scene.build(inputFilePath))
      {
      if (draftMode)
       {
       double aspectRatio = scene.getViewDef()->getAspectRatio();
       scene.getViewDef()->width = (uint) ((1500.0 * aspectRatio) / (aspectRatio + 1.0));
       scene.getViewDef()->height = (uint) (scene.getViewDef()->width / aspectRatio);
       scene.getViewDef()->draftMode = true;
       scene.getViewDef()->antiAliasingDegree = 1;
       scene.getSceneDef()->maxRayDepth = 2;
       }

      scene.render(this);
      renderSuccess = true;
      }
    setRendering(false);
    });

  const static long updateTime = 2000;
  long lastUpdateTime = startTime;
  while (isRendering() && !cancelRender)
    {
    const long currentTime = mathernogl::getTimeMS();
    if (currentTime - lastUpdateTime > updateTime)
      {
      lastUpdateTime = currentTime;
      refreshImage();
      SetStatusText(progressString);
      }
    wxYield();
    }

  thread.join();

  if (cancelRender)
    {
    SetStatusText("Cancelled rendering");
    }
  else
    {
    if (renderSuccess)
      {
      refreshImage();
      float duration = (float)(mathernogl::getTimeMS() - startTime) / 1000;
      if (duration < 4 * 60)
        SetStatusText("Finished rendering in " + std::to_string(duration) + " seconds");
      else
        SetStatusText("Finished rendering in " + std::to_string(int(duration / 60)) + " minutes");
      }
    else
      {
      SetStatusText("Failed to load scene!");
      }
    }
  }

void RSMainFrame::onCancel(wxCommandEvent& event)
  {
  cancelRender = true;
  }

void RSMainFrame::refreshImage()
  {
  if (imagePanel)
    {
    imageDataMutex.lock();
    imagePanel->loadImage(imageWidth, imageHeight, imageData.data());
    imagePanel->Refresh();
    imageDataMutex.unlock();
    }
  }

static const int imageBPP = 3;
void RSMainFrame::onSave(wxCommandEvent& event)
  {
  if (isRendering() || imageData.empty())
    return;

  string inputFilePath = "";
  wxFilePickerCtrl* inputFilePicker = dynamic_cast<wxFilePickerCtrl*>(FindWindowById(ID_INPUTFILE));
  if (inputFilePicker)
    inputFilePath = inputFilePicker->GetFileName().GetName();

  string imageFilePath = mathernogl::removeFileExtension(inputFilePath) + ".png";
  ImageOutputFile imageFileHandler(imageFilePath);
  imageFileHandler.prepare(imageWidth, imageHeight);
  for (int col = 0; col < imageWidth; ++col)
    {
    for (int row = 0; row < imageHeight; ++row)
      {
      const int redIndex = imageBPP * (col + row * imageWidth);
      Vector3D colour;
      colour.x = (float)imageData[redIndex] / 255;
      colour.y = (float)imageData[redIndex+1] / 255;
      colour.z = (float)imageData[redIndex+2] / 255;
      imageFileHandler.paintPixel(col, row, colour);
      }
    }

  imageFileHandler.finalise();
  SetStatusText("Saved render image to '" + imageFilePath + "'");
  }


/*
 * ImageOutput overrides
 */

bool RSMainFrame::prepare(uint imageWidth, uint imageHeight)
  {
  imageDataMutex.lock();
  this->imageWidth = imageWidth;
  this->imageHeight = imageHeight;
  imageData.clear();
  imageData.resize(imageWidth * imageHeight * imageBPP);
  std::fill(imageData.begin(), imageData.end(), (mathernogl::byte) 0);
  imageDataMutex.unlock();
  return true;
  }

void RSMainFrame::paintPixel(uint x, uint y, const Vector3D& colour)
  {
  imageDataMutex.lock();
  const int redIndex = imageBPP * (x + y * imageWidth);
  imageData[redIndex] = mathernogl::byte(colour.x * 255);
  imageData[redIndex+1] = mathernogl::byte(colour.y * 255);
  imageData[redIndex+2] = mathernogl::byte(colour.z * 255);
  imageDataMutex.unlock();
  }

bool RSMainFrame::finalise()
  {
  return false;
  }

bool RSMainFrame::queryBail()
  {
  return cancelRender;
  }

void RSMainFrame::updateProgress(float percent)
  {
  progressString = mathernogl::stringFormat("Rendering... %0.2f%%", percent);
  }

void RSMainFrame::setRendering(bool rendering)
  {
  renderingMutex.lock();
  this->rendering = rendering;
  renderingMutex.unlock();
  }

bool RSMainFrame::isRendering()
  {
  renderingMutex.lock();
  bool res = rendering;
  renderingMutex.unlock();
  return res;
  }

/*
 *  RSApp
 */

bool RSApp::OnInit()
  {
  mathernogl::RandomGenerator::setSeed();
  mathernogl::resetLog();

  RSMainFrame *frame = new RSMainFrame();
  frame->Show( true );
  return true;
  }


