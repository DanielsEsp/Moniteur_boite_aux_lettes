//                           Esp01 LetterBox Monitor
//                           22/08/2021  V1-M00      
//                           30/08/2021  V2-M00     modif pour instal 2 contacts


#include <ESP8266WiFi.h>
#include <espnow.h>
         uint8_t broadcastAddress[] = {0x30, 0x83, 0x98, 0x94, 0x50, 0x6A};// Esp01 relais
         typedef struct struct_message {
                    int idModule;
                    bool Lettre;
                    bool Colis;
                      } struct_message;

         struct_message myData;
         int countSend = 0;
         bool etatSend = false;

#define porte 3
        bool actionPorte;
        int Module = 999;
        bool actionLettre;
        bool actionColis;


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
     esp_now_register_send_cb(OnDataSent);
     esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

     pinMode (porte, INPUT);

}

 
void loop() {
  
     actionPorte = digitalRead (porte);
     Serial.println ();
     
     if (actionPorte == true) {  
        actionColis = true; actionLettre = false;
        Serial.print ("Action COLIS :  "); Serial.print (actionColis); Serial.println (actionLettre);                                                              
     } else {
        actionLettre = true; actionColis = false;
        Serial.print ("ACTION LETTRE :  "); Serial.print (actionLettre); Serial.println (actionColis);
     }
    
     myData.idModule = Module;
     myData.Lettre = actionLettre;
     myData.Colis = actionColis;

     if (etatSend == false && countSend < 10) {
         Serial.print ("Envoi data tentative: "); Serial.println (countSend);    
         esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
         delay (100);
         if (etatSend == true) {
            Serial.println ("Envoi réussi !");
            countSend = 10;
         } else {
            Serial.println ("Erreur envoi !");
            countSend ++;
            delay (1000);
         }
     }
     if (etatSend == true || countSend == 10) {
        ESP.deepSleep(0);
     }
     
}
