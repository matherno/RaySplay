//
// Created by matt on 17/07/18.
//

#include "RSImagePanel.h"

wxBEGIN_EVENT_TABLE(RSImagePanel, wxPanel)
    EVT_PAINT(RSImagePanel::onPaint)
    EVT_SIZE(RSImagePanel::onSize)
wxEND_EVENT_TABLE()

RSImagePanel::RSImagePanel(wxWindow* parent, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, wxID_ANY, pos, size)
  {

  }

bool RSImagePanel::loadImage(const string& filePath)
  {
  wxBitmap bitmap;
  if(bitmap.LoadFile(filePath))
    {
    image = bitmap.ConvertToImage();
    gotImage = true;
    return true;
    }
  return false;
  }

bool RSImagePanel::loadImage(int width, int height, mathernogl::byte* data)
  {
  if(image.Create(width, height, data, true))
    {
    gotImage = true;
    previousWidth = -1;
    previousHeight = -1;
    return true;
    }
  return false;
  }

void RSImagePanel::onPaint(wxPaintEvent& event)
  {
  if (!gotImage)
    return;

  wxPaintDC dc(this);
  int width, height;
  dc.GetSize(&width, &height);

  if(width != previousWidth || height != previousHeight)
    {
    previousWidth = width;
    previousHeight = height;

    wxSize imageSize = image.GetSize();
    float imageAspectRatio = (float)imageSize.x / (float)imageSize.y;
    if ((float)width / imageAspectRatio < height)
      height = (float)width / imageAspectRatio;
    else
      width = (float)height * imageAspectRatio;
    scaledBitmap = wxBitmap(image.Scale(width, height));
    }

  dc.DrawBitmap(scaledBitmap, 0, 0);
  }

void RSImagePanel::onSize(wxSizeEvent& event)
  {
  Refresh();
  }
