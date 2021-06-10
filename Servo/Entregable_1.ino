#include <Adafruit_NeoPixel.h>
#include <Servo.h>
   #include <ESP8266WiFi.h>
//Include Firebase library (this library)
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "MIWIFI_2G_CmKP" ////////////////////AQUI VA EL WIFI/////////////////
#define WIFI_PASSWORD "Jv4f4Cj6"    ////////////////////PASSWORD/////////////////

#define API_KEY "AIzaSyD_pK8vOIPqWIFSv3LE21JXJZCI0nfMh1o" //////////////COPIAR TAL CUAL///////////////////////
#define DATABASE_SECRET "DATABASE_SECRET"
#define USER_EMAIL "prova1@gmail.com"   //////////////usuari de prova, sino executa android i creat el que vulguis////////////////////////
#define USER_PASSWORD "123456"
unsigned long dataMillis = 0;
int count = 0;
#define DATABASE_URL "androidarduino-68fe7-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;




  void setup() {
 Serial.begin(115200);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL */
    config.database_url = DATABASE_URL;

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);

    String base_path = "/UsersData/";

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    /** Assign the maximum retry of token generation */
    config.max_token_generation_retry = 5;

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    String var = "$userId";
    String val = "($userId === auth.uid && auth.token.premium_account === true && auth.token.admin === true)";
    Firebase.RTDB.setReadWriteRules(&fbdo, base_path.c_str(), var.c_str(), val.c_str(), val.c_str(), DATABASE_SECRET);
    pinMode(BUTTON_HIGH, INPUT_PULLUP);
    pinMode(BUTTON_LOW, INPUT_PULLUP);
    pinMode(BUTTON_EMERGENCY, INPUT_PULLUP);
    myservo.attach(servoPin);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    myservo.write(90);
  }

  void loop() {
/////////////////////////////RECUPEREM DE FIREBASE ANDROID->ARDUINO////////////////////////////////////////////////////////////
 if (millis() - dataMillis > 1000 && Firebase.ready())
    { //recupera LED
        dataMillis = millis();
        String pathLED = "/UsersData/";
        pathLED += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
        pathLED += "/dades/int";
        Firebase.RTDB.getInt(&fbdo, pathLED.c_str());
        int state_fb = fbdo.intData();
        Serial.printf("ns: %d\n\n", ns);

        pathLED = "/UsersData/";
        pathLED += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
        pathLED += "/dades/int";
        Firebase.RTDB.getInt(&fbdo, pathLED.c_str());
        int lluminositat_fb = fbdo.intData();
        Serial.printf("ns: %d\n\n", ns);
        if(ns != Stats){
                StatsMachine(ns, lluminositat_fb);
        }
    }
   


////////////////////////////////////////////////////AIXO ES LA FUNCIO QUE ENVIA A FIREBASE (ARDUINO--> ANDROID)//////////////////////////////////////////////////
    Serial.printf("State: %d\n\n", i);
      /*String pathLED = "/UsersData/";
       pathLED += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
       pathLED += "/dades/int"; (variar el valor int per el que recuperem a android--> sens, fall, humi
      Firebase.RTDB.setInt(&fbdo, pathLED.c_str(), i);*/
    switch(i){
      case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off //cascar nivells intensitat
              myservo.write(movv);
              delay(500);
              myservo.write(sttop);
              Serial.printf("movv: %d\n\n", movv);
              Serial.printf("sttop: %d\n\n", sttop);
              break;
      case 1: colorWipe(strip.Color(0, 255, 0), 50);  // Green
              myservo.write(movv);
              delay(500);
              myservo.write(sttop);
              Serial.printf("movv: %d\n\n", movv);
              Serial.printf("sttop: %d\n\n", sttop);
              break;    
      case 2: colorWipe(strip.Color(255, 255, 0), 50);  // Yellow
              myservo.write(movv);
              delay(500);
              myservo.write(sttop);
              Serial.printf("movv: %d\n\n", movv);
              Serial.printf("sttop: %d\n\n", sttop);
              break;    
      case 3: colorWipe(strip.Color(255, 165, 0), 50);  // Orange
              myservo.write(movv);
              delay(500);
              myservo.write(sttop);
              Serial.printf("movv: %d\n\n", movv);
              Serial.printf("sttop: %d\n\n", sttop);
              break;       
      case 4: colorWipe(strip.Color(255, 0, 0), 50);  // Red
              myservo.write(movv);
              delay(500);
              myservo.write(sttop);
              Serial.printf("movv: %d\n\n", movv);
              Serial.printf("sttop: %d\n\n", sttop);
              break;
  
    }
  }
  
  
  // Fill the dots one after the other with a color
  void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      //delay(wait);

    }
  }
