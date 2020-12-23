#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Button2.h>
#include "esp_adc_cal.h"
#include "images/logo.h"

#include "TFTGraph.h"

#define DEBUG

#define CDZ_BGC   TFT_LIGHTGREY         //Current data zone background color
#define CDZ_TXC   TFT_BLACK             //Current data zone text color
#define CDZ_SVC   TFT_GREEN             //Current data zone safe values color
#define CDZ_HVC   TFT_ORANGE            //Current data hazardous values color
#define CDZ_DVC   TFT_RED               //Current data dangerous values color

#define STZ_BGC   TFT_BLACK             //Short-term zone background color
#define STZ_AXC   TFT_WHITE             //Short-term zone axis color
#define STZ_SVC   TFT_GREEN             //Short-term zone safe values color
#define STZ_HVC   TFT_ORANGE            //Short-term hazardous values color
#define STZ_DVC   TFT_RED               //Short-term dangerous values color

#define LTZ_BGC   TFT_PURPLE            //Long-term zone background color
#define LTZ_AXC   TFT_YELLOW            //Long-term zone axis color
#define LTZ_SVC   TFT_GREEN             //Long-term zone safe values color
#define LTZ_HVC   TFT_ORANGE            //Long-term hazardous values color
#define LTZ_DVC   TFT_RED               //Long-term dangerous values color

#define BATT_LVOL    3.4                //Battery minimum voltage
#define BATT_HVOL    4.2                //Battery maximum voltage

#ifdef GHREGHORIO_FOR_T4

//TTGO-T4 Extra Setup
#define LOGO logo_320x240

#define TFT_BACKLIGHT_ON  HIGH
#define TFT_BL               4  // Display backlight control pin
#define BUTTON_1            38
#define BUTTON_2            37
#define BUTTON_3            39

#define DISPLAY_HEIGHT   240 //Display size is in portrait orientation
#define DISPLAY_WIDTH    320 //Display size is in portrait orientation
#define MAXY             DISPLAY_HEIGHT-1 //We use the display in landscape mode
#define MAXX             DISPLAY_WIDTH-1 //We use the display in landscape mode

#define CDZ_LEFT                                     0 //Current data zone left coordinate
#define CDZ_RIGHT                                  159 //Current data zone right coordinate   
#define CDZ_TOP                                      0 //Current data zone top coordinate
#define CDZ_BOTTOM                                 173 //Current data zone bottom coordinate   
#define CDZ_WIDTH             (CDZ_RIGHT)-(CDZ_LEFT)+1 //Current data zone width
#define CDZ_HEIGHT            (CDZ_BOTTOM)-(CDZ_TOP)+1 //Current data zone height

#define STZ_LEFT                         (CDZ_RIGHT)+1 //Short-term zone left coordinate
#define STZ_RIGHT                                 MAXX //Short-term zone right coordinate   
#define STZ_TOP                                CDZ_TOP //Short-term zone top coordinate
#define STZ_BOTTOM                          CDZ_BOTTOM //Short-term zone bottom coordinate   
#define STZ_WIDTH             (STZ_RIGHT)-(STZ_LEFT)+1 //Short-term zone width
#define STZ_HEIGHT            (STZ_BOTTOM)-(STZ_TOP)+1 //Short-term zone height

#define LTZ_LEFT                              CDZ_LEFT //Long-term zone left coordinate
#define LTZ_RIGHT                            STZ_RIGHT //Long-term zone right coordinate   
#define LTZ_TOP                         (CDZ_BOTTOM)+1 //Long-term zone top coordinate
#define LTZ_BOTTOM                                MAXY //Long-term zone bottom coordinate   
#define LTZ_WIDTH             (LTZ_RIGHT)-(LTZ_LEFT)+1 //Long-term zone width
#define LTZ_HEIGHT            (LTZ_BOTTOM)-(LTZ_TOP)+1 //Long-term zone height

#define STZ_XYOR_X                        (STZ_LEFT)+5 //Short-term zone XY axis origin X coordinate
#define STZ_XYOR_Y                      (STZ_BOTTOM)-5 //Short-term zone XY axis origin Y coordinate
#define STZ_XAXIS_END                    (STZ_RIGHT)-5 //Short-term zone X axis end X coordinate
#define STZ_YAXIS_END                      (STZ_TOP)+5 //Short-term zone Y axis end Y coordinate
#define STZ_XAXIS_L     (STZ_XAXIS_END)-(STZ_XYOR_X)+1 //Short-term zone X axis lenght
#define STZ_YAXIS_L     (STZ_XYOR_Y)-(STZ_YAXIS_END)+1 //Short-term zone Y axis lenght

