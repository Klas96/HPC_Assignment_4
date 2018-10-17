#include <stdlib.h>
#include <stdio.h>
#include <CL/cl.h>

// Setup OpenCL
int nrDevis = 1;
clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, nrDevis, &device, NULL);
context = clCreateContext(NULL, nrDevis, &device, NULL, NULL, NULL);
queue = clCreateCommandQueue(context, device, (cl_command_queue_properties)0, NULL);

//Define our kernel:
char *source = {
  "kernel void heat_diffusion(global float *data) {\n"
  "KOD HÄR!"
  " int id get_global_id(0);\n"
  " data[id] = sin(data[id]);\n"
  "}\n"
}

// Compile the kernel
program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
clBouildPtogram(program, 0, NULL, NULL, NULL, NULL);
kernel = clCreateKernel(program, "heat_diffusion", NULL);

//create the memory object
buffer = clCreateBuffer(conetxt, CL_MEM_READ_WRITE, DATA_SIZE, NULL, NULL);

//Copy the data to the input
clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, data, 0, NULL, NULL);

// Execute the kernel
clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
size_t global_dimemsions[] = {LENGTH,0,0};
clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimemsions, NULL, 0, NULL, NULL);

//Read back the results
clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(cl_float)*LENGTH, data, 0, NULL, NULL);

//Wait for ecerything to finish
clFinish(queue);
