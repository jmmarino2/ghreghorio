#include <TFT_eSPI.h>

enum TFTGraph_Type {
                     TFTGRAPHTYPE_BARS,
                     TFTGRAPHTYPE_LINES,   
                     TFTGRAPHTYPE_DOTS
                   };

class TFTGraph
{
  private:
    TFT_eSPI *Tft;
    TFTGraph_Type Type = TFTGRAPHTYPE_BARS;
    int *Values; 
    int Xcoord;                        //X coordinate for upper-left corner of graph area.
    int Ycoord;                        //Y coordinate for upper-left corner of graph area.
    int Xsize;                         //X size, in pixels, for graph area.
    int Ysize;                         //Y size, in pixels, for graph area.
    int OXcoord;                       //X coordinate for origin of axis.
    int OYcoord;                       //Y coordinate for origin of axis.
    int OXlength;                      //Length of X axis.
    int OYlength;                      //Length of Y axis.
    int Xdivint      = 10;             //Spacing between marks in X axis, in pixels.
    int Xdivsize     =  3;             //Lenght of marks in X axis, in pixels. Does not include the X axis itself.
    int Ydivint      = 10;             //Spacing between marks in Y axis, in pixels.
    int Ydivsize     =  3;             //Lenght of marks in Y axis, in pixels. Does not include the X axis itself.
    int Lmargin      =  5;             //Free space at left side between graph and graph area border, in pixels.
    int Rmargin      =  5;             //Free space at right side between graph and graph area border, in pixels.
    int Tmargin      =  5;             //Free space at top side between graph and graph area border, in pixels.
    int Bmargin      =  5;             //Free space at bottom side between graph and graph area border, in pixels.

    int OXColour = TFT_BLACK;          //X axis colour.
    int OYColour = TFT_BLACK;          //Y axis colour.
    int BGColour = TFT_WHITE;          //Background colour.
    int ValuesColour = TFT_BLACK;      //Values colour.
    
 
  public:
    TFTGraph(   TFT_eSPI *tft,
                int xcoord,
                int ycoord,
                int xsize,
                int ysize,
                int oxcolour,
                int oycolour,
                int bgcolour,
                int valuescolour
             )
             :  Tft(tft),
                Xcoord(xcoord),
                Ycoord(ycoord),
                Xsize(xsize),
                Ysize(ysize),
                OXColour(oxcolour),
                OYColour(oycolour),
                BGColour(bgcolour),
                ValuesColour(valuescolour)  {}
    void Init();
    void ClearGraph();
    void ClearValues();
    void DrawAxis();
    void DrawValue(int n);
    void DrawValues();
    void SetValue(int n, int val);
};
