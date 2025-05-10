


//******  MAC Addresss AZ LetterBox MOD  C0:49:EF:CD:50:A8
//******  Proto Github (Usb Power Only)  10:52:1C:64:76:64

#include <Arduino.h>  // Pour sound configuration

#include "FS.h"
#include "SPIFFS.h"
#define   FS_NO_GLOBALS

//____________________________________________________________________________ 
#include <SPI.h>
#include <TFT_eSPI.h>
          TFT_eSPI tft = TFT_eSPI();
#define   tftLed 15
#define   pinBeeper 21

#define   NOMBRE_BOUTONS 2
          TFT_eSPI_Button bouton[NOMBRE_BOUTONS];
          uint16_t calData[6] = { 223, 3392, 303, 1, 7990 };
          uint8_t calDataOK = 0;
          int numero;
          bool alarme = false;
          bool silence = false;
          bool raz = false;
          bool arrivee = true;
          int compTone;
                           
//____________________________________________________________________________ 

#include <WiFi.h>
#include <esp_now.h>
         typedef struct struct_message {
                    int idModule;
                    bool Lettre;
                    bool Colis;
                      } struct_message;

         struct_message boxData;
         bool action = false;
         bool actionLettre = false;
         bool actionColis = false;
//_________________________________________________________________________


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

     memcpy(&boxData, incomingData, sizeof(boxData));
     Serial.print("Bytes received: ");
     Serial.println(len);
     Serial.print("idModule: ");
     Serial.println(boxData.idModule);
     Serial.print("Lettre: ");
     Serial.println(boxData.Lettre);
     if (actionLettre == false) {
         actionLettre = boxData.Lettre;
        }
     Serial.print("Colis: ");
     Serial.println(boxData.Colis);
     if (actionColis == false) {
         actionColis = boxData.Colis;
        }
     Serial.println();
     action = true;
       
}


void setup() {

     Serial.begin(115200); Serial.println();

     tft.init();
     tft.setRotation(3);  // portrait: 0 ou 2, paysage: 1 ou 3.
     pinMode(tftLed, OUTPUT); digitalWrite(tftLed, LOW);
     tft.fillScreen(TFT_BLACK);
     tft.setTextColor(TFT_WHITE); 

     if (!SPIFFS.begin()) {
         Serial.println("SPIFFS initialisation failed!");
         while (1) yield();
        }
     Serial.println("\r\nSPIFFS Ok");
     drawBmp("/initial.bmp", 0, 0);

     WiFi.mode(WIFI_STA);    
     if (esp_now_init() != ESP_OK) {
         Serial.println("Error initializing ESP-NOW");
         return;
        }
//     esp_now_register_recv_cb(OnDataRecv);  
     esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
       
     delay(1000); tft.fillScreen(TFT_BLACK);
     bouton[1].initButton(&tft, 40, 60, 70, 110, TFT_RED, TFT_BLACK, TFT_TRANSPARENT, "", 1);
     bouton[1].drawButton();
     drawBmp("/releve.bmp", 10, 30);     
     bouton[0].initButton(&tft, 40, 180, 70, 110, TFT_LIGHTGREY, TFT_BLACK, TFT_TRANSPARENT, "", 1);
     bouton[0].drawButton();
     drawBmp("/buzzeron.bmp", 10, 150); 

     //sound configuration
     ledcSetup(0,1E5,12);
     ledcAttachPin(21,0);

     drawBmp("/boitevide.bmp", 80, 0);
     
}


void loop() {

     uint16_t t_x = 0, t_y = 0; // coordonnées touchées par l'utilisateur
     boolean pressed = tft.getTouch(&t_y, &t_x); // vrai si contact avec l'écran !!! INVERSE !!!

//   On vérifie si la position du contact correspond à celle d'un bouton
     for (numero = 0; numero < NOMBRE_BOUTONS; numero++) {
          if (pressed && bouton[numero].contains(t_y - 250, t_x)) { // replace zonu de saisie a gauche
              bouton[numero].press(true);             
              Serial.print("Bouton nr: "); Serial.println(numero);
          ledcWriteTone(0, 2000);
          delay(50);
          ledcWriteTone(0,0);
             } else {
              bouton[numero].press(false);  
             }
         }

//   Vérifions maintenant si l'état d'un des boutons a changé
     for (numero = 0; numero < NOMBRE_BOUTONS; numero++) {

//   si le bouton vient d'être pressé...
          if (bouton[numero].justPressed()) {
//   on fait ce que l'utilisateur a demandé:
              switch (numero) {
                      case 0://  premier bouton
                                 Serial.print("Bouton Silence/Buzzer");
                                 silence = !silence;
                                 if (silence == false) {
                                     drawBmp("/buzzeron.bmp", 10, 150);
                                   } else { 
                                     drawBmp("/buzzeroff.bmp", 10, 150); }
                      break;
                      case 1://  deuxième bouton
                                 Serial.print("Bouton RAZ");
//                                 raz = true;
                                 alarme = false;  
                                 actionLettre = false; actionColis = false;
                                 drawBmp("/boiteouverte.bmp", 80, 0); delay(500); 
                                 drawBmp("/boitevide.bmp", 80, 0);
                      break;
                      }/* switch (numero) */

              delay(100); // anti-rebond

             }/* if (bouton[numero].justPressed()) */

          }/* for (uint8_t numero = 0; numero < NOMBRE_BOUTONS; numero++) */

      if (actionLettre == true & actionColis == false & action == true) {
          action = false;
          drawBmp("/lettre.bmp", 80, 0);
          alarme = true;
         }
      if (actionColis == true & actionLettre == false & action == true) {
          action = false;
          drawBmp("/colis.bmp", 80, 0);
          alarme = true;
         }
      if (actionLettre == true & actionColis == true & action == true) {
          action = false;
          drawBmp("/lesdeux.bmp", 80, 0);
          alarme = true;
         }
      if (alarme == true & silence == false) {
          if (compTone <= 100) {
              ledcWriteTone(0, 3000); compTone ++;       
             }
          if (compTone > 100 & compTone <= 300) {
              ledcWriteTone(0, 0); compTone ++; 
             }
          if (compTone > 300) { compTone = 0; }
         }

}
