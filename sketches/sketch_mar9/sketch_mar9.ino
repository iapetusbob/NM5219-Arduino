// creating a function that accepts one string arg, and returns a string
String greet(String name){
  return "Hello, " + name +"!";
}

// initialize the string variable in global enviornment
String name;

void setup() {
  // put your setup code here, to run once:

  // initializing serial monitor to the baud rate of the arduino
  Serial.begin(9600);

  // assigning string value "Yuan Hao" to variable declared in global env
  name = "Yuan Hao";

  // initializing AND assigning string variable greeted with value returned by greet(name)
  String greeted = greet(name);

  // print on serial monitor on new line the value of greeted
  Serial.println(greeted);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}