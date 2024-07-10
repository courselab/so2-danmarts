	.global _start    
	.code16gcc
	
	.section .text
	
/* _start: */
/*     call get_ip_into_ax   # Chama a função get_ip_into_ax para obter o ponteiro de instrução atual em AX */
/*     push %ax              # Empurra o endereço de retorno na pilha */
/*     call main             # Chama a função main */
/*     hlt                   # Halt (para a execução do sistema) quando main retornar */

/* get_ip_into_ax: */
/*     mov (%esp), %ax        # Move o endereço de retorno da pilha para AX */
/*     ret                    # Retorna para o endereço de retorno original */

	
_start:
	call main
	ret


	
