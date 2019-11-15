//Get File from Server
void fetchTeri() {
  state = 2;
  display.print(1200);
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.print("HTTPS Connecting");
    int r = 0; //retry counter
    dispNum = 1200;
    
    httpsClient.setFingerprint(fingerprint);
    httpsClient.setTimeout(15000);

    delay(100);
    while ((!httpsClient.connect(host, port)) && (r < maxFetchRetry)) {
      delay(100);
      Serial.print(".");
      r++;
      dispNum++;
      display.print(dispNum);
    }
    if (r == maxFetchRetry) {
      Serial.println("Connection failed");
      die(false);
    }
    else {
      Serial.println("Connected to");
    }

    Serial.print("requesting URL: ");
    Serial.println(String(host) + String(param));

    httpsClient.print(String("POST ") + param + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Content-Type: application/x-www-form-urlencoded\r\n" +
                      "Content-Length: " + String(String(apikey).length() + 7) + "\r\n" + 
                      "Connection: close\r\n\r\n" + 
                      "apikey=" + apikey);

    Serial.println("request sent");

    while (httpsClient.connected()) {
      String line = httpsClient.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        display.print(1250);
        break;
      }
    }
    String tmp;
    int counter = 0;
    int totalUser = -1;
    display.print(1260);
    while (httpsClient.available()) {
      //Read Line by Line
      tmp = httpsClient.readStringUntil('\n'); 
      if (counter == 1) {
        //The second line submits the total amount of alowed UIDs
        totalUser = tmp.toInt();
        Serial.println("Length: " + String(totalUser));
      }
      //Write UID into Array
      else if (counter-2 < totalUser && counter-2 < maxUser) {
        allowedUID[counter-2] = tmp;
      }
      counter++;
    }
    if (counter <= 2) {
      die(false);
    }
    Serial.println("closing connection");

  }
  state = 3;
}
