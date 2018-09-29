#pragma once

#include <wx/wx.h>
#include <ImageOutput.h>
#include <ImageOutputWindow.h>
#include "RSImagePanel.h"
#include "RSWalkthroughWindow.h"

enum RSControlID
  {
  ID_RENDER,
  ID_CANCEL,
  ID_INPUTFILE,
  ID_DRAFTBOOL,
  ID_SAVE,
  ID_NUMTHREADS,
  ID_WALKTHROUGH,
  ID_UPDATETIMER,
  };

#define COLOUR_LIGHT_GREY   wxColor(160, 160, 160)
#define COLOUR_DARK_GREY    wxColor(20, 20, 20)
#define COLOUR_BLACK        wxColor(0, 0, 0)

class RSMainFrame : public wxFrame, public ImageOutput
  {
private:
  std::vector<mathernogl::byte> imageData;
  std::mutex imageDataMutex;
  uint imageWidth = 0, imageHeight = 0;
  bool cancelRender = false;
  bool rendering = false;
  std::mutex renderingMutex;
  RSImagePanel* imagePanel = nullptr;
  string progressString = "";
  std::unique_ptr<wxTimer> updateTimer;
  std::unique_ptr<RSWalkthroughWindow> walkthroughWindow;

public:
  RSMainFrame();

  // ImageOutput overrides
  virtual bool prepare(uint imageWidth, uint imageHeight) override;
  virtual void paintPixel(uint x, uint y, const Vector3D& colour) override;
  virtual bool finalise() override;
  virtual bool queryBail() override;
  virtual void updateProgress(float percent) override;

private:
  void createMenu();
  void createControls();
  void onExit(wxCommandEvent& event);
  void onRender(wxCommandEvent& event);
  void onCancel(wxCommandEvent& event);
  void onSave(wxCommandEvent& event);
  void onStartWalkthrough(wxCommandEvent& event);
  void onUpdate(wxTimerEvent& event);
  void refreshImage();
  bool isRendering();
  void setRendering(bool rendering);

  string getCurrentSceneFilePath() const;

  wxDECLARE_EVENT_TABLE();
  };

class RSApp : public wxApp
  {
public:
  virtual bool OnInit() override;
  };

wxIMPLEMENT_APP(RSApp);