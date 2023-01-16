
#include <RFM69.h>
#include <SPI.h>

RFM69 radio1(CS, IntG0, false, SPI_device);
RFM69 radio2(CS, IntG0, false, SPI_device);


void setup() {
  pinMode(PA2, OUTPUT);
  digitalWrite(PA2, LOW);

  radio1.initialize(RF69_868MHZ, 1, 30);
  radio2.initialize(RF69_868MHZ, 2, 30);
}
