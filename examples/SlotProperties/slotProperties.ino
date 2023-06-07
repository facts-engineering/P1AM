uint8_t digitalInputBytes = 0;
uint8_t digitalOutputBytes = 0;
uint8_t numberOfSlices = 0;
uint8_t dataInputArray[30];
uint8_t dataInputArray_old[30];
uint8_t dataOutputArray[30];
uint8_t dataOutputArray_old[30];

void setup()
{

  Serial.begin(115200);
  while (!Serial) {}// wait for serial port to connect.

  numberOfSlices = P1.init();

  for (uint8_t slotIndex = 0; slotIndex < numberOfSlices; slotIndex++) {
    moduleProps slotProps = P1.readSlotProps(slotIndex);

    digitalInputBytes += (uint8_t)slotProps.diBytes;
    digitalOutputBytes += (uint8_t)slotProps.doBytes;
  }

  Serial.print("Digital Input Bytes: "); Serial.println(digitalInputBytes);
  Serial.print("Digital Output Bytes: "); Serial.println(digitalOutputBytes);
}

void loop()
{
  if (millis() - checkInputsTimer > 10UL) {
    P1.readBlockData((char*)dataInputArray, (uint16_t)digitalInputBytes, 0, DISCRETE_IN_BLOCK);
    P1.readBlockData((char*)dataOutputArray, (uint16_t)digitalOutputBytes, 0, DISCRETE_OUT_BLOCK);
    checkInputsTimer = millis();
    for (uint8_t inputBlockIndex = 0; inputBlockIndex < digitalInputBytes; inputBlockIndex++) {
      if (dataInputArray[inputBlockIndex] != dataInputArray_old[inputBlockIndex]) {
        for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++) {
          if (bitRead(dataInputArray[inputBlockIndex], bitIndex) != bitRead(dataInputArray_old[inputBlockIndex], bitIndex)) {
            // If input change is detected, do something.
          }
        }
          dataInputArray_old[inputBlockIndex] = dataInputArray[inputBlockIndex];
      }
    }
  }
}
