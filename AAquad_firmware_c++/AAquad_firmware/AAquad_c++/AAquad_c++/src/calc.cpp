#include "calc.h"




int calc::filtered_motors[4] = {0};


int calc::moving_average(int* motors){
	
	uint16_t temp;
	int motor[4];
	
	static int history0[5] = {0};
/*		
	static int history1[5] = {0};
	static int history2[5] = {0};
	static int history3[5] = {0};
*/	
	
	for (int i = 0; i < 4; i++){
		
		history0[i] = history0[i + 1];
	}
	
	history0[4] = motors[0];
	
	temp = (history0[0] + history0[1] + history0[2] + history0[3] + history0[4]) / 5;
	motor[0] = temp;
	
	
/*	
	
	for (int i = 0; i < 4; i++){
		
		history1[i] = history1[i + 1];
	}
	
	history1[4] = motors[1];
	
	temp = (history1[0] + history1[1] + history1[2] + history1[3] + history1[4]) / 5;
	motor[1] = temp;
	
	
	
	
	
	for (int i = 0; i < 4; i++){
		
		history2[i] = history2[i + 1];
	}
	
	history2[4] = motors[2];
	
	temp = (history2[0] + history2[1] + history2[2] + history2[3] + history2[4]) / 5;
	motor[1] = temp;
*/

return motors[0];

}