#define LTZ_XYOR_X                        (LTZ_LEFT)+5 //Long-term zone XY axis origin X coordinate
#define LTZ_XYOR_Y                      (LTZ_BOTTOM)-5 //Long-term zone XY axis origin Y coordinate
#define LTZ_XAXIS_END                    (LTZ_RIGHT)-5 //Long-term zone X axis end X coordinate
#define LTZ_YAXIS_END                      (LTZ_TOP)+5 //Long-term zone Y axis end Y coordinate
#define LTZ_XAXIS_L     (LTZ_XAXIS_END)-(LTZ_XYOR_X)+1 //Long-term zone X axis lenght
#define LTZ_YAXIS_L     (LTZ_XYOR_Y)-(LTZ_YAXIS_END)+1 //Long-term zone Y axis lenght

#endif


#ifdef GHREGHORIO_FOR_T_DISPLAY

//TTGO-T-Display Extra Setup
#define LOGO logo_240x135

#define TFT_BL          4  // Display backlight control pin
#define TFT_BACKLIGHT_ON HIGH  // HIGH or LOW are options
#define ADC_EN          14
#define ADC_PIN         34
#define BUTTON_1        35
#define BUTTON_2        0


#define DISPLAY_HEIGHT   135 //Display size is in portrait orientation
#define DISPLAY_WIDTH    240 //Display size is in portrait orientation
#define MAXY             TFT_WIDTH - 1 //We use the display in landscape mode
#define MAXX             TFT_HEIGHT - 1 //We use the display in landscape mode

#define CDZ_LEFT                                     0 //Current data zone left coordinate
#define CDZ_RIGHT                                  119 //Current data zone right coordinate   
#define CDZ_TOP                                      0 //Current data zone top coordinate
#define CDZ_BOTTOM                                  68 //Current data zone bottom coordinate   
#define CDZ_WIDTH             (CDZ_RIGHT)-(CDZ_LEFT)+1 //Current data zone width
#define CDZ_HEIGHT            (CDZ_BOTTOM)-(CDZ_TOP)+1 //Current data zone height

#define STZ_LEFT                                   120 //Short-term zone left coordinate
#define STZ_RIGHT                                 MAXX //Short-term zone right coordinate   
#define STZ_TOP                                      0 //Short-term zone top coordinate
#define STZ_BOTTOM                                  68 //Short-term zone bottom coordinate   
#define STZ_WIDTH             (STZ_RIGHT)-(STZ_LEFT)+1 //Short-term zone width
#define STZ_HEIGHT            (STZ_BOTTOM)-(STZ_TOP)+1 //Short-term zone height

#define LTZ_LEFT                                     0 //Long-term zone left coordinate
#define LTZ_RIGHT                                 MAXX //Long-term zone right coordinate   
#define LTZ_TOP                         (STZ_BOTTOM)+1 //Long-term zone top coordinate
#define LTZ_BOTTOM                                MAXY //Long-term zone bottom coordinate   
#define LTZ_WIDTH             (LTZ_RIGHT)-(LTZ_LEFT)+1 //Long-term zone width
#define LTZ_HEIGHT            (LTZ_BOTTOM)-(LTZ_TOP)+1 //Long-term zone height

#define STZ_XYOR_X                          STZ_LEFT+5 //Short-term zone XY axis origin X coordinate
#define STZ_XYOR_Y                        STZ_BOTTOM-5 //Short-term zone XY axis origin Y coordinate
#define STZ_XAXIS_END                      STZ_RIGHT-5 //Short-term zone X axis end X coordinate
#define STZ_YAXIS_END                        STZ_TOP+5 //Short-term zone Y axis end Y coordinate
#define STZ_XAXIS_L     (STZ_XAXIS_END)-(STZ_XYOR_X)+1 //Short-term zone X axis lenght
#define STZ_YAXIS_L     (STZ_XYOR_Y)-(STZ_YAXIS_END)+1 //Short-term zone Y axis lenght

#define LTZ_XYOR_X                          LTZ_LEFT+5 //Long-term zone XY axis origin X coordinate
#define LTZ_XYOR_Y                        LTZ_BOTTOM-5 //Long-term zone XY axis origin Y coordinate
#define LTZ_XAXIS_END                      LTZ_RIGHT-5 //Long-term zone X axis end X coordinate
#define LTZ_YAXIS_END                        LTZ_TOP+5 //Long-term zone Y axis end Y coordinate
#define LTZ_XAXIS_L     (LTZ_XAXIS_END)-(LTZ_XYOR_X)+1 //Long-term zone X axis lenght
#define LTZ_YAXIS_L     (LTZ_XYOR_Y)-(LTZ_YAXIS_END)+1 //Long-term zone Y axis lenght

#endif
