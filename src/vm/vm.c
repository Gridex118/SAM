#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

uint16_t stack[MAX_STACK_LENGTH];
uint16_t data_store[MEM_CELL_COUNT/2];
uint16_t code_store[MEM_CELL_COUNT/2];
uint16_t reg_data[R_COUNT];

#define IP reg_data[Rip]
#define SP reg_data[Rsp]
#define CODE_BASE_INDEX (reg_data[Rbindx] & 0x00FF)

#define MACHINE_IS_RUNNING (reg_data[Rhlt] == FALSE)

#define OPCODE(instruction) (instruction >> 12)

static inline void push(uint16_t data){
    if (SP < MAX_STACK_LENGTH){
        stack[SP++] = data;
    } else {
        reg_data[Rerr] = STACK_OVERFLOW;
    }
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

static inline uint16_t eff_addr(uint16_t base_addr, uint16_t base_indx){
    if (base_indx == Rcbindx){
        return (get_byte(reg_data[Rbindx], LOW) + base_addr);
    } else if (base_indx == Rvbindx) {
        return (get_byte(reg_data[Rbindx], HIGH) + base_addr);
    } else {
        return -1;
    }
}

void print_string(){
    uint16_t current_addr = eff_addr(pop(), Rvbindx);
    uint8_t order = HIGH;
    uint8_t current_char;
    do{
        current_char = get_byte(data_store[current_addr], order);
        printf("%c", current_char);
        if (order == LOW){
            ++current_addr;
        }
        order = (order == HIGH ? LOW : HIGH);
    } while (current_char != '\0');
}

void push_input(uint8_t data_type){
    char raw_input[5];    // 2^16 has 5 digits
    fgets(raw_input, 5, stdin);
    assert(raw_input != NULL);
    long num;
    switch(data_type){
        case INTEGER:
            if ((num = strtol(raw_input, NULL, 10)) < UINT16_MAX){
                push((uint16_t) num);
            } else {
                push(UINT16_MAX);
            }
            break;
        case CHAR:
            push((uint16_t) raw_input[0]);
            break;
    }
}

int print_eseq(uint8_t eseq_index){
    char escape_sequences[] = { '\n', '\r' };
    if (eseq_index < len(escape_sequences, sizeof(char))){
        putchar(escape_sequences[eseq_index]);
        return 0;
    } else {
        return -1;
    }
}

void handle_input_output(uint16_t instruction){
    switch((instruction & 0x0FC0) >> 6){
        case INPUT:
            push_input(instruction & 0x003F);
            break;
        case PRINT:
            switch(instruction & 0x003F){
                case INTEGER:
                    printf("%d", pop());
                    break;
                case STRING:
                    print_string();
                    break;
            }
            break;
        case PRINT_ESEQ:{
            if (print_eseq(instruction & 0x003F) == -1){
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
                uint16_t a = pop(), b = pop();
                push((uint16_t) (b / a));    // Firt operand(dividend) was pushed first
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
        case NEQL:
            reg_data[Rcom] = (pop() != pop());
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

void handle_memory_load(uint16_t parameters){
    uint16_t address = (parameters & 0x00FF);
    uint8_t base = ((parameters & 0x0F00) >> 8);
    push(data_store[eff_addr(address, base)]);
}

void handle_memory_storage(uint16_t parameters){
    uint16_t address = (parameters & 0x07FF);
    switch((parameters & 0x0800) >> 11){
        case VAR:{
            data_store[eff_addr(address, Rvbindx)] = pop();
            break;
        }
        case CODE:{
            code_store[eff_addr(address, Rcbindx)] = pop();
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
    uint8_t option = ((instruction & 0x0800) >> 11);
    uint16_t jmp_indx = (instruction & 0x7FF);
    uint16_t new_ip = eff_addr(jmp_indx, Rcbindx);
    switch(option){
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

void handle_function(uint16_t instruction){
    uint8_t option = ((instruction & 0x0800) >> 11);
    uint16_t jmp_indx = (instruction & 0x7FF);
    switch(option){
        case CALL:{
            push(IP);
            push(reg_data[Rbindx]);
            reg_data[Rc] =  SP;
            handle_jump(jmp_indx);
        }
            break;
        case RETURN:
            while (SP > reg_data[Rc]){
                pop();
            }
            reg_data[Rbindx] = pop();
            IP = pop();
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
            handle_memory_storage((instruction & 0x0FFF));
            break;
        case STORER:
            handle_reg_storage(instruction & 0x0FFF);
            break;
        case LOADR:
            handle_reg_load(instruction & 0x0FFF);
            break;
        case JMP:
            handle_jump(instruction);
            break;
        case IO:
            handle_input_output(instruction);
            break;
        case FUNCT:
            handle_function(instruction);
            break;
        case HALT:
            reg_data[Rhlt] = TRUE;
            break;
    }
    if ((CODE_BASE_INDEX + (++IP)) >= MEM_CELL_COUNT/2){
        reg_data[Rhlt] = TRUE;
    }
}

void run_machine(){
    while (MACHINE_IS_RUNNING){
        execute_instruction(code_store[CODE_BASE_INDEX + IP]);
    }
}
