unsigned long timer_previous_millis = 0;

bool spin_timer(){
  const long interval = 1000;
  unsigned long current_millis = millis();
  
  if (current_millis - timer_previous_millis >= interval) {
    return true;
  }
  
  return false;
}

bool reset_timer(){
  timer_previous_millis = millis();
}
