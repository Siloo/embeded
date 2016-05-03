/* datalink rework,  date: 2016-4-26*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "usbkeyboard.h"
/************************************Header Files Included******************************************/
#include "vga_led.h"//import struct, 2016-4-26
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

pthread_t timer_thread;
pthread_t click_thread;

int timer;

/*************************************End Headers*************************************************/
#define MATRIX_ROW 11
#define MATRIX_COL 10
#define time 0

/*************************CODE FOR CALLING THE INTERFACE BETWEEN HARDWARE AND SOFTWARE*******************************/
/*segment: the data you want to write into the hardware
	index: the index of register you want to write into
*/

/*Pass into the index and get the data as the return value
unsigned int read_segments(int index){
	vga_led_arg_t vla;
	vla.digit = index;
	if (ioctl(vga_led_fd, VGA_LED_READ_DIGIT, &vla)) {
      perror("ioctl(VGA_LED_READ_DIGIT) failed");
      return -1;
    }
	return vla.segment;
}
*/

int vga_led_fd;

/****************************************************END CODE*************************************************************/

//initialize blocks
int b1[1][5] = {1, 1, 1, 1, 1};
int b2[5][1] = {1, 1, 1, 1, 1};
int b3[1][4] = {1, 1, 1, 1};
int b4[4][1] = {1, 1, 1, 1};
int b5[2][2] = {{1, 1}, {1, 1}};
int b6[3][3] = {{1, 1, 1}, {1, 0, 0}, {1, 0, 0}};
int b7[3][3] = {{1, 1, 1}, {0, 0, 1}, {0, 0, 1}};
int b8[2][1] = {1, 1};
int b9[3][1] = {1, 1, 1};
int b10[1][1] = {1};
int b11[2][2] = {{1, 1}, {1, 0}};
int b12[2][3] = {{1, 1, 0}, {0, 1, 1}};
int b13[1][3] = {1, 1, 1};
int b14[1][2] = {1, 1};

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

typedef struct block{
	int height;
	int width;
	//int blockArray[]; 
} blockStuct;


blockStuct block1;
blockStuct block2;
blockStuct block3;
blockStuct block4;
blockStuct block5;
blockStuct block6;
blockStuct block7;
blockStuct block8;
blockStuct block9;
blockStuct block10;
blockStuct block11;
blockStuct block12;
blockStuct block13;
blockStuct block14;

int vga_led_fd;
//AVALON-MM INFO:
//		- 16bit data link

//new struct //included in vga_led.h
/*
typedef struct {
	u16 data;
} vga_led_arg_t;
*/

//16bit bus writedata
///index [15:8]
//segment[7:0]

void write_segments(unsigned int index, unsigned int segment)
{
	vga_led_arg_t vla;
	//mask 			   5432109876543210
	const int mask = 0b0000000011111111;
	vla.data = ((mask&index)<<8)|((mask&segment));
	//printf("data=%d\n", vla.data);
    if (ioctl(vga_led_fd, _IOW('q', 1, vga_led_arg_t *), &vla)){
        perror("vga_led write data failed ( project.c -> write_segments)"); 
        return;
    }
	//printf("WS exit");
}

void passToHardware(int** matrix){
	perror("P2H enter");
	int row, col, index; 
	int segment = 0;
	for (row = 0; row < 11; row++){
		for(col= 0; col < 10; col++){
			index = (row*10) + col + 1;
			segment = matrix[row][col];
			write_segments(index, segment);
				
		}
	}
}


int generate()
{
	int i;
	i = (random() % (14-1+1)) + 1;
	return i;
}

