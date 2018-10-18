#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <CL/cl.h>

int main(int argc, char* argv[]){

  //declartaions

  char * endpt;

  const int boxHeight = strtol(argv[1], &endpt,10);
  const int boxWidth = strtol(argv[2], &endpt,10);
  const double iniHeat = strtol(argv[3]+1, &endpt,10);
  const double diffu = strtol(argv[4]+2, &endpt,10);
  const double maxIter = strtol(argv[5]+2, &endpt,10);

  double boxes[boxHeight+2][boxWidth+2];
  //int * test = malloc(sizeof(int));


  for(int i = 0; i < boxHeight+2; i++){
    for(int j = 0; j < boxWidth+2; j++){
      boxes[i][j] = 0;
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

  //OpenCL related declartaions
  //Läser tillbaka data i denna
  //int* test = (int)malloc(sizeof(float)*boxHeight*boxWidth);
  int DATA_SIZE = sizeof(float)*boxHeight*boxWidth;
  int HEIGHT = boxHeight+2;
  int WIDTH = boxWidth+2;

  cl_int err;
  cl_platform_id platform;
  cl_context_properties props[3] = { CL_CONTEXT_PLATFORM, 0, 0 };
  cl_context context;
  cl_device_id device;
  cl_program program;
  cl_kernel kernel;
  cl_mem buffer;
  cl_command_queue  queue;

  // Setup OpenCL
  err = clGetPlatformIDs( 1, &platform, NULL );
  err = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU, 1, &device, NULL);


  props[1] = (cl_context_properties)platform;//isert
  //context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
  context = clCreateContext(props, 1, &device, NULL, NULL, &err);

  //Read in kerneal
  char * source;
  long length;
  FILE * f = fopen ("Kernel.c", "rb");

  if (f){
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);

    source  = malloc (length);

    if (source){
      fread (source, 1, length, f);
    }
    fclose (f);
  }

  printf("Kernals:\n\n");
  printf("%s\n", source);

  // Compile the kernel
  program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, &err);
  //cl_int clBuildProgram (cl_program program,cl_uint num_devices,const cl_device_id *device_list,const char *options,void (*pfn_notify)(cl_program, void *user_data),void *user_data)
  err = clBuildProgram(program, 10, NULL, NULL, NULL, NULL);

  kernel = clCreateKernel(program, "heat_diffuse", NULL);

  //queue = clCreateCommandQueue(context, device, (cl_command_queue_properties)0, NULL);
  queue = clCreateCommandQueueWithProperties(context, device, (cl_command_queue_properties)0, NULL);

  //create the memory object
  buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE, NULL, &err);

  //Copy the data to the input
  //cl_int clEqnWriBuff = clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, boxes, 0, NULL, NULL);
  err = clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, DATA_SIZE, boxes, 0, NULL, NULL);

  // Execute the kernel
  //sätter argument 0 i kernel till buffer
  clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);

  //Antalet thrads som ska köras
  size_t global_dimemsions[] = {HEIGHT,WIDTH,0};
  size_t local_dimemsions[] = {3,3,0};

  //clEnqueueNDRangeKernel(queue, kernel, GLOBAL DIM, LOCAL DIM, global_dimemsions, NULL, 0, NULL, NULL);
  //clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_dimemsions, local_dimemsions, 0, NULL, NULL);
  clEnqueueNDRangeKernel(queue, kernel, global_dimemsions, 3, NULL, global_dimemsions, local_dimemsions, NULL, NULL);

  //Wait for ecerything to finish
  err = clFinish(queue);

  //Read back the results
  //clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(cl_int)*HEIGHT*WIDTH, boxes, 0, NULL, NULL);
  err = clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(float)*HEIGHT*WIDTH, boxes, 0, NULL, NULL);
  err = clFinish(queue);

  
  err == clFinish(queue);
   for(int i = 0; i<boxHeight; i++){
     for(int j = 0; j<boxWidth; j++){
      printf(" %f ",boxes[i][j]); 
     }
     printf("\n");
   }

  clReleaseMemObject(buffer);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  return 0;
}
