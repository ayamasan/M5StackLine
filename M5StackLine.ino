// Bボタン（まん中のボタンを押すとラインにメッセージを送信

#include <M5Stack.h>
#include <WiFiClientSecure.h>

const char* ssid     = "xxxxxxx";
const char* password = "xxxxxxx";
const char* host     = "notify-api.line.me";
const char* token    = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

unsigned long displayTime = 0;
int num = 100;  // 送信データ

boolean line_notify(String msg) 
{
	WiFiClientSecure client;
	client.setInsecure();
	if(!client.connect(host, 443)){
		Serial.println("connect error!");
		M5.Lcd.setTextColor(BLACK, RED);
		M5.Lcd.println("  ERROR : LINE connect error. ");
		return false;
	}
	String query = String("message=") + msg;
	String request = String("")
		+ "POST /api/notify HTTP/1.1\r\n"
		+ "Host: " + host + "\r\n"
		+ "Authorization: Bearer " + token + "\r\n"
		+ "Content-Length: " + String(query.length()) +  "\r\n"
		+ "Content-Type: application/x-www-form-urlencoded\r\n\r\n"
		+ query + "\r\n";
	client.print(request);
	return true;
}


void setup() 
{
	// put your setup code here, to run once:
	M5.begin();
	M5.Lcd.fillScreen(0);
	M5.Lcd.setTextFont(4);
	M5.Lcd.setTextColor(WHITE, BLACK);
	
	M5.Lcd.println(" WIFI connecting...");
	WiFi.begin(ssid, password);
	int i = 0;
	while(WiFi.status() != WL_CONNECTED){
		delay(100);
		i++;
		if(i>100){  // 10secタイムアウト
			M5.Lcd.setTextColor(BLACK, RED);
			M5.Lcd.println(" ERROR : cannot connect to WiFi. ");
			break;
		}
	}
	// WiFi接続成功
	if(i <= 100){
		M5.Lcd.println(" Connected.");
	}
	displayTime = millis();
}


void loop() 
{
	// put your main code here, to run repeatedly:
	M5.update();
	if((WiFi.status() == WL_CONNECTED)){
		if(M5.BtnB.wasReleased()){
			M5.Lcd.fillScreen(BLACK);
			M5.Lcd.setTextFont(4);
			M5.Lcd.setTextColor(WHITE, BLACK);
			M5.Lcd.drawNumber(num, 60, 100, 8);
			String msg = "[" + String(num) + "]を送信しました。";
			// ラインへ送信
			line_notify(msg);
			displayTime = millis();
			num++;
		}
		// 1秒後に画面クリア
		if(millis() - displayTime >= 1000) {
			M5.Lcd.fillScreen(BLACK);
		}
	}
	
}
