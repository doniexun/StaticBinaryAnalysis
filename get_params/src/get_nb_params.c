/*
** get_nb_params.c for  in /home/bartholomew/programmation/c/prototype
**
** Made by Bartholomew
** Bartholomew de la Villardiere <exabot@gmail.com>
**
** Started on  mer. mars 12 04:30:58 2014
** Last update mer. mars 12 04:30:58 2014
*/

#include <inttypes.h>
#include <capstone/capstone.h>
#include <string.h>

typedef enum
{
    not_used_in_fct = 0,
    not_set_in_fct,
    set_in_fct
} t_state;

static void update_register_state(t_state* regs, cs_x86* x86)
{
    cs_x86_op* op = 0;

    if (x86->op_count < 2)
        return;
    /*
     * SOURCE
     */
    op = x86->operands + 1;
    switch ((int)op->type)
    {
        case X86_OP_REG:
            if (regs[op->reg] == not_used_in_fct
                && op->reg != X86_REG_RSP)
            {
                regs[op->reg] = not_set_in_fct;
            }
        case X86_OP_MEM:
            if (op->mem.base != 0
                &&regs[op->mem.base] == not_used_in_fct
                && op->mem.base != X86_REG_RSP
                && op->mem.base != X86_REG_RBP)
            {
                regs[op->mem.base] = not_set_in_fct;
            }
            if (op->mem.index != 0
                && regs[op->mem.index] == not_used_in_fct
                && op->mem.index != X86_REG_RSP
                && op->mem.index != X86_REG_RBP)
            {
                regs[op->mem.index] = not_set_in_fct;
            }
        default:
            break;
    }

    /*
     * DESTINATION
     */
    op = x86->operands;
    switch ((int)op->type)
    {
        case X86_OP_REG:
            if (regs[op->reg] == not_used_in_fct)
                regs[op->reg] = set_in_fct;
            break;
        case X86_OP_MEM:
            if (regs[op->mem.base] == not_used_in_fct && op->mem.base != 0)
                regs[op->mem.base] = set_in_fct;
            if (regs[op->mem.index] == not_used_in_fct && op->mem.index != 0)
                regs[op->mem.index] = set_in_fct;
        default:
            break;
    }
}

unsigned int nb_params(t_state* regs)
{
    unsigned int i = 0;
    unsigned int nb = 0;

    for (i = 0; i < X86_REG_MAX; ++i)
        if (regs[i] == not_set_in_fct)
            ++nb;
    return (nb);
}

unsigned int get_nb_params(const uint8_t* fct)
{
    csh handle;
    cs_insn* insn = 0;
    t_state regs[X86_REG_MAX] = {not_used_in_fct};
    size_t count = 0;
    size_t i = 0;
    unsigned char stop = 0;

    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
        return (-1);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    while (!stop)
    {
        count = cs_disasm_ex(handle, fct, 100, 0, 0, &insn);
        for (i = 0; i < count && !stop; ++i)
            if (strncmp(insn[i].mnemonic, "ret", 3) == 0)
                stop = 1;
            else
                update_register_state(regs, &(insn[i].detail->x86));
    }
    cs_close(&handle);
    return (nb_params(regs));
}

