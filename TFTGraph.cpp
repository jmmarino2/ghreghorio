#include <Arduino.h> //Permite utilizar los comandos de Arduino
#include "TFTGraph.h"


void TFTGraph::Init()
{
  OXlength = Xsize - Lmargin - Rmargin;
  OYlength = Ysize - Tmargin - Bmargin;
  OXcoord = Xcoord + Lmargin;
  OYcoord = Ycoord + Tmargin + OYlength - 1;
  Values = new int[OXlength - 1];
}

void TFTGraph::ClearGraph()
{
  Tft->fillRect(Xcoord, Ycoord, Xsize, Ysize, BGColour);
}

void TFTGraph::ClearValues()
{
  Tft->fillRect(OXcoord + 1, OYcoord - OYlength + 1, OXlength - 1, OYlength - 1, BGColour);
}

void TFTGraph::DrawAxis()
{
  Tft->drawFastHLine(OXcoord, OYcoord, OXlength, OXColour);
  Tft->drawFastVLine(OXcoord, OYcoord - OYlength + 1, OYlength, OYColour);

  for(int i = Xdivint; i < OXlength; i+=Xdivint)
  {
    Tft->drawFastVLine(OXcoord + i, OYcoord + 1, Xdivsize, OXColour);
  }

  for(int i = Ydivint; i < OYlength; i+=Ydivint)
  {
    Tft->drawFastHLine(OXcoord - Ydivsize, OYcoord - i, Ydivsize, OYColour);
  }
}


void TFTGraph::DrawValue(int n)
{
  Tft->drawFastVLine(OXcoord + n + 1, OYcoord - OYlength + 1, OYlength - Values[n] - 1, BGColour);    //To avoid clearing graph, we draw the backgroud of every bar.
  Tft->drawFastVLine(OXcoord + n + 1, OYcoord - Values[n], Values[n], ValuesColour);
}

void TFTGraph::DrawValues()
{
  for (int i = 0; i < (OXlength - 1); i++)
    {
    this->DrawValue(i);
    }
}


void TFTGraph::SetValue(int n, int val)
{
  Values[n] = val;
}
