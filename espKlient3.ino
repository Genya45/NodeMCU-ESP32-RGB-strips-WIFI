
#include <ESP8266WiFi.h>

//#define ledPinR 14 // D5
//#define ledPinG 12 // D6
//#define ledPinB 13 // D7

#define ledPinR 12 // D5
#define ledPinG 14 // D6
#define ledPinB 15 // D8

#define maxPMW 1023



IPAddress ip(192, 168, 8, 200); //static ip
IPAddress gateway(192, 168, 8, 254);
IPAddress subnet(255, 255, 255, 0);
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
int modeLED = 0;

void setup() {

  Serial.begin(115200);

  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);


  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

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
      client.println(".inputH {  background: linear-gradient(to right, hsl(0, 100%, 50%) 0%,hsl(14.4,100%,51%) 4%,hsl(28.8,100%,51%) 8%,hsl(43.2,100%,51%) 12%,hsl(57.6,100%,51%) 16%,hsl(72,100%,51%) 20%,hsl(86.4,100%,51%) 24%,hsl(100.8,100%,51%) 28%,hsl(115.2,100%,51%) 32%,hsl(129.6,100%,51%) 36%,hsl(144,100%,51%) 40%,hsl(158.4,100%,51%) 44%,hsl(172.8,100%,51%) 48%,hsl(187.2,100%,51%) 52%,hsl(201.6,100%,51%) 56%,hsl(216,100%,51%) 60%,hsl(230.4,100%,51%) 64%,hsl(244.8,100%,51%) 68%,hsl(259.2,100%,51%) 72%,hsl(273.6,100%,51%) 76%,hsl(288,100%,51%) 80%,hsl(302.4,100%,51%) 84%,hsl(316.8,100%,51%) 88%,hsl(331.2,100%,51%) 92%,hsl(345.6,100%,51%) 96%,hsl(360,100%,51%) 100%);}");
      client.println(".inputS {  background: linear-gradient(to right, #ffffff 0%, hsl(" + valueStringH + ", 100%, 100%) 100%);}");
      client.println(".inputV {  background: linear-gradient(to right, #000000 0%, #ffffff 100%);}");
      client.println(".divColor{  width: 50px;height: 50px;background: hsl(" + valueStringH + ", 100%, 50%);border-radius: 25px;margin: auto;}");
      client.println("input[type=range] {    -webkit-appearance: none;     border-color: #000000;  border: 1px solid #000000;}");
      client.println("input[type='range']::-webkit-slider-thumb {-webkit-appearance: none;background-color: #ecf0f1;border: 1px solid #bdc3c7;width: 20px;height: 20px;border-radius: 10px;cursor: pointer;}");

      client.println(".button {    background-color: #008CBA;   border: none;    color: white;    padding: 15px 32px;    text-align: center;    text-decoration: none;    display: inline-block;    font-size: 16px;    transition-duration: 0.4s;}");
      client.println(".button:hover {background-color: #006666;    color: white;}");
      client.println(".button:active {  background-color: #3e8e41;  box-shadow: 0 5px #666;  transform: translateY(4px);  }");

      client.println(".slider { width: 300px;  border-radius: 4px; } </style> ");
      // веб-страница:
      client.println(" </head> <body><h1>LED WIFI </h1 > ");
      client.println("<div class = \"divColor\"></div>");
      client.println("<p>Hue: <span id=\"sliderH\">" + valueStringH + "</span>");
      client.println("<input type=\"range\" min=\"0\" max=\"360\" class=\"slider inputH\" id=\"HSlider\" onchange=\"SliderH(this.value)\" value=\"" + valueStringH + "\"/></p>");

      client.println("<p>Saturation: <span id=\"sliderS\">" + valueStringS + "</span>");
      client.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider inputS\" id=\"SSlider\" onchange=\"SliderS(this.value)\" value=\"" + valueStringS + "\"/> </p>");

      client.println("<p>Brightness: <span id=\"sliderV\">" + valueStringV + "</span>");
      client.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider inputV\" id=\"VSlider\" onchange=\"SliderV(this.value)\" value=\"" + valueStringV + "\"/> </p>");


      client.println("<p> <input type=\"button\" value=\"1\" class=\"button\" onclick=\"buttonClick(this.value)\"> <input type=\"button\" value=\"2\" class=\"button\" onclick=\"buttonClick(this.value)\"> <input type=\"button\" value=\"3\" class=\"button\" onclick=\"buttonClick(this.value)\"> </p>");
      client.println("<p> <input type=\"button\" value=\"4\" class=\"button\" onclick=\"buttonClick(this.value)\"> <input type=\"button\" value=\"5\" class=\"button\" onclick=\"buttonClick(this.value)\"> <input type=\"button\" value=\"6\" class=\"button\" onclick=\"buttonClick(this.value)\"> </p>");
      client.println("<p> <input type=\"button\" value=\"7\" class=\"button\" onclick=\"buttonClick(this.value)\"> <input type=\"button\" value=\"8\" class=\"button\" onclick=\"buttonClick(this.value)\"> <input type=\"button\" value=\"9\" class=\"button\" onclick=\"buttonClick(this.value)\"> </p>");



      client.println("<script>var slideH = document.getElementById(\"HSlider\");");
      client.println("var slH = document.getElementById(\"sliderH\");");
      client.println("slH.innerHTML = slideH.value;");
      client.println("slideH.oninput = function() {slH.innerHTML = this.value; fetch(\"/?value=\" + this.value + \"_\" + slideS.value + \"_\" + slideV.value + \"&\");");
      client.println("document.getElementsByClassName('divColor')[0].style.background=\"hsl(\"+slideH.value+\", 100%, 50%)\";");
      client.println("slideS.style.background = \"linear-gradient(to right, #ffffff 0%, hsl(\"+slideH.value+\", 100%, 50%) 100%)\";}");
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



      client.println("function buttonClick(text){fetch(\"/?b=\" + text +\"&\");}");



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
          if (header.indexOf("GET /?b=") >= 0) {
            pos1 = header.indexOf('=');
            pos2 = header.indexOf('&');
            String parseB;
            for (int i = pos1 + 1; i < pos2; i++) {
              parseB += header[i];
            }
            modeLED = parseB.toInt();
            //Serial.println(parseB);
          }
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
