# Moniteur_boite_aux_lettres

https://github.com/user-attachments/assets/addcd679-7d83-48ec-b5e3-4220b234b712

![recepteur](https://github.com/user-attachments/assets/25d1f1a6-e270-4412-85e2-f4e897c6ba41)

![boite](https://github.com/user-attachments/assets/9a92c868-9bde-48ba-a1b4-d735c533ade5)

Montage à base d'un Esp01 avec un contact ILS sur le volet courrier et un autre sur le portillon pour envoyer l'information de réception d'une lettre ou d'un colis sur un module récepteur "at home" du type AZ TFT MOD de 2,8' Touch équipé d'un Esp32.
Dans mon cas, j'ai même mis en place un module relais sur la base d'un Esp01, le récepteur étant trop éloignés. 
Utilisation du protocole EspNow pour la communication.

Schéma du circuit de commande pour la détection "action volet" pour réception courrier et "action porte" lors de l'arrivée d'un colis.
![SchemaBoitte](https://github.com/user-attachments/assets/761221e4-644a-47f2-b587-dc86ec19d954)
L'alimentation est confiée à un coupleur de 3 piles "AA" au lithium type "ENERGIZER"
et le régulation est assurée par un MCP1700-33 pour son courant de fuite très faible.
Ce montage assure une autonomie de trois à quatres mois.

L'arrivée d'une lettre ou d'un colis est signalé par l'icone de la boîte aux lettres et d'un signal sonore.
Le signal sonore peut être coupé ou rétablis par l'appui sur la touche "cloche".
La relève se valide par appui sur la touche adéquate qui affichera à nouveau la boîte vide.




