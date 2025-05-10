//                            Esp01 LetterBox Relais
//                            05/09/2021  V1-M00 


#include <ESP8266WiFi.h>
#include <espnow.h>
         uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xCD, 0x50, 0xA8};// ESP32 AZ TFT MOD
         typedef struct struct_message {
                    int idModule;
                    bool Lettre;
                    bool Colis;
                      } struct_message;

         struct_message myIncomingData;
         struct_message myData;
         bool etatSend = false;
         

void OnDataRecv (uint8_t * mac, uint8_t *incomingData, uint8_t len) {

     memcpy(&myIncomingData, incomingData, sizeof(myIncomingData));
     Serial.print("Bytes received: ");
     Serial.println(len);
     Serial.println (myIncomingData.Lettre);
     Serial.println (myIncomingData.Colis);
     myData.Lettre = myIncomingData.Lettre;
     myData.Colis = myIncomingData.Colis;
     Serial.println (myData.Lettre);
     Serial.println (myData.Colis);
     esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
     delay (100);
     
}


void OnDataSent (uint8_t *mac_addr, uint8_t sendStatus) {
  
     Serial.print("\r\nLast Packet Send Status: ");
     if (sendStatus == 0){
        Serial.println("Delivery success");
        etatSend = true;
     }
     else{
         Serial.println("Delivery fail");
         etatSend = false;
     }
     
}

 
void setup() {
  
     Serial.begin(115200);

     WiFi.mode(WIFI_STA);
     WiFi.disconnect();

     if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
     }

     esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
     esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
     esp_now_register_recv_cb(OnDataRecv);
     esp_now_register_send_cb(OnDataSent);
  
}

  
void loop() {


}
