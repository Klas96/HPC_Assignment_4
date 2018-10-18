
__kernel void heat_diffuse(__global int * boxes) {
  int id = get_global_id(0);

  printf("Hej work item %i här!",id);

  boxes[(id+100) %10][(id+100)%20] = id;
  boxes[3][3] = 1337;

}
