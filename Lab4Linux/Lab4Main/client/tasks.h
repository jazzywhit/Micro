#ifndef TASKS_H
#define TASKS_H

#include "globals.h"

void *userInterface(void *pointer);
void *sensorControl(void *pointer);
void *serverCommunication(void *pointer);
void HTTP_GET(const char *url);

void DisplayData(void);
void ClearTerminal();

#endif

