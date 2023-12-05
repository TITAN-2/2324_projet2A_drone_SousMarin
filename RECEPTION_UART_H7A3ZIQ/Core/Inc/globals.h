// globals.h

#ifndef GLOBALS_H
#define GLOBALS_H

// Déclaration des variables globales
char *user_data= "The application is running\r\n";; //demo data for transmission
uint8_t recvd_data; //receive buffer
uint32_t count; //count how many bytes are received
uint8_t data_buffer[100]; //data buffer

#endif /* GLOBALS_H */
