__kernel void heat_diffuse(__global float * boxes) {
  int id1 = get_global_id(0);
  int id2 = get_global_id(1);
  
  boxes[id1][id2] = 1;
  boxes[6][4] = (float)id;
}
