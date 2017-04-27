/*
 Simu SPI Carte Porteuse ECA Sinters 
 
 V1.0 = created 14/03/2017 by L. PILLON
*/


// SPI library
#include <SPI.h>
#include <Cmd_SPI.h>

// Constante
const int SlaveSelect = 10;

/////////////////////////////////////////////////////////
// Setup
/////////////////////////////////////////////////////////
void setup() {
  // start Serial link
  Serial.begin(9600);
  // start the SPI library
  pinMode(SlaveSelect, OUTPUT);
  digitalWrite(SlaveSelect, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  Serial.print("Hello\n");
  Aff_Menu();
}

/////////////////////////////////////////////////////////
// Loop
/////////////////////////////////////////////////////////
void loop() {
  unsigned long result = 0;
  int incomingByte = 0;

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read() - 48;
    Serial.print("Commande recue =");
    Serial.print(incomingByte);
    Serial.print("\n");
    switch (incomingByte) {
      case (1) :
        CmdSPI_Statut();
        break;
      case (2) :
        CmdSPI_Defaut();
        break;
      case (3) :
        result = CmdSPI_NbPtReflecto();
        CmdSPI_Reflecto(result);
        break;
      case (4) :
        result = CmdSPI_NbPtDfocable();
        CmdSPI_Dfocable(result);
        break;
      case (5) :
        CmdSPI_Config(0x01, 0xAA55);
        break;
      case (6) :
        CmdSPI_StartReflecto();
        break;
      case (7) :
        CmdSPI_Autotest();
        break;
      case (8) :
        CmdSPI_VhdlId();
        break;
      case (9) :
        CmdSPI_VhdlRev();
        break;
        
      default :
        Serial.print("Commande inconnue !!!\n");
        break;
    }
    Aff_Menu();
  }
  delay(100);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//Affichage menu
//
//void
//
//OUTPUT = void
/////////////////////////////////////////////////////////
void Aff_Menu(void) {
  int i = 0;

  Serial.print("\n");
  Serial.print("1 => Statut\n");
  Serial.print("2 => Defaut\n");
  Serial.print("3 => Lecture reflecto\n");
  Serial.print("4 => Lecture defaut cable\n");
  Serial.print("5 => Config mesure\n");
  Serial.print("6 => Lancement reflecto\n");
  Serial.print("7 => Autotest\n");
  Serial.print("8 => VHDL App ID\n");
  Serial.print("9 => VHDL Rev\n");
  Serial.print("\nCommande ??\n");
  
  // return the result:
  return;
}


/////////////////////////////////////////////////////////
//Read from SPI (max. 3 bytes)
//
//int NbrByte = number of byte(s) to read 1 to 3
//
//long OUTPUT = byte(s) read
/////////////////////////////////////////////////////////
unsigned long ReadSPI(byte NbrByte) {
  byte inbyte = 0;           // incoming byte from the SPI
  unsigned long result = 0;   // result to return
  byte i = 0;
  
  // get "NbrByte" from SPI and put them in "result"
  while ((i < NbrByte) && (i < 3)) {
    result = result << 8;
    inbyte = SPI.transfer(0x00);
    result = result | inbyte;
    i++;
  }

  // return the result:
  return (result);
}


/////////////////////////////////////////////////////////
//Write to SPI (max. 4 bytes)
//
//int NbrByte = number of byte(s) to write 1 to 4
//
//void NO OUTPUT
/////////////////////////////////////////////////////////
void WriteSPI(byte NbrByte, byte OutByte[]) {
  byte i = 0;

  // send "NbrByte" to SPI from "OutByte[]"
  while ((i < NbrByte) && (i < 4)) {
    SPI.transfer(OutByte[i]);
    i++;
  }
  
  // return the result:
  return;
}


/////////////////////////////////////////////////////////
//Command SPI : STATUT
//
//INPUT = void
//
//u long OUTPUT = statut
/////////////////////////////////////////////////////////
unsigned long CmdSPI_Statut(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};

  writespi[0] = CMDSPI_STATUT;
   // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(1,writespi);
  result = ReadSPI(3);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);
  
  Serial.print(result,HEX);
  Serial.print("\n");
  
  result = result & MASK_READ_STATUT;

  switch (result) {
    case (0x0000):
      Serial.print("Statut = Init en cours\n");
      break;
    case (0x0001):
      Serial.print("Statut = Attente Config\n");
      break;
    case (0x0002):
      Serial.print("Statut = Prêt\n");
      break;
    case (0x0003):
      Serial.print("Statut = Reflecto en cours\n");
      break;
    case (0x0004):
      Serial.print("Statut = Reflecto et traitement dispo\n");
      break;
    case (0x0005):
      Serial.print("Statut = Autotest en cours\n");
      break;
    case (0x0006):
      Serial.print("Statut = Defaut\n");
      break;
    default:
      Serial.print("Statut inconnu\n");
      break;
  }
  return (result);
}


