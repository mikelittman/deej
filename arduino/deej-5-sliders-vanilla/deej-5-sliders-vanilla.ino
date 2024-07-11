#include "MultiMap.h"

const int NUM_SLIDERS = 4;
const int NUM_DIALS = 4;

const int logarithmicInputs[NUM_SLIDERS] = {A0, A1, A2, A3};
const int linearInputs[NUM_DIALS] = {A6, A7, A8, A9};

int analogSliderValues[NUM_SLIDERS];
int analogDialValues[NUM_DIALS];

void setup()
{
  for (int i = 0; i < NUM_DIALS; i++)
  {
    pinMode(linearInputs[i], INPUT);
  }

  for (int i = 0; i < NUM_SLIDERS; i++)
  {
    pinMode(logarithmicInputs[i], INPUT);
  }

  Serial.begin(9600);
}

void loop()
{
  updateValues();
  broadcastValues(); // Actually send data (all the time)
  // printValues(); // For debug
  delay(10);
}

void updateValues()
{
  for (int i = 0; i < NUM_SLIDERS; i++)
  {
    analogSliderValues[i] = logarithmicToLinearValue(analogRead(logarithmicInputs[i]));
  }
  for (int i = 0; i < NUM_DIALS; i++)
  {
    analogDialValues[i] = analogRead(linearInputs[i]);
  }
}

void broadcastValues()
{
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++)
  {
    builtString += String(analogSliderValues[i]);
  }

  for (int i = 0; i < NUM_DIALS; i++)
  {
    builtString += String(analogDialValues[i]);

    if (i < NUM_DIALS - 1)
    {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

void printValues()
{
  for (int i = 0; i < NUM_SLIDERS; i++)
  {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1)
    {
      Serial.write(" | ");
    }
    else
    {
      Serial.write("\n");
    }
  }

  for (int i = 0; i < NUM_DIALS; i++)
  {
    String printedString = String("Dial #") + String(i + 1) + String(": ") + String(analogDialValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_DIALS - 1)
    {
      Serial.write(" | ");
    }
    else
    {
      Serial.write("\n");
    }
  }
}

int inputMap[] = {0, 1, 4, 15, 27, 56, 83, 185, 308, 520, 720, 979, 1023};
int outputMap[] = {0, 89, 178, 267, 356, 445, 534, 623, 712, 801, 890, 979, 1023};

int logarithmicToLinearValue(int logarithmicValue)
{
  int linearValue = multiMap<int>(logarithmicValue, inputMap, outputMap, 13);
  return linearValue;
}
