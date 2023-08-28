#define BLYNK_TEMPLATE_ID           "TMPLhqBIE-fe"
#define BLYNK_DEVICE_NAME           "PROJECT"
#define BLYNK_AUTH_TOKEN            "nux-Xvkl74-kchQR_RYwsujVp-PS_iQk"

#include <BlynkSimpleEsp32.h>
#include <Ultrasonic.h>


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PLUSNET-3HN9";
char pass[] = "d293a27367";

BlynkTimer timer;

#define trig1 D3
#define echo1 D2
#define trig2 13
#define echo2 14

#define LED 17

Ultrasonic ultrasonic1(trig1, echo1);
Ultrasonic ultrasonic2(trig2, echo2);

int personInCounter = 0;
int personOutCounter = 0;
int currentCounter = 0;

int inStat;
int outStat;

BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V4, 0);
  Blynk.virtualWrite(V5, 0);
  Blynk.virtualWrite(V7, 0);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();

  inStat =  ultrasonic1.read();
  outStat = ultrasonic2.read();

  
  if (inStat < 20){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Person In");
    personInCounter = personInCounter + 1;
    Serial.print("Entered: ");
    Serial.println(personInCounter);
    Blynk.virtualWrite(V4, personInCounter);
    currentCounter = currentCounter + 1;
    Blynk.virtualWrite(V7, currentCounter);
  }
  if (outStat < 20 && personOutCounter < personInCounter){
    Serial.println("Person Out");
    personOutCounter = personOutCounter + 1;
    Serial.print("Exited: "); 
    Serial.println(personOutCounter);
    Blynk.virtualWrite(V5, personOutCounter);
    currentCounter = currentCounter - 1;
    Blynk.virtualWrite(V7, currentCounter);
  }
  if (personInCounter == personOutCounter){
    digitalWrite(LED, LOW);
    Blynk.virtualWrite(V6, "OFF");
  } else {
    digitalWrite(LED, HIGH);
    Blynk.virtualWrite(V6, "ON");
  }
  delay(500);
}
