
#include <ESP8266WiFi.h>

//#define ledPinR 14 // D5
//#define ledPinG 12 // D6
//#define ledPinB 13 // D7

#define ledPinR 12 // D5
#define ledPinG 14 // D6
#define ledPinB 15 // D8

#define maxPMW 1023



WiFiServer server(80);

const char* ssid     = "NoIntetnet";
const char* password = "password";

String header;
String valueStringH = String(5);
String valueStringS = String(5);
String valueStringV = String(5);
int pos1 = 0;
int pos2 = 0;
int globalR, globalG, globalB;

void HSVtoRGB(int H, int S = 100, int V = 100);
void turnRGB(int, int, int);

void setup() {

  Serial.begin(115200);

  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);


  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  digitalWrite(BUILTIN_LED, 1);

  turnRGB(globalR, globalG, globalB);

  WiFiClient client = server.available();

  if (client) {

    if (true) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      //  "Соединение: отключено"
      client.println();

      // показываем веб-страницу:
      client.println("<!DOCTYPE html><html>");
      client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      client.println("<link rel=\"icon\" href=\"data:,\">");
      client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
      client.println(".slider { width: 300px; }</style>");
      // веб-страница:
      client.println("</head><body><h1>LED WIFI</h1>");

      client.println("<p>Hue: <span id=\"sliderH\">" + valueStringH + "</span>");
      client.println("<input type=\"range\" min=\"0\" max=\"360\" class=\"slider\" id=\"HSlider\" onchange=\"SliderH(this.value)\" value=\"" + valueStringH + "\"/></p>");

      client.println("<p>Saturation: <span id=\"sliderS\">" + valueStringS + "</span>");
      client.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"SSlider\" onchange=\"SliderS(this.value)\" value=\"" + valueStringS + "\"/> </p>");

      client.println("<p>Brightness: <span id=\"sliderV\">" + valueStringV + "</span>");
      client.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"VSlider\" onchange=\"SliderV(this.value)\" value=\"" + valueStringV + "\"/> </p>");


      client.println("<script>var slideH = document.getElementById(\"HSlider\");");
      client.println("var slH = document.getElementById(\"sliderH\");");
      client.println("slH.innerHTML = slideH.value;");
      client.println("slideH.oninput = function() {slH.innerHTML = this.value; fetch(\"/?value=\" + this.value + \"_\" + slideS.value + \"_\" + slideV.value + \"&\"); }");
      client.println("function SliderH(pos) {slH.innerHTML = pos; } ");

      client.println("var slideS = document.getElementById(\"SSlider\");");
      client.println("var slS = document.getElementById(\"sliderS\");");
      client.println("slS.innerHTML = slideS.value;");
      client.println("slideS.oninput = function() {slS.innerHTML = this.value; fetch(\"/?value=\" + slideH.value + \"_\" + this.value  + \"_\" + slideV.value + \"&\"); }");
      client.println("function SliderS(pos) {slS.innerHTML = pos; } ");

      client.println("var slideV = document.getElementById(\"VSlider\");");
      client.println("var slV = document.getElementById(\"sliderV\");");
      client.println("slV.innerHTML = slideV.value;");
      client.println("slideV.oninput = function() {slV.innerHTML = this.value; fetch(\"/?value=\" + slideH.value + \"_\" + slideS.value + \"_\" + this.value + \"&\");  }");
      client.println("function SliderV(pos) {slV.innerHTML = pos; } ");





      client.println("</script>");



      client.println("</body></html>");


      // конец HTTP-ответа задается
      // с помощью дополнительной пустой строки:
      client.println();
      // выходим из цикла while():

    }


    turnRGB(globalR, globalG, globalB);
    digitalWrite(BUILTIN_LED, 0);
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      turnRGB(globalR, globalG, globalB);
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          //GET /?value=180& HTTP/1.1
          if (header.indexOf("GET /?value=") >= 0) {
            pos1 = header.indexOf('=');
            pos2 = header.indexOf('&');

            String getH, getS, getV;

            for (int i = pos1 + 1, bb = 0; i < pos2; i++) {
              if (header[i] == '_') {
                bb++;
                continue;
              }
              switch (bb) {
                case 0:
                  getH += header[i];
                  break;
                case 1:
                  getS += header[i];
                  break;
                case 2:
                  getV += header[i];
                  break;
              }

            }

            Serial.println(getH);
            Serial.println(getS);
            Serial.println(getV);
            valueStringH = getH;
            valueStringS = getS;
            valueStringV = getV;

            HSVtoRGB(getH.toInt(), getS.toInt(), getV.toInt());
          }
          else {
            currentLine = "";
          }



          break;
        }
      }
    }
    header = "";
    //client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}




void HSVtoRGB(int H, int S, int V) {

  if (H == 360) {
    H = 0;
  }
  int Hi = (H / 60);
  int Vmin = ((100 - S) * V) / 100;
  int a = ((H % 60) * (V - Vmin)) / 60;
  int Vinc = Vmin + a;
  int Vdec = V - a;

  int r, g, b;
  r = g = b = 0;

  switch (Hi) {
    case 0:
      r = V;
      g = Vinc;
      b = Vmin;
      break;
    case 1:
      r = Vdec;
      g = V;
      b = Vmin;
      break;
    case 2:
      r = Vmin;
      g = V;
      b = Vinc;
      break;
    case 3:
      r = Vmin;
      g = Vdec;
      b = V;
      break;
    case 4:
      r = Vinc;
      g = Vmin;
      b = V;
      break;
    case 5:
      r = V;
      g = Vmin;
      b = Vdec;
      break;
  }
  Serial.println("");

  globalR = (r * maxPMW) / 100;
  globalG = (g * maxPMW) / 100;
  globalB = (b * maxPMW) / 100;
  Serial.print("R = ");
  Serial.println(globalR);
  Serial.print("G = ");
  Serial.println(globalG);
  Serial.print("B = ");
  Serial.println(globalB);
  Serial.println("");

}


void turnRGB(int r, int g, int b) {
  analogWrite(ledPinR, maxPMW - r);
  analogWrite(ledPinG, maxPMW - g);
  analogWrite(ledPinB, maxPMW - b);
}
