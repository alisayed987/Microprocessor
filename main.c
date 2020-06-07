#include "port.h"
#include "keypad.h"
#include "UART.c"
// el main without flash memory driver w bonus el timer ..............................................................
void SystemInit() {}

struct room Rooms_arr[3]; // 1 tiva C can only support 3 rooms maximum(1port->uart,1port->lock,3kepads)
// int numOfDigits = sizeof(Rooms_arr); // if more the 1 digit

uint8 key1=0;
uint8 key2=0;
uint8 key3=0;
uint8 key4=0;

	
int main()
{

	void UART_initialize();  // UART & PORTB initialization
	PORTD_INIT(); // KEYPAD
	
	/*
	receptionest can choose the wanted operation through first Digit to be entered
	if	Options_UART = 1 ------> store room number
		Options_UART = 2 ------> set room status  // 0=Free, 1=Occupied, 2=room cleaning
		Options_UART = 3 ------> get room status  // 0=Free, 1=Occupied, 2=room cleaning
		Options_UART = 4 ------> set password or reset
	*/
	
	while(1)
	{
		uint8 Options_UART=0;
		Options_UART = Recieve();

		if (Options_UART == 1){	 ///////////// Store the room number
		// room in array
		uint8 Position_in_array;
		Position_in_array= Recieve(); //one digit
			
		uint8 enteredRoomNumber;
		enteredRoomNumber= Recieve(); //one digit
		store_Room_Num(Position_in_array,enteredRoomNumber);
		}

		else if(Options_UART == 2){ //////// set room status //////  0=Free, 1=Occupied, 2=room cleaning
		//take room number
			uint8 selected_room_num;
			selected_room_num= Recieve(); //one degit
			
		//take status
		uint8 status = Recieve();
			for (int i=0;i<3;i++){  //search for the room
				if (Rooms_arr[i].roomNum_struct== selected_room_num){
					
					if (status == 0) {//  0=Free
						Rooms_arr[i].locked=1;
						Rooms_arr[i].status=status;
						Rooms_arr[i].Password[0]=NULL;
						Rooms_arr[i].Password[1]=NULL;
						Rooms_arr[i].Password[2]=NULL;
						Rooms_arr[i].Password[3]=NULL;
					}
					else if (status == 2) {// 2=room cleaning
						Rooms_arr[i].locked=0;
						Rooms_arr[i].status=status;
					}
					else if (status == 1) {// 1=Occupied
						Rooms_arr[i].locked=1;
						Rooms_arr[i].status=status;
					}
					else{ //invalid status ,wait to enter valid status Or exit with 'Y'
						do{
							status= Recieve();
						}while(status != 0 || status != 1 || status != 2 || status != 'Y' );
						
					}

					
					
				}
			}


		}

		
		else if(Options_UART == 3){ ///get room status  // 0=Free, 1=Occupied, 2=room cleaning
			//take room number
			uint8 selected_room_num;
			selected_room_num= Recieve();

			for (int i=0;i<3;i++){  //search for the room
				if (Rooms_arr[i].roomNum_struct== selected_room_num){
					send(Rooms_arr[i].roomNum_struct); // status is writen in data reg and ready to be received
				}
			}
		}

		else if(Options_UART == 4){// set password or reset
				//take room number
			uint8 selected_room_num;
			selected_room_num= Recieve();

			for (int i=0;i<3;i++){ //search for the room
				if (Rooms_arr[i].roomNum_struct== selected_room_num){
					//////////*   first room status must be set to occipuied(1) to set password   *///////////
					if(Rooms_arr[i].status == 1){
						for (int k = 0; k < 4; k++)
						{//save password
						Rooms_arr[i].Password[k]= Recieve();
						}
					}

				}
			}
		}
		
		if (Rooms_arr[0].status == 1)
		{
			// suppose the keypad of Rooms_arr[0] on port A
			// suppose the keypad of Rooms_arr[1] on port C
			// suppose the keypad of Rooms_arr[2] on port E


	   //get from the keypad
   	key1 = KeyPad_getPressedKey();
    key2 = KeyPad_getPressedKey();
    key3 = KeyPad_getPressedKey();
    key4 = KeyPad_getPressedKey();		
		 
		while((GPIO_PORTF_DATA_R & 0X01) == 1)
		{}  // waiting for enter
			
			
		
		if((key1 == Rooms_arr[0].Password[0]) && (key2 ==Rooms_arr[0].Password[1]) && (key3 == Rooms_arr[0].Password[2]) && (key4 ==Rooms_arr[0].Password[3]))  
		{	//right password
			Rooms_arr[0].locked=0;
		}
		else
		{	//wrong password
			continue;
		}
	}
			
	}
	
}
