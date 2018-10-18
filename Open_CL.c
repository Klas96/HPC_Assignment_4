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

  //OpenCL related declartaions
  //Läser tillbaka data i denna
  float * data;
  int DATA_SIZE = 1000;
  int HEIGHT = boxHeight;
  int WIDTH = boxWidth;


  cl_context context;
  cl_device_id device;
  cl_program program;
  cl_kernel kernel;
  cl_mem buffer;
  cl_command_queue  queue;

  // Setup OpenCL
  clGetDeviceIDs(NULL,CL_DEVICE_TYPE_GPU, 1, &device, NULL);

  context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
  queue = clCreateCommandQueue(context, device, (cl_command_queue_properties)0, NULL);

  //Define our kernel:
  char *source = {
    "void kernel heat_diffuse(global float *boxes) {"
      //get thread number
      "int id = get_global_id(0);"

      "printf('Hej work item %i här!',id);"
      "boxes[j][k] = boxes[j][k] + diffu*((boxes[j-1][k] + boxes[j+1][k] + boxes[j][k-1] + boxes[j][k+1])/4 - boxes[j][k]);"
    "}"
  };

  // Compile the kernel
  //#define cl_program
  program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  kernel = clCreateKernel(program, "heat_diffusion", NULL);

  //create the memory object
  buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE, NULL, NULL);

  //Copy the data to the input
  cl_int clEqnWriBuff = clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, data, 0, NULL, NULL);

  // Execute the kernel
  //sätter argument 0 i kernel till buffer
  clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);

  //Antalet thrads som ska köras
  size_t global_dimemsions[] = {HEIGHT,WIDTH,0};
  size_t local_dimemsions[] = {3,3,0};

  //clEnqueueNDRangeKernel(queue, kernel, GLOBAL DIM, LOCAL DIM, global_dimemsions, NULL, 0, NULL, NULL);
  clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_dimemsions, local_dimemsions, 0, NULL, NULL);

  //Read back the results
  clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(cl_float)*HEIGHT, data, 0, NULL, NULL);

  //Wait for ecerything to finish
  clFinish(queue);
}
