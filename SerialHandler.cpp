/*
  SerialHandler_h - Library for receiving commands and calling functions.
  Created by Quadrubik, April 21, 2024.
  Released into the public domain.
*/

// Once integrated in begin fcs use miscealnaeous charTo conversion
#include "Arduino.h"
#include "SerialHandler.h"

SerialHandler::SerialHandler(){
    _countCommandN = 0;
    _countCommandB = 0;
    _countCommandI = 0;
    _countCommandF = 0;
    _countCommandC = 0;
    _countCommandSF = 0;
}

// Receive and parse data when whole packet received
// directly send the data to the process method
void SerialHandler::receive(){
    if(Serial.available() and millis() - _lastReceive > _receiveInterval){
        _lastReceive = millis();
        while (Serial.available() > 0){
            char newChar = Serial.read();
            if(newChar != _endMarker){
                _buffer[_counter] = newChar;
                _counter++;
            }
            else{
                _buffer[_counter] = ' ';

                char *args[MAX_ARGS];
                args[0] = strtok(_buffer, " ");
                uint8_t i = 0;
                while(args[i] != NULL){
                    i++;
                    args[i]=strtok(NULL, " ");
                }

                process(args);

                _counter = 0;
            }
        }
    }
}

// Troubleshooting method
void SerialHandler::print(){
    Serial.print("None :");
    Serial.print(_countCommandN);
    Serial.print(" Bool :");
    Serial.print(_countCommandB);
    Serial.print(" Int :");
    Serial.print(_countCommandI);
    Serial.print(" Float :");
    Serial.print(_countCommandF);
    Serial.print(" Char :");
    Serial.print(_countCommandC);
    Serial.print(" SubFloat :");
    Serial.println(_countCommandSF);
}

// Set the acquisition time interval in ms
void SerialHandler::setAcquisitionFrequencie(uint16_t freq){
    _receiveInterval = 1 / float(freq) * 1000;
}

// Search and call appropriate function
void SerialHandler::process(char *args[]){
    processCommand(args, _commandListNone, _countCommandN);
    processCommand(args, _commandListBool, _countCommandB);
    processCommand(args, _commandListInt,  _countCommandI);
    processCommand(args, _commandListFloat, _countCommandI);
    processCommand(args, _commandListChar, _countCommandC);
    processCommand(args, _commandListSubFloat, _countCommandSF);
}

void SerialHandler::processCommand(char *args[], command_none commands[], uint8_t numCommands){
    for (int i = 0; i < numCommands; i++){
        if (strcmp(args[0], commands[i].name) == 0){
            commands[i].function();
            break;
        }
    }
}

void SerialHandler::processCommand(char *args[], command_bool commands[], uint8_t numCommands){
    for (int i = 0; i < numCommands; i++){
        if (strcmp(args[0], commands[i].name) == 0){
            commands[i].function(args[1][0] - 48);
            break;
        }
    }
}

void SerialHandler::processCommand(char *args[], command_int commands[], uint8_t numCommands){
    for (int i = 0; i < numCommands; i++){
        if (strcmp(args[0], commands[i].name) == 0){
            commands[i].function(charToInt(args[1]));
            break;
        }
    }
}

void SerialHandler::processCommand(char *args[], command_float commands[], uint8_t numCommands){
    for (int i = 0; i < numCommands; i++){
        if (strcmp(args[0], commands[i].name) == 0){
            commands[i].function(charToFloat(args[1]));
            break;
        }
    }
}

void SerialHandler::processCommand(char *args[], command_char commands[], uint8_t numCommands){
    for (int i = 0; i < numCommands; i++){
        if (strcmp(args[0], commands[i].name) == 0){
            commands[i].function(args[1]);
        }
    }
}

void SerialHandler::processCommand(char *args[], command_sub_float commands[], uint8_t numCommands){
    for (int i = 0; i < numCommands; i++){
        if (strcmp(args[0], commands[i].name) == 0){
            char *args_aux[2] = {args[1], args[2]};
            processCommand(args_aux, commands[i].commandList, commands[i].numCommands);
        }
    }
}

void SerialHandler::addCommand(const char *name, void (*function)()){
    _commandListNone[_countCommandN] = (command_none){name, function};
    _countCommandN ++;
}
void SerialHandler::addCommand(const char *name, void (*function)(bool)){
    _commandListBool[_countCommandB] = (command_bool){name, function};
    _countCommandB ++;
}
void SerialHandler::addCommand(const char *name, void (*function)(int)){
    _commandListInt[_countCommandI] = (command_int){name, function};
    _countCommandI ++;
}
void SerialHandler::addCommand(const char *name, void (*function)(float)){
    _commandListFloat[_countCommandF] = (command_float){name, function};
    _countCommandF ++;
}
void SerialHandler::addCommand(const char *name, void (*function)(char[])){ 
    _commandListChar[_countCommandN] = (command_char){name, function};
    _countCommandC ++;
}
void SerialHandler::addSubCommand(const char *name, command_float subCommandList[], uint8_t numCommands){
    _commandListSubFloat[_countCommandSF].name = name;
    for (int i = 0; i <= numCommands; i++){
        _commandListSubFloat[_countCommandSF].commandList[i] = subCommandList[i];
    }
    _commandListSubFloat[_countCommandSF].numCommands = numCommands;
    _countCommandSF ++;
}

int SerialHandler::charToInt(char data[])
{
    uint16_t values[9];
    long returnValue = 0;
    uint8_t i = 0;
    int u = 0;
    bool negative = false;
    //First check if value is negative
    if (data[0] == 45) {
      i = 1;
      negative = true;
    }

    //Then get all the digits
    while (isDigit(data[i])) {
      values[u] = data[i] - 48;
      i++;
      u++;
    }

    //Parse the digits into the value 
    for (int v = 0; v < u; v++) {
        returnValue += long(values[v]) * long(pow(10, u - v - 1));
        //Serial.println(long(values[v])*pow(10, u - v - 1));
        //Serial.println(returnValue);
    }

    return negative?-returnValue:returnValue;
}

float SerialHandler::charToFloat(char data[])
{
    int values[9];
    float returnValue = 0.0;
    int i = 0;
    int u = 0;
    int d = 0;
    bool negative = false;
    if (data[0] == 45) {
        i = 1;
        negative = true;
    }
    while (isDigit(data[i]) or data[i]==46) {
        if (data[i]==46){
            d = negative?i-1:i;
        }
        else{
            values[u] = data[i] - 48;
            u++;
        }
        i++;
    }

    d = d==0?u:d;
    for (int v = d-1; v > -u+d-1; v--) {
        returnValue += float(values[-v+d-1]) * float(pow(10, v));
    }
    return negative?-returnValue:returnValue;
}