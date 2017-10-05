/*


Medida de temperatura y humedad con 2 sensores DHT22

Conexiones
* Pin +    a Vcc
* Pin -    a GND
* Pin out  a S

Un sensor DHT22 al pin 4 y otro al 6

Interruptor crepuscular

Medimos la luz y si el valor esta por debajo de lo marcado por un potenciometro
activamos la iluminacion

Conexiones
Potenciometro  A0
LDR           A2

Luz           13

 */


/* ==== Includes - Librerias==== */
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Servo.h>

/* ====  END Includes ==== */
#define DHT1_PIN            6
#define DHT2_PIN            4
#define SERVO_PIN           8

#define ALARMA_PIN          2

#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 8

/* ==== Defines - Constantes ==== */

//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)


#define PIN_LDR A2
#define PIN_POTENCIOMETRO A0

#define PIN_ILUMNINACION 13
#define UMBRAL 20

#define ESPERA 300

/* ==== END Defines ==== */


/* ==== Variables Globales ==== */

DHT_Unified dht1(DHT1_PIN, DHTTYPE);
DHT_Unified dht2(DHT2_PIN, DHTTYPE);

uint32_t delayMS;

float temperatura_interior;
float temperatura_exterior;

Servo servo;
/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */
void setup_Servo(){
  servo.attach(SERVO_PIN);
}

void setup_iluminacion(){
  pinMode(PIN_ILUMNINACION,OUTPUT);
}

void setup_DHT(){
  dht1.begin();
  dht2.begin();

  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;

  Serial.println("Sensor 1");
  dht1.temperature().getSensor(&sensor);
  Serial.println("Temperatura");
  showSensorData(sensor);

  dht1.humidity().getSensor(&sensor);
  Serial.println("Humedad");
  showSensorData(sensor);

  Serial.println("------------------------------------");

  Serial.println("Sensor 2");
  dht2.temperature().getSensor(&sensor);
  Serial.println("Temperatura");
  showSensorData(sensor);

  dht2.humidity().getSensor(&sensor);
  Serial.println("Humedad");
  showSensorData(sensor);

  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void setup_Serial(){
  Serial.begin(9600);
}

void setup() {
  setup_Serial();
  setup_Servo();
  setup_DHT();
  setup_iluminacion();

}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {

  // Control de temperatura
  getData();
  Serial.print(temperatura_interior);
  Serial.print(" ");
  Serial.println(temperatura_exterior);
  delay(delayMS);

  if (temperatura_interior<temperatura_exterior){
    cierraVentanas();
  }
  else{
    abreVentanas();
  }


  // Control de iluminacion
  int iPotenciometro=analogRead(PIN_POTENCIOMETRO);
  int iLuz=analogRead(PIN_LDR);
  Serial.print(iLuz);
  Serial.print(" - ");
  Serial.println(iPotenciometro);

  if(iLuz+UMBRAL>iPotenciometro){
      enciendeLuz();
  } else {
    apagaLuz();
  }
  

}
/* ==== End Loop ==== */

/* ==== Funciones ==== */
void showSensorData(sensor_t sensor){
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");
}

void getData() {

  // Get temperature event and print its value.
  sensors_event_t event;

  dht1.temperature().getEvent(&event);
  temperatura_exterior=event.temperature;

  dht2.temperature().getEvent(&event);
  temperatura_interior=event.temperature;

}

void cierraVentanas(){
  servo.write(20);
  Serial.println("Cerrando Ventanas");
}

void abreVentanas(){
  servo.write(160);
  Serial.println("Abriendo Ventanas");
}

void enciendeLuz(){
  digitalWrite(PIN_ILUMNINACION,HIGH);
  Serial.println("iluminacion encendida");
}

void apagaLuz(){
  digitalWrite(PIN_ILUMNINACION,LOW);
  Serial.println("iluminacion apagada");
}
/* ==== END Functions ==== */
