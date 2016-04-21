#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "usbkeyboard.h"
/************************************Header Files Included******************************************/
#include "vga_led.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
/*************************************End Headers*************************************************/
#define MATRIX_ROW 11
#define MATRIX_COL 10

/*************************CODE FOR CALLING THE INTERFACE BETWEEN HARDWARE AND SOFTWARE*******************************/
/*segment: the data you want to write into the hardware
	index: the index of register you want to write into
*/
void write_segments(unsigned int segment, int index)
{
  vga_led_arg_t vla;
  vla.digit = index;
  vla.segments = segment;
    if (ioctl(vga_led_fd, VGA_LED_WRITE_DIGIT, &vla)) {
      perror("ioctl(VGA_LED_WRITE_DIGIT) failed");
      return;
    }
}
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

void passToHardware(int** matrix){
	int row, col, index; 
	//int indexList[110];
	unsigned int segment;
	for (row = 0; row < 11; row++){
		for (col = 0; col < 10; col++)
		{
			for (index = 1; index < 111; index++){
				segment = matrix[row][col];
				//segment = unsigned int read_segments(index);
				write_segments(segment, index);
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
	int row_to_clean[10];
	int col_to_clean[10];
	int i;
	int t;
	int p, q, r;
	int score;
	for(row = 0; row < 10; row ++){ //get rows that can be cleaned
		for(i = 0; i<10; i++){
			if matrix[row][i] == 1{
				row_to_clean[i] = row;
				score = score + 1;
			}else{
				row_to_clean[i] = 11;
			}
		}
	}
	for(col = 0; col < 10; col++){ //get columns that can be cleaned
		for(t = 0; t <9; t++){
			if matrix[t][col] == 1{
				col_to_clean[i] = col;
				score = score + 1;
			}else{
				col_to_clean[i] = 11;
			}
		}
	}

	for(p = 0; p <9; p ++ ){
		int temp = row_to_clean[p];
		if (temp != 11){
			for(q = 0; q < 9; q++){
				matrix[temp][q] = 0;
			}
		}
		int temp2 = col_to_clean[p];
		if (temp2 != 11){
			for(r = 0; r < 9; r++){
				matrix[r][temp2] = 0;
			}	
		}
	}

	matrix[10][5] = score;
	passToHardware(matrix);

}

int checkIfGameEnd(int** b, int** matrix){
	int i;
	int end = 1;
	int m, n, temp_row, temp_col;
	int p, q;
	for (i = 0; i<3, i++){
		int temp = b[i];
		switch(temp){
			case 0:
				end = 0;
				break;
			case 1:
				int temp[1][5];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						/*start to  extract blocks from matrix if there is an empty bloack and start from this block there are enough blocks to match the shape, 
						regardless if the blocks are empty or not*/
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 4){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 5; temp_col ++){
									/*extract the shape of one of the left shapes and compare*/
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 5; q ++){
											if( b1[p][q] == temp[p][q]){
												break;
											}else{
												/* the game is not end if there is one area that can fit in any of the shapes*/
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
			case 2:
				int temp[5][1];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 4 && 10 - n > 0){
							for(temp_row = 0; temp_row < 5; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 5; p ++){
										for (q = 0; q < 1; q ++){
											if( b2[p][q] == temp[p][q]){
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
				int temp[1][4];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 3){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 4; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 4; q ++){
											if( b3[p][q] == temp[p][q]){
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
				int temp[4][1];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 3 && 10 - n > 0){
							for(temp_row = 0; temp_row < 4; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 4; p ++){
										for (q = 0; q < 1; q ++){
											if( b4[p][q] == temp[p][q]){
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
				int temp[2][2];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 1 && 10 - n > 1){
							for(temp_row = 0; temp_row < 2; temp_row ++){
								for (temp_col = 0; temp_col < 2; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 2; p ++){
										for (q = 0; q < 2; q ++){
											if( b5[p][q] == temp[p][q]){
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
				int temp[3][3];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 2){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 3; q ++){
											if( b6[p][q] == temp[p][q]){
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
				int temp[3][3];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 2){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 3; q ++){
											if( b7[p][q] == temp[p][q]){
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
				int temp[2][1];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 1 && 10 - n > 0){
							for(temp_row = 0; temp_row < 2; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 2; p ++){
										for (q = 0; q < 1; q ++){
											if( b8[p][q] == temp[p][q]){
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
				int temp[3][1];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 0){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 1; q ++){
											if( b9[p][q] == temp[p][q]){
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
				int temp[1][1];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 0){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 1; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 1; q ++){
											if( b10[p][q] == temp[p][q]){
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
				int temp[2][2];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 2 && 10 - n > 2){
							for(temp_row = 0; temp_row < 3; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 3; p ++){
										for (q = 0; q < 3; q ++){
											if( b6[p][q] == temp[p][q]){
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
				int temp[2][3];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 1 && 10 - n > 2){
							for(temp_row = 0; temp_row < 2; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 2; p ++){
										for (q = 0; q < 3; q ++){
											if( b12[p][q] == temp[p][q]){
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
				int temp[1][3];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 2){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 3; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 3; q ++){
											if( b13[p][q] == temp[p][q]){
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
				int temp[1][2];
				for (m = 0; m < 10; m++){
					for (n = 0; n < 10; n++){
						int temp_block = matrix[m][n];
						if (temp_block == 0 && 10 - m > 0 && 10 - n > 1){
							for(temp_row = 0; temp_row < 1; temp_row ++){
								for (temp_col = 0; temp_col < 2; temp_col ++){
									temp[temp_row][temp_col] = matrix[m + temp_row][n + temp_col];
									for (p = 0; p < 1; p ++){
										for (q = 0; q < 2; q ++){
											if( b14[p][q] == temp[p][q]){
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
	break;

	return end;
}

void putBlock(int j, int** b, int** matrix, int** cache){
	int x, y; //initialise position
	int right_bound, left_bound, up_bound, low_bound, xsize, ysize;
	x = 0;
	y = 0;
	int temp = b[j];
	int q = j
	int inChoose;

	switch(temp){
		case 1:
			right_bound = 9 - block1.width;
			low_bound = 9 - block1.height;
			xsize = block1.width;
			ysize = block1.height;
			break;
		case 2:
			right_bound = 9 - block2.width;
			low_bound = 9 - block2.height;
			xsize = block2.width;
			ysize = block2.height;
			break;
		case 3:
			right_bound = 9 - block3.width;
			low_bound = 9 - block3.height;
			xsize = block3.width;
			ysize = block3.height;
			break;
		case 4:
			right_bound = 9 - block4.width;
			low_bound = 9 - block4.height;
			xsize = block4.width;
			ysize = block4.height;
			break;
		case 5:
			right_bound = 9 - block5.width;
			low_bound = 9 - block5.height;
			xsize = block5.width;
			ysize = block5.height;
			break;
		case 6:
			right_bound = 9 - block6.width;
			low_bound = 9 - block6.height;
			xsize = block6.width;
			ysize = block6.height;
			break;
		case 7:
			right_bound = 9 - block7.width;
			low_bound = 9 - block7.height;
			xsize = block7.width;
			ysize = block7.height;
			break;
		case 8:
			right_bound = 9 - block8.width;
			low_bound = 9 - block8.height;
			xsize = block8.width;
			ysize = block8.height;
			break;
		case 9:
			right_bound = 9 - block9.width;
			low_bound = 9 - block9.height;
			xsize = block9.width;
			ysize = block9.height;
			break;
		case 10:
			right_bound = 9 - block10.width;
			low_bound = 9 - block10.height;
			xsize = block10.width;
			ysize = block10.height;
			break;
		case 11:
			right_bound = 9 - block11.width;
			low_bound = 9 - block11.height;
			xsize = block11.width;
			ysize = block11.height;
			break;
		case 12:
			right_bound = 9 - block12.width;
			low_bound = 9 - block12.height;
			xsize = block12.width;
			ysize = block12.height;
			break;
		case 13:
			right_bound = 9 - block13.width;
			low_bound = 9 - block13.height;
			xsize = block13.width;
			ysize = block13.height;
			break;
		case 14:
			right_bound = 9 - block14.width;
			low_bound = 9 - block14.height;
			xsize = block14.width;
			ysize = block14.height;
			break;
		default:
			break;
	}

	for(;;){
		libusb_interrupt_transfer(keyboard, endpoint_address, (unsigned char *) &packet, sizeof(packet), &transferred, 0);
	    if (transferred == sizeof(packet)) {
	      	sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0], packet.keycode[1]);
	    fbputs(keystate, 6, 0);
	    if (packet.keycode[0] == 0x29){
	      	return inChoose = 1;
	      	break;
	    }else if(packet.keycode[0] == 0x4F){
	    	if (x < right_bound){
	    		x = x + 1;
	    		y = y;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	      					//right
	    }else if(packet.keycode[0] == 0x50){
	    	if (x > 0){
	    		x = x - 1;
	    		y = y;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	      					//left
	    }else if(packet.keycode[0] == 0x51){
	    	if (y > 0){
	    		x = x;
	    		y = y + 1;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	      					//down
	    }else if(packet.keycode[0] == 0x52){
	    	if (y < up_bound){
	    		x = x;
	    		y = y - 1;
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, 0);
	    	}else{
	    		x = x;
	    		y = y;
	    	}
	      					//up
	    }else if(packet.keycode[0] == 0x28){ // Enter
	    	int check = checkIfCanPut(xsize, ysize, x, y, cache, 0);
	    	if(check == 1){
	    		inChoose = 1;// can put
	    		moveBlock(temp, xsize, ysize, x, y, matrix, cache, check);
	    	}else{
	    		inChoose = 0;// cant put
	    		break;		
	    	}
	    }
	    }
	}
	return inChoose;
}

int checkIfCanPut(int xsize, int ysize, int x, int y, int** matrix){
	int i, j;
	for (i = x; i < x+ysize; i++){
		for (j = y; j < y+xsize; j ++){
			int temp3 = matrix[i][j];
			int check;
			if (temp3 == 2){
				check = 0;
				break;
			}else{
				check = 1;
			}
			break;
		}
		break;
	}
	return check;
}

void moveBlock(int block, int xsize, int ysize, int x, int y, int** matrix, int** cache, int boolPut){
	int i, j;
	int m, n;
	int** temp; //temp array to store shape of chosen block
	//int** cache;
	int c, cache_row, cache_col;

	//initiliaze cache matrix
	for (c=0; c< 11; c++)
    {
       cache[i] = malloc(11 * sizeof(int));
    }

    for (cache_row = 0; cache_row < 11; cache_row++){
		for (cache_col = 0; cache_col < 10; cache_col++){
			//printf("x == %d", b5[0][1]);
			cache[cache_row][cache_col] = matrix[cache_row][cache_col];
			//array[i][j] = 1;
		}
	}

	for (i=0; i<ysize; i++)
    {
       temp[i] = malloc(xsize * sizeof(int));
    }

    //int m, n;
	for (m = 0; m < ysize; m++){
		for (n = 0; n < xsize; n++){
			//printf("x == %d", b5[0][1]);
			temp[m][n] = 0;
			//array[i][j] = 1;
		}
	}

	int p. q;
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

	int temp_row, temp_col;
	//int result;


	for (temp_row = 0; temp_row < ysize; temp_row ++){
		for (temp_col = 0; temp_col < xsize; temp_col ++){
			if (boolPut == 0){
				/* if did not trigger put block signal, save changes in cache matrix */
				cache[0 + x + temp_row][0 + y + temp_col] = temp[temp_row][temp_col] + cache[0 + x + temp_row][0 + y + temp_col];
				cache[0 + x + temp_row][0 + y + temp_col] = 3;
				passToHardware(cache);
			}else if(boolPut == 1){
				/* if triggered put block signal, save changes in matrix */
				matrix[0 + x + temp_row][0 + y + temp_col] = ma[temp_row][temp_col] + matrix[0 + x + temp_row][0 + y + temp_col];
				passToHardware(matrix);
			}
		}
	}


}

int selectBlock(int length, int *b)
{
	int t = 0; 
	for(;;){
		libusb_interrupt_transfer(keyboard, endpoint_address, (unsigned char *) &packet, sizeof(packet), &transferred, 0);
	    if (transferred == sizeof(packet)) {
	      	sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0], packet.keycode[1]);
	    fbputs(keystate, 6, 0);
	    if (packet.keycode[0] == 0x29){
	      	//esc
	      	break;
	    }else if(packet.keycode[0] == 0x51){
	    	if ( t == 2){
	    		t = 0;
	    		//hightlight b[j]
	    	}else{
	    		t = t + 1;
	    	}
	      	//down
	    }else if(packet.keycode[0] == 0x52){
	    	if ( t == 0){
	    		t = 2;
	    	}else{
	    		t = t - 1;
	    	}
	      	//up
	    }else if(packet.keycode[0] == 0x28){  //enter
	    	if (b[t] == 0){
	    		packet.keycode[0x51];//if chosen block has been used, move cursor to next block
	    	}else{
	    		return t;
	    		break;
	    	}
	    }
	    }
	}
}

void genNewBlock(int set_new_blocks, int** b)
{
	int i, j;
	if (set_new_blocks == 1){
		srandom(time(NULL));
		for (i=0; i< 3; i++)
		{
			b[i] = generate();
		}
		//checkIfFullLine() = 0;
	    //set_new_blocks = 0;
	}else{
		return;
	}

}


init_array(int** array, int rows, int cols){

}

int main(void)
{
	int **b;
	int **matrix
	int rows = MATRIX_ROW;
	int cols = MATRIX_COL;
	int i, k, t;
	int **cache;

	int c, cache_row, cache_col;

	matrix = malloc(rows * sizeof *matrix);
	b = malloc(1 * sizeof *b);
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

    int m, n;
	for (m = 0; m < MATRIX_ROW; m++){
		for (n = 0; n < MATRIX_COL; n++){
			//printf("x == %d", b5[0][1]);
			matrix[m][n] = 0;
			//array[i][j] = 1;
		}
	}

	//initiliaze cache matrix
	for (c=0; c< 11; c++)
    {
       cache[i] = malloc(11 * sizeof(int));
    }

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
	int inChoose = 1;

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
	state b[0] b[1] b[2] end sco  arr   0    0    0

	*/
	matrix[10][0] = inChoose;
	matrix[10][1] = b[0];
	matrix[10][2] = b[1];
	matrix[10][3] = b[2];
	matrix[10][6] = 1;
	passToHardware(matrix);

	int i, j;
	int set_new_blocks;

	for (;;) {
		if (inChoose == 1){
			j = selectBlock(3, b);
			inChoose = 0;
			matrix[10][0] = inChoose;
			matrix[10][6] = j+1
			passToHardware(matrix);

		}else{
			putBlock(j, b, matrix, cache);
			b[j] = 0;//make used blocks blank
			matrix[10][j+1] = 0;

			if (checkIfGameEnd(b, matrix) == 1){
				matrix[10][4] = 1;
				passToHardware(matrix);
				printf("%s\n", "end");
				break;
			}else{
				matrix[10][4] = 0
				clearLine(matrix); 
				if (b[0] == 0){
					if (b[1] == 0){
						if (b[2] == 0){
							set_new_blocks = 1;
							genNewBlock(set_new_blocks);
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
				passToHardware(matrix);
			}
		}
		break;
	}
	break;
}