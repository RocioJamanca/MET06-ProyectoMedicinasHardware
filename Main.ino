// Main
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//Include Firebase library (this library)
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include "certs.h"
#define WIFI_SSID "Gatto" ////////////////////AQUI VA EL WIFI          MIWIFI_2G_CmKP/////////////////
#define WIFI_PASSWORD "12345670"    ////////////////////PASSWORD   Jv4f4Cj6/////////////////

#define API_KEY "AIzaSyD_pK8vOIPqWIFSv3LE21JXJZCI0nfMh1o"
#define DATABASE_SECRET "DATABASE_SECRET"
#define USER_EMAIL "prueba@familiar.com"   
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
X509List cert(cert_GTS_CA_1C3);

int a = 0;
int del = 0;
ICACHE_RAM_ATTR void checkFall(){
  int upperSensor = digitalRead(PRESENCEPIN2);
  if(upperSensor == LOW){
    a=1;
    Serial.println("Fall detected");
  }
}

void setup (){
   Serial.begin(115200);
    WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
     Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

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
  pinMode(PRESENCEPIN1, INPUT);
  pinMode(PRESENCEPIN2, INPUT);
  attachInterrupt(digitalPinToInterrupt(PRESENCEPIN1), checkFall, RISING);
}


void loop(){
  if(a==1){
    String temperature = "/usuarios/";
    Serial.println("Fall detected 2");
    temperature += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
    temperature += "/data/fall";// (variar el valor int per el que recuperem a android--> sens, fall, humi
    Serial.println("Fall detected 3");
    Firebase.RTDB.setFloat(&fbdo, temperature.c_str(), 1);
    Serial.println("Fall detected 4");
    a = 0;
    del = millis();
  }else{
    int now = millis();
    if(abs(now - del) > 10*1000){
      String temperature = "/usuarios/";
      Serial.println("Fall detected 2");
      temperature += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
      temperature += "/data/fall";// (variar el valor int per el que recuperem a android--> sens, fall, humi
      Serial.println("Fall detected 3");
      Firebase.RTDB.setFloat(&fbdo, temperature.c_str(), 0);
      Serial.println("Fall detected 4");
  }
 }
 
  unsigned long currentMillis = millis();
  if (currentMillis - dataMillis > 300 && Firebase.ready()) {   
    dataMillis = currentMillis;
    float tmp = read_sensor_Tmp();
    float hum = read_sensor_Hum();
    if(tmp != 0){
      String temperature = "/usuarios/";
      temperature += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
      temperature += "/data/tmp";// (variar el valor int per el que recuperem a android--> sens, fall, humi
      Firebase.RTDB.setFloat(&fbdo, temperature.c_str(), tmp);
      Serial.printf("tmp: %.6f\n\n", tmp);
    }
  
  if(hum != 0){
    String humidity = "/usuarios/";
    humidity += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
    humidity += "/data/hum";// (variar el valor int per el que recuperem a android--> sens, fall, humi
    Firebase.RTDB.setFloat(&fbdo, humidity.c_str(), hum);
    
    Serial.printf("hum: %.6f\n\n", hum);
  } 
  if (tmp >= 37){
    /*frooooooom */
    String data = "{" ;
    data += "\"to\":\"/topics/client1\","; //a qui li enviem
    data += "\"notification\": {";
    data += "\"body\": \"Temperatura demasiado alta\",";
    data +=        "\"title\": \"Temperatura\"";
    data += "} ";
    data += "}";
        String fcmServer = "fcm.googleapis.com";
        const uint16_t port_ssl = 443;
String url = "/fcm/send";

    WiFiClientSecure client;
    Serial.print("Connecting to ");
  Serial.println(fcmServer);

  client.setTrustAnchors(&cert);

  if (!client.connect(googleapis_host, googleapis_port)) {
    Serial.println("Connection failed");
  } else {
    Serial.println("Connected to the server..");
    client.println("POST /fcm/send HTTP/1.1");
    client.println("Host: fcm.googleapis.com");
    client.println("Authorization: key=AAAA5DxBfVI:APA91bHRopXz-kNb7W5-AdxOw1F4ChSqJjw9YPs00HkymE6xCO4kx3ACVoziJ-Wld-PtG84yLvgwe76ttOgyAWKm8zdZ6s5Jb72dZzcNmezBDaG_WoQORJ4etHF5t_O6q_fKljL6BXc8");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.print("\r\n");
    client.print(data);

    Serial.println("POST /fcm/send HTTP/1.1");
    Serial.println("Authorization: key="Add here the key");
    Serial.println("Content-Type: application/json");
    Serial.println("Host: fcm.googleapis.com");
    Serial.print("Content-Length: ");
    Serial.println(data.length());
    Serial.print("\r\n");
    Serial.print(data);
    }

while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("Headers received");
      
      break;
    }
  }
  String line = client.readStringUntil('\n');
Serial.println(line);
/* TOOOOOOOOOOOOOOO */
  }
  if  (hum >= 65)
  {
        /*frooooooom */
    String data = "{" ;
    data += "\"to\":\"/topics/client1\","; //a qui li enviem
    data += "\"notification\": {";
    data += "\"body\": \"Humedad demasiado alta\",";
    data +=        "\"title\": \"Humedad\"";
    data += "} ";
    data += "}";
        String fcmServer = "fcm.googleapis.com";
        const uint16_t port_ssl = 443;
String url = "/fcm/send";

    WiFiClientSecure client;
    Serial.print("Connecting to ");
  Serial.println(fcmServer);

  client.setTrustAnchors(&cert);

  if (!client.connect(googleapis_host, googleapis_port)) {
    Serial.println("Connection failed");
  } else {
    Serial.println("Connected to the server..");
    client.println("POST /fcm/send HTTP/1.1");
    client.println("Host: fcm.googleapis.com");
    client.println("Authorization: key=AAAA5DxBfVI:APA91bHRopXz-kNb7W5-AdxOw1F4ChSqJjw9YPs00HkymE6xCO4kx3ACVoziJ-Wld-PtG84yLvgwe76ttOgyAWKm8zdZ6s5Jb72dZzcNmezBDaG_WoQORJ4etHF5t_O6q_fKljL6BXc8");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.print("\r\n");
    client.print(data);

    Serial.println("POST /fcm/send HTTP/1.1");
    Serial.println("Authorization: key=AAAA5DxBfVI:APA91bHRopXz-kNb7W5-AdxOw1F4ChSqJjw9YPs00HkymE6xCO4kx3ACVoziJ-Wld-PtG84yLvgwe76ttOgyAWKm8zdZ6s5Jb72dZzcNmezBDaG_WoQORJ4etHF5t_O6q_fKljL6BXc8");
    Serial.println("Content-Type: application/json");
    Serial.println("Host: fcm.googleapis.com");
    Serial.print("Content-Length: ");
    Serial.println(data.length());
    Serial.print("\r\n");
    Serial.print(data);
    }

while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("Headers received");
      
      break;
    }
  }
  String line = client.readStringUntil('\n');
Serial.println(line);
/* TOOOOOOOOOOOOOOO */
    }
  
  }
}    
