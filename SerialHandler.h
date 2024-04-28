/*
  SerialHandler_h - Library for receiving commands and calling functions.
  Created by Quadrubik, April 21, 2024.
  Released into the public domain.

  TODO:
    Check wether second argument is given, if not raise an error
*/
#ifndef SerialHandler_h
#define SerialHandler_h

#include "Arduino.h"    

#define DATA_BUFFER_SIZE 255
#define MAX_ARGS 4

#define NUM_COMMAND_NONE 10
#define NUM_COMMAND_BOOL 10
#define NUM_COMMAND_INT 10
#define NUM_COMMAND_FLOAT 10
#define NUM_COMMAND_CHAR 10
#define NUM_COMMAND_SUB 2

#define MAX_SUBCOMMAND 30

// Command struct definition for every argument type
typedef struct {
  const char *name;
  void (*function)();
}command_none;
typedef struct {
  const char *name;
  void (*function)(bool);
}command_bool;
typedef struct {
  const char *name;
  void (*function)(int);
}command_int;
typedef struct {
  const char *name;
  void (*function)(float);
}command_float;
typedef struct {
  const char *name;
  void (*function)(char[]);
}command_char;
typedef struct {
  const char *name;
  command_float commandList[MAX_SUBCOMMAND];
  uint8_t numCommands;
}command_sub_float;

class SerialHandler{
    public:
        SerialHandler();

        void receive();

        void process(char *args[]);

        void print();
        void setAcquisitionFrequencie(uint16_t freq);
        
        void processCommand(char *args[], command_none commands[], uint8_t numCommands);
        void processCommand(char *args[], command_bool commands[], uint8_t numCommands);
        void processCommand(char *args[], command_int commands[], uint8_t numCommands);
        void processCommand(char *args[], command_float commands[], uint8_t numCommands);
        void processCommand(char *args[], command_char commands[], uint8_t numCommands);
        void processCommand(char *args[], command_sub_float commands[], uint8_t numCommands);

        void addCommand(const char *name, void (*function)());
        void addCommand(const char *name, void (*function)(bool));
        void addCommand(const char *name, void (*function)(int));
        void addCommand(const char *name, void (*function)(float));
        void addCommand(const char *name, void (*function)(char[]));
        void addSubCommand(const char *name, command_float subCommandList[], uint8_t numCommands);

        int charToInt(char data[]);
        float charToFloat(char data[]);
        

    private:
        // Stores incomming data
        char _buffer[DATA_BUFFER_SIZE];
        uint16_t _counter = 0;
        // Set to desired end command marker
        char _endMarker = '\n';

        // Set the serial acquisition interval (ms)
        uint16_t _receiveInterval = 0;
        long _lastReceive = 0;

        // Definition of the structs holding the commands
        uint8_t _countCommandN, _countCommandB, _countCommandI, _countCommandF, _countCommandC; 
        uint8_t _countCommandSF;
        command_none _commandListNone[NUM_COMMAND_NONE];
        command_bool _commandListBool[NUM_COMMAND_BOOL];
        command_int  _commandListInt[NUM_COMMAND_INT];
        command_float  _commandListFloat[NUM_COMMAND_FLOAT];
        command_char _commandListChar[NUM_COMMAND_CHAR];
        command_sub_float _commandListSubFloat[NUM_COMMAND_SUB];
};

#endif