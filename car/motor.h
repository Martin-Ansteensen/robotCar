#ifndef _Motor_h
#define _Motor_h

class Motor {
    private:
        int enable, in1, in2;
        bool reverseDir;
        int extraSpeed;
        float adjustedVelocity;
    
        void forward(){
            if (reverseDir) {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
            } else {
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
            }
        }

        void backward(){
            if (reverseDir) {
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
            } else {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
            }
        }

    public:
        Motor(int enable, int in1, int in2, bool reverseDir, int extraSpeed){
            this->enable = enable;
            this->in1 = in1;
            this->in2 = in2;
            this->reverseDir = reverseDir;
            this->extraSpeed = extraSpeed;
            init();
        }
        
        void init(){
            pinMode(enable, OUTPUT);
            pinMode(in1, OUTPUT);
            pinMode(in2, OUTPUT);
        }

        void disable(){
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
        }

        void drive(float velocity){
            // Set motor direction
            if (velocity > 0){
                forward();
            } else {
                backward();
            }
            
            // Set motor speed
            if (velocity > 0){
              adjustedVelocity = velocity + extraSpeed;
            } else if (velocity < 0){
              adjustedVelocity = velocity - extraSpeed;
            } else {
              adjustedVelocity = 0;
            }
            analogWrite(enable, abs(int(adjustedVelocity)));
        }

};
#endif
