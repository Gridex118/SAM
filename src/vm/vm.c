#include "./vm.h"
#include <stdio.h>
#include <string.h>

uint16_t stack[MAX_STACK_LENGTH];
uint16_t var_store[MEM_CELL_COUNT/2];
uint16_t code_store[MEM_CELL_COUNT/2];
uint16_t reg_data[R_COUNT];

void push(uint16_t data){
    stack[SP++] = data;
}

uint16_t pop(){
    if(SP > 0){
        return stack[--SP];
    } else {
        reg_data[Rerr] = STACK_UNDERFLOW;
        return -1;
    }
}

void handle_input_output(uint16_t instruction){
    switch((instruction & 0x0FC0) >> 6){
        case PRINT:
            switch(instruction & 0x003F){
                case INTEGER:
                    printf("%d", stack[SP-1]);
                    break;
            }
            --SP;
            break;
        case PRINT_ESEQ:
            switch(instruction & 0x003F){
                case NEWLINE:
                    putchar('\n');
                    break;
                case RETURN_CARRIAGE:
                    putchar('\r');
                    break;
            }
            break;
        default:
            reg_data[Rerr] = ILLEGAL_PARAMETER;
    }
}

void handle_arithmetic(uint16_t instruction){
    switch(instruction & 0x0FFF){
        case ADD:
            push(pop() + pop());
            break;
        case SUB:
            push(pop() - pop());
            break;
        case MULT:
            push(pop() * pop());
            break;
        case DIV:
           push((uint16_t) (pop() / pop()));
            break;
        default:
            reg_data[Rerr] = ILLEGAL_PARAMETER;
            break;
    }
}

void handle_logic(uint16_t instruction){
    switch(instruction & 0x0FFF){
        case AND:
            push(pop() & pop());
            break;
        case OR:
            push(pop() | pop());
            break;
        case NOT:
            push(~pop());
            break;
        default:
            reg_data[Rerr] = ILLEGAL_PARAMETER;
    }
}

void handle_bit_shift(uint16_t instruction){
    uint16_t direction = ((instruction & 0x0F00) >> 8);
    uint16_t count = (instruction & 0x00FF);
    switch(direction){
        case LEFT:
            stack[SP-1] = (stack[SP-1] << count);
            break;
        case RIGHT:
            stack[SP-1] = (stack[SP-1] >> count);
            break;
        default:
            reg_data[Rerr] = ILLEGAL_PARAMETER;
    }
}

void handle_comparison(uint16_t instruction){
    switch(instruction & 0x0FFF){
        case EQL:
            reg_data[Rcom] = (pop() == pop());
            break;
        // Since stack[SP-2] is the first operand, the comparison signs must be reversed
        case LS:
            reg_data[Rcom] = (pop() > pop());
            break;
        case GR:
            reg_data[Rcom] = (pop() < pop());
            break;
        default:
            reg_data[Rcom] = ILLEGAL_PARAMETER;
    }
    SP -= 2;
}

void handle_reg_storage(uint16_t reg_index){
    switch(reg_index){
        case Ra:
        case Rb:
        case Rc:
        case Rip:
            reg_data[reg_index] = pop();
            break;
        case Rcbindx:
            reg_data[Rbindx] &= 0xFF00;
            reg_data[Rbindx] |= pop();
            break;
        case Rvbindx:
            reg_data[Rbindx] &= 0x00FF;
            reg_data[Rbindx] |= pop();
            break;
        case Rhlt:
        case Rerr:
        case Rcom:
        default:
            reg_data[Rerr] = REGISTER_ACCESS_DENIED;
            break;
    }
}

void handle_reg_load(uint16_t reg_index){
    switch(reg_index){
        case Rcbindx:
            push(reg_data[Rbindx] & 0x00FF);
            break;
        case Rvbindx:
            push(reg_data[Rbindx] >> 8);
            break;
        default:
            push(reg_data[reg_index]);
            break;
    }
}

void execute_instruction(uint16_t instruction){

    switch(OPCODE(instruction)){
        case PUSH:
            push(instruction & 0x0FFF);
            break;
        case POP:
            reg_data[Ra] = pop();
            break;
        case DUP:
            push(stack[SP-1]);
            break;
        case ARITH:
            handle_arithmetic(instruction);
            break;
        case LOGIC:
            handle_logic(instruction);
            break;
        case BSHIFT:
            handle_bit_shift(instruction);
            break;
        case COMP:
            handle_comparison(instruction);
            break;
        case STORER:
            handle_reg_storage(instruction & 0x0FFF);
            break;
        case LOADR:
            handle_reg_load(instruction & 0x0FFF);
            break;
        case IO:
            handle_input_output(instruction);
            break;
        case HALT:
            reg_data[Rhlt] = TRUE;
            break;
    }

    if ((CODE_BASE_INDEX + ++IP) >= MEM_CELL_COUNT/2){
        reg_data[Rhlt] = TRUE;
    }

}

void run_machine(){
    while (MACHINE_IS_RUNNING){
        execute_instruction(code_store[CODE_BASE_INDEX + IP]);
    }
}

void copy_instructions_to_memory(uint16_t *instructions, uint16_t size){
    memcpy(&code_store, instructions, size);
}
