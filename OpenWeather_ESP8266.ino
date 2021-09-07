#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define ARDUINOJSON_DECODE_UNICODE 1

////////////////////////////////////
// ここの間が個人で設定する項目です。
const char* ssid = "";
const char* password =  "";
//試しに私が取得したAPIKEYを貼り付けて置きます。
const String apikey = ""; 
////////////////////////////////////
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=tokyo,jp&APPID=";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(endpoint + apikey);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);

      //jsonオブジェクトの作成
      String input = payload;
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, input);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
      }

      //各データを抜き出し　各項目の抜き出し方法は下記に記載してあります。
      JsonObject weather_0 = doc["weather"][0];
      const char* weather_0_main = weather_0["main"];
      
      JsonObject main = doc["main"];
      float main_temp = main["temp"]; // 292.19
      
      Serial.print("weather:");
      Serial.println(weather_0_main);
      Serial.print("temperature:");
      Serial.println(main_temp - 273.15);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  delay(30000);
}


/*データ抜出例
float coord_lon = doc["coord"]["lon"]; // 139.6917
float coord_lat = doc["coord"]["lat"]; // 35.6895

JsonObject weather_0 = doc["weather"][0];
int weather_0_id = weather_0["id"]; // 804
const char* weather_0_main = weather_0["main"]; // "Clouds"
const char* weather_0_description = weather_0["description"]; // "overcast clouds"
const char* weather_0_icon = weather_0["icon"]; // "04n"

const char* base = doc["base"]; // "stations"

JsonObject main = doc["main"];
float main_temp = main["temp"]; // 292.19
float main_feels_like = main["feels_like"]; // 292.29
float main_temp_min = main["temp_min"]; // 291.37
float main_temp_max = main["temp_max"]; // 292.6
int main_pressure = main["pressure"]; // 1017
int main_humidity = main["humidity"]; // 82

int visibility = doc["visibility"]; // 10000

JsonObject wind = doc["wind"];
float wind_speed = wind["speed"]; // 0.45
int wind_deg = wind["deg"]; // 319
float wind_gust = wind["gust"]; // 3.58

int clouds_all = doc["clouds"]["all"]; // 99

long dt = doc["dt"]; // 1630937619

JsonObject sys = doc["sys"];
int sys_type = sys["type"]; // 2
long sys_id = sys["id"]; // 2038398
const char* sys_country = sys["country"]; // "JP"
long sys_sunrise = sys["sunrise"]; // 1630873018
long sys_sunset = sys["sunset"]; // 1630918971

long timezone = doc["timezone"]; // 32400
long id = doc["id"]; // 1850144
const char* name = doc["name"]; // "Tokyo"
int cod = doc["cod"]; // 200
 */
