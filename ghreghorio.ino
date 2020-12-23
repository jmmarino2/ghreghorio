//FOR TTGO_T_Display
// Select /User_Setups/Setup25_TTGO_T_Display.h in TFT_eSPI/User_Setup_Select.h
//FOR TTGO_T4
// Select <User_Setups/Setup22_TTGO_T4_v1.3.h> in TFT_eSPI/User_Setup_Select.h

#define GHREGHORIO_FOR_T4
#include "ghreghorio.h"

long currentMillis;
long previousMillis;
unsigned long currentMicros;
unsigned long previousMicros;

unsigned long curCount;       //number of counts in last measuring interval
unsigned long avgCount;       //average number of counts in last n intervals
unsigned long totCount;       //total number of counts
float curDoseRate;            //current dose rate in last measuring interval
float avgDoseRate;            //average dose rate in last n intervals
float totDoseRate;            //accumulated total dose

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

TFTGraph ShortTermGraph(&tft, STZ_LEFT, STZ_TOP, STZ_WIDTH, STZ_HEIGHT, STZ_AXC, STZ_AXC, STZ_BGC, STZ_SVC);
TFTGraph LongTermGraph(&tft, LTZ_LEFT, LTZ_TOP, LTZ_WIDTH, LTZ_HEIGHT, LTZ_AXC, LTZ_AXC, LTZ_BGC, LTZ_SVC);

char buff[512];
int vref = 1100;
int btnCick = false;

int start = 0; //solo para hacer que las gráficas se muevan

//! Long time delay, it is recommended to use shallow sleep, which can effectively reduce the current consumption
void espDelay(int ms)
{   
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,ESP_PD_OPTION_ON);
    esp_light_sleep_start();
}



void setup()
{
  Serial.begin(115200);
  Serial.println("Start");

  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
  //Check type of calibration value used to characterize ADC
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
    Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    vref = adc_chars.vref;
    }
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
    Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
    }
  else
    {
    Serial.println("Default Vref: 1100mV");
    }

  tft.begin();
  tft.setRotation(1);
  tft.setSwapBytes(true);

  if (TFT_BL > 0) // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    {
    pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }

  //drawIntro()

  ShortTermGraph.Init();
  ShortTermGraph.ClearGraph();
  ShortTermGraph.DrawAxis();
  LongTermGraph.Init();
  LongTermGraph.ClearGraph();
  LongTermGraph.DrawAxis();

}



void loop()
{
drawCurrentData();

#ifdef DEBUG
Serial.println("Drawing Short-term Graph:\n");
Serial.printf("STZ_LEFT = %d\n", STZ_LEFT);
Serial.printf("STZ_TOP = %d\n", STZ_TOP);
Serial.printf("STZ_WIDTH = %d\n", STZ_WIDTH);
Serial.printf("STZ_HEIGHT = %d\n", STZ_HEIGHT);
Serial.printf("STZ_XYOR_X = %d\n", STZ_XYOR_X);
Serial.printf("STZ_XYOR_Y = %d\n", STZ_XYOR_Y);
Serial.printf("STZ_XAXIS_END = %d\n", STZ_XAXIS_END);
Serial.printf("STZ_YAXIS_END = %d\n", STZ_YAXIS_END);
Serial.printf("STZ_XAXIS_L = %d\n", STZ_XAXIS_L);
Serial.printf("STZ_YAXIS_L = %d\n", STZ_YAXIS_L);
#endif


for (int i = 0; i < 150; i++)
  {
    ShortTermGraph.SetValue(i, int(80*(1+sin(2*PI*(i+start)/100))));
  }

//ShortTermGraph.ClearValues(); //Si borramos la zona se nota parpadeo
ShortTermGraph.DrawValues();



#ifdef DEBUG
Serial.println("Drawing Long-term Graph:\n");
Serial.printf("LTZ_LEFT = %d\n", LTZ_LEFT);
Serial.printf("LTZ_TOP = %d\n", LTZ_TOP);
Serial.printf("LTZ_WIDTH = %d\n", LTZ_WIDTH);
Serial.printf("LTZ_HEIGHT = %d\n", LTZ_HEIGHT);
Serial.printf("LTZ_XYOR_X = %d\n", LTZ_XYOR_X);
Serial.printf("LTZ_XYOR_Y = %d\n", LTZ_XYOR_Y);
Serial.printf("LTZ_XAXIS_END = %d\n", LTZ_XAXIS_END);
Serial.printf("LTZ_YAXIS_END = %d\n", LTZ_YAXIS_END);
Serial.printf("LTZ_XAXIS_L = %d\n", LTZ_XAXIS_L);
Serial.printf("LTZ_YAXIS_L = %d\n", LTZ_YAXIS_L);
#endif


for (int i = 0; i < 300; i++)
  {
    LongTermGraph.SetValue(i, int(25*(1+sin(2*PI*(i+start)/100))));
  }

//LongTermGraph.ClearValues(); //Si borramos la zona se nota parpadeo
LongTermGraph.DrawValues();



delay(10);
start += 1;
}


void drawCurrentData()
{
  #ifdef DEBUG
  Serial.println("Drawing Current Data:\n");
  Serial.printf("CDZ_LEFT = %d\n", CDZ_LEFT);
  Serial.printf("CDZ_TOP = %d\n", CDZ_TOP);
  Serial.printf("CDZ_WIDTH = %d\n", CDZ_WIDTH);
  Serial.printf("CDZ_HEIGHT = %d\n", CDZ_HEIGHT);
  #endif

  tft.setTextSize(1);
  tft.setTextColor(CDZ_TXC);
  tft.fillRect(CDZ_LEFT, CDZ_TOP, CDZ_WIDTH, CDZ_HEIGHT, CDZ_BGC);
  tft.setCursor(CDZ_LEFT+1, CDZ_TOP+1);
  tft.print("GHREGHORIO");

  String batteryCap = "Batt:" + String( readBatteryCap() ) + "%";

  tft.setCursor(CDZ_RIGHT-1, CDZ_TOP+1);
  tft.print(batteryCap);
}

int readBatteryCap()
{
  return 100;
}

void drawIntro()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.pushImage(0, 0,  MAXX, MAXY, LOGO);
  espDelay(5000);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("This is a homemade",  tft.width() / 2, tft.height() / 2 );
  espDelay(2000);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("Geiger counter.",  tft.width() / 2, tft.height() / 2 );
  espDelay(2000);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("USE IT",  tft.width() / 2, tft.height() / 2 );
  espDelay(2000);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("AT YOUR OWN",  tft.width() / 2, tft.height() / 2 );
  espDelay(2000);

  tft.setTextSize(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.drawString("RISK",  tft.width() / 2, tft.height() / 2 );
  espDelay(2000);
}
