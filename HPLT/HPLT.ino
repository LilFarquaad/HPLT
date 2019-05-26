/* HPLT: Evaluacion de variables ambientales 
 * 
*/

#include <SFE_BMP180.h>
#include <Wire.h>

void Iniciar_bmp(void);
void Sensorbmp180(void);
void Higrometro(void);
void lumicidad(void);

SFE_BMP180 bmp180;

const int higrometro = 0;       // Terminal A0
const int LDRPin = 1;           // Pin del LDR
const long A = 1000;            // Resistencia en oscuridad en KΩ
const int B = 15;               // Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;              // Resistencia calibracion en KΩ

int V;
int ilum;
double PresionNivelMar=1013.25; // Presion sobre el nivel del mar en mbar

void setup() 
{
  Serial.begin(9600);
  Iniciar_bmp();  
}

void loop() 
{
  Sensorbmp180();
  Higrometro();
  lumicidad();
}


/*    ***** FUNCIONES *****     */
void Iniciar_bmp(void)
{
  if (bmp180.begin())
    Serial.println("BMP180 iniciado correctamenten");
  else
  {
    Serial.println("Error al iniciar el BMP180");
    while(1); // bucle infinito
  }
}

void Sensorbmp180(void)
{
  char status;
  double T,P;
  int Altitud;

  status = bmp180.startTemperature();   // Inicio de lectura de temperatura
  if (status != 0)
  {   
    delay(status);                      // Pausa para que finalice la lectura
    status = bmp180.getTemperature(T);  // Obtener la temperatura
    if (status != 0)
    {
      status = bmp180.startPressure(3); // Inicio lectura de presión
      if (status != 0)
      {        
        delay(status);                  // Pausa para que finalice la lectura        
        status = bmp180.getPressure(P,T); // Obtenemos la presión
        if (status != 0)
        {                  
          Serial.print("Temperatura: ");
          Serial.print(T,2);
          Serial.print(" *C , ");
          Serial.print("Presion: ");
          Serial.print(P,2);
          Serial.println(" mb"); 

          //-------Calculamos la altitud--------
          Altitud = bmp180.altitude(P,PresionNivelMar);
          Serial.print("Altitud: ");
          Serial.print(Altitud);
          Serial.println(" m s.n.m.");
        }      
      }      
    }   
  } 
  delay(1000);
}

void Higrometro(void)
{
  //Se hace la lectura analoga del pin A0 (sensor) y se pasa por la funcion
  //map() para ajustar los valores leidos a los porcentajes que queremos utilizar   
  int valorHumedad = map(analogRead(higrometro), 0, 1023, 100, 0);
  
  Serial.print("Humedad del suelo: ");
  Serial.print(valorHumedad);
  Serial.println("%");
 
  delay(1000);
}

void lumicidad(void)
{
  V = analogRead(LDRPin);         
  ilum = ((long)V*A*10)/((long)B*Rc*(1024-V));
  Serial.print("Iluminacion: ");
  Serial.println(ilum);   
  
  delay(1000);
}
