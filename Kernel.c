__kernel void heat_diffuse(__global float * boxes) {
  int id = get_global_id(0);
  boxes[4][4] = 1;
  boxes[6][4] = (float)id;
}
