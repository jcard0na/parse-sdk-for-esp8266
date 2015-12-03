#include <ESP8266WiFi.h>
#include <Parse.h>
#
// NOTE: import Tempereture.json into yoru Parse app before running the test

const char* ssid = "XXXX";
const char* password = "XXXX";

void assert(bool test, int line) {
  if (!test)
    Serial.printf("Assertion failed in line %d\n", line);
}

void basicObjectTest() {
  Serial.println("basic object operation test");
  char* objectId = new char[10];


  Serial.println("create...");
  ParseObjectCreate create;
  create.setClassName("Temperature");
  create.add("temperature", 175.0);
  create.add("leverDown", true);
  {
      ParseResponse createResponse = create.send();
      strcpy(objectId, createResponse.getString("objectId"));
      assert(createResponse.getErrorCode() == 0, __LINE__);
      createResponse.close();
  }

  Serial.println("update...");
  ParseObjectUpdate update;
  update.setClassName("Temperature");
  update.setObjectId(objectId);
  update.add("temperature", 100);
  {
      ParseResponse updateResponse = update.send();
      assert(updateResponse.getErrorCode() == 0, __LINE__);
      updateResponse.close();
  }

  Serial.println("get...");
  ParseObjectGet get;
  get.setClassName("Temperature");
  get.setObjectId(objectId);
  {
      ParseResponse getResponse = get.send();
      double temp = getResponse.getDouble("temperature");
      assert(temp == 100, __LINE__);
      getResponse.close();
  }

  Serial.println("delete...");
  ParseObjectDelete del;
  del.setClassName("Temperature");
  del.setObjectId(objectId);
  {
  ParseResponse delResponse = del.send();
  String expectedResp = "{}\n";
  assert(expectedResp.equals(delResponse.getJSONBody()), __LINE__);
  delResponse.close();
  }

  Serial.println("test passed\n");
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Parse.begin(%%%YOUR_APPLICATION_KEY%%%, %%%YOUR_CLIENT_API_KEY%%%);
  delay(100);
  Serial.println("Parse hello world example started");
}

void loop() {
  delay(100);
  basicObjectTest();
  delay(1000);
}
