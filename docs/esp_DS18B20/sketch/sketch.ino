#include <OneWire.h>
#include <DallasTemperature.h>

// Định nghĩa chân GPIO 4 là chân dữ liệu kết nối với DS18B20
const int oneWireBus = 5;     

// Thiết lập một instance oneWire để giao tiếp với mọi thiết bị OneWire
OneWire oneWire(oneWireBus);

// Truyền instance oneWire cho thư viện Dallas Temperature
DallasTemperature sensors(&oneWire);

void setup(void) {
  Serial.begin(9600);
  
  // Khởi động thư viện đọc nhiệt độ
  sensors.begin();
  Serial.println("Khoi động hệ thống đo nhiệt độ DS18B20...");
}

void loop(void) { 
  // Gửi lệnh yêu cầu đọc nhiệt độ từ tất cả các cảm biến trên bus
  sensors.requestTemperatures(); 
  
  // Lấy giá trị nhiệt độ theo độ C từ cảm biến đầu tiên (index 0)
  float temperatureC = sensors.getTempCByIndex(0);
  
  // Kiểm tra xem cảm biến có phản hồi không
  if(temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.print("Nhiệt độ lốc máy: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  } else {
    Serial.println("Lỗi: Không tìm thấy cảm biến DS18B20!");
  }
  
  // Đợi 2 giây trước khi đọc lại lần tiếp theo
  delay(2000);
}