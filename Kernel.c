//Vill läsa inn martis data samt
//Vill även ta reda på dimensioner av matrisen

void kernel heat_diffuse(global float *test) {
  //get thread number
  int id = get_global_id(0);

  test = id;

  printf("Hej work item %i här!",id);

  boxes[j][k] = boxes[j][k] + diffu*((boxes[j-1][k] + boxes[j+1][k] + boxes[j][k-1] + boxes[j][k+1])/4 - boxes[j][k]);

}
