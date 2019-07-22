;
.define     len    =   4
;.entry   LOOP
.define ssss = 33
.define aaaa = 1111
MAIN:      inc K
    mov *,W
    .extern    W
    add    r2,STR
LOOP: jmp W
    prn    #3
TEST: .data 1
STR: .string "abd"
    sub r1,    r4
    stop
.entry STR
K:     .data 2 , 7
