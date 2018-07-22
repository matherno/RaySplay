#pragma once

#include <wx/wx.h>
#include <RaySplayConst.h>

class RSImagePanel : public wxPanel
  {
private:
  bool gotImage = false;
  wxImage image;
  wxBitmap scaledBitmap;
  int previousWidth = -1, previousHeight = -1;

public:
  RSImagePanel(wxWindow *parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
  bool loadImage(const string& filePath);
  bool loadImage(int width, int height, mathernogl::byte* data);

private:
  void onPaint(wxPaintEvent& event);
  void onSize(wxSizeEvent& event);

  wxDECLARE_EVENT_TABLE();
  };
