
#define encoderCLK_x 5   //D1
#define encoderDT_x 4    //D2
#define encoderSW_x 0    // D3

#define encoderCLK_y 14   //D5
#define encoderDT_y 12    //D6
#define encoderSW_y 13   // D7

int value_x = 0;
int value_y =0;

int crntCLK_x;
int prvsCLK_x;
int SWstate_x=0;

int crntCLK_y;
int prvsCLK_y;
int SWstate_y = 0;


void setup() {

  // Set LEDs and rotary encoder pins as inputs  
  pinMode (encoderCLK_x,INPUT);
  pinMode (encoderDT_x,INPUT);
  pinMode (encoderSW_x, INPUT);

  pinMode (encoderCLK_y,INPUT);
  pinMode (encoderDT_y,INPUT);
  pinMode (encoderSW_y, INPUT);

  Serial.begin (9600);

  // Read the initial state of encoderCLK and assign it to prvsCLK variable
  prvsCLK_x = digitalRead(encoderCLK_x);

  prvsCLK_y = digitalRead(encoderCLK_y);

}

void loop() {

  // Read the current state of encoderCLK
  crntCLK_x = digitalRead(encoderCLK_x);
  // Read the state of the encoder's pushbutton value
  SWstate_x = digitalRead(encoderSW_x);

  crntCLK_y = digitalRead(encoderCLK_y);
  // Read the state of the encoder's pushbutton value
  SWstate_y = digitalRead(encoderSW_y);

  // A pulse occurs if the previous and the current state differ
  if (crntCLK_x != prvsCLK_x){
    // If the encoderDT state is different than the encoderCLK state then the rotary encoder is rotating counterclockwise
    if (digitalRead(encoderDT_x) != crntCLK_x) {
      if (SWstate_x == HIGH ){
        value_x -= 128; // Decrease by 137
      }else{
        value_x -= 1280; // Decrease by 1370 for a faster decrease
      }
      if (value_x<0){
        value_x=0;
      }
    } 
    else {
      // Encoder is rotating clockwise
      if (SWstate_x == HIGH){
        value_x += 128; // Increase by 137
      }else{
        value_x+=128;
      }
      if (value_x>1920){
        value_x=1920;
      }
    }
    //map(value, 0, 1920, 0, 180); // Map servo angle to match 0-180 degrees
    Serial.print("valuex of x :");
    Serial.println(value_x);

   


  } 
  // Update prvsCLK with the current state
  prvsCLK_x = crntCLK_x;
  
  if (crntCLK_y != prvsCLK_y){
    // If the encoderDT state is different than the encoderCLK state then the rotary encoder is rotating counterclockwise
    if (digitalRead(encoderDT_y) != crntCLK_y) {
      if (SWstate_y == HIGH ){
        value_y -= 108; // Decrease by 137
      }else{
        value_y -= 1080; // Decrease by 1370 for a faster decrease
      }
      if (value_y<0){
        value_y=0;
      }
    } 
    else {
      // Encoder is rotating clockwise
      if (SWstate_y == HIGH){
        value_y += 108; // Increase by 137
      }else{
        value_y+=108;
      }
      if (value_y>1080){
        value_y=1080;
      }
    }
     Serial.print("valuex of y :");
    Serial.println(value_y);
  } 

 prvsCLK_y = crntCLK_y;
}
