//Wifi

void connectWifi() {
  state = 1;
  dispNum = 1100;
  display.print(dispNum);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  int r = 0; //Retry Counter
  while (WiFi.status() != WL_CONNECTED && (r < maxWifiRetry)) {
    delay(500);
    Serial.print(".");
    r++;
    dispNum++;
    display.print(dispNum);
  }
  if (r == maxWifiRetry) {
    die(false);
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  delay(300);
  state = 3;
}

void disableWifi() {
  state = 1;
  WiFi.mode(WIFI_OFF); //Disable Wifi
  delay(200);
  state =3;
}
