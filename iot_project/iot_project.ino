const int inter_time = 1000;
int time = 0;
int count = 0;
/*mode*/
const int LED_leftt = 22;
const int LED_right = 24;
int mode = -1;
#define LEFTT 0
#define RIGHT 1
/*sensor*/
const int trig_front = 2;
const int echo_front = 3;
const int trig_right = 4;
const int echo_right = 5;
const int trig_leftt = 6;
const int echo_leftt = 7;
/*motor*/
const int motorIn1 = 8;
const int motorIn2 = 9;
const int motorIn3 = 10;
const int motorIn4 = 11;
/*stop*/
int stop_time = 0;
int tmp_f = 0, tmp_l = 0, tmp_r = 0;

/*function*/
int defMode();
float detectDistance(int, int);
int isStop(int, int, int);
void leftt(int);
void right(int);

void setup() {
    Serial.begin(9600);
    /*mode*/
    pinMode (LED_leftt, OUTPUT);
    pinMode (LED_right, OUTPUT);
    digitalWrite(LED_leftt, LOW);
    digitalWrite(LED_right, LOW);
    /*sensor*/
    pinMode (trig_front, OUTPUT);
    pinMode (echo_front, INPUT);
    pinMode (trig_leftt, OUTPUT);
    pinMode (echo_leftt, INPUT);
    pinMode (trig_right, OUTPUT);
    pinMode (echo_right, INPUT);
    /*motor*/
    pinMode (motorIn1, OUTPUT);
    pinMode (motorIn2, OUTPUT);
    pinMode (motorIn3, OUTPUT);
    pinMode (motorIn4, OUTPUT);
}

void loop() {
    /*mode*/
    while(1){
        if(defMode() == 1){
            break;
        }
    }
    /*senser and run*/
    float distance_front, distance_leftt, distance_right;
    while(1){
        Serial.print("Data:");
        Serial.println(++count);
        distance_front = detectDistance(trig_front, echo_front);
        distance_leftt = detectDistance(trig_leftt, echo_leftt);
        distance_right = detectDistance(trig_right, echo_right);
//        if(distance_front > 50 && distance_leftt > 50 && distance_right > 50){
//            break;
//        }
        if(mode == RIGHT){
            if(distance_front < 5){
                analogWrite(motorIn1, 0);
                analogWrite(motorIn2, 100);
                analogWrite(motorIn3, 0);
                analogWrite(motorIn4, 100);
                delay(300);
                analogWrite(motorIn1, 0);
                analogWrite(motorIn2, 0);
                analogWrite(motorIn3, 0);
                analogWrite(motorIn4, 0);
                delay(500);
                if(distance_leftt < 8 && distance_right < 8){
                    right(600);
                }
                else if(distance_right < 8){
                    leftt(300);
                }
                else if(distance_leftt < 8){
                    right(300);
                }
            }
            else{
                analogWrite(motorIn1, 140);
                analogWrite(motorIn2, 0);
                analogWrite(motorIn3, 140);
                analogWrite(motorIn4, 0);
            }
        }
            
    }
    time = time + inter_time;
//    delay(inter_time);
}
int defMode(){
    int distance_leftt = detectDistance(trig_leftt, echo_leftt);
    int distance_right = detectDistance(trig_right, echo_right);
    if(mode == -1 && distance_leftt < 7 && distance_right > 7){
        mode = LEFTT;
        digitalWrite(LED_leftt, HIGH);
        digitalWrite(LED_right, LOW);
        return 1;
    }
    else if(mode == -1 && distance_leftt > 7 && distance_right < 7){
        mode = RIGHT;
        digitalWrite(LED_right, HIGH);
        digitalWrite(LED_leftt, LOW);
        return 1;
    }
    return 0;
}

float detectDistance(int trig, int echo){
    float duration, distance;
    digitalWrite(trig, HIGH);
    delayMicroseconds(1000);
    digitalWrite(trig, LOW);
    duration = pulseIn (echo, HIGH);
    distance = (duration/2)/29;
//    if(trig == trig_front){
//        Serial.print("F:");
//    }
//    else if(trig == trig_leftt){
//        Serial.print("L:");
//    }
//    else{
//        Serial.print("R:");
//    }
//    Serial.print(distance);
//    Serial.println(" cm");
//    delay(inter_time);
    return distance;
}

int isStop(int front, int leftt, int right){
    if(tmp_f != front && tmp_l != leftt && tmp_r != right){
        tmp_f = front;
        tmp_l = leftt;
        tmp_r = right;
        stop_time = 0;
        return 0;
    }
    else if(stop_time > 2000){
        stop_time = 0;
        Serial.println("stop");
        return 1;
    }
    else{
        stop_time += inter_time;
        return 0;
    }
}

void leftt(int delay_t){
    analogWrite(motorIn1, 0);
    analogWrite(motorIn2, 80);
    analogWrite(motorIn3, 80);
    analogWrite(motorIn4, 0);
    delay(delay_t);
}
void right(int delay_t){
    analogWrite(motorIn1, 80);
    analogWrite(motorIn2, 0);
    analogWrite(motorIn3, 0);
    analogWrite(motorIn4, 80);
    delay(delay_t);
}

