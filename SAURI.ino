#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h> 

const int humedad = A0;
const int echo = 8;
const int trig = 9;
const int motor = 11;
const int ledhumedad = 12;
const int ledagua = 13;



const byte pinDatos = 10; //Definimos el pin de datos donde leerá nuestro sensor de temperatura
OneWire oneWireObjeto(pinDatos);
DallasTemperature sensorDS18B20(&oneWireObjeto);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Definimos los pines que usará nuestra LCD de información
float temperatura_obtenida;
long distancia_obtenida;
long humedad_obtenida;



void setup() {
  // inicializamos los componentes cuando inicie el arduino
  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(9600);
  sensorDS18B20.begin();
  pinMode(trig, OUTPUT); /*activación del pin 9 como salida: para el pulso ultrasónico*/
  pinMode(echo, INPUT);
  pinMode(humedad, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(ledhumedad, OUTPUT);
  pinMode(ledagua, OUTPUT);

}

void loop() {
  lcd.clear();
  temperatura_obtenida = temperatura(); //llamamos a los metodos para obtener los valores 
  humedad_obtenida = Obtenerhumedad();
  distancia_obtenida = ObtenerDistancia();

  if(humedad_obtenida <=30 ){
    if(temperatura_obtenida>=28){
      
    }
    else 
    if((temperatura_obtenida >= 16) && (temperatura_obtenida<28)){ //Comparamos la temperatura del suelo y si no está caliente:
       digitalWrite(motor, HIGH); //encendemos el motor de la bomba de agua
       lcd.clear(); //limpiamos la pantalla para imprimir nuevamnete
       lcd.print("...Regando..."); //imprimimos el estado regando
       delay(10000); //esperamos 10 segundos de riego
       digitalWrite(motor, LOW); //apagamos la bomba de agua
       lcd.clear();//
       lcd.print("Riego finalizado");//actualizamos el estado del riego a finalizado
    }
  }

  delay(2000); //esperamos 2 segundos hasta que vuelva a calcular todo de nuevo
}

//metodo para calcular la temperatura
float temperatura(){
  sensorDS18B20.requestTemperatures();
  float temperatura = sensorDS18B20.getTempCByIndex(0); 
  lcd.setCursor(0,0); 
  lcd.print("T: "); //imprimimos en la LCD la temperatura calculada
  lcd.print(temperatura);
  Serial.print("Temperatura es: "); //imprimimos en el monitor serial la temperatura (esto es por si deseamos usar Bluetooth)
  Serial.println(temperatura);
  return temperatura; //retornamos el valor de temperatura obtenido
}

//metodo para calcular la humedad
int Obtenerhumedad()
{
  int humedad_obt = analogRead(A0); //obtenemos el valor en voltaje de la humedad (0 hasta 1023
  Serial.println(humedad_obt);
  int humedad = map(humedad_obt,1023,0,0,100); //la humedad obtenida la mapeamos para calcular el porcentaje de humedad en el suelo
  lcd.print(" H:");
  lcd.print(humedad);
  lcd.print("%");
  if(humedad <= 30) //si la humedad sensada es menor a 30%, encenderá un led indicando
  {
    digitalWrite(ledhumedad, HIGH);
  }
  else //si no es así, la apagará
  {
    digitalWrite(ledhumedad, LOW);
  }
  Serial.print("La humedad es = ");
  Serial.print(humedad);
  Serial.println("%");
  return humedad;
}

float ObtenerDistancia(){
  digitalWrite(9,LOW); /* Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);
  digitalWrite(9, HIGH); /* envío del pulso ultrasónico*/
  delayMicroseconds(10);
  long tiempo=pulseIn(8, HIGH); /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío
  del pulso ultrasónico y cuando el sensor recibe el rebote, es decir: desde que el pin 12 empieza a recibir el rebote, HIGH, hasta que
  deja de hacerlo, LOW, la longitud del pulso entrante*/
  long distancia= int(0.017*tiempo); /*fórmula para calcular la distancia obteniendo un valor entero*/   
  int agua = map(distancia, 10, 26, 100, 0);
  Serial.print(distancia);
  lcd.setCursor(0,1);
  lcd.print("Nivel Agua: ");
  lcd.print(agua);
  lcd.print("%");
  if(agua <= 15) //si el nivel del agua es de menos del 15% encenderá el led que indica poca agua
  {
    digitalWrite(ledagua, HIGH);
  }
  if(agua>15) //si es mayor al 15%, apagará el led
  {
    digitalWrite(ledagua, LOW);
  }
  Serial.print("Distancia ");
  Serial.print(agua);
  Serial.println(" %");
  return distancia;
}

