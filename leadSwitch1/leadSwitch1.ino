void setup() {
  pinMode(13, INPUT);             // 디지털 3번핀을 입력모드로 설정
  Serial.begin(115200);              // 시리얼 통신을 시작하며, 속도는 115200으로 설정
}

void loop() {
  int a = digitalRead(13);          // 변수 a를 선언하여 디지털 3번핀의 값을 입력
  Serial.println(a);                   // 변수 a를 시리얼 모니터에 출력
  delay(100);                        // 1초 대기
}
