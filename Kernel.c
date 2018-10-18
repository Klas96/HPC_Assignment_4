__kernel void heat_diffuse(__global int * boxes) {
  int id = get_global_id(0);
  boxes = 1337;
}
