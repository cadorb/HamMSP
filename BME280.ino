void getBME280(){
float pressure;
  
  //get and print temperatures
  Serial.print("Temp: ");
  Serial.print(bme280.getTemperature());
  Serial.println("C");//The unit for  Celsius because original arduino don't support speical symbols
  Ciao.write(CONNECTOR, TOPIC_TEMP, String(bme280.getTemperature())); // pushes data into a channel
  
  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  Serial.print(pressure = bme280.getPressure());
  Serial.println("Pa"); 
  Ciao.write(CONNECTOR, TOPIC_PRES, String(bme280.getPressure()));

  //get and print altitude data
  Serial.print("Altitude: ");
  Serial.print(bme280.calcAltitude(pressure));
  Serial.println("m");

  //get and print humidity data
  Serial.print("Humidity: ");
  Serial.print(bme280.getHumidity());
  Serial.println("%");
  Ciao.write(CONNECTOR, TOPIC_HUMI, String(bme280.getHumidity()));
}
