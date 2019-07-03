 #pragma once //Redefinition error will occur is this is removed, purpose is to only get defined once,
              // even with multiple include statements
 
 namespace StateLoops {
     class State {
         public:
          void loop();
     };
 }