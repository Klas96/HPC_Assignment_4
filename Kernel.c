__kernel void heat_diffuse(__global float * boxes) {
  int id = get_global_id(0);
  boxes[3][3] = 1337;
}
