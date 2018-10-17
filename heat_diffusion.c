#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]){
  //input arguments
  //./heat_diffusion 1000 100 -i1e10 -d0.02 -n20

  char * endpt;

  const int boxHeight = strtol(argv[1], &endpt,10);
  const int boxWidth = strtol(argv[2], &endpt,10);
  const double iniHeat = strtol(argv[3]+1, &endpt,10);
  const double diffu = strtol(argv[4]+2, &endpt,10);
  const double maxIter = strtol(argv[5]+2, &endpt,10);


  //Intiate box with padding
  double boxes[boxHeight+2][boxWidth+2];
  double boxesLoc[boxHeight+2][boxWidth+2];
  for(int i = 0; i < boxHeight+2; i++){
    for(int j = 0; j < boxWidth+2; j++){
      boxes[i][j] = 0;
      boxesLoc[i][j] = 0;
    }
  }

  if(boxHeight%2 == 1 && boxWidth%2 == 1){
    boxes[(int)boxWidth/2+1][(int)boxHeight/2+1] = iniHeat;
  }
  if(boxHeight%2 == 1 && boxWidth%2 == 0){
      boxes[(int)boxWidth/2][(int)boxHeight/2+1] = iniHeat/2;
      boxes[(int)boxWidth/2+1][(int)boxHeight/2+1] = iniHeat/2;
  }
  if(boxHeight%2 == 0 && boxWidth%2 == 1){
      boxes[(int)boxWidth/2+1][(int)boxHeight/2] = iniHeat/2;
      boxes[(int)boxWidth/2+1][(int)boxHeight/2+1] = iniHeat/2;
  }
  if(boxHeight%2 == 0 && boxWidth%2 == 0){
      boxes[(int)boxWidth/2+1][(int)boxHeight/2] = iniHeat/4;
      boxes[(int)boxWidth/2+1][(int)boxHeight/2+1] = iniHeat/4;
      boxes[(int)boxWidth/2][(int)boxHeight/2] = iniHeat/4;
      boxes[(int)boxWidth/2][(int)boxHeight/2+1] = iniHeat/4;
  }

  for(int i = 0; i < maxIter; i++){
    for(int j = 1; j < boxHeight+1; j++){
      for(int k = 1; k < boxWidth+1; k++){
        //boxes[j][k] = boxes[j][k] + c*((h(i-1,j) + h(i+1,j) + h(i,j-1) + h(i,j+1))/4 - h(i,j) )
        //What about the boundary????
        //printf(" %f ",boxes[j][k]);
        boxesLoc[j][k] = boxes[j][k] + diffu*((boxes[j-1][k] + boxes[j+1][k] + boxes[j][k-1] + boxes[j][k+1])/4 - boxes[j][k]);
      }
    //printf("\n");
    }
    memcpy(boxes, boxesLoc, sizeof(boxes));
  //printf("\n\n\n");
  }

  //take avrage
  double sum = 0;
  for(int j = 1; j < boxHeight+1; j++){
    for(int k = 1; k < boxWidth+1; k++){
      sum += boxes[j][k];
    }
  }

  double avrage = sum/(boxHeight*boxWidth);

  //dif from avrage
  double difFromAvr[boxHeight][boxWidth];
  for(int j = 1; j < boxHeight+1; j++){
    for(int k = 1; k < boxWidth+1; k++){
      difFromAvr[boxHeight][boxWidth] = abs(avrage - boxes[j][k]);
    }
  }

  return 0;
}
