class Light {
  
  PVector location;
  
  int id, level, adjustment;
  
  Light(int _id, int _x, int _y){
    location = new PVector(_x, _y);
    id = _id;
    level = int(noise(location.x,location.y)*255);
    adjustment = 0;
  }
  
  void update(int _c){
  
    if (adjustment < 300){
      for (int i=0; i<3; i++){
        println(sensors[i].getRange(this));
        adjustment += sensors[i].getRange(this);
      }
    }
    
    if (adjustment > 0){
      adjustment -= 2;
    }

    println("----");
    println(adjustment);
    
    
    level = int(noise(location.x+_c/100.0,location.y)*255) + 2 * adjustment;
    
  }
  
  void plot(){
    noStroke();    
    fill(max(0,  min(255,level)));
    ellipse(location.x,location.y,100,100);
  }
  
  int getValue(){
    return level;
  }
  
  PVector getLocation(){
    return location;
  }
  
  void adjustValue(float _multiplier){
    level *= _multiplier;
  }
  
}

class Sensor {
  
  PVector location;
  
  int id;
  boolean status;
  
  Sensor(int _id, int _x, int _y){
    location = new PVector(_x, _y);
    id = _id;
//    x = _x;
//    y = _y;
    status = false;
  }
  
  void updateStatus(boolean _status){
    status = _status;
  }
  
  void toggleStatus(){
    status = !status;
  }
  
  void plot(){
    if (status){
      noFill();
      stroke(255,0,0);
      strokeWeight(2);
      ellipse(location.x,location.y,20,20);
    }else{
      noStroke();
      fill(255,0,0);
      ellipse(location.x,location.y,20,20);
    }
  }
  
  float getRange(Light pt){
    if (status){
      //get distance
      float d = PVector.dist(new PVector(location.x,0), new PVector(pt.getLocation().x, 0));
      //convert distance to go from negative to positive
      float t = (-d + 150)/5;
      //implement siqmoid function
      float sig = 1.0/(1.0 + exp(-t));
//      println(t);
//      println(sig);
//      println("---");
      return (sig * 5);
    }else{
      return 0;
    }
  }
  
}

Light[] lights = new Light[17];
Sensor[] sensors = new Sensor[3];

int count;

void setup(){
  size(720, 320);
  frameRate(20);
  
  lights[0] = new Light(0, 60, 60);
  lights[1] = new Light(1, 180, 60);
  lights[2] = new Light(2, 300, 60);
  lights[3] = new Light(3, 420, 60);
  lights[4] = new Light(4, 540, 60);
  lights[5] = new Light(5, 660, 60);
  lights[6] = new Light(6, 120, 160);
  lights[7] = new Light(7, 240, 160);
  lights[8] = new Light(8, 360, 160);
  lights[9] = new Light(9, 480, 160);
  lights[10] = new Light(10, 600, 160);
  lights[11] = new Light(11, 60, 260);
  lights[12] = new Light(12, 180, 260);
  lights[13] = new Light(13, 300, 260);
  lights[14] = new Light(14, 420, 260);
  lights[15] = new Light(15, 540, 260);
  lights[16] = new Light(16, 660, 260);
  
  sensors[0] = new Sensor(16, 120, 227);
  sensors[1] = new Sensor(16, 360, 227);
  sensors[2] = new Sensor(16, 600, 227);
  
  noStroke();
}

void draw(){
  
  background(255, 204, 0);
  
  println("-----");
  
  for (int i = 0; i < 17; i++){
    lights[i].update(count);
  }
  
  int values = 0;
  
  for (int i = 0; i < 17; i++){
    values += lights[i].getValue();
  }
  
  float targetValue = 3000; // variable to control overall light level
  float multiplier = targetValue / values;

  for (int i = 0; i < 17; i++){
    lights[i].adjustValue(multiplier);
  }
  
  values = 0;
  
  for (int i = 0; i < 17; i++){
    values += lights[i].getValue();
  }
  
//  println(values);
  
  for (int i = 0; i < 17; i++){
    lights[i].plot();
  }
  
  for (int i = 0; i < 3; i++){
    sensors[i].plot();
  }
  
  count++;
}

void keyPressed() {
  if (key == 'a') {
    sensors[0].toggleStatus();
  } 
  
  if (key == 's') {
    sensors[1].toggleStatus();
  } 
  
  if (key == 'd') {
    sensors[2].toggleStatus();
  }
}
//
//void keyReleased() {
//  if (key == 'a') {
//    sensors[0].toggleStatus();
//  } 
//  
//  if (key == 's') {
//    sensors[1].toggleStatus();
//  } 
//  
//  if (key == 'd') {
//    sensors[2].toggleStatus();
//  }
//}
