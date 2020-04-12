#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <credentials.h>
#define RELAY D7

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.print(topic);
    payload[length] = '\0';
    String s = String((char *)payload);
    Serial.println(s);

    if (s.equals(String("ON"))) {
        digitalWrite(RELAY, LOW);
    } else {
        digitalWrite(RELAY, HIGH);
    }
};

void connect() {
    while(!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
            Serial.println("connected");
            client.subscribe("rishabarora2008@gmail.com/Automation");
        }
        else {
            Serial.println("failed with state: ");
            Serial.print(client.state());
            Serial.println("trying again in 5 seconds");
            delay(5000);
        }
    }
};

void setup() {
    Serial.begin(115200);
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, HIGH);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Waiting to connect to WiFi");
    }
    Serial.println("WiFi is connected");
    Serial.println(WiFi.localIP());

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    connect();

    client.subscribe("rishabarora2008@gmail.com/Automation");
}

void loop() {
    if (!client.connected()){
        connect();
    }
    client.loop();
}    