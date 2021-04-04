
#include <SPI.h>
#include <Wire.h>
#include <RH_ASK.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define SSD1306_LCDHEIGHT 64
RH_ASK rf_driver;

String t_ate;
String h_ate;

String t_poele;
String h_poele;

void setup()
{ display.clearDisplay();
  rf_driver.init();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  Serial.begin(9600);
}

void loop()
{
  updateDisplay();
  // Set buffer to size of expected message
  uint8_t buf[30];//change 15 to 25
  uint8_t buflen = sizeof(buf);
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen)) {


    char msg[buflen];
    //PARSE incoming byte array from the transmitter
    //
    String ID = "";
    String h_str = "";
    String t_str = "";
    int sepIndex = 0;
    for (int i = 0; i < buflen; i++) {
      msg[i] = (char)buf[i];
      if ((char) buf[i] == ':') {
        //Identifie separator in the message
        sepIndex++;
        continue;
      }
      switch (sepIndex) {
        case 0://Before first separator : ID 
          ID = ID + (char)buf[i];
          break;
        case 1://After first separator : humidity
          h_str = h_str + (char)buf[i];
          break;
        case 2:// After second separator : temperature
          t_str = t_str + (char)buf[i];
      }

    }
    msg[buflen] =  '\0';
    Serial.print("Received-");
    Serial.println(msg);

    // Store the new value in memory
    if (ID == "ATE") {
      t_ate = t_str;
      h_ate = h_str;
    }
    else if (ID == "Poele") {
      t_poele = t_str;
      h_poele = h_str;
    }
   
    //Serial.println(ID);


  }

}
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1.8);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //Toggle the LCD display between the two readings from
  //The two transmitters. 
  if (millis() / 2000 % 2 == 0) {
    display.println("Atelier");
    display.println("T = " + t_ate + " C");
    display.println("H = " + h_ate+" %");
  }
  else {
    display.println("Poele");
    display.println("T = " + t_poele + " C");
    display.println("H = " + h_poele+" %");
  }
  display.display();


}
