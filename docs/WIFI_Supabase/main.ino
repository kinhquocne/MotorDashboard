#include <WiFi.h>
#include <HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <secrets.h>

// 3. Cấu hình chân DS18B20
const int oneWireBus = 5;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  //DS18B20
  sensors.begin();
  Serial.println("Khoi động hệ thống đo nhiệt độ DS18B20...");
  
  // Bắt đầu kết nối Wi-Fi
  Serial.print("Đang kết nối mạng Wokwi-GUEST...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //WL_CONNECTED là const 3, đã kết nối và có ip
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối Wi-Fi thành công!");
}

void loop() {
  //DS18B20
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) {   //DEVICE_DISCONNECTED_C là -127
    Serial.print("Nhiệt độ lốc máy: "); 
    Serial.print(tempC);
    Serial.println(" °C");
    
    // Kiểm tra Wi-Fi và đẩy dữ liệu lên Supabase
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http; //tạo trình duyệt ảo giống chrome,.. cho esp32
      http.begin(supabase_url);
      
      // Các Header bắt buộc để vượt qua cổng bảo mật của Supabase
      http.addHeader("apikey", supabase_key);
      http.addHeader("Authorization", String("Bearer ") + supabase_key);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Prefer", "return=minimal");

      // Đóng gói dữ liệu thành chuẩn JSON
      String payload = "{\"temperature\": " + String(tempC) + "}";
      
      // Gửi đi bằng phương thức POST
      int httpResponseCode = http.POST(payload);
      
      if (httpResponseCode == 201) {
        Serial.println("-> Đã lưu lên Supabase thành công! (Mã 201)");
      } else {
        Serial.print("-> Lỗi đẩy dữ liệu. Mã lỗi: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
  } else {
    Serial.println("Lỗi: Không đọc được cảm biến!");
  }
  
  // Nghỉ 5 giây trước khi gửi lần tiếp theo, mẫu 2s để test
  delay(2); 
}