void clearLine(int** matrix)
{
	int row;
	int col;
	int row_to_clean[10] = {11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
	int col_to_clean[10] = {11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
	int i, j;
	int t, k;
	int p, q, r;
	int score;
	score = 0;
	for(row = 0; row < 10; row ++){ //get rows that can be cleaned
		//for(i = 0; i<10; i++){
			if (matrix[row][0] == 1 && matrix[row][1] == 1 && matrix[row][2] == 1 && matrix[row][3] == 1 && matrix[row][4] == 1 && matrix[row][5] == 1 && matrix[row][6] == 1 && matrix[row][7] == 1 && matrix[row][8] == 1 && matrix[row][9] == 1){
				printf("clean line %d \n", row);
				row_to_clean[row] = row;
			}
			//else{
			//	row_to_clean[i] = 11;
			//}
		//}
	}
	for(col = 0; col < 10; col++){ //get columns that can be cleaned
		//for(j = 0; j <10; j++){
			if (matrix[0][col] == 1 && matrix[1][col] == 1 && matrix[2][col] == 1 && matrix[3][col] == 1 && matrix[4][col] == 1 && matrix[5][col] == 1 && matrix[6][col] == 1 && matrix[7][col] == 1 && matrix[8][col] == 1 && matrix[9][col] == 1){
				printf("clean line %d \n", col);
				col_to_clean[col] = col;
			}
			//else{
			//	col_to_clean[j] = 11;
			//}
		//}
	}

	for(p = 0; p <10; p ++ ){
		int temp = row_to_clean[p];
		printf("row=%d\n", temp);
		if (temp != 11){
			score = score + 1;
			for(q = 0; q < 10; q++){
				matr   ix[temp][q] = 0;
			}
			printf("cleared row %d \n", temp);
		}
	}

	for(k = 0; k <10; k ++ ){
		int temp2 = col_to_clean[k];
		printf("col=%d\n", temp2);
		if (temp2 != 11){
			score = score + 1;
			for(r = 0; r < 10; r++){
				matrix[r][temp2] = 0;
			}
			printf("cleared line %d \n", temp2);	
		}
	}
	

	matrix[10][5] = matrix[10][5] + score;
	int send_score = matrix[10][5];
	int hundred, ten, one;
	/*********************************************TO DO*********************************************/
	//hundred = send_score/100
	//write_segments(106, send_score);//send score;


}

int checkIfGameEnd(int* b, int** matrix){
	int i;
	int end = 1;
	int m, n, temp_row, temp_col;
	int p, q;
	int temp = 0;
	int temp1[1][5];
	int temp2[5][1];
	int temp3[1][4];
	int temp4[4][1];
	int temp5[2][2];
	int temp6[3][3];
	int temp7[3][3];
	int temp8[2][1];
	int temp9[3][1];
	int temp10[1][1];
	int temp11[2][2];
	int temp12[2][3];
	int temp13[1][3];
	int temp14[1][2];
	for (i = 0; i<3; i++){
		temp = b[i];
		switch(temp){
			case 0:
				end = 0;
				break;
			case 1:
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						/*start to  extract blocks from matrix if there is an empty bloack and start from this block there are enough blocks to match the shape, 
						regardless if the blocks are empty or not*/
						if (temp_block == 0 && 10 - m > 0 && 10 - n < 6){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 5; temp_col ++){
									/*extract the shape of one of the left shapes and compare*/
									temp1[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 5; q ++){
											if( b1[p][q] == temp1[p][q]){
												break;
											}else{
												/* the game is not end if there is one area that can fit in any of the shapes*/
												end = 0;
												break;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 2:
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 4 && 10 - n > 0){
							for(temp_row = 0; temp_row < 5; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp2[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 5; p ++){
										for (q = 0; q < 1; q ++){
											if( b2[p][q] == temp2[p][q]){
												break;
											}else{

												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 3:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 3){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 4; temp_col ++){
									temp3[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 4; q ++){
											if( b3[p][q] == temp3[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 4:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 3 && 10 - n > 0){
							for(temp_row = 0; temp_row < 4; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp4[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 4; p ++){
										for (q = 0; q < 1; q ++){
											if( b4[p][q] == temp4[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 5:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 1 && 10 - n > 1){
							for(temp_row = 0; temp_row < 2; temp_row ++){
								for (temp_col = 0; temp_col < 2; temp_col ++){
									temp5[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 2; p ++){
										for (q = 0; q < 2; q ++){
											if( b5[p][q] == temp5[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 6:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 2){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp6[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 3; q ++){
											if( b6[p][q] == temp6[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 7:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 2){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp7[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 3; q ++){
											if( b7[p][q] == temp7[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 8:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 1 && 10 - n > 0){
							for(temp_row = 0; temp_row < 2; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp8[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 2; p ++){
										for (q = 0; q < 1; q ++){
											if( b8[p][q] == temp8[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 9:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 0){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp9[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 1; q ++){
											if( b9[p][q] == temp9[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 10:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 0){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp10[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 1; q ++){
											if( b10[p][q] == temp10[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 11:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 2){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp11[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 3; q ++){
											if( b6[p][q] == temp11[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 12:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 1 && 10 - n > 2){
							for(temp_row = 0; temp_row < 2; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp12[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 2; p ++){
										for (q = 0; q < 3; q ++){
											if( b12[p][q] == temp12[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 13:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 2){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp13[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 3; q ++){
											if( b13[p][q] == temp13[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			case 14:
				
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 1){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 2; temp_col ++){
									temp14[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 2; q ++){
											if( b14[p][q] == temp14[p][q]){
												break;
											}else{
												end = 0;
											}
											break;
										}
										break;
									}
									break;
								}
								break;
							}
							break;
						}else{
							break;
						}
						break;
					}
					break;
				}
				break;
			default:
				break;
		}
		break;
	}

	return end;
}

//CHECK INPUT HERE
void putBlock(int j, int* b, int** matrix, int** cache){
	int right_bound, left_bound, up_bound, low_bound, xsize, ysize;
	int temp = b[j];
	int q = j;
	int inChoose;
	struct usb_keyboard_packet packet;
	int transferred;
  	char keystate[12];
	fprintf(stderr, "putting\n");
	fprintf(stderr,"%d\n", q);
	fprintf(stderr, "%d\n", temp);

	switch(temp){
		case 1:
			right_bound = 5;
			low_bound = 9;
			xsize = 5;
			ysize = 1;
			break;
		case 2:
			right_bound = 9;
			low_bound = 5;
			xsize = 1;
			ysize = 5;
			break;
		case 3:
			right_bound = 6;
			low_bound = 9;
			xsize = 4;
			ysize = 1;
			break;
		case 4:
			right_bound = 9;
			low_bound = 6;
			xsize = 1;
			ysize = 4;
			break;
		case 5:
			right_bound = 8;
			low_bound = 8;
			xsize = 2;
			ysize = 2;
			break;
		case 6:
			right_bound = 7;
			low_bound = 7;
			xsize = 3;
			ysize = 3;
			break;
		case 7:
			right_bound = 7;
			low_bound = 7;
			xsize = 3;
			ysize = 3;
			break;
		case 8:
			right_bound = 9;
			low_bound = 8;
			xsize = 1;
			ysize = 2;
			break;
		case 9:
			right_bound = 9;
			low_bound = 7;
			xsize = 1;
			ysize = 3;
			break;
		case 10:
			right_bound = 9;
			low_bound = 9;
			xsize = 1;
			ysize = 1;
			break;
		case 11:
			right_bound = 8;
			low_bound = 8;
			xsize = 2;
			ysize = 2;
			break;
		case 12:
			right_bound = 7;
			low_bound = 8;
			xsize = 3;
			ysize = 2;
			break;
		case 13:
			right_bound = 7;
			low_bound = 9;
			xsize = 3;
			ysize = 1;
			break;
		case 14:
			right_bound = 8;
			low_bound = 9;
			xsize = 2;
			ysize = 1;
			break;
		default:
			break;
	}
	int loop_flag = 1;
	
	fprintf(stderr, "moving\n");
	int x = 0;
	int y = 0;
	while(loop_flag == 1){
		libusb_interrupt_transfer(keyboard, endpoint_address, (unsigned char *) &packet, sizeof(packet), &transferred, 0);
	    if (transferred == sizeof(packet)) {
	    	pthread_create(&click_thread, NULL, click_thread_f, NULL);
	      	//sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0], packet.keycode[1]);
	    if (packet.keycode[0] == 0x29){
	      	matrix[10][0] = 1;
	      	break;
	    }else if(packet.keycode[0] == 0x4F){
	    	//current position
			fprintf(stderr, "right\n");
			fprintf(stderr, "%d\n", x);
			fprintf(stderr, "%d\n", y);

	    	if (x < right_bound){
	    		x = x + 1;
	    		y = y;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	    }else if(packet.keycode[0] == 0x50){
	    	//current position
			fprintf(stderr, "left\n");
			fprintf(stderr, "%d\n", x);
			fprintf(stderr, "%d\n", y);

	    	if (x > 0){
	    		x = x - 1;
	    		y = y;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	    }else if(packet.keycode[0] == 0x51){
	    	//current position
			fprintf(stderr, "down\n");
			fprintf(stderr, "%d\n", x);
			fprintf(stderr, "%d\n", y);

	    	if (y < low_bound){
	    		x = x;
	    		y = y + 1;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	    }else if(packet.keycode[0] == 0x52){
	    	//current position
			fprintf(stderr, "up\n");
			fprintf(stderr, "%d\n", x);
			fprintf(stderr, "%d\n", y);

	    	if (y == 0){
	    		x = x;
	    		y = y;
	    	}else{
	    		x = x;
	    		y = y - 1;
			moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}

	    }else if(packet.keycode[0] == 0x28){ // Enter
	    	int check = 0;
	    	check = moveBlock(temp, xsize, ysize, x, y, matrix, cache, 1); // tell moveBlock to apply change to cache matrix

	    	if(check == 1){
	    		inChoose = 1;
	    		//moveBlock(temp, xsize, ysize, x, y, matrix, cache, 1); // if can put, apply change to matrix
	    		loop_flag = 0;
	    		break;
	    	}else if(check == 0){
	    		inChoose = 0;// if cant put, keep in putBlock loop
	    	}
	    }
	    }
	}
	matrix[10][0] = inChoose;
}


int checkIfCanPut(int** cache){
	int i, j;
	int check;
	check = 1;
	int temp3;
	temp3 = 0;
	for(i = 0; i<10; i++){
		for(j = 0; j < 10; j++){
			temp3 = cache[i][j];
			if (temp3 == 3){
				check = 0;
				break;
			}
		}
	}
	return check;
}


int moveBlock(int block, int xsize, int ysize, int x, int y, int** matrix, int** cache, int boolPut){
	int i, j;
	int m, n;
	int** temp; //temp array to store shape of chosen block
	//int** cache;
	int c, cache_row, cache_col;
	fprintf(stderr, "here2\n");
	int check;

	//initiliaze cache matrix
	cache = malloc(11 * sizeof *cache);
	for (c=0; c< 11; c++)
    	{
       		cache[c] = malloc(10 * sizeof(int));
    	}

	fprintf(stderr, "cache initiliazing\n");

    for (cache_row = 0; cache_row < 11; cache_row++){
			for (cache_col = 0; cache_col < 10; cache_col++){
				cache[cache_row][cache_col] = matrix[cache_row][cache_col];
			}
	}	
	fprintf(stderr, "cache initialized from matrix\n");
	
	temp = malloc(ysize * sizeof *temp);
	for (i=0; i< ysize; i++)
    	{
       		temp[i] = malloc(xsize * sizeof(int));
    	}
	
	fprintf(stderr, "temp array initiliazing\n");
	
	for (m = 0; m < ysize; m++){
		for (n = 0; n < xsize; n++){
			//printf("x == %d", b5[0][1]);
			temp[m][n] = 0;
			//array[i][j] = 1;
		}
	}
	fprintf(stderr, "temp array initiliazed \n");

	int p, q;
	switch(block){
		case 1:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b1[p][q];
				}
			}
			break;
		case 2:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b2[p][q];
				}
			}
			break;
		case 3:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b3[p][q];
				}
			}
			break;
		case 4:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b4[p][q];
				}
			}
			break;
		case 5:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b5[p][q];
				}
			}
			break;
		case 6:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b6[p][q];
				}
			}
			break;
		case 7:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b7[p][q];
				}
			}
			break;
		case 8:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b8[p][q];
				}
			}
			break;
		case 9:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b9[p][q];
				}
			}
			break;
		case 10:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b10[p][q];
				}
			}
			break;
		case 11:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b11[p][q];
				}
			}
			break;
		case 12:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b12[p][q];
				}
			}
			break;
		case 13:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b13[p][q];
				}
			}
			break;
		case 14:
			for (p = 0; p < ysize; p++){
				for (q = 0; q < xsize; q++){
					temp[p][q] = b14[p][q];
				}
			}
			break;
		default:
			//printf(" y = %d\n", 0);
			break;
	}


	if (boolPut == 0){
		int temp_row, temp_col;
		int check = 0;
		for (temp_row = 0; temp_row < ysize; temp_row ++){
			for (temp_col = 0; temp_col < xsize; temp_col++){
				cache[0 + y + temp_row][0 + x + temp_col] = temp[temp_row][temp_col] + cache[0 + y + temp_row][0 + x + temp_col];
					int judge;
					judge = cache[0 + y + temp_row][0 + x + temp_col];
					int block_part = temp[temp_row][temp_col];
					if (judge == 2 && block_part == 1){
						cache[0 + y + temp_row][0 + x + temp_col] = 3; // 不能放下,有重合部分变红
					}else if (judge == 1 && block_part == 1){
						cache[0 + y + temp_row][0 + x + temp_col] = 2; // 不能放下,不重合部分变绿
					}
			}
		}
		passToHardware(cache);
		return 0;

	}else if(boolPut == 1){
		int temp_row, temp_col;
		int check = 0;
		for (temp_row = 0; temp_row < ysize; temp_row ++){
			for (temp_col = 0; temp_col < xsize; temp_col++){
				cache[0 + y + temp_row][0 + x + temp_col] = temp[temp_row][temp_col] + cache[0 + y + temp_row][0 + x + temp_col];
					int judge;
					judge = cache[0 + y + temp_row][0 + x + temp_col];
					int block_part = temp[temp_row][temp_col];
					if (judge == 2 && block_part == 1){
						cache[0 + y + temp_row][0 + x + temp_col] = 3; // 不能放下,有重合部分变红
					}else if (judge == 1 && block_part == 1){
						cache[0 + y + temp_row][0 + x + temp_col] = 2; // 不能放下,不重合部分变绿
					}
			}
		}
		passToHardware(cache);
		check = checkIfCanPut(cache);


		if(check == 0){
			return 0;
		}else if(check == 1){
			int put_x, put_y;
			for( put_x = 0 ; put_x < ysize ; put_x ++){
				for( put_y = 0; put_y < xsize; put_y ++){
					matrix[0 + y + put_x][0 + x + put_y] = temp[put_x][put_y] + matrix[0 + y + put_x][0 + x + put_y];

				}
			}
			passToHardware(matrix);
			return 1;

		}

	}

}



//CHECK INPUT VALUE 
int selectBlock(int length, int *b, int **cache)
{
	int t = 0; 
	int select_flag = 1;
	fprintf(stderr, "sB en\n");
	struct usb_keyboard_packet packet;
	int transferred;
  	char keystate[12];
	while(select_flag == 1){

		libusb_interrupt_transfer(keyboard, endpoint_address, (unsigned char *) &packet, sizeof(packet), &transferred, 0);
	    if (transferred == sizeof(packet)) {
	    	pthread_create(&click_thread, NULL, click_thread_f, NULL);
	      	//sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0], packet.keycode[1]);
	    if (packet.keycode[0] == 0x29){
			fprintf(stderr,"esc pressed\n");
	    }else if(packet.keycode[0] == 0x51){
			fprintf(stderr,"down pressed\n");
	    	if ( t == 2){
	    		t = 0;
	    		//hightlight b[j]
	    	}else{
	    		t = t + 1;
	    	}
	    	write_segments(107, t);
	      	//down
	    }else if(packet.keycode[0] == 0x52){
			fprintf(stderr,"up pressed\n");
	    	if ( t == 0){
	    		t = 2;
	    	}else{
	    		t = t - 1;
	    	}
	    	write_segments(107, t);
	      	//up
	    }else if(packet.keycode[0] == 0x28){  //enter
			fprintf(stderr,"enter pressed\n");
	    	if (b[t] == 0){
	    		packet.keycode[0x51];//if chosen block has been used, move cursor to next block
	    	}else{
			select_flag = 0;
	    		return t;
	    		select_flag = 0;
	    		//break;
	    	}
	    }
	    }
	}
}

void genNewBlock(int set_new_blocks, int* b)
{
	int i, j;
	if (set_new_blocks == 1){
		srandom(time(NULL));
		for (i=0; i< 3; i++)
		{
			b[i] = generate();
			//printf("%d\n", b[i]);
		}
		//checkIfFullLine() = 0;
	    //set_new_blocks = 0;
	}else{
		return;
	}

}

void showBloack(int j, int** cache){
	int temp;
	temp = cache[10][j+1];
	switch(temp){
		case 1:
			cache[0][0] = cache[0][1] = cache[0][2] = cache[0][3] = cache[0][4] = 1;
			break;
		case 2:
			cache[0][0] = cache[1][0] = cache[2][0] = cache[3][0] = cache[4][0] = 1;
			break;
		case 3:
			cache[0][0] = cache[0][1] = cache[0][2] = cache[0][3] = 1;
			break;
		case 4:
			cache[0][0] = cache[1][0] = cache[2][0] = cache[3][0] = 1;
			break;
		case 5:
			cache[0][0] = cache[1][0] = cache[0][1] = cache[1][1] = 1;
			break;
		case 6:
			cache[0][0] = cache[0][1] = cache[0][2] = cache[1][0] = cache[2][0] = 1;
			break;
		case 7:
			cache[0][0] = cache[0][1] = cache[0][2] = cache[1][2] = cache[2][2] = 1;
			break;
		case 8:
			cache[0][0] = cache[1][0] = 1;
			break;
		case 9:
			cache[0][0] = cache[0][1] = cache[0][2] = 1;
			break;
		case 10:
			cache[1][1] = 1;
			break;
		case 11:
			cache[0][0] = cache[0][1] = cache[1][0] = 1;
			break;
		case 12:
			cache[0][0] = cache[0][1] = cache[1][1] = cache[1][2] = 1;
			break;
		case 13:
			cache[0][0] = cache[0][1] = cache[0][2] = 1;
			break;
		case 14:
			cache[0][0] = cache[0][1] = 1;
			break;
		default:
			perror("no data input");
			break;

	}

	fprintf(stderr, "showed block on main");
}


void *timer_thread_f(){
	int start = clock();
	int end = 0;
	while(1){
		end = clock();
		if (end - start == 300000){
			pthread_exit(NULL);
			return 0;
			}
		} 
	}

void *click_thread_f(){
	int start = clock();
	int end = 0;
	/******************************send start music signal************************/
	while(1){
		end = clock();
		if (end - start == 1000){
			/**************send end music signal**************/
			pthread_exit(NULL);
		}
	}

}

//***********************************************************************
int main(void)
{
	vga_led_arg_t vla;
 	static const char filename[] = "/dev/vga_led";

	fprintf(stderr, "loading MOD");

  	if ( (vga_led_fd = open(filename, O_RDWR)) == -1) {
    	fprintf(stderr, "could not open %s\n", filename);
    	return -1;
  	}
	
	int *b;
	int **matrix;
	int rows = MATRIX_ROW;
	int cols = MATRIX_COL;
	int i, k, t;
	int **cache;

	struct usb_keyboard_packet packet;
	int transferred;
  	char keystate[12];
	int c, cache_row, cache_col;

	matrix = malloc(rows * sizeof *matrix);
	b = (int *)malloc(3 * sizeof (int));
	cache = malloc(rows * sizeof *cache);
	for (k=0; k<1; k++)
    {
       b[k] = malloc(cols * sizeof(int));
    }

    for (t = 0; t < 3; t ++){
    	b[t] = 0;
    }

    for (i=0; i<rows; i++)
    {
       matrix[i] = malloc(cols * sizeof(int));
    }
	
	for (c=0; c< rows; c++)
    {
       cache[c] = malloc(cols * sizeof(int));
    }

    int m, n;
	for (m = 0; m < MATRIX_ROW; m++){
		for (n = 0; n < MATRIX_COL; n++){
			//printf("x == %d", b5[0][1]);
			matrix[m][n] = 0;
			//array[i][j] = 1;
			//fprintf(stderr, "initlized");
		}
	}

	//initiliaze cache matrix


    for (cache_row = 0; cache_row < 11; cache_row++){
		for (cache_col = 0; cache_col < 10; cache_col++){
			//printf("x == %d", b5[0][1]);
			cache[cache_row][cache_col] = matrix[cache_row][cache_col];
			//array[i][j] = 1;
		}
	}

	int inChoose;
	int end;

	block1.height = 1;
	block1.width = 5;
	//block1.blockArray = b1;
	
	block2.height = 5;
	block2.width = 1;
	//block1.blockArray = b2;

	block3.height = 1;
	block3.width = 4;
	//block1.blockArray = b3;

	block4.height = 4;
	block4.width = 1;
	//block4.blockArray = b4;

	block5.height = 2;
	block5.width = 2;
	//block1.blockArray = b5;

	block6.height = 3;
	block6.width = 3;
	//block1.blockArray = b6;

	block7.height = 1;
	block7.width = 5;
	//block1.blockArray = b7;

	block8.height = 2;
	block8.width = 1;
	//block1.blockArray = b8;

	block9.height = 3;
	block9.width = 1;
	//block1.blockArray = b9;

	block10.height = 1;
	block10.width = 1;
	//block1.blockArray = b10;

	block11.height = 2;
	block11.width = 3;
	//block1.blockArray = b11;

	block12.height = 2;
	block12.width = 3;
	//block1.blockArray = b1;2

	block13.height = 1;
	block13.width = 3;
	//block1.blockArray = b13;

	block14.height = 1;
	block14.width = 2;
	//block1.blockArray = b14;


	genNewBlock(1, b);
	inChoose = 1;

	/*  matrix structure

	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0
	 0    0    0    0    0    0    0    0    0    0
	state b[0] b[1] b[2] end scr   j    0    0    0

	*/
	matrix[10][0] = inChoose;
	matrix[10][1] = b[0];
	matrix[10][2] = b[1];
	matrix[10][3] = b[2];
	cache[10][0] = inChoose;
	cache[10][1] = b[0];
	cache[10][2] = b[1];
	cache[10][3] = b[2];
	perror("started");
	passToHardware(matrix);
	perror("p2h out");
	int j;
	int set_new_blocks;
	
	//printf("keyboard?????");
	if ((keyboard = openkeyboard(&endpoint_address)) == NULL){
		fprintf(stderr, "keyboard not found" );
		exit(1);
	}else{
		perror("keyboard found");
	}

	pthread_create(&timer_thread, NULL, timer_thread_f, NULL);

 for (;;) {
    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    
	if (transferred == sizeof(packet)) {
		fprintf(stderr, "key press catch\n");
		
		//Main loop
		if (inChoose == 1){
			j = selectBlock(3, b, matrix);
			//fprintf(stderr, "sB out");
			inChoose = 0;
			matrix[10][0] = inChoose;
			cache[10][0] = inChoose;
			showBloack(j, cache);
			passToHardware(cache);

		}else{
			fprintf(stderr, "put Block next\n");
			fprintf(stderr,"%d\n", j);
			putBlock(j, b, matrix, cache);
			b[j] = 0;//make used blocks blank
			matrix[10][j+1] = 0;
			passToHardware(matrix);
			clearLine(matrix);
			int boolEnd;
			boolEnd = checkIfGameEnd(b, matrix);
			if (boolEnd == 1){
				matrix[10][4] = 1;
				passToHardware(matrix);
				fprintf(stderr,"%s\n", "end");
				break;
			}else{
				matrix[10][4] = 0;
				if (b[0] == 0){
					if (b[1] == 0){
						if (b[2] == 0){
							set_new_blocks = 1;
							genNewBlock(set_new_blocks, b);
						}
					}else{
						set_new_blocks = 0;
					}
				}else{
					set_new_blocks = 0;
				}
				//genNewBlock(set_new_blocks);
				inChoose = 1;
				matrix[10][0] = 1;
				matrix[10][1] = b[0];
				matrix[10][2] = b[1];
				matrix[10][3] = b[2];
				cache[10][0] = 1;
				cache[10][1] = b[0];
				cache[10][2] = b[1];
				cache[10][3] = b[2];
				passToHardware(matrix);
			}
		}
		//break;
		

		}
	}
		//return 0;
};
