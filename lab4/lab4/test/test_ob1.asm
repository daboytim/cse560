Main     ORI 15
         EXT Displ,V
         ENT Start
         EXT X
;
Start    BRS 3,Displ       Display 6..0
         LD  1,V            r1 <- M[V]
         ST  1,X            M[X] <- r1
         BRS 3,Displ       Display 2..0
         BR  0,0            ; halt
         END