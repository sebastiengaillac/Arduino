
    #include <Wire.h>   //Librairie Wire pour la communication I2C
    #define HMC5803L_Address 0x1E  //Adresse I2C du module  

    #define X 3  //Adresses de registres pour les données X Y et Z
    #define Y 7
    #define Z 5
    double Xmagnetic;
    double Ymagnetic;
    double Zmagnetic;
    double Module_magnetic;
    double angle;

    void setup()
    {
      Serial.begin(9600);
      Wire.begin();         //Initialisation de la livrairie Wire
        Init_HMC5803L();    //Initialiser le module boussole
    }

    // Boucle de mesures de champ magnétique
    void loop()
    {
      Xmagnetic = HMC5803L_Read(X);  //lecture sur 3 axes et sortie sur le port sériel
      Ymagnetic = HMC5803L_Read(Y);
      Zmagnetic = HMC5803L_Read(Z);
      Serial.print(" ");
      Serial.print (Xmagnetic);
      Serial.print(" ");
      Serial.print (Ymagnetic);
      Serial.print(" ");
      Serial.print (Zmagnetic);

       //Module du champ
      Module_magnetic = Xmagnetic * Xmagnetic + Ymagnetic * Ymagnetic + Zmagnetic * Zmagnetic;
      Module_magnetic = sqrt(Module_magnetic);
      Serial.print(" M= ");
      Serial.print(Module_magnetic);
     
      //Calculer l'angle de la boussole à partir de X et Y (à plat)
      angle= atan2(Ymagnetic,Xmagnetic) * (180 / 3.14159265); // angle en degres
      if (angle<0) {angle=angle+360;}
      Serial.print("  "); 
      Serial.print("Angle  ");  //en degres
      Serial.println(angle);

     delay(100);
    }


     
    // === Fonction qui initialise le module boussole (à lancer une seule fois)
    void Init_HMC5803L(void)
    {
      /* Set the module to 8x averaging and 15Hz measurement rate */
      Wire.beginTransmission(HMC5803L_Address);
      Wire.write(0x00);
      Wire.write(0x70);
             
      Wire.write(0x01);
      Wire.write(0xA0);   //Règle un gain de 5
      Wire.endTransmission();
    }

    // === Fonction qui lit le module boussole (registre d'un des 3 axes, retourne 16 bits)
    int HMC5803L_Read(byte Axis)
    {
      int Result;
       /* Initiate a single measurement */
      Wire.beginTransmission(HMC5803L_Address);
      Wire.write(0x02);
      Wire.write(0x01);
      Wire.endTransmission();
      delay(6);
     
      /* Move modules the resiger pointer to one of the axis data registers */
      Wire.beginTransmission(HMC5803L_Address);
      Wire.write(Axis);
      Wire.endTransmission();
       
      /* Read the data from registers (there are two 8 bit registers for each axis) */ 
      Wire.requestFrom(HMC5803L_Address, 2);
      Result = Wire.read() << 8;
      Result |= Wire.read();

      return Result;
    }

