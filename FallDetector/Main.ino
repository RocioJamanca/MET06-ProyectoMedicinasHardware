// Main
#include <ESP8266WiFi.h>
//Include Firebase library (this library)
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "" ////////////////////AQUI VA EL WIFI/////////////////
#define WIFI_PASSWORD ""    ////////////////////PASSWORD/////////////////

#define API_KEY "AIzaSyD_pK8vOIPqWIFSv3LE21JXJZCI0nfMh1o"
#define DATABASE_SECRET "DATABASE_SECRET"
#define USER_EMAIL "anna@gmail.com"   
#define USER_PASSWORD "123456"
unsigned long dataMillis = 0;
int count = 0;
#define DATABASE_URL "androidarduino-68fe7-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// PINS
#define DHTPIN 5  //GPIO5 - D1
#define BUTTONPIN 2 //GPIO2 - D4 
#define PRESENCEPIN1 0  //GPIO0 - D3  n //LowerSensor - RED
#define PRESENCEPIN2 14 //GPIO14 - D5 //UpperSensor - GREEN
// SENSOR CONSTANTS
#define DHTTYPE DHT22       // DHT 22  (AM2302)
#define maximumTemperature 40
#define maximumHumidity 90
#define meaurementInterval 5000

-void setup (){
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
    // Setup serial monitor
  setup_sensor();
  setup_button();
  setup_fallDetector();
}

void loop(){
  read_sensor();
  if (millis() - dataMillis > 1000 && Firebase.ready())
    {    Serial.printf("State: %d\n\n", i);
      String pathLED = "/UsersData/";
       pathLED += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
       pathLED += "/dades/int";// (variar el valor int per el que recuperem a android--> sens, fall, humi
      Firebase.RTDB.setInt(&fbdo, pathLED.c_str(), i);
        }
    }     
}
