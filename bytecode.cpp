//
// Created by XiaoLi on 25-8-17.
//
#include "bytecode.h"

#include <stdexcept>
#include <string>

namespace lvm::bytecode
{
    std::string getInstructionName(const uint8_t code)
    {
        switch (code)
        {
        case NOP:
            return "NOP";
        case PUSH_1:
            return "PUSH_1";
        case PUSH_2:
            return "PUSH_2";
        case PUSH_4:
            return "PUSH_4";
        case PUSH_8:
            return "PUSH_8";
        case POP_1:
            return "POP_1";
        case POP_2:
            return "POP_2";
        case POP_4:
            return "POP_4";
        case POP_8:
            return "POP_8";
        case LOAD_1:
            return "LOAD_1";
        case LOAD_2:
            return "LOAD_2";
        case LOAD_4:
            return "LOAD_4";
        case LOAD_8:
            return "LOAD_8";
        case STORE_1:
            return "STORE_1";
        case STORE_2:
            return "STORE_2";
        case STORE_4:
            return "STORE_4";
        case STORE_8:
            return "STORE_8";
        case CMP:
            return "CMP";
        case ATOMIC_CMP:
            return "ATOMIC_CMP";
        case MOV_E:
            return "MOV_E";
        case MOV_NE:
            return "MOV_NE";
        case MOV_L:
            return "MOV_L";
        case MOV_LE:
            return "MOV_LE";
        case MOV_G:
            return "MOV_G";
        case MOV_GE:
            return "MOV_GE";
        case MOV_UL:
            return "MOV_UL";
        case MOV_ULE:
            return "MOV_ULE";
        case MOV_UG:
            return "MOV_UG";
        case MOV_UGE:
            return "MOV_UGE";
        case MOV:
            return "MOV";
        case MOV_IMMEDIATE1:
            return "MOV_IMMEDIATE1";
        case MOV_IMMEDIATE2:
            return "MOV_IMMEDIATE2";
        case MOV_IMMEDIATE4:
            return "MOV_IMMEDIATE4";
        case MOV_IMMEDIATE8:
            return "MOV_IMMEDIATE8";
        case JUMP:
            return "JUMP";
        case JUMP_IMMEDIATE:
            return "JUMP_IMMEDIATE";
        case JE:
            return "JE";
        case JNE:
            return "JNE";
        case JL:
            return "JL";
        case JLE:
            return "JLE";
        case JG:
            return "JG";
        case JGE:
            return "JGE";
        case JUL:
            return "JUL";
        case JULE:
            return "JULE";
        case JUG:
            return "JUG";
        case JUGE:
            return "JUGE";
        case MALLOC:
            return "MALLOC";
        case FREE:
            return "FREE";
        case REALLOC:
            return "REALLOC";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case MOD:
            return "MOD";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case XOR:
            return "XOR";
        case NOT:
            return "NOT";
        case NEG:
            return "NEG";
        case SHL:
            return "SHL";
        case SHR:
            return "SHR";
        case USHR:
            return "USHR";
        case INC:
            return "INC";
        case DEC:
            return "DEC";
        case ADD_DOUBLE:
            return "ADD_DOUBLE";
        case SUB_DOUBLE:
            return "SUB_DOUBLE";
        case MUL_DOUBLE:
            return "MUL_DOUBLE";
        case DIV_DOUBLE:
            return "DIV_DOUBLE";
        case MOD_DOUBLE:
            return "MOD_DOUBLE";
        case ADD_FLOAT:
            return "ADD_FLOAT";
        case SUB_FLOAT:
            return "SUB_FLOAT";
        case MUL_FLOAT:
            return "MUL_FLOAT";
        case DIV_FLOAT:
            return "DIV_FLOAT";
        case MOD_FLOAT:
            return "MOD_FLOAT";
        case ATOMIC_ADD:
            return "ATOMIC_ADD";
        case ATOMIC_SUB:
            return "ATOMIC_SUB";
        case ATOMIC_MUL:
            return "ATOMIC_MUL";
        case ATOMIC_DIV:
            return "ATOMIC_DIV";
        case ATOMIC_MOD:
            return "ATOMIC_MOD";
        case ATOMIC_AND:
            return "ATOMIC_AND";
        case ATOMIC_OR:
            return "ATOMIC_OR";
        case ATOMIC_XOR:
            return "ATOMIC_XOR";
        case ATOMIC_SHL:
            return "ATOMIC_SHL";
        case ATOMIC_SHR:
            return "ATOMIC_SHR";
        case ATOMIC_USHR:
            return "ATOMIC_USHR";
        case ATOMIC_INC:
            return "ATOMIC_INC";
        case ATOMIC_DEC:
            return "ATOMIC_DEC";
        case ATOMIC_ADD_DOUBLE:
            return "ATOMIC_ADD_DOUBLE";
        case ATOMIC_SUB_DOUBLE:
            return "ATOMIC_SUB_DOUBLE";
        case ATOMIC_MUL_DOUBLE:
            return "ATOMIC_MUL_DOUBLE";
        case ATOMIC_DIV_DOUBLE:
            return "ATOMIC_DIV_DOUBLE";
        case ATOMIC_MOD_DOUBLE:
            return "ATOMIC_MOD_DOUBLE";
        case ATOMIC_ADD_FLOAT:
            return "ATOMIC_ADD_FLOAT";
        case ATOMIC_SUB_FLOAT:
            return "ATOMIC_SUB_FLOAT";
        case ATOMIC_MUL_FLOAT:
            return "ATOMIC_MUL_FLOAT";
        case ATOMIC_DIV_FLOAT:
            return "ATOMIC_DIV_FLOAT";
        case ATOMIC_MOD_FLOAT:
            return "ATOMIC_MOD_FLOAT";
        case CAS:
            return "CAS";
        case INVOKE:
            return "INVOKE";
        case INVOKE_IMMEDIATE:
            return "INVOKE_IMMEDIATE";
        case RETURN:
            return "RETURN";
        case INTERRUPT:
            return "INTERRUPT";
        case INTERRUPT_RETURN:
            return "INTERRUPT_RETURN";
        case INT_TYPE_CAST:
            return "INT_TYPE_CAST";
        case LONG_TO_DOUBLE:
            return "LONG_TO_DOUBLE";
        case DOUBLE_TO_LONG:
            return "DOUBLE_TO_LONG";
        case DOUBLE_TO_FLOAT:
            return "DOUBLE_TO_FLOAT";
        case FLOAT_TO_DOUBLE:
            return "FLOAT_TO_DOUBLE";
        case OPEN:
            return "OPEN";
        case CLOSE:
            return "CLOSE";
        case READ:
            return "READ";
        case WRITE:
            return "WRITE";
        case CREATE_FRAME:
            return "CREATE_FRAME";
        case DESTROY_FRAME:
            return "DESTROY_FRAME";
        case EXIT:
            return "EXIT";
        case EXIT_IMMEDIATE:
            return "EXIT_IMMEDIATE";
        case GET_FIELD_ADDRESS:
            return "GET_FIELD_ADDRESS";
        case GET_LOCAL_ADDRESS:
            return "GET_LOCAL_ADDRESS";
        case GET_PARAMETER_ADDRESS:
            return "GET_PARAMETER_ADDRESS";
        case CREATE_THREAD:
            return "CREATE_THREAD";
        case THREAD_CONTROL:
            return "THREAD_CONTROL";
        case LOAD_FIELD:
            return "LOAD_FIELD";
        case STORE_FIELD:
            return "STORE_FIELD";
        case LOAD_LOCAL:
            return "LOAD_LOCAL";
        case STORE_LOCAL:
            return "STORE_LOCAL";
        case LOAD_PARAMETER:
            return "LOAD_PARAMETER";
        case STORE_PARAMETER:
            return "STORE_PARAMETER";
        case JUMP_IF_TRUE:
            return "JUMP_IF_TRUE";
        case JUMP_IF_FALSE:
            return "JUMP_IF_FALSE";
        case SYSCALL:
            return "SYSCALL";
        case THREAD_FINISH:
            return "THREAD_FINISH";
        case NEG_DOUBLE:
            return "NEG_DOUBLE";
        case NEG_FLOAT:
            return "NEG_FLOAT";
        case ATOMIC_NEG_DOUBLE:
            return "ATOMIC_NEG_DOUBLE";
        case ATOMIC_NEG_FLOAT:
            return "ATOMIC_NEG_FLOAT";
        case JUMP_IF:
            return "JUMP_IF";
        case INVOKE_NATIVE:
            return "INVOKE_NATIVE";

        default:
            return "UNKNOWN";
        }
    }

