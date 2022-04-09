#include "Watchy_Pokemon.h"

const uint8_t BATTERY_DISPLAY_START_Y = 34;
const uint8_t BATTERY_DISPLAY_START_X = 40;
const uint8_t BATTERY_DISPLAY_HEIGHT = 4;
const uint8_t BATTERY_SEGMENT_WIDTH = 20;

void WatchyPokemon::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    drawPokemon();
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
}

void WatchyPokemon::drawPokemon(){
    display.drawBitmap(0, 0, pokemon, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);
}

void WatchyPokemon::drawTime(){
    display.setCursor(12, 155);
    if(currentTime.Hour < 10){
        display.print('0');
    }
    display.print(currentTime.Hour);
    display.print(':');
    if(currentTime.Minute < 10){
        display.print('0');
    }
    display.print(currentTime.Minute);
}

void WatchyPokemon::drawDate(){
    // blank out bitmap 19 / 19, replace with date
    display.fillRect(120, 110, 60, 10, GxEPD_WHITE);
    display.setCursor(126, 122);
    uint8_t day = currentTime.Day;
    if (day < 10){
        display.print(" ");
    }
    display.print(day);
    display.print("/");
    uint8_t month = currentTime.Month;
    if (month < 10){
        display.print(" ");
    }
    display.print(month);
}

void WatchyPokemon::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.setCursor(12, 176);
    display.println(stepCount);
}

// Use Eevee's health bar as the battery gauge
void WatchyPokemon::drawBattery(){
    int8_t batteryFull = 3;
    display.fillRect(BATTERY_DISPLAY_START_X, BATTERY_DISPLAY_START_Y, BATTERY_SEGMENT_WIDTH * batteryFull, BATTERY_DISPLAY_HEIGHT, GxEPD_WHITE);//clear battery segments
    float VBAT = getBatteryVoltage();
    int8_t batteryLevel = 0;
    if(VBAT > 4.1){
        batteryLevel = batteryFull;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }

    int8_t batteryDisplayWidth = batteryLevel * BATTERY_SEGMENT_WIDTH;
    display.fillRect(BATTERY_DISPLAY_START_X, BATTERY_DISPLAY_START_Y, batteryDisplayWidth, BATTERY_DISPLAY_HEIGHT, GxEPD_BLACK);
}