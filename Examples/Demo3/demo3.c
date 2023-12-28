/*  Demo 3
    Gestione pronto soccorso
    
    L'ospedale ha "MAX_SALE" disponibili per operare i pazienti
    ogni sala opera in parallelo rispetto alle altre.
    
    Ai pazienti viene assegnato un colore che identifica la gravità e un tempo entro il quale
    se l'operazione non viene conclusa la sitauzione del paziente si aggrava:
    
            codice verde --> codice arancione (10 secondi)
            codice arancione --> codice rosso (5 secondi)
            codice rosso --> paziente morto (2.5 secondi)

    
    Ipotesi: 
        - quando un paziente entra in una delle sale operatorie, non può uscire prima che 
    l'operazione sia terminata
        - per semplicità ad ogni codice è assegnato un tempo di durata dell'operazione 
        - i pazienti vengono operati per prorità di colore e differenza tra tempo necessario all'operazione
        di operazione e tempo di arrivo 



    Le sale operatorie sono identificate come una coda di MAX_SALE elementi

    I pazienti sono rappresentati con tre code (in base al colore associato al paziente)

    Attraverso i semafori si gestisce quale delle code dei pazienti entra in sala
*/