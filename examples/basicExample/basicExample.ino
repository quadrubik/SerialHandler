#include "SerialHandler.h"

SerialHandler handler;

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("Serial Start");

  // Feeding the handler its commands
  handler.addCommand("NONE", noneTest);
  handler.addCommand("BOOL", boolTest);
  handler.addCommand("INT", intTest);
  handler.addCommand("FLOAT", floatTest);

  command_float SETcommandList[3];
  uint8_t SETcommandCount = initializeSETSubCommand(SETcommandList);
  handler.addSubCommand("SET", SETcommandList, SETcommandCount);

  handler.setAcquisitionFrequencie(100);
  
  handler.print();
}

void loop() {
  handler.receive();
}

// function definition
void noneTest(){
  Serial.println("Received NONE");
}

void boolTest(bool statut){
  Serial.print("Received ");
  Serial.println(statut?"true":"false");
}

void intTest(int value){
  Serial.print("Received int value : ");
  Serial.println(value);
}

void floatTest(float value){
  Serial.print("Received float value : ");
  Serial.println(value);
}

uint8_t initializeSETSubCommand(command_float commandList[]){
  commandList[0] = (command_float){"FLOAT1", subFloatTest1};
  commandList[1] = (command_float){"FLOAT2", subFloatTest2};
  commandList[2] = (command_float){"FLOAT3", subFloatTest3};
  return 2;
}

void subFloatTest1(float value){
  Serial.print("Received float1 value : ");
  Serial.println(value);
}

void subFloatTest2(float value){
  Serial.print("Received float2 value : ");
  Serial.println(value);
}

void subFloatTest3(float value){
  Serial.print("Received float3 value : ");
  Serial.println(value);
}