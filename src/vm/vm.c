#include "./vm.h"
#include <stdio.h>
#include <string.h>

uint16_t stack[MAX_STACK_LENGTH];
uint16_t var_store[MEM_CELL_COUNT/2];
uint16_t code_store[MEM_CELL_COUNT/2];
uint16_t reg_data[R_COUNT];

static inline void push(uint16_t data){
    stack[SP++] = data;
}

static inline uint16_t pop(){
    if (SP > 0){
        return stack[--SP];
    } else {
        reg_data[Rerr] = STACK_UNDERFLOW;
        return -1;
    }
}

static inline uint8_t get_byte(uint16_t word, uint8_t order){
    if (order == LOW){
        return (word & 0x00FF);
    } else if (order == HIGH) {
        return (word >> 8);
    } else {
        return -1;
    }
}

static inline uint16_t get_eff_addr(uint16_t base_addr, uint16_t base_indx){
    if (base_indx == Rcbindx){
        return get_byte(reg_data[Rbindx], LOW) + base_addr;
    } else if (base_indx == Rvbindx) {
        return get_byte(reg_data[Rbindx], HIGH) + base_addr;
    } else {
        return -1;
    }
}

void handle_input_output(uint16_t instruction){
    switch((instruction & 0x0FC0) >> 6){
        printf("%d\n", (instruction & 0x0FC0));
        case PRINT:
            switch(instruction & 0x003F){
                case INTEGER:
                    printf("%d", pop());
                    break;
            }
            break;
        case PRINT_ESEQ:{
            char escape_sequences[] = {'\n', '\r'};
            uint16_t eseq_index = (instruction & 0x003F);
            if (eseq_index < len(escape_sequences, sizeof(char))){
                putchar(escape_sequences[eseq_index]);
            } else {
                reg_data[Rerr] = ILLEGAL_PARAMETER;
            }
            break;
        }
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
            if (stack[SP-2] != 0){
                push((uint16_t) (pop() / pop()));
            } else {
                reg_data[Rerr] = ZERO_DIV_ERROR;
            }
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
            push(pop() << count);
            break;
        case RIGHT:
            push(pop() >> count);
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
}

void handle_memory_load(uint16_t address){
    push(var_store[get_eff_addr(address, Rvbindx)]);
}

void handle_memory_storage(uint16_t address, uint16_t memory_section){
    switch(memory_section){
        case VAR:{
            var_store[get_eff_addr(address, Rvbindx)] = pop();
            break;
        }
        case CODE:{
            code_store[get_eff_addr(address, Rvbindx)] = pop();
            break;
        }
    }
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
            push(get_byte(reg_data[Rbindx], LOW));
            break;
        case Rvbindx:
            push(get_byte(reg_data[Rbindx], HIGH));
            break;
        default:
            push(reg_data[reg_index]);
            break;
    }
}

void handle_jump(uint16_t instruction){
    uint16_t new_ip = get_eff_addr(pop(), Rcbindx);
    switch(instruction & 0x0FFF){
        case UNCOND:
            IP = new_ip;
            break;
        case COND:
            if(reg_data[Rcom] == TRUE){
                IP = new_ip;
                reg_data[Rcom] = FALSE;
            }
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
        case COMPARE:
            handle_comparison(instruction);
            break;
        case LOADM:
            handle_memory_load(instruction & 0x0FFF);
            break;
        case STOREM:
            handle_memory_storage((instruction & 0x0FFF), VAR);
            break;
        case STOREC:
            handle_memory_storage((instruction & 0x0FFF), CODE);
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
        case JMP:
            handle_jump(instruction);
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
