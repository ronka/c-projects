;
.define     len    =   4  
;.entry   LOOP
.define ssss=33
.define aaaa = 1111   
MAIN:      inc K
    mov *,W
    .extern    W
    add    r2,STR
LOOP: jmp W
    prn    #3
    TEST2: .data 3
TEST: .data 2,5,3,2
STR: .string "abd"   
STR2:   .string "dd"
    sub r1,    r4
.entry STR
K:     .data 2 , 7
