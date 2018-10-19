#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <CL/cl.h>


const char *getErrorString(cl_int error){
switch(error){
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}



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
  cl_command_queue queue;

  cl_uint nmb_platforms;
  cl_uint nmb_devices;

  // Setup OpenCL
  err = clGetPlatformIDs( 1, &platform, &nmb_platforms);

  if(err != CL_SUCCESS){
    printf("Error in clGetPlatformIDs\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

  err = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU, 1, &device, &nmb_devices);


  if(err != CL_SUCCESS){
    printf("Error in clGetDeviceIDs\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

  props[1] = (cl_context_properties)platform;//isert
  //context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
  context = clCreateContext(props, 1, &device, NULL, NULL, &err);

  if(err != CL_SUCCESS){
    printf("Error in clCreateContext\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

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

  if(err != CL_SUCCESS){
    printf("Error in clCreateProgramWithSource\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

  //cl_int clBuildProgram (cl_program program,cl_uint num_devices,const cl_device_id *device_list,const char *options,void (*pfn_notify)(cl_program, void *user_data),void *user_data)
  err = clBuildProgram(program, 1, device, NULL, NULL, NULL);

  if(err != CL_SUCCESS){
    printf("Error in clBuildProgram\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

  kernel = clCreateKernel(program, "heat_diffuse", NULL);

  //queue = clCreateCommandQueue(context, device, (cl_command_queue_properties)0, NULL);
  queue = clCreateCommandQueueWithProperties(context, device, (cl_command_queue_properties)0, &err);

  if(err != CL_SUCCESS){
    printf("Error in clCreateCommandQueueWithProperties\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }
  //create the memory object
  buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE, NULL, &err);

  if(err != CL_SUCCESS){
    printf("Error in clCreateBuffer\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

  //Copy the data to the input
  //cl_int clEqnWriBuff = clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, boxes, 0, NULL, NULL);
  err = clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, DATA_SIZE, boxes, 0, NULL, NULL);


  if(err != CL_SUCCESS){
    printf("Error in clEnqueueWriteBuffer\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }
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

  if(err != CL_SUCCESS){
    printf("Error in clFinish\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }

  //Read back the results
  //clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(cl_int)*HEIGHT*WIDTH, boxes, 0, NULL, NULL);
  err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(float)*HEIGHT*WIDTH, boxes, 0, NULL, NULL);
  if(err != CL_SUCCESS){
    printf("Error in clEnqueueReadBuffer\n");
    char * errorstring = getErrorString(err);
    printf("%s\n", errorstring);
  }
  err = clFinish(queue);

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
