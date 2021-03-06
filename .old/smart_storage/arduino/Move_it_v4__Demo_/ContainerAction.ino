void containerAction(char Mode){
  if (Mode == 'G' || Mode == 'P'){
    if (Mode == 'P' ) {
      delay(500);
      Serial.println("Extra Place");
      
   
      positions[1] = -stepsY-Yconstant;

      stepperY.moveTo(positions[1]);
      while (stepperY.distanceToGo() != 0 && state != 4) {
        stepperY.run();
      }
    }

    if (state != 4) Serial.println("Extending Arm");


    if (Mode == 'G' ) stepperZ.moveTo(ZconstantG);
    else if (Mode == 'P' ) stepperZ.moveTo(ZconstantP);

    while(stepperZ.distanceToGo() != 0  && state != 4){
      stepperZ.run();
    }
    

    if (state != 4) Serial.println("Container reached");
    if (state != 4 && Mode == 'G')  Serial.println("Hooking container");
    if (state != 4 && Mode == 'P')  Serial.println("Placing container");

    positions[0] = -stepsX;
    if (Mode == 'G' ) positions[1] = -stepsY-Yconstant;
    else if (Mode == 'P' ) positions[1] = -stepsY;


    stepperY.moveTo(positions[1]);
    while (stepperY.distanceToGo() != 0 && state != 4) {
      stepperY.run();
    }


 
    if (state != 4) Serial.println("retracting");

    stepperZ.moveTo(500);

    while(stepperZ.distanceToGo() != 0 && state != 4){
      stepperZ.run();
    }

    if (state != 4) Serial.println("Container grabbed");

  }
}






