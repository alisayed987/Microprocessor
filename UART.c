#include "TM4C123GH6PM.h"
#include "std_types.h"

struct room
{
    uint16 roomNum_struct;
    uint8 Password[4];
    uint8 status;
    bool locked; // 1=locked , 0=unlocked
};


//only if large number of rooms used to collect room numbers degit from the array to single integer

 /*uint16 fromArrayToInt(uint8 arr[] ,uint8 digits){

	int result = 0;
	for (int temp = 0; temp < digits; temp++) {
		result *= 10;
		result += arr[temp];
	}
	return result;
 } */

void UART_initialize(void){
    SYSCTL_RCGCUART_R |= 0x0002;
    SYSCTL_RCGCGPIO_R |= 0x0002;
    UART1_CTL_R &= ~0x0001;
    UART1_IBRD_R = 27;
    UART1_FBRD_R = 8;
    UART1_LCRH_R = 0x0070;
    UART1_CTL_R =   0x0301;
    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R =(GPIO_PORTB_PCTL_R & 0xFFFFFF00+0x00000011);
    GPIO_PORTB_DEN_R |= 0X03;
    GPIO_PORTB_AMSEL_R &= ~0X03;
    GPIO_PORTB_DIR_R &= 0x00; 
}


int store_Room_Num(uint8 positionInArray,uint8 Num){

    Rooms_arr[positionInArray].roomNum_struct= Num;
}


uint16 Recieve(){
    uint16 recieved_value;
    while(UART1_FR_R & 0x10 == 0x10){
        recieved_value = UART1_DR_R;
    }
    return recieved_value;
}

uint16 send(uint8 selected_room){
    uint16 send;
    while(UART1_FR_R & 0x20 == 0x20){
        UART1_DR_R = Rooms_arr[selected_room].status;
    }
}