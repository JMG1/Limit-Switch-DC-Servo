const int MA_F_PIN = 10;
const int MA_R_PIN = 11;
const int MA_SW_PIN = 7;

void setup()
{
  Serial.begin(115200);
  pinMode( MA_F_PIN, OUTPUT );
  pinMode( MA_R_PIN, OUTPUT );
  pinMode( MA_SW_PIN, INPUT );
  
}

int ma_steps = 0;

String complete_instruction[6];  // contains the decoded instruction
bool INIT = false;

int motor_speed = 50;

void loop()
{
  while(!Serial.available()) {}  // if there is nothing on serial, do nothing
  int  i = 0;
  char raw_instruction[25];
  if(Serial.available())
  {  // if something comes from serial, read it and store it in raw_instruction char array
    delay(10); // delay to allow buffer to fill
    while(Serial.available() > 0)
    {
      raw_instruction[i] = Serial.read();
      i++;
    }
  }
  if( strlen( raw_instruction ) > 0 )  // if a new raw_instruction has been read
  {
    // clean raw_instruction before decoding (overwrite non filled array positions with empty spaces)
    for( int n = i; n < 25; n++ ) { raw_instruction[n] = ' '; }
    // decode the instruction (4 fields) (iterator n = field, iterator j = character)
    int j = 0;
    for( int n = 0; n < 5; n++ )
    { 
      while( j < 25 )
      {
        if( raw_instruction[j] == ' ' )
        {
          j++;
          break;
        }
        else
        {
          complete_instruction[n] += raw_instruction[j];
        }
        j++;
      }
    }
    // print decoded instruction by serial
    if( complete_instruction[0] == "INIT" )
    {
      INIT = true; // start reading program
      Serial.println( "DONE" );
    }
    if( INIT == true )
    {
      Serial.println( complete_instruction[0] );
      if( complete_instruction[0] == "POS" )
      {
        ma_steps = complete_instruction[1].toInt();
        Serial.println(ma_steps);
        move_motor();
      }
      if( complete_instruction[0] == "SPEED" )
      {
        motor_speed = complete_instruction[1].toInt();
        Serial.println(motor_speed);
        Serial.println("DONE");
      }
      if( complete_instruction[0] == "END" )
      {
        INIT = false;
        Serial.println( "DONE" );
      }
    }
    // erase complete_instruction array
    for( int i = 0; i < 5; i++ )
    {
      complete_instruction[i] = "";
    }
  }

}





void move_motor()
{
  bool ma_init_state = digitalRead( MA_SW_PIN );
  while( (abs(ma_steps)>0) )
  {
    /// CHECK SWITCH STATE ------------------------------------------------
    if( digitalRead( MA_SW_PIN ) != ma_init_state )
    {
      ma_steps =  ( abs(ma_steps) - 1 )*( ma_steps / abs(ma_steps) );
      ma_init_state = digitalRead( MA_SW_PIN );
    }
    
    /// SET MOTOR A SPEED -------------------------------------------------
    if( ma_steps > 0 )
    {
      digitalWrite( MA_R_PIN, LOW );
      analogWrite( MA_F_PIN, motor_speed );
    }
    else if( ma_steps < 0 )
    {
      digitalWrite( MA_F_PIN, LOW );
      analogWrite( MA_R_PIN, motor_speed );
    }
    else
    {
      digitalWrite( MA_R_PIN, LOW );
      digitalWrite( MA_F_PIN, LOW );
    }
  }
}