/////////////////////////////////////////////////////////
//Command SPI : DEFAUT
//
//INPUT = void
//
//u long OUTPUT = defaut
/////////////////////////////////////////////////////////
unsigned long CmdSPI_Defaut(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};

  writespi[0] = CMDSPI_DEFAUT;
   // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(1,writespi);
  result = ReadSPI(3);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);
  
  Serial.print(result,HEX);
  Serial.print("\n");
  
  result = result & MASK_READ_DEFAUT;

  switch (result) {
    case (0x0000):
      Serial.print("Defaut = Pas de defaut\n");
      break;
    case (0x0001):
      Serial.print("Defaut = Defaut alimentation\n");
      break;
    case (0x0002):
      Serial.print("Defaut = Defaut DAC/ADC\n");
      break;
    case (0x0003):
      Serial.print("Defaut = Defaut mesure OMTDR\n");
      break;
    default:
      Serial.print("Defaut inconnu\n");
      break;
  }
  return (result);
}


/////////////////////////////////////////////////////////
//Command SPI : NBRE POINT REFLECTO
//
//INPUT = void
//
//u long OUTPUT = nbre de pts de reflecto a lire
/////////////////////////////////////////////////////////
unsigned long CmdSPI_NbPtReflecto(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};

  writespi[0] = CMDSPI_NBPTREFLEC;
   // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(1,writespi);
  result = ReadSPI(3);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);
  
  result = result & MASK_READ_NBPTREFLEC;
  Serial.print("Nbr de points de reflecto a lire = ");
  Serial.print(result);
  Serial.print("\n");
  
  return (result);
}


/////////////////////////////////////////////////////////
//Command SPI : REFLECTO
//
//INPUT = nbre de points a lire
//
//OUTPUT = void
/////////////////////////////////////////////////////////
void CmdSPI_Reflecto(unsigned long nbpt_reflec) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};
  unsigned long i = 0;

  Serial.print("Points reflecto = \n");
  writespi[0] = CMDSPI_READREFLEC;

  // Lit l'ensemble des points du reflecto
  for (i = 0; i < nbpt_reflec; i++) {
    // Enable SS
    digitalWrite(SlaveSelect, LOW);
    // Commande SPI
    WriteSPI(1,writespi);
    result = ReadSPI(3);
    //Disable SS
    digitalWrite(SlaveSelect, HIGH);
    
    result = result & MASK_READ_REFLEC;
    Serial.print(result);
    Serial.print("\n");
  }
  
  Serial.print("Fin Reflecto\n");
  
  return;
}


/////////////////////////////////////////////////////////
//Command SPI : NBRE POINT DEFAUT CABLE
//
//INPUT = void
//
//u long OUTPUT = nbre de pts de defauts a lire
/////////////////////////////////////////////////////////
unsigned long CmdSPI_NbPtDfocable(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};

  writespi[0] = CMDSPI_NBDFOCABLE;
   // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(1,writespi);
  result = ReadSPI(3);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);
  
  result = result & MASK_READ_NBDFOCABLE;
  Serial.print("Nbr de points de defaut a lire = ");
  Serial.print(result);
  Serial.print("\n");
  
  return (result);
}


