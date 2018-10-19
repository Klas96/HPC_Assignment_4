__kernel void heat_diffuse(float * boxes) {
  int id0 = get_global_id(0);
  int id1 = get_global_id(1);
  boxes[id0][id1] = 1;
}
