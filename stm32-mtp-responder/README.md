
## MTP RESPONDER FOR STM32  

idée n°1 : suppr /class, garder /core  
implémenter les descriptors + callback de classe de zéro  mais utiliser connecter à usbd-core avec `USBD_ClassTypeDef`  


filtre wireshark souris : usb.src != "2.4.1" && usb.dst != "2.4.1"