/////////////////////////////////////////////////////////
//Command SPI : DEFAUT CABLE
//
//INPUT = nbre de points a lire
//
//OUTPUT = void
/////////////////////////////////////////////////////////
void CmdSPI_Dfocable(unsigned long nb_dfo) {
  unsigned long result = 0;   // result to return
  unsigned long result1 = 0;   // result to return
  unsigned long result2 = 0;   // result to return
  byte writespi[4] = {0,0,0,0};
  unsigned long i = 0;
  
  Serial.print("Points reflecto = \n");
  writespi[0] = CMDSPI_READDFOCABLE;

  // Lit l'ensemble des points du reflecto
  for (i = 0; i < nb_dfo; i++) {
    // Enable SS
    digitalWrite(SlaveSelect, LOW);
    // Commande SPI
    WriteSPI(1,writespi);
    result = ReadSPI(3);
    //Disable SS
    digitalWrite(SlaveSelect, HIGH);
    
    result2 = result & MASK_READ_NUMDFOCABLE;
    result2 = result2 >> 16;
    Serial.print("Numéro défaut = \n");
    Serial.print(result2);
    Serial.print("\n");
    result1 = result & MASK_READ_DFOCABLE;
    Serial.print("Emplacement défaut = \n");
    Serial.print(result1);
    Serial.print("\n");
  }
  
  Serial.print("Fin defaut\n");
  
  return;
}


/////////////////////////////////////////////////////////
//Command SPI : CONFIG MESURE
//
//INPUT = nbre de points a lire
//
//OUTPUT = void
/////////////////////////////////////////////////////////
void CmdSPI_Config(byte cabletype, unsigned int cablelength) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};
  unsigned long i = 0;

  Serial.print("Ecriture config \n");
  writespi[0] = CMDSPI_CONFIGMEAS;
  writespi[1] = 0x07 & cabletype;
  writespi[2] = (byte)(cablelength >> 8);
  writespi[3] = (byte)cablelength;
  
  // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(4,writespi);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);

  Serial.print("Done !!!\n");

  return;
}


/////////////////////////////////////////////////////////
//Command SPI : LANCEMENT REFLECTO
//
//INPUT = void
//
//OUTPUT = void
/////////////////////////////////////////////////////////
void CmdSPI_StartReflecto(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,1};
  unsigned long i = 0;

  Serial.print("Lancement Reflecto \n");
  writespi[0] = CMDSPI_STARTREFLEC;
    
  // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(4,writespi);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);

  Serial.print("Done !!!\n");

  return;
}


/////////////////////////////////////////////////////////
//Command SPI : AUTOTEST
//
//INPUT = void
//
//OUTPUT = void
/////////////////////////////////////////////////////////
void CmdSPI_Autotest(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};
  unsigned long i = 0;

  Serial.print("Autotest \n");
  writespi[0] = CMDSPI_BIT;
    
  // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(4,writespi);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);

  Serial.print("Done !!!\n");

  return;
}


/////////////////////////////////////////////////////////
//Command SPI : VHDL APP ID
//
//INPUT = void
//
//u long OUTPUT = statut
/////////////////////////////////////////////////////////
unsigned long CmdSPI_VhdlId(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};

  writespi[0] = CMDSPI_VHDLID;
   // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(1,writespi);
  result = ReadSPI(3);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);

  result = result & MASK_READ_VHDLID;
  
  Serial.print("VHDL APP ID = \n");
  Serial.print(result,HEX);
  Serial.print("\n");

  return (result);
}


/////////////////////////////////////////////////////////
//Command SPI : VHDL REV
//
//INPUT = void
//
//u long OUTPUT = statut
/////////////////////////////////////////////////////////
unsigned long CmdSPI_VhdlRev(void) {
  unsigned long result = 0;   // result to return
  byte writespi[4] = {0,0,0,0};

  writespi[0] = CMDSPI_VHDLRV;
   // Enable SS
  digitalWrite(SlaveSelect, LOW);
  // Commande SPI
  WriteSPI(1,writespi);
  result = ReadSPI(3);
  //Disable SS
  digitalWrite(SlaveSelect, HIGH);

  result = result & MASK_READ_VHDLRV;
  
  Serial.print("VHDL APP REV = \n");
  Serial.print(result,HEX);
  Serial.print("\n");

  return (result);
}