    uint8_t parseInstructionCode(std::string code)
    {
        if (code == "NOP")
            return NOP;
        if (code == "PUSH_1")
            return PUSH_1;
        if (code == "PUSH_2")
            return PUSH_2;
        if (code == "PUSH_4")
            return PUSH_4;
        if (code == "PUSH_8")
            return PUSH_8;
        if (code == "POP_1")
            return POP_1;
        if (code == "POP_2")
            return POP_2;
        if (code == "POP_4")
            return POP_4;
        if (code == "POP_8")
            return POP_8;
        if (code == "LOAD_1")
            return LOAD_1;
        if (code == "LOAD_2")
            return LOAD_2;
        if (code == "LOAD_4")
            return LOAD_4;
        if (code == "LOAD_8")
            return LOAD_8;
        if (code == "STORE_1")
            return STORE_1;
        if (code == "STORE_2")
            return STORE_2;
        if (code == "STORE_4")
            return STORE_4;
        if (code == "STORE_8")
            return STORE_8;
        if (code == "CMP")
            return CMP;
        if (code == "ATOMIC_CMP")
            return ATOMIC_CMP;
        if (code == "MOV_E")
            return MOV_E;
        if (code == "MOV_NE")
            return MOV_NE;
        if (code == "MOV_L")
            return MOV_L;
        if (code == "MOV_LE")
            return MOV_LE;
        if (code == "MOV_G")
            return MOV_G;
        if (code == "MOV_GE")
            return MOV_GE;
        if (code == "MOV_UL")
            return MOV_UL;
        if (code == "MOV_ULE")
            return MOV_ULE;
        if (code == "MOV_UG")
            return MOV_UG;
        if (code == "MOV_UGE")
            return MOV_UGE;
        if (code == "MOV")
            return MOV;
        if (code == "MOV_IMMEDIATE1")
            return MOV_IMMEDIATE1;
        if (code == "MOV_IMMEDIATE2")
            return MOV_IMMEDIATE2;
        if (code == "MOV_IMMEDIATE4")
            return MOV_IMMEDIATE4;
        if (code == "MOV_IMMEDIATE8")
            return MOV_IMMEDIATE8;
        if (code == "JUMP")
            return JUMP;
        if (code == "JUMP_IMMEDIATE")
            return JUMP_IMMEDIATE;
        if (code == "JE")
            return JE;
        if (code == "JNE")
            return JNE;
        if (code == "JL")
            return JL;
        if (code == "JLE")
            return JLE;
        if (code == "JG")
            return JG;
        if (code == "JGE")
            return JGE;
        if (code == "JUL")
            return JUL;
        if (code == "JULE")
            return JULE;
        if (code == "JUG")
            return JUG;
        if (code == "JUGE")
            return JUGE;
        if (code == "MALLOC")
            return MALLOC;
        if (code == "FREE")
            return FREE;
        if (code == "REALLOC")
            return REALLOC;
        if (code == "ADD")
            return ADD;
        if (code == "SUB")
            return SUB;
        if (code == "MUL")
            return MUL;
        if (code == "DIV")
            return DIV;
        if (code == "MOD")
            return MOD;
        if (code == "AND")
            return AND;
        if (code == "OR")
            return OR;
        if (code == "XOR")
            return XOR;
        if (code == "NOT")
            return NOT;
        if (code == "NEG")
            return NEG;
        if (code == "INC")
            return INC;
        if (code == "DEC")
            return DEC;
        if (code == "ADD_DOUBLE")
            return ADD_DOUBLE;
        if (code == "SUB_DOUBLE")
            return SUB_DOUBLE;
        if (code == "MUL_DOUBLE")
            return MUL_DOUBLE;
        if (code == "DIV_DOUBLE")
            return DIV_DOUBLE;
        if (code == "MOD_DOUBLE")
            return MOD_DOUBLE;
        if (code == "ADD_FLOAT")
            return ADD_FLOAT;
        if (code == "SUB_FLOAT")
            return SUB_FLOAT;
        if (code == "MUL_FLOAT")
            return MUL_FLOAT;
        if (code == "DIV_FLOAT")
            return DIV_FLOAT;
        if (code == "MOD_FLOAT")
            return MOD_FLOAT;
        if (code == "ATOMIC_ADD")
            return ATOMIC_ADD;
        if (code == "ATOMIC_SUB")
            return ATOMIC_SUB;
        if (code == "ATOMIC_MUL")
            return ATOMIC_MUL;
        if (code == "ATOMIC_DIV")
            return ATOMIC_DIV;
        if (code == "ATOMIC_MOD")
            return ATOMIC_MOD;
        if (code == "ATOMIC_AND")
            return ATOMIC_AND;
        if (code == "ATOMIC_OR")
            return ATOMIC_OR;
        if (code == "ATOMIC_XOR")
            return ATOMIC_XOR;
        if (code == "ATOMIC_NOT")
            return ATOMIC_NOT;
        if (code == "ATOMIC_NEG")
            return ATOMIC_NEG;
        if (code == "ATOMIC_INC")
            return ATOMIC_INC;
        if (code == "ATOMIC_DEC")
            return ATOMIC_DEC;
        if (code == "ATOMIC_ADD_DOUBLE")
            return ATOMIC_ADD_DOUBLE;
        if (code == "ATOMIC_SUB_DOUBLE")
            return ATOMIC_SUB_DOUBLE;
        if (code == "ATOMIC_MUL_DOUBLE")
            return ATOMIC_MUL_DOUBLE;
        if (code == "ATOMIC_DIV_DOUBLE")
            return ATOMIC_DIV_DOUBLE;
        if (code == "ATOMIC_MOD_DOUBLE")
            return ATOMIC_MOD_DOUBLE;
        if (code == "ATOMIC_ADD_FLOAT")
            return ATOMIC_ADD_FLOAT;
        if (code == "ATOMIC_SUB_FLOAT")
            return ATOMIC_SUB_FLOAT;
        if (code == "ATOMIC_MUL_FLOAT")
            return ATOMIC_MUL_FLOAT;
        if (code == "ATOMIC_DIV_FLOAT")
            return ATOMIC_DIV_FLOAT;
        if (code == "ATOMIC_MOD_FLOAT")
            return ATOMIC_MOD_FLOAT;
        if (code == "CAS")
            return CAS;
        if (code == "INVOKE")
            return INVOKE;
        if (code == "INVOKE_IMMEDIATE")
            return INVOKE_IMMEDIATE;
        if (code == "RETURN")
            return RETURN;
        if (code == "INTERRUPT")
            return INTERRUPT;
        if (code == "INTERRUPT_RETURN")
            return INTERRUPT_RETURN;
        if (code == "INT_TYPE_CAST")
            return INT_TYPE_CAST;
        if (code == "LONG_TO_DOUBLE")
            return LONG_TO_DOUBLE;
        if (code == "DOUBLE_TO_LONG")
            return DOUBLE_TO_LONG;
        if (code == "DOUBLE_TO_FLOAT")
            return DOUBLE_TO_FLOAT;
        if (code == "FLOAT_TO_DOUBLE")
            return FLOAT_TO_DOUBLE;
        if (code == "OPEN")
            return OPEN;
        if (code == "CLOSE")
            return CLOSE;
        if (code == "READ")
            return READ;
        if (code == "WRITE")
            return WRITE;
        if (code == "CREATE_FRAME")
            return CREATE_FRAME;
        if (code == "DESTROY_FRAME")
            return DESTROY_FRAME;
        if (code == "EXIT")
            return EXIT;
        if (code == "EXIT_IMMEDIATE")
            return EXIT_IMMEDIATE;
        if (code == "GET_FIELD_ADDRESS")
            return GET_FIELD_ADDRESS;
        if (code == "GET_LOCAL_ADDRESS")
            return GET_LOCAL_ADDRESS;
        if (code == "GET_PARAMETER_ADDRESS")
            return GET_PARAMETER_ADDRESS;
        if (code == "CREATE_THREAD")
            return CREATE_THREAD;
        if (code == "THREAD_CONTROL")
            return THREAD_CONTROL;
        if (code == "LOAD_FIELD")
            return LOAD_FIELD;
        if (code == "STORE_FIELD")
            return STORE_FIELD;
        if (code == "LOAD_LOCAL")
            return LOAD_LOCAL;
        if (code == "STORE_LOCAL")
            return STORE_LOCAL;
        if (code == "LOAD_PARAMETER")
            return LOAD_PARAMETER;
        if (code == "STORE_PARAMETER")
            return STORE_PARAMETER;
        if (code == "JUMP_IF_TRUE")
            return JUMP_IF_TRUE;
        if (code == "JUMP_IF_FALSE")
            return JUMP_IF_FALSE;
        if (code == "SYSCALL")
            return SYSCALL;
        if (code == "THREAD_FINISH")
            return THREAD_FINISH;
        if (code == "NEG_DOUBLE")
            return NEG_DOUBLE;
        if (code == "NEG_FLOAT")
            return NEG_FLOAT;
        if (code == "ATOMIC_NEG_DOUBLE")
            return ATOMIC_NEG_DOUBLE;
        if (code == "ATOMIC_NEG_FLOAT")
            return ATOMIC_NEG_FLOAT;
        if (code == "JUMP_IF")
            return JUMP_IF;
        if (code == "INVOKE_NATIVE")
            return INVOKE_NATIVE;
        throw std::runtime_error("Unknown instruction: " + code);
    }
}
